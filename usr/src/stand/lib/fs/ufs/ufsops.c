/*
 * Copyright (c) 1991-1997, Sun Microsystems, Inc.
 * All Rights Reserved.
 */

#pragma ident	"@(#)ufsops.c	1.34	98/01/28 SMI"

#include <sys/param.h>
#include <sys/vnode.h>
#include <sys/fs/ufs_fsdir.h>
#include <sys/fs/ufs_fs.h>
#include <sys/fs/ufs_inode.h>
#include <sys/sysmacros.h>
#include <sys/promif.h>

#include <sys/stat.h>
#include <sys/bootvfs.h>
#include <sys/bootdebug.h>
#include <sys/salib.h>
#include <sys/sacache.h>


int print_cache_stats = 0;

static fileid_t *head;
extern char *strcpy();

/* Only got one of these...ergo, only 1 fs open at once */
/* static */
devid_t		*ufs_devp;

struct dirinfo {
	int 	loc;
	fileid_t *fi;
};

/*
 *  Function prototypes
 */
static int	boot_ufs_mountroot(char *str);
static int	boot_ufs_unmountroot(void);
static int	boot_ufs_open(char *filename, int flags);
static int	boot_ufs_close(int fd);
static ssize_t	boot_ufs_read(int fd, caddr_t buf, size_t size);
static off_t	boot_ufs_lseek(int, off_t, int);
static int	boot_ufs_fstat(int fd, struct stat *stp);
static void	boot_ufs_closeall(int flag);
static int	boot_ufs_getdents(int fd, struct dirent *dep, unsigned size);

struct boot_fs_ops boot_ufs_ops = {
	"ufs",
	boot_ufs_mountroot,
	boot_ufs_unmountroot,
	boot_ufs_open,
	boot_ufs_close,
	boot_ufs_read,
	boot_ufs_lseek,
	boot_ufs_fstat,
	boot_ufs_closeall,
	boot_ufs_getdents
};

static 	ino_t	find(fileid_t *filep, char *path);
static	ino_t	dlook(fileid_t *filep, char *path);
static 	daddr32_t	sbmap(fileid_t *filep, daddr32_t bn);
static  struct direct *readdir(struct dirinfo *dstuff);
extern	int	diskread(fileid_t *filep);


/* These are the pools of buffers, etc. */
#define	NBUFS	(NIADDR+1)
/* Compilers like to play with alignment, so force the issue here */
static union {
	char		*blk[NBUFS];
	daddr32_t		*dummy;
} b;
daddr32_t		blknos[NBUFS];

/*
 *	There is only 1 open (mounted) device at any given time.
 *	So we can keep a single, global devp file descriptor to
 *	use to index into the di[] array.  This is not true for the
 *	fi[] array.  We can have more than one file open at once,
 *	so there is no global fd for the fi[].
 *	The user program must save the fd passed back from open()
 *	and use it to do subsequent read()'s.
 */

static int
openi(fileid_t *filep, ino_t inode)
{
	int retval;
	register struct dinode *dp;
	register devid_t *devp = filep->fi_devp;

	/* Try the inode cache first */
	if ((filep->fi_inode = get_icache(devp->di_dcookie, inode)) != NULL)
		return (0);
	/* Nope, not there so lets read it off the disk. */
	filep->fi_offset = 0;
	filep->fi_blocknum = fsbtodb(&devp->un_fs.di_fs,
				itod(&devp->un_fs.di_fs, inode));

	/* never more than 1 disk block */
	filep->fi_count = devp->un_fs.di_fs.fs_bsize;
	filep->fi_memp = filep->fi_buf;

	/* Maybe the block is in the disk block cache */
	if ((filep->fi_memp = get_bcache(filep)) == NULL)
		/* Not in the block cache so read it from disk */
		if (retval = set_bcache(filep))
			return (retval);

	dp = (struct dinode *)filep->fi_memp;
	filep->fi_inode = (struct inode *)
	    bkmem_alloc(sizeof (struct inode));
	bzero((char *)filep->fi_inode, sizeof (struct inode));
	filep->fi_inode->i_ic =
	    dp[itoo(&devp->un_fs.di_fs, inode)].di_un.di_icom;
	filep->fi_inode->i_number = inode;
	set_icache(devp->di_dcookie, inode, (void *)filep->fi_inode,
	    sizeof (struct inode));
	return (0);
}

static fileid_t *
find_fp(int fd)
{
	fileid_t *filep = head;

	if (fd >= 0) {
		while ((filep = filep->fi_forw) != head)
			if (fd == filep->fi_filedes)
				return (filep->fi_taken ? filep : 0);
	}

	return (0);
}

static ino_t
find(fileid_t *filep, char *path)
{
	register char *q;
	char c;
	ino_t inode;
	char lpath[256];
	char *lpathp = lpath;
	int len, r;
	devid_t	*devp;

	if (path == NULL || *path == '\0') {
		printf("null path\n");
		return ((ino_t)0);
	}

	bzero(lpath, sizeof (lpath));
	bcopy(path, lpath, strlen(path));
	devp = filep->fi_devp;
	while (*lpathp) {
		/* if at the beginning of pathname get root inode */
		r = (lpathp == lpath);
		if (r && openi(filep, (ino_t)UFSROOTINO))
			return ((ino_t)0);
		while (*lpathp == '/')
			lpathp++;	/* skip leading slashes */
		q = lpathp;
		while (*q != '/' && *q != '\0')
			q++;		/* find end of component */
		c = *q;
		*q = '\0';		/* terminate component */

		/* Bail out early if opening root */
		if (r && (*lpathp == '\0'))
			return ((ino_t)UFSROOTINO);
		if ((inode = dlook(filep, lpathp)) != 0) {
			if (openi(filep, inode))
				return ((ino_t)0);
			if ((filep->fi_inode->i_smode & IFMT) == IFLNK) {
				filep->fi_blocknum =
				    fsbtodb(&devp->un_fs.di_fs,
				    filep->fi_inode->i_db[0]);
				filep->fi_count = DEV_BSIZE;
				/* check the block cache */
				if ((filep->fi_memp = get_bcache(filep)) == 0)
					if (set_bcache(filep))
						return ((ino_t)0);
				len = strlen(filep->fi_memp);
				if (filep->fi_memp[0] == '/')
					/* absolute link */
					lpathp = lpath;
				/* copy rest of unprocessed path up */
				bcopy(q, lpathp + len, strlen(q + 1) + 2);
				/* point to unprocessed path */
				*(lpathp + len) = c;
				/* prepend link in before unprocessed path */
				bcopy(filep->fi_memp, lpathp, len);
				lpathp = lpath;
				continue;
			} else
				*q = c;
			if (c == '\0')
				break;
			lpathp = q;
			continue;
		} else {
			return ((ino_t)0);
		}
	}
	return (inode);
}

static daddr32_t
sbmap(fileid_t *filep, daddr32_t bn)
{
	struct inode *inodep;
	int i, j, sh;
	daddr32_t nb, *bap;
	daddr32_t *db;
	devid_t	*devp;

	devp = filep->fi_devp;
	inodep = filep->fi_inode;
	db = inodep->i_db;

	/*
	 * blocks 0..NDADDR are direct blocks
	 */
	if (bn < NDADDR) {
		nb = db[bn];
		return (nb);
	}

	/*
	 * addresses NIADDR have single and double indirect blocks.
	 * the first step is to determine how many levels of indirection.
	 */
	sh = 1;
	bn -= NDADDR;
	for (j = NIADDR; j > 0; j--) {
		sh *= NINDIR(&devp->un_fs.di_fs);
		if (bn < sh)
			break;
		bn -= sh;
	}
	if (j == 0) {
		return ((daddr32_t)0);
	}

	/*
	 * fetch the first indirect block address from the inode
	 */
	nb = inodep->i_ib[NIADDR - j];
	if (nb == 0) {
		return ((daddr32_t)0);
	}

	/*
	 * fetch through the indirect blocks
	 */
	for (; j <= NIADDR; j++) {
		if (blknos[j] != nb) {
			filep->fi_blocknum = fsbtodb(&devp->un_fs.di_fs, nb);
			filep->fi_count = devp->un_fs.di_fs.fs_bsize;
			/* First look through the disk block cache */
			if ((filep->fi_memp = get_bcache(filep)) == 0) {
				if (set_bcache(filep)) /* Gotta do I/O */
					return (0);
			}
			b.blk[j] = filep->fi_memp;
			blknos[j] = nb;
		}
		bap = (daddr32_t *)b.blk[j];
		sh /= NINDIR(&devp->un_fs.di_fs);
		i = (bn / sh) % NINDIR(&devp->un_fs.di_fs);
		nb = bap[i];
		if (nb == 0) {
			return ((daddr32_t)0);
		}
	}
	return (nb);
}

static ino_t
dlook(fileid_t *filep, char *path)
{
	devid_t *devp = filep->fi_devp;
	register struct direct *dp;
	register struct inode *ip;
	struct dirinfo dirp;
	register int len;
	ino_t in;

	ip = filep->fi_inode;
	if (path == NULL || *path == '\0')
		return (0);
	if ((ip->i_smode & IFMT) != IFDIR) {
		return (0);
	}
	if (ip->i_size == 0) {
		return (0);
	}
	len = strlen(path);
	/* first look through the directory entry cache */
	if ((in = get_dcache(devp->di_dcookie, path, ip->i_number)) != 0)
		return (in);
	dirp.loc = 0;
	dirp.fi = filep;
	for (dp = readdir(&dirp); dp != NULL; dp = readdir(&dirp)) {
		if (dp->d_ino == 0)
			continue;
		if (dp->d_namlen == len && strcmp(path, dp->d_name) == 0) {
			/* put this entry into the cache */
			set_dcache(devp->di_dcookie, path,
			    filep->fi_inode->i_number, dp->d_ino);
			return (dp->d_ino);
		}
		/* Allow "*" to print all names at that level, w/out match */
		if (strcmp(path, "*") == 0)
			printf("%s\n", dp->d_name);
	}
	return (0);
}

/*
 * get next entry in a directory.
 */
struct direct *
readdir(struct dirinfo *dstuff)
{
	register struct direct *dp;
	register fileid_t *filep;
	register daddr32_t lbn, d;
	register int off;
	devid_t	*devp;

	filep = dstuff->fi;
	devp = filep->fi_devp;
	for (;;) {
		if (dstuff->loc >= filep->fi_inode->i_size) {
			return (NULL);
		}
		off = blkoff(&devp->un_fs.di_fs, dstuff->loc);
		if (off == 0) {
			lbn = lblkno(&devp->un_fs.di_fs, dstuff->loc);
			d = sbmap(filep, lbn);

			if (d == 0)
				return (NULL);

			filep->fi_blocknum = fsbtodb(&devp->un_fs.di_fs, d);
			filep->fi_count =
			    blksize(&devp->un_fs.di_fs, filep->fi_inode, lbn);
			/* check the block cache */
			if ((filep->fi_memp = get_bcache(filep)) == 0)
				if (set_bcache(filep))
					return (NULL);
		}
		dp = (struct direct *)(filep->fi_memp + off);
		dstuff->loc += dp->d_reclen;
		if (dp->d_ino == 0)
			continue;
		return (dp);
	}
}

/*
 * Get the next block of data from the file.  If possible, dma right into
 * user's buffer
 */
static int
getblock(fileid_t *filep, caddr_t buf, int count, int *rcount)
{
	register struct fs *fs;
	register caddr_t p;
	register int off, size, diff;
	register daddr32_t lbn;
	devid_t	*devp;
#ifndef	i386
	static int	pos;
	static char 	ind[] = "|/-\\";	/* that's entertainment? */
	static int	blks_read;
#endif
	extern int read_opt;

	devp = filep->fi_devp;
	p = filep->fi_memp;
	if (filep->fi_count <= 0) {

		/* find the amt left to be read in the file */
		diff = filep->fi_inode->i_size - filep->fi_offset;
		if (diff <= 0) {
			printf("Short read\n");
			return (-1);
		}

		fs = &devp->un_fs.di_fs;
		/* which block (or frag) in the file do we read? */
		lbn = lblkno(fs, filep->fi_offset);

		/* which physical block on the device do we read? */
		filep->fi_blocknum = fsbtodb(fs, sbmap(filep, lbn));

		off = blkoff(fs, filep->fi_offset);

		/* either blksize or fragsize */
		size = blksize(fs, filep->fi_inode, lbn);
		filep->fi_count = size;
		filep->fi_memp = filep->fi_buf;

		/*
		 * optimization if we are reading large blocks of data then
		 * we can go directly to user's buffer
		 */
		*rcount = 0;
		if (off == 0 && count >= size) {
			filep->fi_memp = buf;
			if (diskread(filep)) {
				return (-1);
			}
			*rcount = size;
			filep->fi_count = 0;
			read_opt++;
#ifndef	i386
			if ((blks_read++ & 0x3) == 0)
				printf("%c\b", ind[pos++ & 3]);
#endif
			return (0);
		} else
			if (diskread(filep))
				return (-1);

		/*
		 * round and round she goes (though not on every block..
		 * - OBP's take a fair bit of time to actually print stuff)
		 * On x86, the screen oriented bootconf program doesn't
		 * want this noise...
		 */
#ifndef	i386
		if ((blks_read++ & 0x3) == 0)
			printf("%c\b", ind[pos++ & 3]);
#endif

		if (filep->fi_offset - off + size >= filep->fi_inode->i_size)
			filep->fi_count = diff + off;
		filep->fi_count -= off;
		p = &filep->fi_memp[off];
	}
	filep->fi_memp = p;
	return (0);
}


/*
 *  This is the high-level read function.  It works like this.
 *  We assume that our IO device buffers up some amount of
 *  data ant that we can get a ptr to it.  Thus we need
 *  to actually call the device func about filesize/blocksize times
 *  and this greatly increases our IO speed.  When we already
 *  have data in the buffer, we just return that data (with bcopy() ).
 */

static ssize_t
boot_ufs_read(int fd, caddr_t buf, size_t count)
{
	register int i, j;
	caddr_t	n;
	int rcount;
	fileid_t *filep;

	if (!(filep = find_fp(fd))) {
		return (-1);
	}

	if (filep->fi_offset + count > filep->fi_inode->i_size)
		count = filep->fi_inode->i_size - filep->fi_offset;

	/* that was easy */
	if ((i = count) <= 0)
		return (0);

	n = buf;
	while (i > 0) {
		/* If we need to reload the buffer, do so */
		if ((j = filep->fi_count) <= 0) {
			getblock(filep, buf, i, &rcount);
			i -= rcount;
			buf += rcount;
			filep->fi_offset += rcount;
		} else {
			/* else just bcopy from our buffer */
			j = MIN(i, j);
			bcopy(filep->fi_memp, buf, (unsigned)j);
			buf += j;
			filep->fi_memp += j;
			filep->fi_offset += j;
			filep->fi_count -= j;
			i -= j;
		}
	}
	return (buf - n);
}

/*
 *	This routine will open a device as it is known by the V2 OBP.
 *	Interface Defn:
 *	err = boot_ufs_mountroot(string);
 *		err = 0 on success
 *		err = -1 on failure
 *	string:	char string describing the properties of the device.
 *	We must not dork with any fi[]'s here.  Save that for later.
 */

static int
boot_ufs_mountroot(char *str)
{
	int	h;

	/*
	 * Open the device and setup the read of the ufs superblock
	 * only the first time mountroot is called.  Subsequent calls
	 * to mountroot succeed immediatly
	 */
	if (ufs_devp == NULL) {

		/*
		 * Encode the knowledge that we normally boot from the 'a'
		 * slice of the leaf device on the OBP path. Since v2path
		 * points to 'str' as well, changing str should have the
		 * desired result.
		 */
#ifdef sparc
		if (prom_getversion() > 0) {
#endif
			if (str[strlen(str) - 2] != ':')
				(void) strcat(str, ":a");
#ifdef sparc
		}
#endif
		h = prom_open(str);
		if (h == 0) {
			printf("Cannot open %s\n", str);
			return (-1);
		}

		ufs_devp = (devid_t *)bkmem_alloc(sizeof (devid_t));
		ufs_devp->di_taken = 1;
		ufs_devp->di_dcookie = h;
		ufs_devp->di_desc = (char *)bkmem_alloc(strlen(str) + 1);
		(void) strcpy(ufs_devp->di_desc, str);
		bzero(ufs_devp->un_fs.dummy, SBSIZE);
		head = (fileid_t *)bkmem_alloc(sizeof (fileid_t));
		head->fi_back = head->fi_forw = head;
		head->fi_filedes = 0;
		head->fi_taken = 0;
	}
	return (0);
}

/*
 * Unmount the currently mounted root fs.  In practice, this means
 * closing all open files and releasing resources.  All of this
 * is done by boot_ufs_closeall().
 */

int
boot_ufs_unmountroot(void)
{
	if (ufs_devp == NULL)
		return (-1);

	boot_ufs_closeall(1);

	return (0);
}

/*
 *	We allocate an fd here for use when talking
 *	to the file itself.
 */

static int	ufs_dev_open = 0;

/*ARGSUSED*/
static int
boot_ufs_open(char *filename, int flags)
{
	fileid_t	*filep;
	ino_t	inode;
	static int	filedes = 1;

	/* build and link a new file descriptor */
	filep = (fileid_t *)bkmem_alloc(sizeof (fileid_t));
	filep->fi_back = head->fi_back;
	filep->fi_forw = head;
	head->fi_back->fi_forw = filep;
	head->fi_back = filep;
	filep->fi_filedes = filedes++;
	filep->fi_taken = 1;
	filep->fi_path = (char *)bkmem_alloc(strlen(filename) + 1);
	(void) strcpy(filep->fi_path, filename);
	filep->fi_devp = ufs_devp; /* dev is already "mounted" */
	filep->fi_inode = NULL;
	bzero(filep->fi_buf, MAXBSIZE);

	/* No need to re-read SB on every file open */
	if (!ufs_dev_open) {
		/* setup read of the superblock */
		filep->fi_blocknum = SBLOCK;
		filep->fi_count = (u_int)SBSIZE;
		filep->fi_memp = (caddr_t)&(ufs_devp->un_fs.di_fs);
		filep->fi_offset = 0;

		if (diskread(filep)) {
			boot_ufs_close(filep->fi_filedes);
			return (-1);
		}

		if (ufs_devp->un_fs.di_fs.fs_magic != FS_MAGIC)  {
			boot_ufs_close(filep->fi_filedes);
			return (-1);
		}
		ufs_dev_open = 1;
	}

	inode = find(filep, filename);
	if (inode == (ino_t)0) {
		boot_ufs_close(filep->fi_filedes);
		return (-1);
	}
	if (openi(filep, inode)) {
		boot_ufs_close(filep->fi_filedes);
		return (-1);
	}

	filep->fi_offset = filep->fi_count = 0;

	return (filep->fi_filedes);
}

/*
 *  We don't do any IO here.
 *  We just play games with the device pointers.
 */

static off_t
boot_ufs_lseek(int fd, off_t addr, int whence)
{
	fileid_t *filep;

	/* Make sure user knows what file he is talking to */
	if (!(filep = find_fp(fd)))
		return (-1);

	switch (whence) {
	case SEEK_CUR:
		filep->fi_offset += addr;
		break;
	case SEEK_SET:
		filep->fi_offset = addr;
		break;
	default:
	case SEEK_END:
		printf("ufs_lseek(): invalid whence value %d\n", whence);
		break;
	}

	filep->fi_blocknum = addr / DEV_BSIZE;
	filep->fi_count = 0;

	return (0);
}

/*
 * ufs_fstat() only supports size and mode at present time.
 */

static int
boot_ufs_fstat(int fd, struct stat *stp)
{
	fileid_t *filep;

	if (!(filep = find_fp(fd)))
		return (-1);

	stp->st_mode = 0;
	stp->st_size = 0;

	if (filep->fi_inode == 0)
		return (0);

	switch (filep->fi_inode->i_smode & IFMT) {
	case IFDIR:
		stp->st_mode = S_IFDIR;
		break;
	case IFLNK:
		stp->st_mode = S_IFLNK;
		break;
	case IFREG:
		stp->st_mode = S_IFREG;
		break;
	default:
		break;
	}
	stp->st_size = filep->fi_inode->i_size;

	return (0);
}

static int
boot_ufs_close(int fd)
{
	fileid_t *filep;

	/* Make sure user knows what file he is talking to */
	if (!(filep = find_fp(fd)))
		return (-1);

	if (filep->fi_taken && (filep != head)) {
		/* Clear the ranks */
		bkmem_free(filep->fi_path, strlen(filep->fi_path)+1);
		filep->fi_blocknum = filep->fi_count = filep->fi_offset = 0;
		filep->fi_memp = (caddr_t)0;
		filep->fi_devp = 0;
		filep->fi_taken = 0;

		/* unlink and deallocate node */
		filep->fi_forw->fi_back = filep->fi_back;
		filep->fi_back->fi_forw = filep->fi_forw;
		bkmem_free((char *)filep, sizeof (fileid_t));

		return (0);
	} else {
		/* Big problem */
		printf("\nFile descrip %d not allocated!", fd);
		return (-1);
	}
}

/*ARGSUSED*/
static void
boot_ufs_closeall(int flag)
{
	fileid_t *filep = head;

	while ((filep = filep->fi_forw) != head)
		if (filep->fi_taken)
			if (boot_ufs_close(filep->fi_filedes))
				prom_panic("Filesystem may be inconsistent.\n");

	release_cache(ufs_devp->di_dcookie);
	(void) prom_close(ufs_devp->di_dcookie);
	ufs_devp->di_taken = 0;
	if (verbosemode & print_cache_stats)
		print_cache_data();
	bkmem_free((char *)ufs_devp, sizeof (devid_t));
	bkmem_free((char *)head, sizeof (fileid_t));
	ufs_devp = (devid_t *)NULL;
	ufs_dev_open = 0;	/* force the superblock to be reread */
}

static int
boot_ufs_getdents(int fd, struct dirent *dep, unsigned size)
{
	/*
	 * Read directory entries from the file open on "fd" into the
	 * "size"-byte buffer at "dep" until the buffer is exhausted
	 * or we reach EOF on the directory.  Returns the number of
	 * entries read.
	 */
	int n;
	fileid_t *fp;
	unsigned long oldoff, oldblok;

#define	SLOP (sizeof (struct dirent) - (int)&((struct dirent *)0)->d_name[1])

	if (fp = find_fp(fd)) {
		/*
		 *  File is open, check type to make sure it's a directory.
		 */

		while ((fp->fi_inode->i_smode & IFMT) == IFLNK) {
			/*
			 * If file is a symbolic link, we'll follow
			 * it JIC it points to a directory!
			 */
			fileid_t fx;
			char pn[MAXPATHLEN];
			fp->fi_count = DEV_BSIZE;
			fp->fi_blocknum = fsbtodb(&fp->fi_devp->un_fs.di_fs,
			    fp->fi_inode->i_db[0]);

			if ((!(fp->fi_memp =
			    get_bcache(fp)) && set_bcache(fp)) ||
			    !(n = find(&fx, strcpy(pn, fp->fi_memp))) ||
			    openi(fp = &fx, n)) {
				/*
				 * Either (a) we got an I/O error
				 * reading the path name, (b) the path
				 * name points to a non-existant file,
				 * or (c) we got an I/O error reading
				 * the target inode!
				 */
				return (-1);
			}
		}

	    if ((fp->fi_inode->i_smode & IFMT) == IFDIR) {
			/*
			 * If target file is a directory, go ahead
			 * and read it.  This consists of making
			 * repeated calls to readdir() until we reach
			 * end-of-file or run out of buffer space.
			 */
			int cnt = 0;
			struct direct *dp;
			struct dirinfo dir;

			dir.fi = fp;
			oldblok = fp->fi_blocknum;
			dir.loc = oldoff = fp->fi_offset;

			for (dp = readdir(&dir); dp; dp = readdir(&dir)) {
				/*
				 *  Read all directory entries in the file ...
				 */

				if (dp->d_ino) {
					/*
					 *  Next entry is valid.
					 * Compute name length and
					 * break loop if there's not
					 * enough space in the output
					 * buffer for the next entry.
					 *
					 *  NOTE: "SLOP" is the number
					 * of bytes inserted into the
					 * dirent struct's "d_name"
					 * field by the compiler to
					 * preserve alignment.
					 */
					dep->d_ino = dp->d_ino;
					n = strlen(dp->d_name);
					n = roundup((sizeof (struct dirent) +
					    ((n > SLOP) ? n : 0)),
					    sizeof (off_t));

					if (n > size)
						break; /* user buffer is full */

					oldblok = fp->fi_blocknum;
					oldoff = dir.loc;
					size -= n;
					cnt += 1;

					(void) strcpy(dep->d_name, dp->d_name);
					dep->d_off = dir.loc;
					dep->d_reclen = (u_short)n;

					dep = (struct dirent *)
					    ((char *)dep + n);
				}
			}
			/*
			 * Remember where we left off for next time
			 */
			fp->fi_blocknum = oldblok;
			fp->fi_offset = oldoff;

			return (cnt);
		}
	}

#undef SLOP

	return (-1);
}
