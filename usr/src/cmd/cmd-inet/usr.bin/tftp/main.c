/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#pragma ident	"@(#)main.c	1.7	99/08/19 SMI"	/* SVr4.0 1.5	*/

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 		PROPRIETARY NOTICE (Combined)
 *
 * This source code is unpublished proprietary information
 * constituting, or derived under license from AT&T's UNIX(r) System V.
 * In addition, portions of such source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 *
 *
 *
 * 		Copyright Notice
 *
 * Notice of copyright on this source code product does not indicate
 * publication.
 *
 * 	(c) 1986,1987,1988.1989  Sun Microsystems, Inc
 * 	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
 *                 All rights reserved.
 *
 */


/*
 * TFTP User Program -- Command Interface.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <setjmp.h>
#include <ctype.h>
#include <netdb.h>
#include <fcntl.h>
#ifdef TLI
#include <tiuser.h>
#endif TLI

#ifdef SYSV
#define	rindex		strrchr
#define	index		strchr
#define	bcopy(a, b, c)	memcpy((b), (a), (c))
#define	bzero(s, n)	memset((s), 0, (n))
#endif /* SYSV */

#define	TIMEOUT		5		/* secs between rexmt's */

struct	sockaddr_in6 sin6;
int	f;
short   port;
int	trace;
int	verbose;
int	connected;
char	mode[32];
char	line[200];
int	margc;
char	*margv[20];
char	*prompt = "tftp";
jmp_buf	toplevel;
void	intr();
struct	servent *sp;
int	quit(), help(), setverbose(), settrace(), status();
int	get(), put(), setpeer(), modecmd(), setrexmt(), settimeout();
int	setbinary(), setascii();
static char	*finddelimiter();
static char 	*removebrackets();

#define	HELPINDENT (sizeof ("connect"))

/* convert aligned IPv4 address into an IPv4-mapped IPv6 address */
#define	ipv4_to_ipv6(v4, v6)	(((ulong_t *)v6)[0] = 0, \
				((ulong_t *)v6)[1] = 0, \
				((ulong_t *)v6)[2] = 0xffff, \
				((ulong_t *)v6)[3] = (v4))



struct cmd {
	char	*name;
	char	*help;
	int	(*handler)();
};

char	vhelp[] = "toggle verbose mode";
char	thelp[] = "toggle packet tracing";
char	chelp[] = "connect to remote tftp";
char	qhelp[] = "exit tftp";
char	hhelp[] = "print help information";
char	shelp[] = "send file";
char	rhelp[] = "receive file";
char	mhelp[] = "set file transfer mode";
char	sthelp[] = "show current status";
char	xhelp[] = "set per-packet retransmission timeout";
char	ihelp[] = "set total retransmission timeout";
char    ashelp[] = "set mode to netascii";
char    bnhelp[] = "set mode to octet";

/* BEGIN CSTYLED */
struct cmd cmdtab[] = {
	{ "connect",	chelp,		setpeer },
	{ "mode",       mhelp,          modecmd },
	{ "put",	shelp,		put },
	{ "get",	rhelp,		get },
	{ "quit",	qhelp,		quit },
	{ "verbose",	vhelp,		setverbose },
	{ "trace",	thelp,		settrace },
	{ "status",	sthelp,		status },
	{ "binary",     bnhelp,         setbinary },
	{ "ascii",      ashelp,         setascii },
	{ "rexmt",	xhelp,		setrexmt },
	{ "timeout",	ihelp,		settimeout },
	{ "?",		hhelp,		help },
	0
};
/* END CSTYLED */

struct	cmd *getcmd();
char	*tail();
char	*index();
char	*rindex();

main(argc, argv)
	char *argv[];
{
	struct sockaddr_in6 sin6;
	int top;

	sp = getservbyname("tftp", "udp");
	if (sp == 0) {
		fprintf(stderr, "tftp: udp/tftp: unknown service\n");
		exit(1);
	}

	port = sp->s_port;
	bzero((char *)&sin6, sizeof (sin6));
	sin6.sin6_family = AF_INET6;
#ifdef TLI
	f = t_open("/dev/udp", O_RDWR, NULL);
	if (f < 0) {
		t_error("tftp: t_open");
		exit(3);
	}
	{
		struct t_bind tb;

		bzero((char *)&tb, sizeof (struct t_bind));
		tb.qlen = 0;
		tb.addr.buf = (char *)&sin6;
		tb.addr.len = tb.addr.maxlen = sizeof (struct sockaddr_in6);

		if (t_bind(f, &tb, 0) == -1) {
			t_error("tftp: t_bind");
			exit(1);
		}
	}
#else
	f = socket(AF_INET6, SOCK_DGRAM, 0);
	if (f < 0) {
		perror("tftp: socket");
		exit(3);
	}
	if (bind(f, (struct sockaddr *)&sin6, sizeof (sin6)) < 0) {
		perror("tftp: bind");
		exit(1);
	}
#endif TLI
	strcpy(mode, "netascii");
	signal(SIGINT, (void (*)())intr);
	if (argc > 1) {
		if (setjmp(toplevel) != 0)
			exit(0);
		setpeer(argc, argv);
	}
	top = setjmp(toplevel) == 0;
	for (;;)
		command(top);
}

char    hostname[100];

setpeer(argc, argv)
	int argc;
	char *argv[];
{
	struct hostent *host;
	int error_num;
	struct in6_addr ipv6addr;
	struct in_addr ipv4addr;
	char *hostnameinput;

	if (argc < 2) {
		strcpy(line, "Connect ");
		printf("(to) ");
		gets(&line[strlen(line)]);
		makeargv();
		argc = margc;
		argv = margv;
	}
	if (argc > 3) {
		printf("usage: %s host-name [port]\n", argv[0]);
		return;
	}
	hostnameinput = removebrackets(argv[1]);

	bzero(&sin6, sizeof (sin6));
	sin6.sin6_family = AF_INET6;
	if (host = getipnodebyname(hostnameinput, AF_INET6,
	    AI_ALL | AI_ADDRCONFIG | AI_V4MAPPED, &error_num)) {
		memcpy((caddr_t)&sin6.sin6_addr,
			host->h_addr_list[0], host->h_length);
		/*
		 * If host->h_name is a IPv4-mapped IPv6 literal, we'll convert
		 * it to IPv4 literal address.
		 */
		if ((inet_pton(AF_INET6, host->h_name, &ipv6addr) > 0) &&
		    IN6_IS_ADDR_V4MAPPED(&ipv6addr)) {
			IN6_V4MAPPED_TO_INADDR(&ipv6addr, &ipv4addr);
			inet_ntop(AF_INET, &ipv4addr, hostname,
			    sizeof (hostname));
		} else {
			strcpy(hostname, host->h_name);
		}
		freehostent(host);
	} else {
		/* Keeping with previous semantics */
		connected = 0;
		if (error_num == TRY_AGAIN) {
			printf("%s: unknown host (try again later)\n",
			    hostnameinput);
		} else {
			printf("%s: unknown host\n", hostnameinput);
		}
		return;
	}

	port = sp->s_port;
	if (argc == 3) {
		port = atoi(argv[2]);
		if (port < 0) {
			printf("%s: bad port number\n", argv[2]);
			connected = 0;
			return;
		}
		port = htons(port);
	}
	connected = 1;
}

struct	modes {
	char *m_name;
	char *m_mode;
} modes[] = {
	{ "ascii",	"netascii" },
	{ "netascii",   "netascii" },
	{ "binary",     "octet" },
	{ "image",      "octet" },
	{ "octet",     "octet" },
/*      { "mail",       "mail" },       */
	{ 0,		0 }
};

modecmd(argc, argv)
	char *argv[];
{
	register struct modes *p;
	char *sep;

	if (argc < 2) {
		printf("Using %s mode to transfer files.\n", mode);
		return;
	}
	if (argc == 2) {
		for (p = modes; p->m_name; p++)
			if (strcmp(argv[1], p->m_name) == 0)
				break;
		if (p->m_name) {
			setmode(p->m_mode);
			return;
		}
		printf("%s: unknown mode\n", argv[1]);
		/* drop through and print usage message */
	}

	printf("usage: %s [", argv[0]);
	sep = " ";
	for (p = modes; p->m_name; p++) {
		printf("%s%s", sep, p->m_name);
		if (*sep == ' ')
			sep = " | ";
	}
	printf(" ]\n");
}

setbinary(argc, argv)
char *argv[];
{
	setmode("octet");
}

setascii(argc, argv)
char *argv[];
{
	setmode("netascii");
}

setmode(newmode)
char *newmode;
{
	strcpy(mode, newmode);
	if (verbose)
		printf("mode set to %s\n", mode);
}


/*
 * Send file(s).
 */
put(argc, argv)
	char *argv[];
{
	int fd;
	register int n;
	register char *cp, *targ;
	struct in6_addr	ipv6addr;
	struct in_addr ipv4addr;

	if (argc < 2) {
		strcpy(line, "send ");
		printf("(file) ");
		gets(&line[strlen(line)]);
		makeargv();
		argc = margc;
		argv = margv;
	}
	if (argc < 2) {
		putusage(argv[0]);
		return;
	}
	targ = argv[argc - 1];
	if (finddelimiter(argv[argc - 1])) {
		char *cp;
		struct hostent *hp;
		int error_num;

		for (n = 1; n < argc - 1; n++)
			if (finddelimiter(argv[n])) {
				putusage(argv[0]);
				return;
			}
		cp = argv[argc - 1];
		targ = finddelimiter(cp);
		*targ++ = 0;
		cp = removebrackets(cp);

		if ((hp = getipnodebyname(cp, AF_INET6, AI_ALL | AI_ADDRCONFIG |
		    AI_V4MAPPED, &error_num)) == NULL) {
			if (error_num == TRY_AGAIN) {
				printf("%s: Unknown host (try again later).\n",
				    cp);
			} else {
				printf("%s: Unknown host.\n", cp);
			}
			return;
		}
		memcpy((caddr_t)&sin6.sin6_addr, hp->h_addr_list[0],
		    hp->h_length);

		sin6.sin6_family = AF_INET6;
		connected = 1;
		/*
		 * If hp->h_name is a IPv4-mapped IPv6 literal, we'll convert
		 * it to IPv4 literal address.
		 */
		if ((inet_pton(AF_INET6, hp->h_name, &ipv6addr) > 0) &&
		    IN6_IS_ADDR_V4MAPPED(&ipv6addr)) {
			IN6_V4MAPPED_TO_INADDR(&ipv6addr, &ipv4addr);
			inet_ntop(AF_INET, &ipv4addr, hostname,
			    sizeof (hostname));
		} else {
			strcpy(hostname, hp->h_name);
		}
	}
	if (!connected) {
		printf("No target machine specified.\n");
		return;
	}
	if (argc < 4) {
		cp = argc == 2 ? tail(targ) : argv[1];
		fd = open(cp, O_RDONLY);
		if (fd < 0) {
			fprintf(stderr, "tftp: "); perror(cp);
			return;
		}
		if (verbose)
			printf("putting %s to %s:%s [%s]\n",
				cp, hostname, targ, mode);
		sin6.sin6_port = port;
		sendfile(fd, targ, mode);
		return;
	}
	/* this assumes the target is a directory */
	/* on a remote unix system.  hmmmm.  */
	cp = index(targ, '\0');
	*cp++ = '/';
	for (n = 1; n < argc - 1; n++) {
		strcpy(cp, tail(argv[n]));
		fd = open(argv[n], O_RDONLY);
		if (fd < 0) {
			fprintf(stderr, "tftp: "); perror(argv[n]);
			continue;
		}
		if (verbose)
			printf("putting %s to %s:%s [%s]\n",
				argv[n], hostname, targ, mode);
		sin6.sin6_port = port;
		sendfile(fd, targ, mode);
	}
}

putusage(s)
	char *s;
{
	printf("usage: %s file ... host:target, or\n", s);
	printf("       %s file ... target (when already connected)\n", s);
}

/*
 * Receive file(s).
 */
get(argc, argv)
	char *argv[];
{
	int fd;
	register int n;
	register char *cp;
	char *src;
	struct in6_addr ipv6addr;
	struct in_addr ipv4addr;
	int error_num;

	if (argc < 2) {
		strcpy(line, "get ");
		printf("(files) ");
		gets(&line[strlen(line)]);
		makeargv();
		argc = margc;
		argv = margv;
	}
	if (argc < 2) {
		getusage(argv[0]);
		return;
	}
	if (!connected) {
		for (n = 1; n < argc; n++)
			if (finddelimiter(argv[n]) == 0) {
				getusage(argv[0]);
				return;
			}
	}
	for (n = 1; n < argc; n++) {
		src = finddelimiter(argv[n]);
		if (src == NULL)
			src = argv[n];
		else {
			struct hostent *hp;
			char *hostnameinput;

			*src++ = 0;
			hostnameinput = removebrackets(argv[n]);

			if ((hp = getipnodebyname(hostnameinput, AF_INET6,
			    AI_ALL | AI_ADDRCONFIG | AI_V4MAPPED,
			    &error_num)) == NULL) {
				if (error_num == TRY_AGAIN) {
					printf("%s: Unknown host (try again "
					    "later).\n", hostnameinput);
				} else {
					printf("%s: Unknown host.\n",
					    hostnameinput);
				}
				continue;
			}
			memcpy((caddr_t)&sin6.sin6_addr, hp->h_addr_list[0],
			hp->h_length);

			sin6.sin6_family = AF_INET6;
			connected = 1;
			/*
			 * If hp->h_name is a IPv4-mapped IPv6 literal, we'll
			 * convert it to IPv4 literal address.
			 */
			if ((inet_pton(AF_INET6, hp->h_name, &ipv6addr) > 0) &&
			    IN6_IS_ADDR_V4MAPPED(&ipv6addr)) {
				IN6_V4MAPPED_TO_INADDR(&ipv6addr, &ipv4addr);
				inet_ntop(AF_INET, &ipv4addr, hostname,
				    sizeof (hostname));
			} else {
				strcpy(hostname, hp->h_name);
			}
		}
		if (argc < 4) {
			cp = argc == 3 ? argv[2] : tail(src);
			fd = creat(cp, 0644);
			if (fd < 0) {
				fprintf(stderr, "tftp: "); perror(cp);
				return;
			}
			if (verbose)
				printf("getting from %s:%s to %s [%s]\n",
					hostname, src, cp, mode);
			sin6.sin6_port = port;
			recvfile(fd, src, mode);
			break;
		}
		cp = tail(src);	/* new .. jdg */
		fd = creat(cp, 0644);
		if (fd < 0) {
			fprintf(stderr, "tftp: "); perror(cp);
			continue;
		}
		if (verbose)
			printf("getting from %s:%s to %s [%s]\n",
				hostname, src, cp, mode);
		sin6.sin6_port = port;
		recvfile(fd, src, mode);
	}
}

getusage(s)
char *s;
{
	printf("usage: %s host:file host:file ... file, or\n", s);
	printf("       %s file file ... file if connected\n", s);
}

int	rexmtval = TIMEOUT;

setrexmt(argc, argv)
	char *argv[];
{
	int t;

	if (argc < 2) {
		strcpy(line, "Rexmt-timeout ");
		printf("(value) ");
		gets(&line[strlen(line)]);
		makeargv();
		argc = margc;
		argv = margv;
	}
	if (argc != 2) {
		printf("usage: %s value\n", argv[0]);
		return;
	}
	t = atoi(argv[1]);
	if (t < 0)
		printf("%s: bad value\n", argv[1]);
	else
		rexmtval = t;
}

int	maxtimeout = 5 * TIMEOUT;

settimeout(argc, argv)
	char *argv[];
{
	int t;

	if (argc < 2) {
		strcpy(line, "Maximum-timeout ");
		printf("(value) ");
		gets(&line[strlen(line)]);
		makeargv();
		argc = margc;
		argv = margv;
	}
	if (argc != 2) {
		printf("usage: %s value\n", argv[0]);
		return;
	}
	t = atoi(argv[1]);
	if (t < 0)
		printf("%s: bad value\n", argv[1]);
	else
		maxtimeout = t;
}

status(argc, argv)
	char *argv[];
{
	if (connected)
		printf("Connected to %s.\n", hostname);
	else
		printf("Not connected.\n");
	printf("Mode: %s Verbose: %s Tracing: %s\n", mode,
		verbose ? "on" : "off", trace ? "on" : "off");
	printf("Rexmt-interval: %d seconds, Max-timeout: %d seconds\n",
		rexmtval, maxtimeout);
}

void
intr()
{
	signal(SIGALRM, SIG_IGN);
	alarm(0);
	longjmp(toplevel, -1);
}

char *
tail(filename)
	char *filename;
{
	register char *s;

	while (*filename) {
		s = rindex(filename, '/');
		if (s == NULL)
			break;
		if (s[1])
			return (s + 1);
		*s = '\0';
	}
	return (filename);
}

/*
 * Command parser.
 */
command(top)
	int top;
{
	register struct cmd *c;

	if (!top)
		putchar('\n');
	for (;;) {
		printf("%s> ", prompt);
		if (gets(line) == 0) {
			if (feof(stdin)) {
				quit();
			} else {
				continue;
			}
		}
		if (line[0] == 0)
			continue;
		makeargv();
		c = getcmd(margv[0]);
		if (c == (struct cmd *)-1) {
			printf("?Ambiguous command\n");
			continue;
		}
		if (c == 0) {
			printf("?Invalid command\n");
			continue;
		}
		(*c->handler)(margc, margv);
	}
}

struct cmd *
getcmd(name)
	register char *name;
{
	register char *p, *q;
	register struct cmd *c, *found;
	register int nmatches, longest;

	longest = 0;
	nmatches = 0;
	found = 0;
	if (name == (char *)0)
	    return (found);
	for (c = cmdtab; p = c->name; c++) {
		for (q = name; *q == *p++; q++)
			if (*q == 0)		/* exact match? */
				return (c);
		if (!*q) {			/* the name was a prefix */
			if (q - name > longest) {
				longest = q - name;
				nmatches = 1;
				found = c;
			} else if (q - name == longest)
				nmatches++;
		}
	}
	if (nmatches > 1)
		return ((struct cmd *)-1);
	return (found);
}

/*
 * Given a string, this function returns the pointer to the delimiting ':'.
 * The string can contain an IPv6 literal address, which should be inside a
 * pair of brackets, e.g. [1::2]. Any colons inside a pair of brackets are not
 * accepted as delimiters. Returns NULL if delimiting ':' is not found.
 */
char *
finddelimiter(str)
	char *str;
{
	boolean_t is_bracket_open = B_FALSE;
	char *cp;

	for (cp = str; *cp != '\0'; cp++) {
		if (*cp == '[')
			is_bracket_open = B_TRUE;
		else if (*cp == ']')
			is_bracket_open = B_FALSE;
		else if (*cp == ':' && !is_bracket_open)
			return (cp);
	}
	return (NULL);
}

/*
 * Given a string which is possibly surrounded by brackets, e.g. [1::2], this
 * function returns a string after removing those brackets. If the brackets
 * don't match, it does nothing.
 */
char *
removebrackets(str)
	char *str;
{
	char *newstr = str;

	if ((str[0] == '[') && (str[strlen(str) - 1] == ']')) {
		newstr = str + 1;
		str[strlen(str) - 1] = '\0';
	}
	return (newstr);
}

/*
 * Slice a string up into argc/argv.
 */
makeargv()
{
	register char *cp;
	register char **argp = margv;

	margc = 0;
	for (cp = line; *cp; ) {
		while (isspace(*cp))
			cp++;
		if (*cp == '\0')
			break;
		*argp++ = cp;
		margc += 1;
		while (*cp != '\0' && !isspace(*cp))
			cp++;
		if (*cp == '\0')
			break;
		*cp++ = '\0';
	}
	*argp++ = 0;
}

/*VARARGS*/
quit()
{
	exit(0);
}

/*
 * Help command.
 */
help(argc, argv)
	int argc;
	char *argv[];
{
	register struct cmd *c;

	if (argc == 1) {
		printf("Commands may be abbreviated.  Commands are:\n\n");
		for (c = cmdtab; c->name; c++)
			printf("%-*s\t%s\n", HELPINDENT, c->name, c->help);
		return;
	}
	while (--argc > 0) {
		register char *arg;
		arg = *++argv;
		c = getcmd(arg);
		if (c == (struct cmd *)-1)
			printf("?Ambiguous help command %s\n", arg);
		else if (c == (struct cmd *)0)
			printf("?Invalid help command %s\n", arg);
		else
			printf("%s\n", c->help);
	}
}

/*VARARGS*/
settrace()
{
	trace = !trace;
	printf("Packet tracing %s.\n", trace ? "on" : "off");
}

/*VARARGS*/
setverbose()
{
	verbose = !verbose;
	printf("Verbose mode %s.\n", verbose ? "on" : "off");
}
