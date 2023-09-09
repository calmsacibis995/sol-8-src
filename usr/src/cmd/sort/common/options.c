/*
 * Copyright (c) 1998-1999 by Sun Microsystems, Inc.
 * All rights reserved.
 */

#pragma ident	"@(#)options.c	1.6	99/11/24 SMI"

#include "options.h"

/*
 * While a key specifier in the obsolete +m ... -n form is being defined (that
 * is, before the closing -n is seen), a narrower set of options is permitted.
 * We specify this smaller set of options in OLD_SPEC_OPTIONS_STRING.
 */
#define	OPTIONS_STRING		"cmuo:T:z:dfiMnrbt:k:S:0123456789"
#define	OLD_SPEC_OPTIONS_STRING	"bdfiMnrcmuoTztkS"

#define	OPTIONS_OLDSPEC		0x1	/* else new-style spec */
#define	OPTIONS_STARTSPEC	0x2	/* else end spec */

static int
is_number(char *C)
{
	size_t	i;

	for (i = 0; i < strlen(C); i++)
		if (!isdigit(C[i]))
			return (0);

	return (1);
}

/*
 * If a field specified by the -k option or by the +n syntax contains any
 * modifiers, then the current global field modifiers are not inherited.
 */
static int
field_spec_has_modifiers(char *C, int length)
{
	int p_nonmodifiers = strspn(C, ",.1234567890");

	if (p_nonmodifiers == length)
		return (0);

	return (1);
}

static void
field_apply_all(field_t *fc, flag_t flags)
{
	field_t *f;

	for (f = fc; f; f = f->f_next)
		if ((f->f_options & FIELD_MODIFIERS_DEFINED) == 0)
			f->f_options |= flags;
}

/*
 * One of sort(1)'s standard peculiarities is the calculation of the character
 * offsets for the old style field specifications.  We simply quote from the
 * Single Unix standard:
 *
 *	+w.xT -y.zU
 *
 * is equivalent to
 *
 * 	undefined		when z == 0, U contains b, and -t is set
 * 	-k w+1.x+1T,y.0U	when z == 0 otherwise
 * 	-k w+1.x+1T,y+1.zU	when z > 0
 *
 * Undoubtedly, this seemed logical at the time.
 */
static int
parse_field_spec(field_t *F, char *C, int flags, int length)
{
	int	p_period = MIN(length, strcspn(C, "."));
	int	p_modifiers = MIN(length, strspn(C, ".1234567890"));
	int	p_boundary = MIN(p_period, p_modifiers);
	int	field = 0;
	int	offset = 0;
	int	offset_seen = 0;
	int	i;
	int	blanks_flag = 0;

	for (i = 0; i < p_boundary; i++)
		if (isdigit(C[i]))
			field = (10 * field) + (C[i] - '0');
		else
			return (1);

	if (p_period < p_modifiers)
		for (i = p_period + 1; i < p_modifiers; i++)
			if (isdigit(C[i])) {
				offset_seen++;
				offset = (10 * offset) + (C[i] - '0');
			} else {
				return (1);
			}

	if (p_modifiers < length) {
		for (i = p_modifiers; i < length; i++) {
			switch (C[i]) {
				case 'b':
					blanks_flag = 1;
					break;
				case 'd':
					F->f_options |= FIELD_DICTIONARY_ORDER;
					break;
				case 'f':
					F->f_options |= FIELD_FOLD_UPPERCASE;
					break;
				case 'i':
					F->f_options |=
					    FIELD_IGNORE_NONPRINTABLES;
					break;
				case 'M':
					F->f_species = MONTH;
					break;
				case 'n':
					F->f_species = NUMERIC;
					break;
				case 'r':
					F->f_options |=
					    FIELD_REVERSE_COMPARISONS;
					break;
				default:
					terminate(SE_USAGE);
					break;
			}
		}
	}

	if (flags & OPTIONS_STARTSPEC) {
		F->f_start_field = field;
		F->f_start_offset = offset;
		if (flags & OPTIONS_OLDSPEC) {
			F->f_start_field++;
			if (offset_seen)
				F->f_start_offset++;
		}
		F->f_options |= blanks_flag ? FIELD_IGNORE_BLANKS_BEFORE : 0;
	} else {
		F->f_end_field = field;
		F->f_end_offset = offset;
		if (flags & OPTIONS_OLDSPEC && offset)
			F->f_end_field++;
		F->f_options |= blanks_flag ? FIELD_IGNORE_BLANKS_AFTER : 0;
	}

	return (0);
}

static void
parse_new_field_spec(sort_t *S, char *arg)
{
	int length = strlen(arg);
	int p_comma = MIN(length, strcspn(arg, ","));
	field_t *nF;
	int p;

	/*
	 * New field specifiers do not inherit from the general specifier if
	 * they have any modifiers set.  (This is specifically tested in the VSC
	 * test suite, assertion 32 for POSIX.cmd/sort.)
	 */
	if (field_spec_has_modifiers(arg, length)) {
		nF = field_new(NULL);
		nF->f_options = FIELD_MODIFIERS_DEFINED;
	} else {
		nF = field_new(S);
	}
	p = parse_field_spec(nF, arg, OPTIONS_STARTSPEC, p_comma);

	if (p != 0)
		terminate(SE_USAGE);

	if (p_comma < length) {
		p = parse_field_spec(nF, &(arg[p_comma + 1]), 0,
		    strlen(&(arg[p_comma + 1])));
		if (p != 0)
			terminate(SE_USAGE);
	}

	/*
	 * If the starting field exceeds a defined ending field, convention
	 * dictates that the field is ignored.
	 */
	if (nF->f_end_field == 0 || nF->f_start_field < nF->f_end_field ||
	    (nF->f_start_field == nF->f_end_field &&
	    nF->f_start_offset <= nF->f_end_offset))
		field_add_to_chain(&(S->m_fields_head), nF);
}

/*
 * parse_old_field_spec() is getopt()-aware; it may modify the values of optind,
 * optarg, and so forth, to correctly determine the characteristics being
 * assigned to the current field.
 */
static int
parse_old_field_spec(sort_t *S, int argc, char *argv[])
{
	field_t *nF;
	int c, p, prev_ind;
	char *arg = argv[optind];

	if (field_spec_has_modifiers(arg + 1, strlen(arg + 1))) {
		nF = field_new(NULL);
		nF->f_options = FIELD_MODIFIERS_DEFINED;
	} else {
		nF = field_new(S);
	}

	p = parse_field_spec(nF, arg + 1, OPTIONS_OLDSPEC | OPTIONS_STARTSPEC,
	    strlen(arg + 1));

	if (p != 0) {
		field_delete(nF);
		return (0);
	}

	/*
	 * In the case that getopt() returns '?' (unrecognized option) or EOF
	 * (non-option argument), the field is considered closed.
	 */
	for (arg = argv[++optind], prev_ind = optind; optind < argc;
	    arg = argv[optind], prev_ind = optind) {
		if (strlen(arg) >= 2 && *arg == '-' && isdigit(*(arg + 1))) {
			(void) parse_field_spec(nF, arg + 1, OPTIONS_OLDSPEC,
			    strlen(arg) - 1);
			field_add_to_chain(&(S->m_fields_head), nF);
			optind++;
			return (1);
		}

		if ((c = getopt(argc, argv, OLD_SPEC_OPTIONS_STRING)) != EOF) {
			switch (c) {
			case 'b':
				nF->f_options |= FIELD_IGNORE_BLANKS_BEFORE;
				break;
			case 'd':
				nF->f_options |= FIELD_DICTIONARY_ORDER;
				break;
			case 'f':
				nF->f_options |= FIELD_FOLD_UPPERCASE;
				break;
			case 'i':
				nF->f_options |= FIELD_IGNORE_NONPRINTABLES;
				break;
			case 'M':
				nF->f_species = MONTH;
				break;
			case 'n':
				nF->f_species = NUMERIC;
				break;
			case 'r':
				nF->f_options |= FIELD_REVERSE_COMPARISONS;
				break;
			case '?':
			case 'c':
			case 'm':
			case 'u':
			case 'o':
			case 'T':
			case 'z':
			case 't':
			case 'k':
			case 'S':
			default:
				if (optind == prev_ind)
					optarg--;
				else
					optind--;
				field_add_to_chain(&(S->m_fields_head), nF);
				return (1);
			}
		} else {
			break;
		}
		prev_ind = optind;
	}

	field_add_to_chain(&(S->m_fields_head), nF);
	return (1);
}

int
options(sort_t *S, int argc, char *argv[])
{
	int c;

	optind = 1;
	while (optind < argc) {
		if (strncmp("-y", argv[optind], strlen("-y")) == 0) {
			/*
			 * The -y [kmem] option violates the standard syntax
			 * outlined in intro(1).  we have to be a little fancy
			 * to determine if the next argument is a valid integer.
			 * (note, of course, that the previous sort(1) had no
			 * mechanism to resolve a final
			 *	-y 99999
			 * into
			 *	-y, file 99999
			 * or
			 *	-y 99999, file stdin
			 *
			 * Now one can unambiguously use
			 *	-y -- 99999
			 * and
			 *	-y 99999 -
			 * to distinguish these cases.
			 *
			 * That said, we do not use the information passed using
			 * -y option in sort(1); we provide the argument to
			 * preserve compatibility for existing scripts.
			 */
			if (strlen(argv[optind]) == strlen("-y") &&
			    is_number(argv[optind + 1]))
				optind += 2;
			else
				optind += 1;
		}

		if ((c = getopt(argc, argv, OPTIONS_STRING)) != EOF) {
			switch (c) {
			case 'c':
				S->m_check_if_sorted_only = 1;
				break;

			case 'm':
				S->m_merge_only = 1;
				break;

			case 'u':
				S->m_unique_lines = 1;
				break;

			case 'o':
				S->m_output_filename = optarg;
				break;

			case 'T':
				S->m_tmpdir_template = optarg;
				break;

			case 'z':
				/*
				 * ignore optarg -- obsolete
				 */
				break;

			case 'd':
				S->m_field_options |= FIELD_DICTIONARY_ORDER;
				field_apply_all(S->m_fields_head,
				    FIELD_DICTIONARY_ORDER);
				break;

			case 'f':
				S->m_field_options |= FIELD_FOLD_UPPERCASE;
				field_apply_all(S->m_fields_head,
				    FIELD_FOLD_UPPERCASE);
				break;

			case 'i':
				S->m_field_options |=
				    FIELD_IGNORE_NONPRINTABLES;
				field_apply_all(S->m_fields_head,
				    FIELD_IGNORE_NONPRINTABLES);
				break;

			case 'M':
				S->m_default_species = MONTH;
				S->m_field_options &=
				    ~(FIELD_IGNORE_BLANKS_BEFORE |
				    FIELD_IGNORE_BLANKS_AFTER);
				break;

			case 'n':
				S->m_default_species = NUMERIC;
				{
					field_t *f;

					for (f = S->m_fields_head; f;
					    f = f->f_next)
						if ((f->f_options &
						    FIELD_MODIFIERS_DEFINED) ==
						    0)
							f->f_species = NUMERIC;
				}
				break;

			case 'b':
				S->m_field_options |=
				    FIELD_IGNORE_BLANKS_BEFORE;
				break;

			case 'r':
				S->m_field_options |=
				    FIELD_REVERSE_COMPARISONS;
				field_apply_all(S->m_fields_head,
				    FIELD_REVERSE_COMPARISONS);
				break;

			case 't':
				/*
				 * delimiter
				 */
				if (S->m_single_byte_locale) {
					/*
					 * Most debuggers can't take tabs as
					 * input arguments, so we provide an
					 * escape sequence to allow testing of
					 * this special case for the DEBUG
					 * version.
					 */
					S->m_field_separator.sc =
#ifdef DEBUG
					    xstreql(optarg, "\\t") ? '\t' :
#endif
					    optarg[0];
				} else
					(void) mbtowc(&S->m_field_separator.wc,
					    optarg, MB_CUR_MAX);
				break;

			case 'k':
				/*
				 * key
				 */
				(void) parse_new_field_spec(S, optarg);
				break;

			case 'S':
				S->m_memory_limit = strtomem(optarg);
#ifdef DEBUG
				(void) fprintf(stderr, CMDNAME
				    ": limiting size to %d bytes\n",
				    S->m_memory_limit);
#endif /* DEBUG */
				break;

			/*
			 * We never take a naked -999; these should always be
			 * associated with a preceding +000.
			 */
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				terminate(SE_BAD_SPECIFIER);
				break;
			case '?':
				/* error case */
				terminate(SE_USAGE);
				break;
			}

			/*
			 * Go back for next argument.
			 */
			continue;
		}

		/*
		 * There are three (interpretable) possibilities for getopt() to
		 * return EOF with arguments on the command line: we have seen
		 * the "end-of-options" token, --, we have encountered the
		 * old-style field definition, +NNN, or we have found a
		 * filename.
		 *
		 * In the second case, we must also search for the optional -NNN
		 * field terminal definition.  (since "+joe", for instance, is
		 * a valid filename, we must handle this pattern as well.)  This
		 * is performed by parse_old_field_spec().
		 */
		if (xstreql(argv[optind - 1], "--")) {
			/*
			 * Process all arguments following end-of-options token
			 * as filenames.
			 */
			while (optind < argc) {
				if (xstreql(argv[optind], "-"))
					S->m_input_from_stdin = 1;
				else
					stream_add_file_to_chain(
					    &(S->m_input_streams),
					    argv[optind]);
				optind++;
			}

			break;
		}

		if (optind < argc) {
			if (xstreql(argv[optind], "-")) {
				S->m_input_from_stdin = 1;
				optind++;
			} else if (*(argv[optind]) != '+' ||
			    !parse_old_field_spec(S, argc, argv)) {
				/*
				 * It's a filename, because it either doesn't
				 * start with '+', or if it did, it wasn't an
				 * actual field specifier.
				 */
				stream_add_file_to_chain(&(S->m_input_streams),
				    argv[optind]);
				optind++;
			}
		}
	}

	if (S->m_input_streams == NULL)
		S->m_input_from_stdin = 1;

	if (S->m_output_filename == NULL)
		S->m_output_to_stdout = 1;

	/*
	 * If no fields, then one great field.  However, if the -b option was
	 * set globally, be sure to ignore it, as per UNIX98.
	 */
	if (S->m_fields_head == NULL) {
		S->m_field_options &= ~FIELD_IGNORE_BLANKS_BEFORE;

		(void) parse_new_field_spec(S, "1");
		/*
		 * "Entire line" fast path is only valid if no delimiter has
		 * been set and no modifiers have been applied.
		 */
		if (S->m_field_separator.wc == 0 &&
		    S->m_default_species == ALPHA &&
		    S->m_field_options == 0)
			S->m_entire_line = 1;
	}

	return (0);
}
