/*
 * Copyright (c) 1996-1998 Sun Microsystems, Inc.
 * All rights reserved.
 */

#pragma ident	"@(#)msgfmt.c	1.12	98/03/25 SMI"

#include <string.h>
#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/param.h>
#include <signal.h>
#include <malloc.h>
#include <libintl.h>
#include <stdlib.h>
#include "../../lib/libc/inc/msgfmt.h"

#define	DOMAIN_TOKEN	L"domain"	/* domain token in po file  */
#define	MSGID_TOKEN	L"msgid"	/* msg id token in po file  */
#define	MSGSTR_TOKEN	L"msgstr"	/* target str token in po file */
#define	DEFAULT_DOMAIN	"messages"	/* implementation dependent */
					/* default domain name	    */

#define	MAX_VALUE_LEN		2047	/* size of msg id and target str */

struct msg_chain {
	char	*msgid;		/* msg id string */
	char	*msgstr;	/* msg target string */
	int	msgid_offset;	/* msg id offset in mo file */
	int	msgstr_offset;	/* msg target string offset in mo file */
	struct  msg_chain *next;	/* next node */
};

struct  domain_struct {
	char			*domain;	/* domain name */
	struct msg_chain	*first_elem;	/* head of msg link list */
	struct msg_chain	*current_elem;	/* most recently used msg */
	struct domain_struct	*next;		/* next domain node */
};

#ifdef __STDC__
static void	read_psffm(char *);
static void	handle_duplicate_msgid(char *);
static void	sortit(char *, char *);
static wchar_t	*consume_whitespace(wchar_t *);
static char	expand_meta(wchar_t **);
static struct domain_struct	*find_domain_node(char *);
static void	insert_message(struct domain_struct *, char *, char *);
static void	output_all_mo_files();
static void	output_one_mo_file(struct domain_struct *);
static void	check_next_char(wchar_t *, int);
#else
static void	read_psffm();
static void	handleduplicate_msgid();
static void	sortit();
static wchar_t	*consume_whitespace();
static char	expand_meta();
static struct domain_struct	*find_domain_node();
static void	insert_message();
static void	output_all_mo_files();
static void	output_one_mo_file();
static void	check_next_char();
#endif

#ifdef DEBUG
static void	printlist();
#endif

#define	USAGE	"usage: msgfmt [-v] [-o filename] [files ... ]\n"

extern	char *optarg;
extern	int optind;
extern	int opterr;
static	wchar_t	linebuf[MAX_INPUT];	/* MAX_INPUT is 512 in param.h */
static	char	gcurrent_domain[TEXTDOMAINMAX+1];
static	char	*gmsgid;		/* Stores msgid when read po file */
static	char	*gmsgstr;		/* Stores msgstr when read po file */
static	int	gmsgid_size;		/* The current size of msgid buffer */
static	int	gmsgstr_size;		/* The current size of msgstr buffer */
static	char	*outfile = NULL;
static	int	linenum;		/* The line number in the file */
static	int	msgid_linenum;		/* The last msgid token line number */
static	int	msgstr_linenum;		/* The last msgstr token line number */

static	int	oflag = 0;
static	int	verbose = 0;

static	struct domain_struct *first_domain = NULL;
static	struct domain_struct *last_used_domain = NULL;

/*
 * msgfmt - Generate binary tree for runtime gettext() using psffm: "Portable
 * Source File Format for Messages" file template. This file may have
 * previously been generated by the xgettext filter for c source files.
 */

main(argc, argv)
	int	argc;
	char	*argv[];
{
	int	errflg = 0;
	int	c;

	(void) setlocale(LC_ALL, "");

	opterr = 0;
	while ((c = getopt(argc, argv, "vo:")) != -1) {
		switch (c) {
		case 'o':
			oflag = 1;
			outfile = optarg;
			break;
		case 'v':
			verbose = 1;
			break;
		default:
			errflg = 1;
		}
	} /* while */

	if (errflg) {
		(void) fprintf(stderr, "%s\n", USAGE);
		exit(2);
	}

	/* If no .po files specified, then error */
	if (optind >= argc) {
		(void) fprintf(stderr, "%s\n", USAGE);
		exit(2);

	}

	/*
	 * read all portable object files specified in command arguments.
	 * Allocate initial size for msgid and msgstr. If it needs more
	 * spaces, realloc later.
	 */
	if (((gmsgid  = (char *) malloc(MAX_VALUE_LEN)) == NULL) ||
	((gmsgstr = (char *) malloc(MAX_VALUE_LEN)) == NULL)) {
		(void) fprintf(stderr, "failed to allocate memory");
		exit(2);
	}

	gmsgid_size = gmsgstr_size = MAX_VALUE_LEN;
	(void) memset(gmsgid, 0, gmsgid_size);
	(void) memset(gmsgstr, 0, gmsgstr_size);

	for (; optind < argc; optind++) {

		if (verbose)
			(void) fprintf(stderr, "Processing file \"%s\"...\n",
				argv[optind]);

		read_psffm(argv[optind]);

	} /* for */

	output_all_mo_files();

#ifdef DEBUG
	printlist();
#endif

	return (0);

} /* main */



/*
 * read_psffm - read in "psffm" format file, check syntax, printing error
 * messages as needed, output binary tree to file <domain>
 */

static void
read_psffm(filename)
	char	*filename;
{
	FILE		*fp;
	static char	msgfile[MAXPATHLEN];
	wchar_t		*linebufptr;
	char		*bufptr = 0;
	int		whitespace = 0;
	int		quotefound = 0;	/* double quote was seen */
	int		inmsgid = 0;	/* indicates "msgid" was seen */
	int		inmsgstr = 0;	/* indicates "msgstr" was seen */
	int		indomain = 0;	/* indicates "domain" was seen */
	wchar_t		wc;
	char		mb;
	int		n;
	char		token_found;	/* Boolean value */
	unsigned int	bufptr_index = 0; /* current index of bufptr */

	/*
	 * For each po file to be read,
	 * 1) set domain to default and
	 * 2) set linenumer to 0.
	 */
	(void) strcpy(gcurrent_domain, DEFAULT_DOMAIN);
	linenum = 0;

	if ((fp = fopen(filename, "r")) == NULL) {
		perror(filename);
		exit(2);
	}

	for (;;) {
		if (fgetws(linebuf, MAX_INPUT, fp) == NULL) {
			if (ferror(fp)) {
				perror(filename);
				exit(2);
			}
			break; /* End of File. */
		}
		linenum++;

		linebufptr = linebuf;
		whitespace = 0;

		switch (*linebufptr) {
			case L'#':	/* comment    */
			case L'\n':	/* empty line */
				continue;
			case L'\"': /* multiple lines of msgid and msgstr */
			case L' ':
			case L'\t':
				/* whitespace should be handled correctly */
				whitespace = 1;
				break;
		}

		/*
		 * Process MSGID Tokens.
		 */
		token_found = (wcsncmp(MSGID_TOKEN, linebuf,
				wcslen(MSGID_TOKEN)) == 0) ? 1 : 0;

		if (token_found || (whitespace && inmsgid)) {

			if (token_found)
				check_next_char(linebuf, wcslen(MSGID_TOKEN)
				+1);

			if (inmsgid && !whitespace) {
				(void) fprintf(stderr,
"Warning, Consecutive MSGID tokens encountered at line number: %d, ignored.\n",
					msgid_linenum);
				continue;
			}
			if (inmsgstr) {
				sortit(gmsgid, gmsgstr);
				(void) memset(gmsgid, 0, gmsgid_size);
				(void) memset(gmsgstr, 0, gmsgstr_size);
			}

			if (inmsgid) {
				/* multiple lines of msgid */
				linebufptr = consume_whitespace(linebuf);
				/* cancel the previous null termination */
				bufptr_index--;
			} else {
				/*
				 * The first line of msgid.
				 * Save linenum of msgid to be used when
				 * printing warning or error message.
				 */
				msgid_linenum = linenum;
				linebufptr = consume_whitespace
					(linebuf +
					wcslen(MSGID_TOKEN));
				bufptr = gmsgid;
				bufptr_index = 0;
			}

			inmsgid = 1;
			inmsgstr = 0;
			indomain = 0;
			goto load_buffer;
		}

		/*
		 * Process MSGSTR Tokens.
		 */
		token_found = (wcsncmp(MSGSTR_TOKEN, linebuf,
				wcslen(MSGSTR_TOKEN)) == 0) ? 1 : 0;
		if (token_found || (whitespace && inmsgstr)) {

			if (token_found)
				check_next_char(linebuf,
				wcslen(MSGSTR_TOKEN)+1);

			if (inmsgstr && !whitespace) {
				(void) fprintf(stderr,
"Warning, Consecutive MSGSTR tokens encountered at line number: %d, ignored.\n",
					msgstr_linenum);
				continue;
			}
			if (inmsgstr) {
				/* multiple lines of msgstr */
				linebufptr = consume_whitespace(linebuf);
				/* cancel the previous null termination */
				bufptr_index--;
			} else {
				/*
				 * The first line of msgstr.
				 * Save linenum of msgid to be used when
				 * printing warning or error message.
				 */
				msgstr_linenum = linenum;
				linebufptr = consume_whitespace
					(linebuf +
					wcslen(MSGSTR_TOKEN));
				bufptr = gmsgstr;
				bufptr_index = 0;
			}

			inmsgstr = 1;
			inmsgid = 0;
			indomain = 0;
			goto load_buffer;
		}

		/*
		 * Process DOMAIN Tokens.
		 * Add message id and message string to sorted list
		 * if msgstr was processed last time.
		 */
		token_found = (wcsncmp(DOMAIN_TOKEN, linebuf,
					wcslen(DOMAIN_TOKEN)) == 0) ? 1 : 0;
		if ((token_found) || (whitespace && indomain)) {
			if (token_found)
				check_next_char(linebuf, wcslen(DOMAIN_TOKEN)
				+1);

			/*
			 * process msgid and msgstr pair for previous domain
			 */
			if (inmsgstr) {
				sortit(gmsgid, gmsgstr);
			}

			/* refresh msgid and msgstr buffer */
			if (inmsgstr || inmsgid) {
				(void) memset(gmsgid, 0, gmsgid_size);
				(void) memset(gmsgstr, 0, gmsgstr_size);
			}

			if (indomain) {
				/* multiple lines of domain */
				linebufptr = consume_whitespace(linebuf);
				/* cancel the previous null termination */
				bufptr_index--;
			} else {
				linebufptr = consume_whitespace(linebuf
						+ wcslen(DOMAIN_TOKEN));
				(void) memset(gcurrent_domain, 0,
						sizeof (gcurrent_domain));
				bufptr = gcurrent_domain;
				bufptr_index = 0;
			}

			indomain = 1;
			inmsgid = 0;
			inmsgstr = 0;
		} /* if */

load_buffer:
		/*
		 * Now, fill up the buffer pointed by bufptr.
		 * At this point bufptr should point to one of
		 * msgid, msgptr, or current_domain.
		 * Otherwise, the entire line is ignored.
		 */

		if (!bufptr) {
			(void) fprintf(stderr,
		"Warning, Syntax at line number: %d, line ignored\n", linenum);
			continue;
		}

		if (*linebufptr++ != L'\"') {
			(void) fprintf(stderr,
	"Warning, Syntax at line number: %d, Missing \", ignored\n", linenum);
			--linebufptr;
		}
		quotefound = 0;

		/*
		 * If there is not enough space in the buffer,
		 * increase buffer by MAX_INPUT by remalloc.
		 */
		if (bufptr == gmsgid) {
			if (gmsgid_size < (bufptr_index + MAX_INPUT)) {
				if ((gmsgid = bufptr = (char *) realloc
				(bufptr, gmsgid_size + MAX_INPUT)) == NULL) {
					(void) fprintf(stderr,
					"failed to allocate memory");
					exit(2);
				}
				gmsgid_size += MAX_INPUT;
			}
		} else if (bufptr == gmsgstr) {
			if (gmsgstr_size < (bufptr_index + MAX_INPUT)) {
				if ((gmsgstr = bufptr = (char *) realloc
				(bufptr, gmsgstr_size + MAX_INPUT)) == NULL) {
					(void) fprintf(stderr,
					"failed to allocate memory");
					exit(2);
				}
				gmsgstr_size += MAX_INPUT;
			}
		}

		while (wc = *linebufptr++) {
			switch (wc) {
			case L'\n':
				if (!quotefound) {
					(void) fprintf(stderr,
	"Warning, Syntax at line number: %d, Missing \" at EOL, ignored\n",
						linenum);
				}
				break;

			case L'\"':
				quotefound = 1;
				break;

			case L'\\':
				if ((mb = expand_meta(&linebufptr)) != NULL)
					bufptr[bufptr_index++] = mb;
				break;

			default:
				if ((n = wctomb(&bufptr[bufptr_index], wc)) > 0)
					bufptr_index += n;
			} /* switch */
			if (quotefound) {
				/*
				 * Check if any remaining characters
				 * after closing quote.
				 */
				if ((*linebufptr != L'\n') &&
					(*(linebufptr+1) != 0)) {
				linebufptr[wcslen(linebufptr)] = 0;
					(void) fprintf(stderr,
"Warning, \"%.*S\" is ignored after closing \" at line number %d.\n",
					(int)wcslen(linebufptr)-1, linebufptr,
					linenum);
				}
				break;
			}
		} /* while */

		bufptr[bufptr_index++] = '\0';

		(void) strcpy(msgfile, gcurrent_domain);
		(void) strcat(msgfile, ".mo");
	} /* for(;;) */

	if (inmsgstr) {
		sortit(gmsgid, gmsgstr);
	}

	(void) fclose(fp);
	return;

} /* read_psffm */


/*
 * Remove leading and trailing white spaces and tabs.
 */
static wchar_t *
consume_whitespace(buf)
	wchar_t	*buf;
{
	wchar_t	*bufptr = buf;
	wchar_t	*tail;
	wchar_t	c;
	int	done = 0;

	/*
	 * Remove leading white spaces.
	 */
	while (c = *bufptr++) {
		switch (c) {
		case L' ':
		case L'\t':
			continue;
		default:
			bufptr = bufptr - 1;
			done = 1;
			break;
		}
		if (done) {
			break;
		}
	}

	/*
	 * Remove trailing white spaces between the end of message
	 * and newline.
	 */
	done = 0;
	tail = bufptr + wcslen(bufptr) - 1;
	while ((c = *tail) != NULL) {
		switch (c) {
		case L' ':
		case L'\t':
		case L'\n':
			tail--;
			continue;
		default:
			*++tail = L'\n';
			*++tail = 0;
			done = 1;
			break;
		}
		if (done) {
			break;
		}
	}

	return (bufptr);

} /* consume_white_space */


/*
 * handle escape sequences.
 */
static char
expand_meta(buf)
	wchar_t	**buf;
{
	wchar_t	wc = **buf;
	char	n;

	switch (wc) {
	case L'"':
		(*buf)++;
		return ('\"');
	case L'\\':
		(*buf)++;
		return ('\\');
	case L'b':
		(*buf)++;
		return ('\b');
	case L'f':
		(*buf)++;
		return ('\f');
	case L'n':
		(*buf)++;
		return ('\n');
	case L'r':
		(*buf)++;
		return ('\r');
	case L't':
		(*buf)++;
		return ('\t');
	case L'v':
		(*buf)++;
		return ('\v');
	case L'0':
	case L'1':
	case L'2':
	case L'3':
	case L'4':
	case L'5':
	case L'6':
	case L'7':
		/*
		 * This case handles \ddd where ddd is octal number.
		 * There could be one, two, or three octal numbers.
		 */
		(*buf)++;
		n = (char)(wc - L'0');
		wc = **buf;
		if (wc >= L'0' && wc <= L'7') {
			(*buf)++;
			n = 8*n + (char)(wc - L'0');
			wc = **buf;
			if (wc >= L'0' && wc <= L'7') {
				(*buf)++;
				n = 8*n + (char)(wc - L'0');
			}
		}
		return (n);
	default:
		return (NULL);
	}
} /* expand_meta */


/*
 * Print Warning message for duplicate message ID
 */
static void
handle_duplicate_msgid(msgid)
	char	*msgid;
{
	/*
	 * Duplicate MSG-ID warning is printed only if user
	 * specifies -v optiotn.
	 */

	if (!verbose)
		return;

	(void) fprintf(stderr,
	"Warning, Duplicate id \"%s\" at line number: %d, line ignored\n",
		msgid, msgid_linenum);
	return;

} /* handle_duplicate_msgid */


/*
 * Check if the next character is possible valid character.
 * Otherwise, it is an error and exit.
 */
static void
check_next_char(linebuf, len)
	wchar_t	*linebuf;
	int	len;
{
	if ((linebuf[len - 1] == L' ') ||
		(linebuf[len - 1] == L'\t') ||
		(linebuf[len - 1] == L'\n') ||
		(linebuf[len - 1] == 0)) {

		return;
	}

	(void) fprintf(stderr,
		"Error, No space after directive at line number %d.\n",
		linenum);
	(void) fprintf(stderr, "Exiting...\n");
	exit(2);

} /* check_next_char */

/*
 * Finds the head of the current domain linked list and
 * call insert_message() to insert msgid and msgstr pair
 * to the linked list.
 */
static void
sortit(msgid, msgstr)
	char	*msgid;
	char 	*msgstr;
{
	struct domain_struct	*dom;

#ifdef DEBUG
	(void) fprintf(stderr,
		"==> sortit(), domain=<%s> msgid=<%s> msgstr=<%s>\n",
		gcurrent_domain, msgid, msgstr);
#endif

	/*
	 * If "-o filename" is specified, then all "domain" directive
	 * are ignored and, all messages will be stored in domain
	 * whose name is filename.
	 */
	if (oflag) {
		dom = find_domain_node(outfile);
	} else {
		dom = find_domain_node(gcurrent_domain);
	}

	insert_message(dom, msgid, msgstr);
}

/*
 * This routine inserts message in the current domain message list.
 * It is inserted in ascending order.
 */
static void
insert_message(dom, msgid, msgstr)
	struct domain_struct	*dom;
	char			*msgid;
	char			*msgstr;
{
	struct msg_chain	*p1 = NULL;
	struct msg_chain	*node, *prev_node;
	int			b;

	/*
	 * Find the optimal starting search position.
	 * The starting search position is either the first node
	 * or the current_elem of domain.
	 * The current_elem is the pointer to the node which
	 * is most recently accessed in domain.
	 */
	if (dom->current_elem != NULL) {
		b = strcmp(msgid, dom->current_elem->msgid);
		if (b == 0) {
			handle_duplicate_msgid(msgid);
			return;
		} else if (b > 0) { /* to implement descending order */
			p1 = dom->first_elem;
		} else {
			p1 = dom->current_elem;
		}
	} else {
		p1 = dom->first_elem;
	}

	/*
	 * search msgid insert position in the list
	 * Search starts from the node pointed by p1.
	 */
	prev_node = NULL;
	while (p1) {
		b = strcmp(msgid, p1->msgid);
		if (b == 0) {
			handle_duplicate_msgid(msgid);
			return;
		} else if (b < 0) {  /* to implement descending order */
			/* move to the next node */
			prev_node = p1;
			p1 = p1->next;
		} else {
			/* insert a new msg node */
			if ((node = (struct msg_chain *) malloc
			(sizeof (struct msg_chain))) == NULL) {
				(void) fprintf(stderr,
					"failed to allocate memory");
				exit(2);
			}
			node->next = p1;
			if (((node->msgid  = strdup(msgid)) == NULL) ||
			((node->msgstr = strdup(msgstr)) == NULL)) {
				(void) fprintf(stderr,
					"failed to allocate memory");
				exit(2);
			}

			if (prev_node) {
				prev_node->next = node;
			} else {
				dom->first_elem = node;
			}
			dom->current_elem = node;
			return;
		}
	} /* while */

	/*
	 * msgid is smaller than any of msgid in the list or
	 * list is empty.
	 * Therefore, append it.
	 */
	if ((node = (struct msg_chain *) malloc(sizeof (struct msg_chain)))
	== NULL) {
		(void) fprintf(stderr, "failed to allocate memory");
		exit(2);
	}
	node->next = NULL;
	if (((node->msgid  = strdup(msgid)) == NULL) ||
	((node->msgstr = strdup(msgstr)) == NULL)) {
		(void) fprintf(stderr, "failed to allocate memory");
		exit(2);
	}
	if (prev_node) {
		prev_node->next = node;
	} else {
		dom->first_elem = node;
	}
	dom->current_elem = node;

	return;

} /* insert_message */


/*
 * This routine will find head of the linked list for the given
 * domain_name. This looks up cache entry first and if cache misses,
 * scans the list.
 * If not found, then create a new node.
 */
static struct domain_struct *
find_domain_node(domain_name)
	char *domain_name;
{
	struct domain_struct	*p1 = NULL;
	struct domain_struct	*node;
	struct domain_struct	*prev_node;
	int			b;


	/* for perfomance, check cache 'last_used_domain' */
	if (last_used_domain) {
		b = strcmp(domain_name, last_used_domain->domain);
		if (b == 0) {
			return (last_used_domain);
		} else if (b < 0) {
			p1 = first_domain;
		} else {
			p1 = last_used_domain;
		}
	} else {
		p1 = first_domain;
	}

	prev_node = NULL;
	while (p1) {
		b = strcmp(domain_name, p1->domain);
		if (b == 0) {
			/* node found */
			last_used_domain = p1;
			return (p1);
		} else if (b > 0) {
			/* move to the next node */
			prev_node = p1;
			p1 = p1->next;
		} else {
			/* insert a new domain node */
			if ((node = (struct domain_struct *) malloc
			(sizeof (struct domain_struct))) == NULL) {
				(void) fprintf(stderr,
					"failed to allocate memory");
				exit(2);
			}
			node->next = p1;
			if ((node->domain = strdup(domain_name)) == NULL) {
				(void) fprintf(stderr,
					"failed to allocate memory");
				exit(2);
			}
			node->first_elem = NULL;
			node->current_elem = NULL;
			if (prev_node) {
				/* insert the node in the middle */
				prev_node->next = node;
			} else {
				/* node inserted is the smallest */
				first_domain = node;
			}
			last_used_domain = node;
			return (node);
		}
	} /* while */

	/*
	 * domain_name is larger than any of domain name in the list or
	 * list is empty.
	 */
	if ((node = (struct domain_struct *)
	malloc(sizeof (struct domain_struct))) == NULL) {
		(void) fprintf(stderr, "failed to allocate memory");
		exit(2);
	}
	node->next = NULL;
	if ((node->domain = strdup(domain_name)) == NULL) {
		(void) fprintf(stderr, "failed to allocate memory");
		exit(2);
	}
	node->first_elem = NULL;
	node->current_elem = NULL;
	if (prev_node) {
		/* domain list is not empty */
		prev_node->next = node;
	} else {
		/* domain list is empty */
		first_domain = node;
	}
	last_used_domain = node;

	return (node);

} /* find_domain_node */


/*
 * binary_compute() is used for pre-computing a binary search.
 */
static int
binary_compute(i, j, more, less)
	int	i, j;
	int	more[], less[];
{
	int	k;

	if (i > j) {
		return (LEAFINDICATOR);
	}
	k = (i + j) / 2;

	less[k] = binary_compute(i, k - 1, more, less);
	more[k] = binary_compute(k + 1, j, more, less);

	return (k);

} /* binary_compute */


/*
 * Write all domain data to file.
 * Each domain will create one file.
 */
static void
output_all_mo_files()
{
	struct domain_struct 	*p;

	p = first_domain;
	while (p) {
		/*
		 * generate message object file only if there is
		 * at least one element.
		 */
		if (p->first_elem) {
			output_one_mo_file(p);
		}
		p = p->next;
	}
	return;

} /* output_all_mo_files */


/*
 * Write one domain data list to file.
 */
static void
output_one_mo_file(dom)
	struct domain_struct	*dom;
{
	FILE	*fp;
	struct msg_chain	*p;
	int	message_count;
	int	string_count_msgid = 0;
	int	string_count_msg = 0;
	int	msgid_index = 0;
	int	msgstr_index = 0;
	int	*less, *more;
	int	i;
	char	fname [TEXTDOMAINMAX+1];

	if (!dom || !dom->first_elem)
		return;

	/*
	 * If -o flag is specified, then file name is used as domain name.
	 * If not, ".mo" is appended to the domain name.
	 */
	(void) strcpy(fname, dom->domain);
	if (!oflag) {
		(void) strcat(fname, ".mo");
	}
	fp = fopen(fname, "w");
	if (fp == NULL) {
		perror("cannot open file");
		exit(2);
	}

	/* compute offsets and counts */
	message_count = 0;
	p = dom->first_elem;
	while (p) {
		p->msgid_offset = msgid_index;
		p->msgstr_offset = msgstr_index;
		msgid_index += strlen(p->msgid) + 1;
		msgstr_index += strlen(p->msgstr) + 1;
		message_count++;
		p = p->next;
	}

	/*
	 * Fill up less and more entries to be used for binary search.
	 */
	string_count_msgid = msgid_index;
	string_count_msg = msgstr_index;
	if ((less = (int *) calloc(message_count, sizeof (int))) == NULL) {
		perror("msgfmt output_one_mo_file()");
		exit(8);
	}
	if ((more = (int *) calloc(message_count, sizeof (int))) == NULL) {
		perror("msgfmt output_one_mo_file()");
		exit(8);
	}

	(void) binary_compute(0, message_count - 1, more, less);

#ifdef DEBUG
	{
	int i;
	for (i = 0; i < message_count; i++) {
		(void) fprintf(stderr, "  less[%2d]=%2d, more[%2d]=%2d\n",
				i, less[i], i, more[i]);
	}
	}
#endif

	/*
	 * write out the message object file.
	 * The middle one is the first message to check by gettext().
	 */
	i = (message_count - 1) / 2;
	(void) fwrite(&i, sizeof (int), 1, fp);
	(void) fwrite(&message_count, sizeof (int), 1, fp);
	(void) fwrite(&string_count_msgid, sizeof (int), 1, fp);
	(void) fwrite(&string_count_msg, sizeof (int), 1, fp);
	i = MSG_STRUCT_SIZE * message_count;
	(void) fwrite(&i, sizeof (int), 1, fp);

	/* march through linked list and write out all nodes. */
	i = 0;
	p = dom->first_elem;
	while (p) {	/* put out message struct */
		(void) fwrite(&less[i], sizeof (int), 1, fp);
		(void) fwrite(&more[i], sizeof (int), 1, fp);
		(void) fwrite(&p->msgid_offset, sizeof (int), 1, fp);
		(void) fwrite(&p->msgstr_offset, sizeof (int), 1, fp);
		i++;
		p = p->next;
	}

	/* put out message id strings */
	p = dom->first_elem;
	while (p) {
		(void) fwrite(p->msgid, strlen(p->msgid)+1, 1, fp);
		p = p->next;
	}

	/* put out message strings */
	p = dom->first_elem;
	while (p) {
		(void) fwrite(p->msgstr, strlen(p->msgstr)+1, 1, fp);
		p = p->next;
	}

	(void) fclose(fp);

	return;

} /* output_one_mo_file */



/*
 * This is debug function. Not compiled in the final executable.
 */
#ifdef DEBUG
static void
printlist()
{
	struct domain_struct	*p;
	struct msg_chain	*m;

	(void) fprintf(stderr, "\n=== Printing contents of all domains ===\n");
	p = first_domain;
	while (p) {
		(void) fprintf(stderr, "domain name = <%s>\n", p->domain);
		m = p->first_elem;
		while (m) {
			(void) fprintf(stderr, "   msgid=<%s>, msgstr=<%s>\n",
					m->msgid, m->msgstr);
			m = m->next;
		}
		p = p->next;
	}
} /* printlist */
#endif DEBUG
