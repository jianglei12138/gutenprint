/*
 * "$Id: genppd.c,v 1.205 2015/10/17 16:27:18 rlk Exp $"
 *
 *   PPD file generation program for the CUPS drivers.
 *
 *   Copyright 1993-2008 by Mike Sweet and Robert Krawitz.
 *
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *   for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Contents:
 *
 *   main()              - Process files on the command-line...
 *   cat_ppd()           - Copy the named PPD to stdout.
 *   generate_ppd()      - Generate a PPD file.
 *   getlangs()          - Get a list of available translations.
 *   help()              - Show detailed help.
 *   is_special_option() - Determine if an option should be grouped.
 *   list_ppds()         - List the available drivers.
 *   print_group_close() - Close a UI group.
 *   print_group_open()  - Open a new UI group.
 *   printlangs()        - Print list of available translations.
 *   printmodels()       - Print a list of available models.
 *   usage()             - Show program usage.
 *   write_ppd()         - Write a PPD file.
 */

/*
 * Include necessary headers...
 */

#pragma GCC diagnostic ignored "-Wformat-nonliteral"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#if defined(HAVE_VARARGS_H) && !defined(HAVE_STDARG_H)
#include <varargs.h>
#else
#include <stdarg.h>
#endif

#ifdef CUPS_DRIVER_INTERFACE
#  ifdef HAVE_LIBZ
#    undef HAVE_LIBZ
#  endif /* HAVE_LIBZ */
#endif /* CUPS_DRIVER_INTERFACE */
#ifdef HAVE_LIBZ
#include <zlib.h>
static int use_compression = 1;
#endif

static const char *cups_modeldir = CUPS_MODELDIR;
static const char *gpext = "";

#include <cups/cups.h>
#include <cups/raster.h>

#include "i18n.h"

static int use_base_version = 0;

/*
 * Some applications use the XxYdpi tags rather than the actual
 * hardware resolutions to decide what resolution to print at.  Some
 * applications get very unhappy if the vertical resolution exceeds
 * a certain amount.  Some of those applications even get very unhappy if
 * the PPD file even contains a resolution that exceeds that limit.
 * And they're not even free source applications.
 * Feh.
 */
#define MAXIMUM_SAFE_PPD_Y_RESOLUTION (720)
#define MAXIMUM_SAFE_PPD_X_RESOLUTION (1500)

typedef enum
{
  PPD_STANDARD = 0,
  PPD_SIMPLIFIED = 1,
  PPD_NO_COLOR_OPTS = 2
} ppd_type_t;

/*
 * Note:
 *
 * The current release of ESP Ghostscript is fully Level 3 compliant,
 * so we can report Level 3 support by default...
 */

int cups_ppd_ps_level = CUPS_PPD_PS_LEVEL;
int localize_numbers = 0;

/*
 * File handling stuff...
 */

static const char *ppdext = ".ppd";

typedef struct				/**** Media size values ****/
{
  const char	*name,			/* Media size name */
		*text;			/* Media size text */
  int		width,			/* Media width */
		height,			/* Media height */
		left,			/* Media left margin */
		right,			/* Media right margin */
		bottom,			/* Media bottom margin */
		top;			/* Media top margin */
} paper_t;

const char *special_options[] =
{
  "PageSize",
  "MediaType",
  "InputSlot",
  "Resolution",
  "OutputOrder",
  "Quality",
  "Duplex",
  NULL
};

/*
 * TRANSLATORS:
 * Please keep these translated names SHORT.  The number of bytes in
 * the parameter class name plus the number of bytes in the parameter
 * name must not exceed 38 BYTES (not characters!)
 */

const char *parameter_class_names[] =
{
  _("Printer Features"),
  _("Output Control")
};

const char *parameter_level_names[] =
{
  _("Common"),
  _("Extra 1"),
  _("Extra 2"),
  _("Extra 3"),
  _("Extra 4"),
  _("Extra 5")
};

#ifdef HAVE_LIBZ
typedef union
{
  gzFile gzf;
  FILE *f;
} _gpfile;

typedef _gpfile *gpFile;
#else
#define gpFile FILE *
#endif

/*
 * Local functions...
 */

#ifdef CUPS_DRIVER_INTERFACE
static int	cat_ppd(const char *uri);
static int	list_ppds(const char *argv0);
#else  /* !CUPS_DRIVER_INTERFACE */
static int	generate_ppd(const char *prefix, int verbose,
		             const stp_printer_t *p, const char *language,
			     ppd_type_t ppd_type);
static int	generate_model_ppds(const char *prefix, int verbose,
				    const stp_printer_t *printer,
				    const char *language, int which_ppds);
static void	help(void);
static void	printlangs(char** langs);
static void	printmodels(int verbose);
static void	usage(void);
static gpFile	gpopen(const char *path, const char *mode);
static int	gpclose(gpFile f);
#endif /* !CUPS_DRIVER_INTERFACE */
static int	gpputs(gpFile f, const char *s);
static int	gpprintf(gpFile f, const char *format, ...);
static char	**getlangs(void);
static int	is_special_option(const char *name);
static void	print_group_close(gpFile fp, stp_parameter_class_t p_class,
				  stp_parameter_level_t p_level,
				  const char *language,
				  const stp_string_list_t *po);
static void	print_group_open(gpFile fp, stp_parameter_class_t p_class,
				 stp_parameter_level_t p_level,
				 const char *language,
				 const stp_string_list_t *po);
static int	write_ppd(gpFile fp, const stp_printer_t *p,
		          const char *language, const char *ppd_location,
			  ppd_type_t ppd_type, const char *filename);


/*
 * Global variables...
 */


#ifdef CUPS_DRIVER_INTERFACE

/*
 * 'main()' - Process files on the command-line...
 */

const char slang_c[] = "LANG=C";
const char slcall_c[] = "LC_ALL=C";
const char slcnumeric_c[] = "LC_NUMERIC=C";
char lang_c[sizeof(slang_c) + 1];
char lcall_c[sizeof(slcall_c) + 1];
char lcnumeric_c[sizeof(slcnumeric_c) + 1];

int				    /* O - Exit status */
main(int  argc,			    /* I - Number of command-line arguments */
     char *argv[])		    /* I - Command-line arguments */
{
 /*
  * Force POSIX locale, since stp_init incorrectly calls setlocale...
  */

  strcpy(lang_c, slang_c);
  strcpy(lcall_c, slcall_c);
  strcpy(lcnumeric_c, slcnumeric_c);
  putenv(lang_c);
  putenv(lcall_c);
  putenv(lcnumeric_c);

 /*
  * Initialise libgutenprint
  */

  stp_init();

 /*
  * Process command-line...
  */

  if (argc == 2 && !strcmp(argv[1], "list"))
    return (list_ppds(argv[0]));
  else if (argc == 3 && !strcmp(argv[1], "cat"))
    return (cat_ppd(argv[2]));
  else if (argc == 2 && !strcmp(argv[1], "org.gutenprint.multicat"))
    {
      char buf[1024];
      int status = 0;
      while (fgets(buf, sizeof(buf) - 1, stdin))
	{
	  size_t len = strlen(buf);
	  if (len == 0)
	    continue;
	  if (buf[len - 1] == '\n')
	    buf[len - 1] = '\0';
	  status |= cat_ppd(buf);
	  fputs("*%*%EOFEOF\n", stdout);
	  (void) fflush(stdout);
	}
    }
  else if (argc == 2 && !strcmp(argv[1], "VERSION"))
    {
      printf("%s\n", VERSION);
      return (0);
    }
  else if (argc == 2 && !strcasecmp(argv[1], "org.gutenprint.extensions"))
    {
      printf("org.gutenprint.multicat");
      return (0);
    }
  else
    {
      fprintf(stderr, "Usage: %s list\n", argv[0]);
      fprintf(stderr, "       %s cat URI\n", argv[0]);
      return (1);
    }
  return (0);
}


/*
 * 'cat_ppd()' - Copy the named PPD to stdout.
 */

static int				/* O - Exit status */
cat_ppd(const char *uri)	/* I - Driver URI */
{
  char			scheme[64],	/* URI scheme */
			userpass[32],	/* URI user/pass (unused) */
			hostname[32],	/* URI hostname */
			resource[1024];	/* URI resource */
  int			port;		/* URI port (unused) */
  http_uri_status_t	status;		/* URI decode status */
  const stp_printer_t	*p;		/* Printer driver */
  const char		*lang = NULL;
  char			*s;
  char			filename[1024],		/* Filename */
			ppd_location[1024];	/* Installed location */
  const char 		*infix = "";
  ppd_type_t 		ppd_type = PPD_STANDARD;

  if ((status = httpSeparateURI(HTTP_URI_CODING_ALL, uri,
                                scheme, sizeof(scheme),
                                userpass, sizeof(userpass),
				hostname, sizeof(hostname),
		                &port, resource, sizeof(resource)))
				    < HTTP_URI_OK)
  {
    fprintf(stderr, "ERROR: Bad ppd-name \"%s\" (%d)!\n", uri, status);
    return (1);
  }

  if (strcmp(scheme, "gutenprint." GUTENPRINT_RELEASE_VERSION) != 0)
    {
      fprintf(stderr, "ERROR: Gutenprint version mismatch!\n");
      return(1);
    }

  s = strchr(resource + 1, '/');
  if (s)
    {
      lang = s + 1;
      *s = '\0';
    }

  if ((p = stp_get_printer_by_driver(hostname)) == NULL)
  {
    fprintf(stderr, "ERROR: Unable to find driver \"%s\"!\n", hostname);
    return (1);
  }

  if (strcmp(resource + 1, "simple") == 0)
    {
      infix = ".sim";
      ppd_type = PPD_SIMPLIFIED;
    }
  else if (strcmp(resource + 1, "nocolor") == 0)
    {
      infix = ".nc";
      ppd_type = PPD_NO_COLOR_OPTS;
    }

  /*
   * This isn't really the right thing to do.  We really shouldn't
   * be embedding filenames in automatically generated PPD files, but
   * if the user ever decides to go back from generated PPD files to
   * static PPD files we'll need to have this for genppdupdate to work.
   */
  snprintf(filename, sizeof(filename) - 1, "stp-%s.%s%s%s",
	   hostname, GUTENPRINT_RELEASE_VERSION, infix, ppdext);
  snprintf(ppd_location, sizeof(ppd_location) - 1, "%s%s%s/ppd/%s%s",
	   cups_modeldir,
	   cups_modeldir[strlen(cups_modeldir) - 1] == '/' ? "" : "/",
	   lang ? lang : "C",
	   filename, gpext);

  return (write_ppd(stdout, p, lang, ppd_location, ppd_type, filename));
}

/*
 * 'list_ppds()' - List the available drivers.
 */

static int				/* O - Exit status */
list_ppds(const char *argv0)		/* I - Name of program */
{
  const char		*scheme;	/* URI scheme */
  int			i;		/* Looping var */
  const stp_printer_t	*printer;	/* Pointer to printer driver */

  if ((scheme = strrchr(argv0, '/')) != NULL)
    scheme ++;
  else
    scheme = argv0;

  for (i = 0; i < stp_printer_model_count(); i++)
    if ((printer = stp_get_printer_by_index(i)) != NULL)
    {
      const char *device_id;
      if (!strcmp(stp_printer_get_family(printer), "ps") ||
	  !strcmp(stp_printer_get_family(printer), "raw"))
        continue;

      device_id = stp_printer_get_device_id(printer);
      printf("\"%s://%s/expert\" "
             "%s "
	     "\"%s\" "
             "\"%s" CUPS_PPD_NICKNAME_STRING VERSION "\" "
	     "\"%s\"\n",
             scheme, stp_printer_get_driver(printer),
	     "en",
	     stp_printer_get_manufacturer(printer),
	     stp_printer_get_long_name(printer),
	     device_id ? device_id : "");

#ifdef GENERATE_SIMPLIFIED_PPDS
      printf("\"%s://%s/simple\" "
             "%s "
	     "\"%s\" "
             "\"%s" CUPS_PPD_NICKNAME_STRING VERSION " Simplified\" "
	     "\"%s\"\n",
             scheme, stp_printer_get_driver(printer),
	     "en",
	     stp_printer_get_manufacturer(printer),
	     stp_printer_get_long_name(printer),
	     device_id ? device_id : "");
#endif

#ifdef GENERATE_NOCOLOR_PPDS
      printf("\"%s://%s/nocolor\" "
             "%s "
	     "\"%s\" "
             "\"%s" CUPS_PPD_NICKNAME_STRING VERSION " No color options\" "
	     "\"%s\"\n",
             scheme, stp_printer_get_driver(printer),
	     "en",
	     stp_printer_get_manufacturer(printer),
	     stp_printer_get_long_name(printer),
	     device_id ? device_id : "");
#endif
    }

  return (0);
}
#endif /* CUPS_DRIVER_INTERFACE */

#ifndef CUPS_DRIVER_INTERFACE

/*
 * 'main()' - Process files on the command-line...
 */

int				    /* O - Exit status */
main(int  argc,			    /* I - Number of command-line arguments */
     char *argv[])		    /* I - Command-line arguments */
{
  int		i;		    /* Looping var */
  const char	*prefix;	    /* Directory prefix for output */
  const char	*language = NULL;   /* Language */
  const stp_printer_t *printer;	    /* Pointer to printer driver */
  int           verbose = 0;        /* Verbose messages */
  char          **langs = NULL;     /* Available translations */
  char          **models = NULL;    /* Models to output, all if NULL */
  int           opt_printlangs = 0; /* Print available translations */
  int           opt_printmodels = 0;/* Print available models */
  int           which_ppds = 2;	    /* Simplified PPD's = 1, full = 2,
				       no color opts = 4 */

 /*
  * Parse command-line args...
  */

  prefix   = CUPS_MODELDIR;

  for (;;)
  {
    if ((i = getopt(argc, argv, "23hvqc:p:l:LMVd:saNCbZz")) == -1)
      break;

    switch (i)
    {
    case '2':
      cups_ppd_ps_level = 2;
      break;
    case '3':
      cups_ppd_ps_level = 3;
      break;
    case 'h':
      help();
      exit(EXIT_SUCCESS);
      break;
    case 'v':
      verbose = 1;
      break;
    case 'q':
      verbose = 0;
      break;
    case 'c':
      fputs("ERROR: -c option no longer supported!\n", stderr);
      break;
    case 'p':
      prefix = optarg;
#  ifdef DEBUG
      fprintf(stderr, "DEBUG: prefix: %s\n", prefix);
#  endif
      break;
    case 'l':
      language = optarg;
      break;
    case 'L':
      opt_printlangs = 1;
      break;
    case 'M':
      opt_printmodels = 1;
      break;
    case 'd':
      cups_modeldir = optarg;
      break;
    case 's':
      which_ppds = 1;
      break;
    case 'a':
      which_ppds = 3;
      break;
    case 'C':
      which_ppds |= 4;
      break;
    case 'N':
      localize_numbers = !localize_numbers;
      break;
    case 'V':
      printf("cups-genppd version %s, "
	     "Copyright 1993-2008 by Michael R Sweet and Robert Krawitz.\n\n",
	     VERSION);
      printf("Default CUPS PPD PostScript Level: %d\n", cups_ppd_ps_level);
      printf("Default PPD location (prefix):     %s\n", CUPS_MODELDIR);
      printf("Default base locale directory:     %s\n\n", PACKAGE_LOCALE_DIR);
      puts("This program is free software; you can redistribute it and/or\n"
	   "modify it under the terms of the GNU General Public License,\n"
	   "version 2, as published by the Free Software Foundation.\n"
	   "\n"
	   "This program is distributed in the hope that it will be useful,\n"
	   "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	   "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	   "GNU General Public License for more details.\n");
      exit(EXIT_SUCCESS);
      break;
    case 'b':
      use_base_version = 1;
      break;
    case 'z':
#ifdef HAVE_LIBZ
      use_compression = 1;
#endif
      break;
    case 'Z':
#ifdef HAVE_LIBZ
      use_compression = 0;
#endif
      break;
    default:
      usage();
      exit(EXIT_FAILURE);
      break;
    }
  }
#ifdef HAVE_LIBZ  
  if (use_compression)
    gpext = ".gz";
  else
#endif	  
    gpext = "";
  if (optind < argc) {
    int n, numargs;
    numargs = argc-optind;
    models = stp_malloc((numargs+1) * sizeof(char*));
    for (n=0; n<numargs; n++)
      {
	models[n] = argv[optind+n];
      }
    models[numargs] = (char*)NULL;
  }

/*
 * Initialise libgutenprint
 */

  stp_init();

  langs = getlangs();

 /*
  * Print lists
  */

  if (opt_printlangs)
    {
      printlangs(langs);
      exit(EXIT_SUCCESS);
    }

  if (opt_printmodels)
    {
      printmodels(verbose);
      exit(EXIT_SUCCESS);
    }

 /*
  * Write PPD files...
  */

  if (models)
    {
      int n;
      for (n=0; models[n]; n++)
	{
	  printer = stp_get_printer_by_driver(models[n]);
	  if (!printer)
	    printer = stp_get_printer_by_long_name(models[n]);

	  if (printer)
	    {
	      if (generate_model_ppds(prefix, verbose, printer, language,
				      which_ppds))
		return 1;
	    }
	  else
	    {
	      printf("Driver not found: %s\n", models[n]);
	      return (1);
	    }
	}
      stp_free(models);
    }
  else
    {
      for (i = 0; i < stp_printer_model_count(); i++)
	{
	  printer = stp_get_printer_by_index(i);

	  if (printer)
	    {
	      if (generate_model_ppds(prefix, verbose, printer, language,
				      which_ppds))
		return 1;
	    }
	}
    }
  if (!verbose)
    fprintf(stderr, " done.\n");

  return (0);
}

static int
generate_model_ppds(const char *prefix, int verbose,
		    const stp_printer_t *printer, const char *language,
		    int which_ppds)
{
  if ((which_ppds & 1) &&
      generate_ppd(prefix, verbose, printer, language, PPD_SIMPLIFIED))
    return (1);
  if ((which_ppds & 2) &&
      generate_ppd(prefix, verbose, printer, language, PPD_STANDARD))
    return (1);
  if ((which_ppds & 4) &&
      generate_ppd(prefix, verbose, printer, language, PPD_NO_COLOR_OPTS))
    return (1);
  return 0;
}

/*
 * 'generate_ppd()' - Generate a PPD file.
 */

static int				/* O - Exit status */
generate_ppd(
    const char          *prefix,	/* I - PPD directory prefix */
    int                 verbose,	/* I - Verbosity level */
    const stp_printer_t *p,		/* I - Driver */
    const char          *language,	/* I - Primary language */
    ppd_type_t          ppd_type)	/* I - full, simplified, no color */
{
  int		status;			/* Exit status */
  gpFile	fp;			/* File to write to */
  char		filename[1024],		/* Filename */
		ppd_location[1024];	/* Installed location */
  struct stat   dir;                    /* Prefix dir status */
  const char    *ppd_infix;
  static int	ppd_counter = 0; 	/* Notification counter */

 /*
  * Skip the PostScript drivers...
  */

  if (!strcmp(stp_printer_get_family(p), "ps") ||
      !strcmp(stp_printer_get_family(p), "raw"))
    return (0);

 /*
  * Make sure the destination directory exists...
  */

  if (stat(prefix, &dir) && !S_ISDIR(dir.st_mode))
  {
    if (mkdir(prefix, 0777))
    {
      printf("cups-genppd: Cannot create directory %s: %s\n",
	     prefix, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

 /*
  * The files will be named stp-<driver>.<major>.<minor>.ppd, for
  * example:
  *
  * stp-escp2-ex.5.0.ppd
  *
  * or
  *
  * stp-escp2-ex.5.0.ppd.gz
  */

  switch (ppd_type)
    {
    case PPD_SIMPLIFIED:
      ppd_infix = ".sim";
      break;
    case PPD_NO_COLOR_OPTS:
      ppd_infix = ".nc";
      break;
    default:
      ppd_infix = "";
    }

  snprintf(filename, sizeof(filename) - 1, "%s/stp-%s.%s%s%s%s",
	   prefix, stp_printer_get_driver(p), GUTENPRINT_RELEASE_VERSION,
	   ppd_infix, ppdext, gpext);

 /*
  * Open the PPD file...
  */

  if ((fp = gpopen(filename, "wb")) == NULL)
  {
    fprintf(stderr, "cups-genppd: Unable to create file \"%s\" - %s.\n",
            filename, strerror(errno));
    return (2);
  }

  if (verbose)
    fprintf(stderr, "Writing %s...\n", filename);
  else if ((ppd_counter++ % 50) == 0)
    fprintf(stderr, ".");

  snprintf(ppd_location, sizeof(ppd_location), "%s%s%s/%s",
	   cups_modeldir,
	   cups_modeldir[strlen(cups_modeldir) - 1] == '/' ? "" : "/",
	   language ? language : "C",
	   basename(filename));

  snprintf(filename, sizeof(filename) - 1, "stp-%s.%s%s%s",
	   stp_printer_get_driver(p), GUTENPRINT_RELEASE_VERSION,
	   ppd_infix, ppdext);

  status = write_ppd(fp, p, language, ppd_location, ppd_type,
		     basename(filename));

  gpclose(fp);

  return (status);
}

/*
 * 'help()' - Show detailed help.
 */

void
help(void)
{
  puts("Generate Gutenprint PPD files for use with CUPS\n\n");
  usage();
  puts("\nExamples: LANG=de_DE cups-genppd -p ppd -c /usr/share/locale\n"
       "          cups-genppd -L -c /usr/share/locale\n"
       "          cups-genppd -M -v\n\n"
       "Commands:\n"
       "  -h            Show this help message.\n"
       "  -L            List available translations (message catalogs).\n"
       "  -M            List available printer models.\n"
       "  -V            Show version information and defaults.\n"
       "  The default is to output PPDs.\n");
  puts("Options:\n"
       "  -N            Localize numbers.\n"
       "  -l locale     Output PPDs translated with messages for locale.\n"
       "  -p prefix     Output PPDs in directory prefix.\n"
       "  -d prefix     Embed directory prefix in PPD file.\n"
       "  -s            Generate simplified PPD files.\n"
       "  -a            Generate all (simplified and full) PPD files.\n"
       "  -q            Quiet mode.\n"
       "  -v            Verbose mode.\n");
  puts(
#ifdef HAVE_LIBZ
       "  -z            Compress PPD files.\n"
       "  -Z            Don't compress PPD files.\n"
#endif
       "models:\n"
       "  A list of printer models, either the driver or quoted full name.\n");
}

/*
 * 'usage()' - Show program usage.
 */

void
usage(void)
{
  puts("Usage: cups-genppd "
        "[-l locale] [-p prefix] [-s | -a] [-q] [-v] models...\n"
        "       cups-genppd -L\n"
	"       cups-genppd -M [-v]\n"
	"       cups-genppd -h\n"
	"       cups-genppd -V\n");
}

/*
 * 'printlangs()' - Print list of available translations.
 */

void
printlangs(char **langs)		/* I - Languages */
{
  if (langs)
    {
      int n = 0;
      while (langs && langs[n])
	{
	  puts(langs[n]);
	  n++;
	}
    }
  exit(EXIT_SUCCESS);
}


/*
 * 'printmodels()' - Print a list of available models.
 */

void
printmodels(int verbose)		/* I - Verbosity level */
{
  const stp_printer_t *p;
  int i;

  for (i = 0; i < stp_printer_model_count(); i++)
    {
      p = stp_get_printer_by_index(i);
      if (p &&
	  strcmp(stp_printer_get_family(p), "ps") != 0 &&
	  strcmp(stp_printer_get_family(p), "raw") != 0)
	{
	  if(verbose)
	    printf("%-20s%s\n", stp_printer_get_driver(p),
		   stp_printer_get_long_name(p));
	  else
	    printf("%s\n", stp_printer_get_driver(p));
	}
    }
  exit(EXIT_SUCCESS);
}

static gpFile
gpopen(const char *path, const char *mode)
{
#ifdef HAVE_LIBZ
  gpFile f = stp_malloc(sizeof(_gpfile));
  if (use_compression)
    {
      f->gzf = gzopen(path, mode);
      if (!f->gzf)
	{
	  stp_free(f);
	  return NULL;
	}
      return f;
    }
  else
#endif
    {
      FILE *fl = fopen(path, mode);
#ifdef HAVE_LIBZ
      if (fl)
	{
	  f->f = fl;
	  return f;
	}
      else
	{
	  stp_free(f);
	  return NULL;
	}
#else
      return fl;
#endif
    }
}

static int
gpclose(gpFile f)
{
  int status;
#ifdef HAVE_LIBZ
  if (use_compression)
    status = gzclose(f->gzf);
  else
    status = fclose(f->f);
  stp_free(f);
#else
  status = fclose(f);
#endif
  return status;
}

#endif /* !CUPS_DRIVER_INTERFACE */

static int
gpputs(gpFile f, const char *s)
{
#ifdef HAVE_LIBZ
  if (use_compression)
    return gzputs(f->gzf, s);
  else
    return fputs(s, f->f);
#else
  return fputs(s, f);
#endif
}

static int
gpprintf(gpFile f, const char *format, ...)
{
  int status;
  int current_allocation = 64;
  char *result = stp_malloc(current_allocation);
  while (1)
    {
      int bytes;
      va_list args;
      va_start(args, format);
      bytes = vsnprintf(result, current_allocation, format, args);
      va_end(args);
      if (bytes >= 0 && bytes < current_allocation)
	break;
      else
	{
	  stp_free(result);
	  if (bytes < 0)
	    current_allocation *= 2;
	  else
	    current_allocation = bytes + 1;
	  result = stp_malloc(current_allocation);
	}
    }
  status = gpputs(f, result);
  stp_free(result);
  return status;
}

/*
 * 'getlangs()' - Get a list of available translations.
 */

char **					/* O - Array of languages */
getlangs(void)
{
  int		i;			/* Looping var */
  char		*ptr;			/* Pointer into string */
  static char	all_linguas[] = ALL_LINGUAS;
					/* List of languages from configure.ac */
  static char **langs = NULL;		/* Array of languages */


  if (!langs)
  {
   /*
    * Create the langs array...
    */

    for (i = 1, ptr = strchr(all_linguas, ' '); ptr; ptr = strchr(ptr + 1, ' '))
      i ++;

    langs = calloc(i + 1, sizeof(char *));

    langs[0] = all_linguas;
    for (i = 1, ptr = strchr(all_linguas, ' '); ptr; ptr = strchr(ptr + 1, ' '))
    {
      *ptr     = '\0';
      langs[i] = ptr + 1;
      i ++;
    }
  }

  return (langs);
}


/*
 * 'is_special_option()' - Determine if an option should be grouped.
 */

static int				/* O - 1 if non-grouped, 0 otherwise */
is_special_option(const char *name)	/* I - Option name */
{
  int i = 0;
  while (special_options[i])
    {
      if (strcmp(name, special_options[i]) == 0)
	return 1;
      i++;
    }
  return 0;
}

/*
 * strlen returns the number of characters.  PPD file limitations are
 * defined in bytes.  So we need something to count bytes, not merely
 * characters.
 */

static size_t
bytelen(const char *buffer)
{
  size_t answer = 0;
  while (*buffer++ != '\0')
    answer++;
  return answer;
}

/*
 * Use our localization routine to correctly do localization on all
 * systems.  The standard lookup routine has trouble creating multi-locale
 * files on many systems, and on some systems there's not even a reliable
 * way to use something other than the system locale.
 */
#ifdef _
#undef _
#endif
#define _(x) stp_i18n_lookup(po, x)

#define PPD_MAX_SHORT_NICKNAME (31)

static void
print_ppd_header(gpFile fp, ppd_type_t ppd_type, int model, const char *driver,
		 const char *family, const char *long_name,
		 const char *manufacturer, const char *device_id,
		 const char *ppd_location,
		 const char *language, const stp_string_list_t *po,
		 char **all_langs)
{
  char short_long_name[(PPD_MAX_SHORT_NICKNAME) + 1];
 /*
  * Write a standard header...
  */
  gpputs(fp, "*PPD-Adobe: \"4.3\"\n");
  gpputs(fp, "*% PPD file for CUPS/Gutenprint.\n");
  gpputs(fp, "*% Copyright 1993-2008 by Mike Sweet and Robert Krawitz.\n");
  gpputs(fp, "*% This program is free software; you can redistribute it and/or\n");
  gpputs(fp, "*% modify it under the terms of the GNU General Public License,\n");
  gpputs(fp, "*% version 2, as published by the Free Software Foundation.\n");
  gpputs(fp, "*%\n");
  gpputs(fp, "*% This program is distributed in the hope that it will be useful, but\n");
  gpputs(fp, "*% WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY\n");
  gpputs(fp, "*% or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License\n");
  gpputs(fp, "*% for more details.\n");
  gpputs(fp, "*%\n");
  gpputs(fp, "*% You should have received a copy of the GNU General Public License\n");
  gpputs(fp, "*% along with this program; if not, write to the Free Software\n");
  gpputs(fp, "*% Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.\n");
  gpputs(fp, "*%\n");
  gpputs(fp, "*FormatVersion:	\"4.3\"\n");
  if (use_base_version)
    gpputs(fp, "*FileVersion:	\"" BASE_VERSION "\"\n");
  else
    gpputs(fp, "*FileVersion:	\"" VERSION "\"\n");
  /* Specify language of PPD translation */
  /* TRANSLATORS: Specify the language of the PPD translation.
   * Use the English name of your language here, e.g. "Swedish" instead of
   * "Svenska". */
  gpprintf(fp, "*LanguageVersion: %s\n", _("English"));
  if (language)
    gpputs(fp, "*LanguageEncoding: UTF-8\n");
  else
    gpputs(fp, "*LanguageEncoding: ISOLatin1\n");
 /*
  * Strictly speaking, the PCFileName attribute should be a 12 character
  * max (12345678.ppd) filename, as a requirement of the old PPD spec.
  * The following code generates a (hopefully unique) 8.3 filename from
  * the driver name, and makes the filename all UPPERCASE as well...
  */

  gpprintf(fp, "*PCFileName:	\"STP%05d.PPD\"\n",
	   stp_get_printer_index_by_driver(driver) +
	   ((int) ppd_type * stp_printer_model_count()));
  gpprintf(fp, "*Manufacturer:	\"%s\"\n", manufacturer);

 /*
  * The Product attribute specifies the string returned by the PostScript
  * interpreter.  The last one will appear in the CUPS "product" field,
  * while all instances are available as attributes.  Rather than listing
  * the PostScript interpreters we might encounter, we instead just list
  * a single product line with the "long name" to be compatible with other
  * CUPS-based drivers. (This is a change from Gutenprint 5.0 and earlier)
  */

  gpprintf(fp, "*Product:	\"(%s)\"\n", long_name);

 /*
  * The ModelName attribute now provides the long name rather than the
  * short driver name...  The rastertoprinter driver looks up both...
  */

  gpprintf(fp, "*ModelName:     \"%s\"\n", long_name);
  strncpy(short_long_name, long_name, PPD_MAX_SHORT_NICKNAME);
  short_long_name[PPD_MAX_SHORT_NICKNAME] = '\0';
  gpprintf(fp, "*ShortNickName: \"%s\"\n", short_long_name);

 /*
  * NOTE - code in rastertoprinter looks for this version string.
  * If this is changed, the corresponding change must be made in
  * rastertoprinter.c.  Look for "ppd->nickname"
  */
  gpprintf(fp, "*NickName:      \"%s%s%s%s\"\n",
	   long_name, CUPS_PPD_NICKNAME_STRING, VERSION,
	   (ppd_type == PPD_SIMPLIFIED ? " Simplified" :
	    ppd_type == PPD_NO_COLOR_OPTS ? " No Color Options" : ""));
  if (cups_ppd_ps_level == 2)
    gpputs(fp, "*PSVersion:	\"(2017.000) 550\"\n");
  else
    gpputs(fp, "*PSVersion:	\"(3010.000) 0\"\n");
  gpprintf(fp, "*LanguageLevel:	\"%d\"\n", cups_ppd_ps_level);
}

static void
print_ppd_header_3(gpFile fp, ppd_type_t ppd_type, int model,
		   const char *driver,
		   const char *family, const char *long_name,
		   const char *manufacturer, const char *device_id,
		   const char *ppd_location,
		   const char *language, const stp_string_list_t *po,
		   char **all_langs)
{
  int i;
  gpputs(fp, "*FileSystem:	False\n");
  gpputs(fp, "*LandscapeOrientation: Plus90\n");
  gpputs(fp, "*TTRasterizer:	Type42\n");

  gpputs(fp, "*cupsVersion:	1.2\n");
  
  gpprintf(fp, "*cupsFilter:	\"application/vnd.cups-raster 100 rastertogutenprint.%s\"\n", GUTENPRINT_RELEASE_VERSION);
  if (strcasecmp(manufacturer, "EPSON") == 0)
    gpputs(fp, "*cupsFilter:	\"application/vnd.cups-command 33 commandtoepson\"\n");
  if (device_id)
    gpprintf(fp, "*1284DeviceID: \"%s\"\n", device_id);
  if (!language)
  {
   /*
    * Generate globalized PPDs when POSIX language is requested...
    */

    const char *prefix = "*cupsLanguages: \"";

    for (i = 0; all_langs[i]; i ++)
    {
      if (!strcmp(all_langs[i], "C") || !strcmp(all_langs[i], "en"))
        continue;

      gpprintf(fp, "%s%s", prefix, all_langs[i]);
      prefix = " ";
    }

    if (!strcmp(prefix, " "))
      gpputs(fp, "\"\n");
  }
}

static void
print_ppd_header_2(gpFile fp, ppd_type_t ppd_type, int model, const char *driver,
		   const char *family, const char *long_name,
		   const char *manufacturer, const char *device_id,
		   const char *ppd_location,
		   const char *language, const stp_string_list_t *po,
		   char **all_langs)
{
  gpprintf(fp, "*StpDriverName:	\"%s\"\n", driver);
  gpprintf(fp, "*StpDriverModelFamily:	\"%d_%s\"\n", model, family);
  gpprintf(fp, "*StpPPDLocation: \"%s\"\n", ppd_location);
  gpprintf(fp, "*StpLocale:	\"%s\"\n", language ? language : "C");
}

static void
print_page_sizes(gpFile fp, stp_vars_t *v, int simplified,
		 const stp_string_list_t *po)
{
  int variable_sizes = 0;
  stp_parameter_t desc;
  int num_opts;
  paper_t *the_papers;
  int i;
  int		width, height,		/* Page information */
		bottom, left,
		top, right;
  int		min_width,		/* Min/max custom size */
		min_height,
		max_width,
		max_height;
  const stp_param_string_t *opt;
  int cur_opt = 0;

  stp_describe_parameter(v, "PageSize", &desc);
  num_opts = stp_string_list_count(desc.bounds.str);
  the_papers = stp_malloc(sizeof(paper_t) * num_opts);
  for (i = 0; i < num_opts; i++)
    {
      const stp_papersize_t *papersize;
      opt = stp_string_list_param(desc.bounds.str, i);
      papersize = stp_get_papersize_by_name(opt->name);

      if (!papersize)
	{
	  printf("Unable to lookup size %s!\n", opt->name);
	  continue;
	}

      if (strcmp(opt->name, "Custom") == 0)
	{
	  variable_sizes = 1;
	  continue;
	}
      if (simplified && num_opts >= 10 &&
	  (papersize->paper_unit == PAPERSIZE_ENGLISH_EXTENDED ||
	   papersize->paper_unit == PAPERSIZE_METRIC_EXTENDED))
	continue;

      width  = papersize->width;
      height = papersize->height;

      if (width <= 0 || height <= 0)
	continue;

      stp_set_string_parameter(v, "PageSize", opt->name);

      stp_get_media_size(v, &width, &height);
      stp_get_maximum_imageable_area(v, &left, &right, &bottom, &top);

      if (left < 0)
	left = 0;
      if (right > width)
	right = width;
      if (bottom > height)
	bottom = height;
      if (top < 0)
	top = 0;

      the_papers[cur_opt].name   = opt->name;
      the_papers[cur_opt].text   = stp_i18n_lookup(po, opt->text);
      the_papers[cur_opt].width  = width;
      the_papers[cur_opt].height = height;
      the_papers[cur_opt].left   = left;
      the_papers[cur_opt].right  = right;
      the_papers[cur_opt].bottom = height - bottom;
      the_papers[cur_opt].top    = height - top;

      cur_opt++;
      stp_clear_string_parameter(v, "PageSize");
    }

  /*
   * The VariablePaperSize attribute is obsolete, however some popular
   * applications still look for it to provide custom page size support.
   */

  gpprintf(fp, "*VariablePaperSize: %s\n\n", variable_sizes ? "true" : "false");

  if (stp_parameter_has_category_value(v, &desc, "Color", "Yes"))
    gpputs(fp, "*ColorKeyWords: \"PageSize\"\n");
  gpprintf(fp, "*OpenUI *PageSize/%s: PickOne\n", _("Media Size"));
  gpputs(fp, "*OPOptionHints PageSize: \"dropdown\"\n");
  gpputs(fp, "*OrderDependency: 10 AnySetup *PageSize\n");
  gpprintf(fp, "*StpStp%s: %d %d %d %d %d %.3f %.3f %.3f\n",
	   desc.name, desc.p_type, desc.is_mandatory,
	   desc.p_class, desc.p_level, desc.channel, 0.0, 0.0, 0.0);
  gpprintf(fp, "*DefaultPageSize: %s\n", desc.deflt.str);
  gpprintf(fp, "*StpDefaultPageSize: %s\n", desc.deflt.str);
  for (i = 0; i < cur_opt; i ++)
    {
      gpprintf(fp,  "*PageSize %s", the_papers[i].name);
      gpprintf(fp, "/%s:\t\"<</PageSize[%d %d]/ImagingBBox null>>setpagedevice\"\n",
	       the_papers[i].text, the_papers[i].width, the_papers[i].height);
    }
  gpputs(fp, "*CloseUI: *PageSize\n\n");

  if (stp_parameter_has_category_value(v, &desc, "Color", "Yes"))
    gpputs(fp, "*ColorKeyWords: \"PageRegion\"\n");
  gpprintf(fp, "*OpenUI *PageRegion/%s: PickOne\n", _("Media Size"));
  gpputs(fp, "*OPOptionHints PageRegion: \"dropdown\"\n");
  gpputs(fp, "*OrderDependency: 10 AnySetup *PageRegion\n");
  gpprintf(fp, "*DefaultPageRegion: %s\n", desc.deflt.str);
  gpprintf(fp, "*StpDefaultPageRegion: %s\n", desc.deflt.str);
  for (i = 0; i < cur_opt; i ++)
    {
      gpprintf(fp,  "*PageRegion %s", the_papers[i].name);
      gpprintf(fp, "/%s:\t\"<</PageSize[%d %d]/ImagingBBox null>>setpagedevice\"\n",
	       the_papers[i].text, the_papers[i].width, the_papers[i].height);
    }
  gpputs(fp, "*CloseUI: *PageRegion\n\n");

  gpprintf(fp, "*DefaultImageableArea: %s\n", desc.deflt.str);
  gpprintf(fp, "*StpDefaultImageableArea: %s\n", desc.deflt.str);
  for (i = 0; i < cur_opt; i ++)
    {
      gpprintf(fp,  "*ImageableArea %s", the_papers[i].name);
      gpprintf(fp, "/%s:\t\"%d %d %d %d\"\n", the_papers[i].text,
	       the_papers[i].left, the_papers[i].bottom,
	       the_papers[i].right, the_papers[i].top);
    }
  gpputs(fp, "\n");

  gpprintf(fp, "*DefaultPaperDimension: %s\n", desc.deflt.str);
  gpprintf(fp, "*StpDefaultPaperDimension: %s\n", desc.deflt.str);

  for (i = 0; i < cur_opt; i ++)
    {
      gpprintf(fp, "*PaperDimension %s", the_papers[i].name);
      gpprintf(fp, "/%s:\t\"%d %d\"\n",
	       the_papers[i].text, the_papers[i].width, the_papers[i].height);
    }
  gpputs(fp, "\n");

  if (variable_sizes)
    {
      stp_get_size_limit(v, &max_width, &max_height, &min_width, &min_height);
      stp_set_string_parameter(v, "PageSize", "Custom");
      stp_get_media_size(v, &width, &height);
      stp_get_maximum_imageable_area(v, &left, &right, &bottom, &top);
      if (left < 0)
	left = 0;
      if (top < 0)
	top = 0;
      if (bottom > height)
	bottom = height;
      if (right > width)
	width = right;

      gpprintf(fp, "*MaxMediaWidth:  \"%d\"\n", max_width);
      gpprintf(fp, "*MaxMediaHeight: \"%d\"\n", max_height);
      gpprintf(fp, "*HWMargins:      %d %d %d %d\n",
	       left, height - bottom, width - right, top);
      gpputs(fp, "*CustomPageSize True: \"pop pop pop <</PageSize[5 -2 roll]/ImagingBBox null>>setpagedevice\"\n");
      gpprintf(fp, "*ParamCustomPageSize Width:        1 points %d %d\n",
	       min_width, max_width);
      gpprintf(fp, "*ParamCustomPageSize Height:       2 points %d %d\n",
	       min_height, max_height);
      gpputs(fp, "*ParamCustomPageSize WidthOffset:  3 points 0 0\n");
      gpputs(fp, "*ParamCustomPageSize HeightOffset: 4 points 0 0\n");
      gpputs(fp, "*ParamCustomPageSize Orientation:  5 int 0 0\n\n");
      stp_clear_string_parameter(v, "PageSize");
    }

  stp_parameter_description_destroy(&desc);
  if (the_papers)
    stp_free(the_papers);
}

static void
print_color_setup(gpFile fp, int simplified, int printer_is_color,
		  const stp_string_list_t *po)
{
  gpputs(fp, "*ColorKeyWords: \"ColorModel\"\n");
  gpprintf(fp, "*OpenUI *ColorModel/%s: PickOne\n", _("Color Model"));
  gpputs(fp, "*OPOptionHints ColorModel: \"radiobuttons\"\n");
  gpputs(fp, "*OrderDependency: 2 AnySetup *ColorModel\n");

  if (printer_is_color)
    {
      gpputs(fp, "*DefaultColorModel: RGB\n");
      gpputs(fp, "*StpDefaultColorModel: RGB\n");
    }
  else
    {
      gpputs(fp, "*DefaultColorModel: Gray\n");
      gpputs(fp, "*StpDefaultColorModel: Gray\n");
    }

  gpprintf(fp, "*ColorModel Gray/%s:\t\"<<"
               "/cupsColorSpace %d"
	       "/cupsColorOrder %d"
	       "%s"
	       ">>setpagedevice\"\n",
           _("Grayscale"), CUPS_CSPACE_W, CUPS_ORDER_CHUNKED,
	   simplified ? "/cupsBitsPerColor 8/cupsPreferredBitsPerColor 16" : "");
  gpprintf(fp, "*ColorModel Black/%s:\t\"<<"
               "/cupsColorSpace %d"
	       "/cupsColorOrder %d"
	       "%s"
	       ">>setpagedevice\"\n",
           _("Inverted Grayscale"), CUPS_CSPACE_K, CUPS_ORDER_CHUNKED,
	   simplified ? "/cupsBitsPerColor 8/cupsPreferredBitsPerColor 16" : "");

  if (printer_is_color)
  {
    gpprintf(fp, "*ColorModel RGB/%s:\t\"<<"
                 "/cupsColorSpace %d"
		 "/cupsColorOrder %d"
	         "%s"
		 ">>setpagedevice\"\n",
             _("RGB Color"), CUPS_CSPACE_RGB, CUPS_ORDER_CHUNKED,
	     simplified ? "/cupsBitsPerColor 8/cupsPreferredBitsPerColor 16" : "");
    gpprintf(fp, "*ColorModel CMY/%s:\t\"<<"
                 "/cupsColorSpace %d"
		 "/cupsColorOrder %d"
	         "%s"
		 ">>setpagedevice\"\n",
             _("CMY Color"), CUPS_CSPACE_CMY, CUPS_ORDER_CHUNKED,
	     simplified ? "/cupsBitsPerColor 8/cupsPreferredBitsPerColor 16" : "");
    gpprintf(fp, "*ColorModel CMYK/%s:\t\"<<"
                 "/cupsColorSpace %d"
		 "/cupsColorOrder %d"
	         "%s"
		 ">>setpagedevice\"\n",
             _("CMYK"), CUPS_CSPACE_CMYK, CUPS_ORDER_CHUNKED,
	     simplified ? "/cupsBitsPerColor 8/cupsPreferredBitsPerColor 16" : "");
    gpprintf(fp, "*ColorModel KCMY/%s:\t\"<<"
                 "/cupsColorSpace %d"
		 "/cupsColorOrder %d"
	         "%s"
		 ">>setpagedevice\"\n",
             _("KCMY"), CUPS_CSPACE_KCMY, CUPS_ORDER_CHUNKED,
	     simplified ? "/cupsBitsPerColor 8/cupsPreferredBitsPerColor 16" : "");
  }

  gpputs(fp, "*CloseUI: *ColorModel\n\n");
  if (!simplified)
    {
      /*
       * 8 or 16 bit color (16 bit is slower)
       */
      gpputs(fp, "*ColorKeyWords: \"StpColorPrecision\"\n");
      gpprintf(fp, "*OpenUI *StpColorPrecision/%s: PickOne\n", _("Color Precision"));
      gpputs(fp, "*OPOptionHints StpColorPrecision: \"radiobuttons\"\n");
      gpputs(fp, "*OrderDependency: 1 AnySetup *StpColorPrecision\n");
      gpputs(fp, "*DefaultStpColorPrecision: Normal\n");
      gpputs(fp, "*StpDefaultStpColorPrecision: Normal\n");
      gpprintf(fp, "*StpColorPrecision Normal/%s:\t\"<<"
	           "/cupsBitsPerColor 8>>setpagedevice\"\n", _("Normal"));
      gpprintf(fp, "*StpColorPrecision Best/%s:\t\"<<"
		   "/cupsBitsPerColor 8"
		   "/cupsPreferredBitsPerColor 16>>setpagedevice\"\n", _("Best"));
      gpputs(fp, "*CloseUI: *StpColorPrecision\n\n");
    }
}

static void
print_group(
    gpFile                fp,		/* I - File to write to */
    const char		  *what,
    stp_parameter_class_t p_class,	/* I - Option class */
    stp_parameter_level_t p_level,	/* I - Option level */
    const char		  *language,	/* I - Language */
    const stp_string_list_t     *po)		/* I - Message catalog */
{
  char buf[64];
  const char *class = stp_i18n_lookup(po, parameter_class_names[p_class]);
  const char *level = stp_i18n_lookup(po, parameter_level_names[p_level]);
  size_t bytes = bytelen(class) + bytelen(level);
  snprintf(buf, 40, "%s%s%s", class, bytes < 39 ? " " : "", level);
  gpprintf(fp, "*%sGroup: C%dL%d/%s\n", what, p_class, p_level, buf);
  if (language && !strcmp(language, "C") && !strcmp(what, "Open"))
    {
      char		**all_langs = getlangs();/* All languages */
      const char *lang;
      int langnum;

      for (langnum = 0; all_langs[langnum]; langnum ++)
	{
	  const stp_string_list_t *altpo;

	  lang = all_langs[langnum];

	  if (!strcmp(lang, "C") || !strcmp(lang, "en"))
	    continue;
	  if ((altpo = stp_i18n_load(lang)) != NULL)
	    {
	      class = stp_i18n_lookup(altpo, parameter_class_names[p_class]);
	      level = stp_i18n_lookup(altpo, parameter_level_names[p_level]);
	      bytes = bytelen(class) + bytelen(level);
	      snprintf(buf, 40, "%s%s%s", class, bytes < 39 ? " " : "", level);
	      gpprintf(fp, "*%s.Translation C%dL%d/%s: \"\"\n",
		       lang, p_class, p_level, buf);
            }
	}
    }
  gpputs(fp, "\n");
}

/*
 * 'print_group_close()' - Close a UI group.
 */

static void
print_group_close(
    gpFile                fp,		/* I - File to write to */
    stp_parameter_class_t p_class,	/* I - Option class */
    stp_parameter_level_t p_level,	/* I - Option level */
    const char		 *language,	/* I - language */
    const stp_string_list_t    *po)		/* I - Message catalog */
{
  print_group(fp, "Close", p_class, p_level, NULL, NULL);
}


/*
 * 'print_group_open()' - Open a new UI group.
 */

static void
print_group_open(
    gpFile                fp,		/* I - File to write to */
    stp_parameter_class_t p_class,	/* I - Option class */
    stp_parameter_level_t p_level,	/* I - Option level */
    const char		 *language,	/* I - language */
    const stp_string_list_t    *po)		/* I - Message catalog */
{
  print_group(fp, "Open", p_class, p_level, language ? language : "C", po);
}

static void
print_one_option(gpFile fp, stp_vars_t *v, const stp_string_list_t *po,
		 ppd_type_t ppd_type, const stp_parameter_t *lparam,
		 const stp_parameter_t *desc)
{
  int num_opts;
  int i;
  const stp_param_string_t *opt;
  int printed_default_value = 0;
  int simplified = ppd_type == PPD_SIMPLIFIED;
  char		dimstr[255];		/* Dimension string */
  int print_close_ui = 1;
  int is_color_opt = stp_parameter_has_category_value(v, desc, "Color", "Yes");
  int skip_color = (ppd_type == PPD_NO_COLOR_OPTS && is_color_opt);
  if (is_color_opt)
    gpprintf(fp, "*ColorKeyWords: \"Stp%s\"\n", desc->name);
  gpprintf(fp, "*OpenUI *Stp%s/%s: PickOne\n",
	   desc->name, stp_i18n_lookup(po, desc->text));
  gpprintf(fp, "*OrderDependency: 10 AnySetup *Stp%s\n", desc->name);
  switch (desc->p_type)
    {
    case STP_PARAMETER_TYPE_STRING_LIST:
      num_opts = stp_string_list_count(desc->bounds.str);
      if (! skip_color)
	{
	  if (num_opts > 3)
	    gpprintf(fp, "*OPOptionHints Stp%s: \"dropdown\"\n", lparam->name);
	  else
	    gpprintf(fp, "*OPOptionHints Stp%s: \"radiobuttons\"\n", lparam->name);
	}
      gpprintf(fp, "*StpStp%s: %d %d %d %d %d %.3f %.3f %.3f\n",
	       desc->name, desc->p_type, desc->is_mandatory, desc->p_class,
	       desc->p_level, desc->channel, 0.0, 0.0, 0.0);
      if (desc->is_mandatory)
	{
	  gpprintf(fp, "*DefaultStp%s: %s\n", desc->name, desc->deflt.str);
	  gpprintf(fp, "*StpDefaultStp%s: %s\n", desc->name, desc->deflt.str);
	}
      else
	{
	  gpprintf(fp, "*DefaultStp%s: None\n", desc->name);
	  gpprintf(fp, "*StpDefaultStp%s: None\n", desc->name);
	  gpprintf(fp, "*Stp%s %s/%s: \"\"\n", desc->name, "None", _("None"));
	}
      for (i = 0; i < num_opts; i++)
	{
	  opt = stp_string_list_param(desc->bounds.str, i);
	  if (skip_color && strcmp(opt->name, desc->deflt.str) != 0)
	    gpprintf(fp, "*?Stp%s %s/%s: \"\"\n",
		     desc->name, opt->name, stp_i18n_lookup(po, opt->text));
	  else
	    gpprintf(fp, "*Stp%s %s/%s: \"\"\n",
		     desc->name, opt->name, stp_i18n_lookup(po, opt->text));
	}
      break;
    case STP_PARAMETER_TYPE_BOOLEAN:
      gpprintf(fp, "*OPOptionHints Stp%s: \"checkbox\"\n", lparam->name);
      gpprintf(fp, "*StpStp%s: %d %d %d %d %d %.3f %.3f %.3f\n",
	       desc->name, desc->p_type, desc->is_mandatory, desc->p_class,
	       desc->p_level, desc->channel, 0.0, 0.0,
	       desc->deflt.boolean ? 1.0 : 0.0);
      if (desc->is_mandatory)
	{
	  gpprintf(fp, "*DefaultStp%s: %s\n", desc->name,
		   desc->deflt.boolean ? "True" : "False");
	  gpprintf(fp, "*StpDefaultStp%s: %s\n", desc->name,
		   desc->deflt.boolean ? "True" : "False");
	  if (skip_color)
	    gpprintf(fp, "*Stp%s %s/%s: \"\"\n",
		     desc->name, desc->deflt.boolean ? "True" : "False",
		     desc->deflt.boolean ? _("Yes") : _("No"));
	}
      else
	{
	  gpprintf(fp, "*DefaultStp%s: None\n", desc->name);
	  gpprintf(fp, "*StpDefaultStp%s: None\n", desc->name);
	  gpprintf(fp, "*Stp%s %s/%s: \"\"\n", desc->name, "None", _("None"));
	}
      gpprintf(fp, "*%sStp%s %s/%s: \"\"\n",
	       (skip_color ? "?" : ""), desc->name, "False", _("No"));
      gpprintf(fp, "*%sStp%s %s/%s: \"\"\n",
	       (skip_color ? "?" : ""), desc->name, "True", _("Yes"));
      break;
    case STP_PARAMETER_TYPE_DOUBLE:
      gpprintf(fp, "*OPOptionHints Stp%s: \"slider input spinbox\"\n",
	       lparam->name);
      gpprintf(fp, "*StpStp%s: %d %d %d %d %d %.3f %.3f %.3f\n",
	       desc->name, desc->p_type, desc->is_mandatory, desc->p_class,
	       desc->p_level, desc->channel, desc->bounds.dbl.lower,
	       desc->bounds.dbl.upper, desc->deflt.dbl);
      gpprintf(fp, "*DefaultStp%s: None\n", desc->name);
      gpprintf(fp, "*StpDefaultStp%s: None\n", desc->name);
      if (!skip_color)
	{
	  for (i = desc->bounds.dbl.lower * 1000;
	       i <= desc->bounds.dbl.upper * 1000 ; i += 100)
	    {
	      if (desc->deflt.dbl * 1000 == i && desc->is_mandatory)
		{
		  gpprintf(fp, "*Stp%s None/%.3f: \"\"\n",
			   desc->name, ((double) i) * .001);
		  printed_default_value = 1;
		}
	      else
		gpprintf(fp, "*Stp%s %d/%.3f: \"\"\n",
			 desc->name, i, ((double) i) * .001);
	    }
	}
      if (!desc->is_mandatory)
	gpprintf(fp, "*Stp%s None/%s: \"\"\n", desc->name, _("None"));
      else if (! printed_default_value)
	gpprintf(fp, "*Stp%s None/%.3f: \"\"\n", desc->name, desc->deflt.dbl);
      gpprintf(fp, "*CloseUI: *Stp%s\n\n", desc->name);

      /*
       * Add custom option code and value parameter...
       */

      gpprintf(fp, "*CustomStp%s True: \"pop\"\n", desc->name);
      gpprintf(fp, "*ParamCustomStp%s Value/%s: 1 real %.3f %.3f\n\n",
	       desc->name, _("Value"),  desc->bounds.dbl.lower,
	       desc->bounds.dbl.upper);
      if (!simplified && !skip_color)
	{
	  if (is_color_opt)
	    gpprintf(fp, "*ColorKeyWords: \"StpFine%s\"\n", desc->name);
	  gpprintf(fp, "*OpenUI *StpFine%s/%s %s: PickOne\n",
		   desc->name, stp_i18n_lookup(po, desc->text),
		   _("Fine Adjustment"));
	  gpprintf(fp, "*OPOptionHints StpFine%s: \"hide\"\n", lparam->name);
	  gpprintf(fp, "*StpStpFine%s: %d %d %d %d %d %.3f %.3f %.3f\n",
		   desc->name, STP_PARAMETER_TYPE_INVALID, 0,
		   0, 0, -1, 0.0, 0.0, 0.0);
	  gpprintf(fp, "*DefaultStpFine%s: None\n", desc->name);
	  gpprintf(fp, "*StpDefaultStpFine%s: None\n", desc->name);
	  gpprintf(fp, "*StpFine%s None/0.000: \"\"\n", desc->name);
	  for (i = 0; i < 100; i += 5)
	    gpprintf(fp, "*StpFine%s %d/%.3f: \"\"\n",
		     desc->name, i, ((double) i) * .001);
	  gpprintf(fp, "*CloseUI: *StpFine%s\n\n", desc->name);
	}
      print_close_ui = 0;

      break;
    case STP_PARAMETER_TYPE_DIMENSION:
      gpprintf(fp, "*OPOptionHints Stp%s: \"length slider input spinbox\"\n",
	       lparam->name);
      gpprintf(fp, "*StpStp%s: %d %d %d %d %d %.3f %.3f %.3f\n",
	       desc->name, desc->p_type, desc->is_mandatory,
	       desc->p_class, desc->p_level, desc->channel,
	       (double) desc->bounds.dimension.lower,
	       (double) desc->bounds.dimension.upper,
	       (double) desc->deflt.dimension);
      if (desc->is_mandatory)
	{
	  gpprintf(fp, "*DefaultStp%s: %d\n",
		   desc->name, desc->deflt.dimension);
	  gpprintf(fp, "*StpDefaultStp%s: %d\n",
		   desc->name, desc->deflt.dimension);
	}
      else
	{
	  gpprintf(fp, "*DefaultStp%s: None\n", desc->name);
	  gpprintf(fp, "*StpDefaultStp%s: None\n", desc->name);
	  gpprintf(fp, "*Stp%s %s/%s: \"\"\n", desc->name, "None", _("None"));
	}
      if (!skip_color)
	{
	  for (i = desc->bounds.dimension.lower;
	       i <= desc->bounds.dimension.upper; i++)
	    {
	      snprintf(dimstr, sizeof(dimstr), _("%.1f mm"),
		       (double)i * 25.4 / 72.0);
	      gpprintf(fp, "*Stp%s %d/%s: \"\"\n", desc->name, i, dimstr);
	    }
	}

      print_close_ui = 0;
      gpprintf(fp, "*CloseUI: *Stp%s\n\n", desc->name);

#if 0
      /* This needs to be enabled if/when dimensions become floating point */
      /*
       * Add custom option code and value parameter...
       */

      gpprintf(fp, "*CustomStp%s True: \"pop\"\n", desc->name);
      gpprintf(fp, "*ParamCustomStp%s Value/%s: 1 points %d %d\n\n",
	       desc->name, _("Value"), desc->bounds.dimension.lower,
	       desc->bounds.dimension.upper);
#endif

      break;
    case STP_PARAMETER_TYPE_INT:
      gpprintf(fp, "*OPOptionHints Stp%s: \"input spinbox\"\n", lparam->name);
      gpprintf(fp, "*StpStp%s: %d %d %d %d %d %.3f %.3f %.3f\n",
	       desc->name, desc->p_type, desc->is_mandatory, desc->p_class,
	       desc->p_level, desc->channel,
	       (double) desc->bounds.integer.lower,
	       (double) desc->bounds.integer.upper,
	       (double) desc->deflt.integer);
      if (desc->is_mandatory)
	{
	  gpprintf(fp, "*DefaultStp%s: %d\n", desc->name, desc->deflt.integer);
	  gpprintf(fp, "*StpDefaultStp%s: %d\n", desc->name, desc->deflt.integer);
	  if (skip_color)
	    gpprintf(fp, "*Stp%s %d/%d: \"\"\n", desc->name,
		     desc->deflt.integer, desc->deflt.integer);
	}
      else
	{
	  gpprintf(fp, "*DefaultStp%s: None\n", desc->name);
	  gpprintf(fp, "*StpDefaultStp%s: None\n", desc->name);
	  gpprintf(fp, "*Stp%s %s/%s: \"\"\n", desc->name, "None", _("None"));
	}
      for (i = desc->bounds.integer.lower; i <= desc->bounds.integer.upper; i++)
	{
	  gpprintf(fp, "*%sStp%s %d/%d: \"\"\n",
		   (skip_color ? "?" : ""), desc->name, i, i);
	}

      print_close_ui = 0;
      gpprintf(fp, "*CloseUI: *Stp%s\n\n", desc->name);

      break;
    default:
      break;
    }
  if (print_close_ui)
    gpprintf(fp, "*CloseUI: *Stp%s\n\n", desc->name);
}

static void
print_one_localization(gpFile fp, const stp_string_list_t *po,
		       int simplified, const char *lang,
		       const stp_parameter_t *lparam,
		       const stp_parameter_t *desc)
{
  int num_opts;
  int i;
  const stp_param_string_t *opt;
  char		dimstr[255];		/* Dimension string */

  gpprintf(fp, "*%s.Translation Stp%s/%s: \"\"\n", lang,
	   desc->name, stp_i18n_lookup(po, desc->text));
  switch (desc->p_type)
    {
    case STP_PARAMETER_TYPE_STRING_LIST:
      if (!desc->is_mandatory)
	gpprintf(fp, "*%s.Stp%s %s/%s: \"\"\n", lang, desc->name,
		 "None", _("None"));
      num_opts = stp_string_list_count(desc->bounds.str);
      for (i = 0; i < num_opts; i++)
	{
	  opt = stp_string_list_param(desc->bounds.str, i);
	  gpprintf(fp, "*%s.Stp%s %s/%s: \"\"\n", lang,
		   desc->name, opt->name, stp_i18n_lookup(po, opt->text));
	}
      break;

    case STP_PARAMETER_TYPE_BOOLEAN:
      if (!desc->is_mandatory)
	gpprintf(fp, "*%s.Stp%s %s/%s: \"\"\n", lang, desc->name,
		 "None", _("None"));
      gpprintf(fp, "*%s.Stp%s %s/%s: \"\"\n", lang, desc->name, "False", _("No"));
      gpprintf(fp, "*%s.Stp%s %s/%s: \"\"\n", lang, desc->name, "True", _("Yes"));
      break;

    case STP_PARAMETER_TYPE_DOUBLE:
      if (localize_numbers)
	{
	  for (i = desc->bounds.dbl.lower * 1000;
	       i <= desc->bounds.dbl.upper * 1000; i += 100)
	    {
	      if (desc->deflt.dbl * 1000 == i && desc->is_mandatory)
		gpprintf(fp, "*%s.Stp%s None/%.3f: \"\"\n", lang,
			 desc->name, ((double) i) * .001);
	      else
		gpprintf(fp, "*%s.Stp%s %d/%.3f: \"\"\n", lang,
			 desc->name, i, ((double) i) * .001);
	    }
	}
      if (!desc->is_mandatory)
	gpprintf(fp, "*%s.Stp%s None/%s: \"\"\n", lang, desc->name, _("None"));
      gpprintf(fp, "*%s.ParamCustomStp%s Value/%s: \"\"\n", lang,
	       desc->name, _("Value"));
      if (!simplified)
	{
	  gpprintf(fp, "*%s.Translation StpFine%s/%s %s: \"\"\n", lang,
		   desc->name, stp_i18n_lookup(po, desc->text),
		   _("Fine Adjustment"));
	  gpprintf(fp, "*%s.StpFine%s None/%.3f: \"\"\n", lang,
		   desc->name, 0.0);
	  if (localize_numbers)
	    {
	      for (i = 0; i < 100; i += 5)
		gpprintf(fp, "*%s.StpFine%s %d/%.3f: \"\"\n", lang,
			 desc->name, i, ((double) i) * .001);
	    }
	}
      break;

    case STP_PARAMETER_TYPE_DIMENSION:
      if (!desc->is_mandatory)
	gpprintf(fp, "*%s.Stp%s %s/%s: \"\"\n", lang, desc->name,
		 "None", _("None"));
      /* Unlike the other fields, dimensions are not strictly numbers */
      for (i = desc->bounds.dimension.lower;
	   i <= desc->bounds.dimension.upper; i++)
	{
	  snprintf(dimstr, sizeof(dimstr), _("%.1f mm"),
		   (double)i * 25.4 / 72.0);
	  gpprintf(fp, "*%s.Stp%s %d/%s: \"\"\n", lang,
		   desc->name, i, dimstr);
	}
#if 0
      /* This needs to be enabled if/when dimensions are floating point */
      gpprintf(fp, "*%s.ParamCustomStp%s Value/%s: \"\"\n", lang,
	       desc->name, _("Value"));
#endif
      break;

    case STP_PARAMETER_TYPE_INT:
      if (!desc->is_mandatory)
	gpprintf(fp, "*%s.Stp%s %s/%s: \"\"\n", lang, desc->name,
		 "None", _("None"));
      if (localize_numbers)
	{
	  for (i = desc->bounds.integer.lower;
	       i <= desc->bounds.integer.upper; i++)
	    {
	      gpprintf(fp, "*%s.Stp%s %d/%d: \"\"\n", lang, desc->name, i, i);
	    }
	}
      break;

    default:
      break;
    }
}

static void
print_standard_fonts(gpFile fp)
{
  gpputs(fp, "\n*DefaultFont: Courier\n");
  gpputs(fp, "*Font AvantGarde-Book: Standard \"(001.006S)\" Standard ROM\n");
  gpputs(fp, "*Font AvantGarde-BookOblique: Standard \"(001.006S)\" Standard ROM\n");
  gpputs(fp, "*Font AvantGarde-Demi: Standard \"(001.007S)\" Standard ROM\n");
  gpputs(fp, "*Font AvantGarde-DemiOblique: Standard \"(001.007S)\" Standard ROM\n");
  gpputs(fp, "*Font Bookman-Demi: Standard \"(001.004S)\" Standard ROM\n");
  gpputs(fp, "*Font Bookman-DemiItalic: Standard \"(001.004S)\" Standard ROM\n");
  gpputs(fp, "*Font Bookman-Light: Standard \"(001.004S)\" Standard ROM\n");
  gpputs(fp, "*Font Bookman-LightItalic: Standard \"(001.004S)\" Standard ROM\n");
  gpputs(fp, "*Font Courier: Standard \"(002.004S)\" Standard ROM\n");
  gpputs(fp, "*Font Courier-Bold: Standard \"(002.004S)\" Standard ROM\n");
  gpputs(fp, "*Font Courier-BoldOblique: Standard \"(002.004S)\" Standard ROM\n");
  gpputs(fp, "*Font Courier-Oblique: Standard \"(002.004S)\" Standard ROM\n");
  gpputs(fp, "*Font Helvetica: Standard \"(001.006S)\" Standard ROM\n");
  gpputs(fp, "*Font Helvetica-Bold: Standard \"(001.007S)\" Standard ROM\n");
  gpputs(fp, "*Font Helvetica-BoldOblique: Standard \"(001.007S)\" Standard ROM\n");
  gpputs(fp, "*Font Helvetica-Narrow: Standard \"(001.006S)\" Standard ROM\n");
  gpputs(fp, "*Font Helvetica-Narrow-Bold: Standard \"(001.007S)\" Standard ROM\n");
  gpputs(fp, "*Font Helvetica-Narrow-BoldOblique: Standard \"(001.007S)\" Standard ROM\n");
  gpputs(fp, "*Font Helvetica-Narrow-Oblique: Standard \"(001.006S)\" Standard ROM\n");
  gpputs(fp, "*Font Helvetica-Oblique: Standard \"(001.006S)\" Standard ROM\n");
  gpputs(fp, "*Font NewCenturySchlbk-Bold: Standard \"(001.009S)\" Standard ROM\n");
  gpputs(fp, "*Font NewCenturySchlbk-BoldItalic: Standard \"(001.007S)\" Standard ROM\n");
  gpputs(fp, "*Font NewCenturySchlbk-Italic: Standard \"(001.006S)\" Standard ROM\n");
  gpputs(fp, "*Font NewCenturySchlbk-Roman: Standard \"(001.007S)\" Standard ROM\n");
  gpputs(fp, "*Font Palatino-Bold: Standard \"(001.005S)\" Standard ROM\n");
  gpputs(fp, "*Font Palatino-BoldItalic: Standard \"(001.005S)\" Standard ROM\n");
  gpputs(fp, "*Font Palatino-Italic: Standard \"(001.005S)\" Standard ROM\n");
  gpputs(fp, "*Font Palatino-Roman: Standard \"(001.005S)\" Standard ROM\n");
  gpputs(fp, "*Font Symbol: Special \"(001.007S)\" Special ROM\n");
  gpputs(fp, "*Font Times-Bold: Standard \"(001.007S)\" Standard ROM\n");
  gpputs(fp, "*Font Times-BoldItalic: Standard \"(001.009S)\" Standard ROM\n");
  gpputs(fp, "*Font Times-Italic: Standard \"(001.007S)\" Standard ROM\n");
  gpputs(fp, "*Font Times-Roman: Standard \"(001.007S)\" Standard ROM\n");
  gpputs(fp, "*Font ZapfChancery-MediumItalic: Standard \"(001.007S)\" Standard ROM\n");
  gpputs(fp, "*Font ZapfDingbats: Special \"(001.004S)\" Standard ROM\n");
}

/*
 * 'write_ppd()' - Write a PPD file.
 */

static int				/* O - Exit status */
write_ppd(
    gpFile              fp,		/* I - File to write to */
    const stp_printer_t *p,		/* I - Printer driver */
    const char          *language,	/* I - Primary language */
    const char		*ppd_location,	/* I - Location of PPD file */
    ppd_type_t          ppd_type,	/* I - 1 = simplified options */
    const char		*filename)	/* I - input filename */
{
  int		i, j, k, l;		/* Looping vars */
  int		num_opts;		/* Number of printer options */
  int		xdpi, ydpi;		/* Resolution info */
  stp_vars_t	*v;			/* Variable info */
  const char	*driver;		/* Driver name */
  const char	*family;		/* Printer family */
  int		model;			/* Internal model ID */
  const char	*long_name;		/* Driver long name */
  const char	*manufacturer;		/* Manufacturer of printer */
  const char	*device_id;		/* IEEE1284 device ID */
  const stp_vars_t *printvars;		/* Printer option names */
  int           nativecopies = 0;       /* Printer natively generates copies */
  stp_parameter_t desc;
  stp_parameter_list_t param_list;
  const stp_param_string_t *opt;
  int has_quality_parameter = 0;
  int printer_is_color = 0;
  int simplified = ppd_type == PPD_SIMPLIFIED;
  int skip_color = ppd_type == PPD_NO_COLOR_OPTS;
  int maximum_level = simplified ?
    STP_PARAMETER_LEVEL_BASIC : STP_PARAMETER_LEVEL_ADVANCED4;
  char		*default_resolution = NULL;  /* Default resolution mapped name */
  stp_string_list_t *resolutions = stp_string_list_create();
  char		**all_langs = getlangs();/* All languages */
  const stp_string_list_t	*po = stp_i18n_load(language);
					/* Message catalog */


 /*
  * Initialize driver-specific variables...
  */

  driver     = stp_printer_get_driver(p);
  family     = stp_printer_get_family(p);
  model      = stp_printer_get_model(p);
  long_name  = stp_printer_get_long_name(p);
  manufacturer = stp_printer_get_manufacturer(p);
  device_id  = stp_printer_get_device_id(p);
  printvars  = stp_printer_get_defaults(p);

  print_ppd_header(fp, ppd_type, model, driver, family, long_name,
		   manufacturer, device_id, ppd_location, language, po,
		   all_langs);


  /* Set Job Mode to "Job" as this enables the Duplex option */
  v = stp_vars_create_copy(printvars);
  stp_set_string_parameter(v, "JobMode", "Job");

  /* Assume that color printers are inkjets and should have pages reversed */
  stp_describe_parameter(v, "PrintingMode", &desc);
  if (desc.p_type == STP_PARAMETER_TYPE_STRING_LIST)
    {
      if (stp_string_list_is_present(desc.bounds.str, "Color"))
	{
	  printer_is_color = 1;
	  gpputs(fp, "*ColorDevice:	True\n");
	}
      else
	{
	  printer_is_color = 0;
	  gpputs(fp, "*ColorDevice:	False\n");
	}
      if (strcmp(desc.deflt.str, "Color") == 0)
	gpputs(fp, "*DefaultColorSpace:	RGB\n");
      else
	gpputs(fp, "*DefaultColorSpace:	Gray\n");
    }
  stp_parameter_description_destroy(&desc);

  stp_describe_parameter(v, "NativeCopies", &desc);
  if (desc.p_type == STP_PARAMETER_TYPE_BOOLEAN)
    nativecopies = stp_get_boolean_parameter(v, "NativeCopies");

  stp_parameter_description_destroy(&desc);

  if (nativecopies)
    gpputs(fp, "*cupsManualCopies: False\n");
  else
    gpputs(fp, "*cupsManualCopies: True\n");

  print_ppd_header_3(fp, ppd_type, model,
		     driver, family, long_name,
		     manufacturer, device_id, ppd_location, language, po,
		     all_langs);

  /* Macintosh color management */

#ifdef __APPLE__
  gpputs(fp, "*cupsICCProfile Gray../Grayscale:	\"/System/Library/ColorSync/Profiles/sRGB Profile.icc\"\n");
  gpputs(fp, "*cupsICCProfile RGB../Color:	\"/System/Library/ColorSync/Profiles/sRGB Profile.icc\"\n");
  gpputs(fp, "*cupsICCProfile CMYK../Color:	\"/System/Library/ColorSync/Profiles/Generic CMYK Profile.icc\"\n");
  gpputs(fp, "*APSupportsCustomColorMatching: true\n");
  gpputs(fp, "*APDefaultCustomColorMatchingProfile: sRGB\n");
  gpputs(fp, "*APCustomColorMatchingProfile: sRGB\n");
#endif

  gpputs(fp, "\n");

  print_ppd_header_2(fp, ppd_type, model, driver, family, long_name,
		     manufacturer, device_id, ppd_location, language, po,
		     all_langs);

 /*
  * Get the page sizes from the driver...
  */

  if (printer_is_color)
    stp_set_string_parameter(v, "PrintingMode", "Color");
  else
    stp_set_string_parameter(v, "PrintingMode", "BW");
  stp_set_string_parameter(v, "ChannelBitDepth", "8");
  print_page_sizes(fp, v, simplified, po);

 /*
  * Do we support color?
  */

  print_color_setup(fp, simplified, printer_is_color, po);

 /*
  * Media types...
  */

  stp_describe_parameter(v, "MediaType", &desc);

  if (desc.p_type == STP_PARAMETER_TYPE_STRING_LIST && desc.is_active &&
      stp_string_list_count(desc.bounds.str) > 0)
  {
    int is_color_opt =
      stp_parameter_has_category_value(v, &desc, "Color", "Yes");
    int nocolor = skip_color && is_color_opt;
    num_opts = stp_string_list_count(desc.bounds.str);
    if (is_color_opt)
      gpprintf(fp, "*ColorKeyWords: \"MediaType\"\n");
    gpprintf(fp, "*OpenUI *MediaType/%s: PickOne\n", _("Media Type"));
    gpputs(fp, "*OPOptionHints MediaType: \"dropdown\"\n");
    gpputs(fp, "*OrderDependency: 10 AnySetup *MediaType\n");
    gpprintf(fp, "*StpStp%s: %d %d %d %d %d %.3f %.3f %.3f\n",
	     desc.name, desc.p_type, desc.is_mandatory,
	     desc.p_class, desc.p_level, desc.channel, 0.0, 0.0, 0.0);
    gpprintf(fp, "*DefaultMediaType: %s\n", desc.deflt.str);
    gpprintf(fp, "*StpDefaultMediaType: %s\n", desc.deflt.str);

    for (i = 0; i < num_opts; i ++)
    {
      opt = stp_string_list_param(desc.bounds.str, i);
      gpprintf(fp, "*%sMediaType %s/%s:\t\"<</MediaType(%s)>>setpagedevice\"\n",
	       nocolor && strcmp(opt->name, desc.deflt.str) != 0 ? "?" : "",
               opt->name, stp_i18n_lookup(po, opt->text), opt->name);
    }

    gpputs(fp, "*CloseUI: *MediaType\n\n");
  }
  stp_parameter_description_destroy(&desc);

 /*
  * Input slots...
  */

  stp_describe_parameter(v, "InputSlot", &desc);

  if (desc.p_type == STP_PARAMETER_TYPE_STRING_LIST && desc.is_active &&
      stp_string_list_count(desc.bounds.str) > 0)
  {
    int is_color_opt =
      stp_parameter_has_category_value(v, &desc, "Color", "Yes");
    int nocolor = skip_color && is_color_opt;
    num_opts = stp_string_list_count(desc.bounds.str);
    if (is_color_opt)
      gpprintf(fp, "*ColorKeyWords: \"InputSlot\"\n");
    gpprintf(fp, "*OpenUI *InputSlot/%s: PickOne\n", _("Media Source"));
    gpputs(fp, "*OPOptionHints InputSlot: \"dropdown\"\n");
    gpputs(fp, "*OrderDependency: 10 AnySetup *InputSlot\n");
    gpprintf(fp, "*StpStp%s: %d %d %d %d %d %.3f %.3f %.3f\n",
	     desc.name, desc.p_type, desc.is_mandatory,
	     desc.p_class, desc.p_level, desc.channel, 0.0, 0.0, 0.0);
    gpprintf(fp, "*DefaultInputSlot: %s\n", desc.deflt.str);
    gpprintf(fp, "*StpDefaultInputSlot: %s\n", desc.deflt.str);

    for (i = 0; i < num_opts; i ++)
    {
      opt = stp_string_list_param(desc.bounds.str, i);
      gpprintf(fp, "*%sInputSlot %s/%s:\t\"<</MediaClass(%s)>>setpagedevice\"\n",
	       nocolor && strcmp(opt->name, desc.deflt.str) != 0 ? "?" : "",
               opt->name, stp_i18n_lookup(po, opt->text), opt->name);
    }

    gpputs(fp, "*CloseUI: *InputSlot\n\n");
  }
  stp_parameter_description_destroy(&desc);

 /*
  * Quality settings
  */

  stp_describe_parameter(v, "Quality", &desc);
  if (desc.p_type == STP_PARAMETER_TYPE_STRING_LIST && desc.is_active)
    {
      int is_color_opt =
	stp_parameter_has_category_value(v, &desc, "Color", "Yes");
      int nocolor = skip_color && is_color_opt;
      if (is_color_opt)
	gpprintf(fp, "*ColorKeyWords: \"Quality\"\n");
      stp_clear_string_parameter(v, "Resolution");
      has_quality_parameter = 1;
      num_opts = stp_string_list_count(desc.bounds.str);
      gpprintf(fp, "*OpenUI *StpQuality/%s: PickOne\n", stp_i18n_lookup(po, desc.text));
      if (num_opts > 3)
	gpputs(fp, "*OPOptionHints Quality: \"radiobuttons\"\n");
      else
	gpputs(fp, "*OPOptionHints Quality: \"dropdown\"\n");
      gpputs(fp, "*OrderDependency: 10 AnySetup *StpQuality\n");
      gpprintf(fp, "*StpStp%s: %d %d %d %d %d %.3f %.3f %.3f\n",
	       desc.name, desc.p_type, desc.is_mandatory,
	       desc.p_type, desc.p_level, desc.channel, 0.0, 0.0, 0.0);
      gpprintf(fp, "*DefaultStpQuality: %s\n", desc.deflt.str);
      gpprintf(fp, "*StpDefaultStpQuality: %s\n", desc.deflt.str);
      for (i = 0; i < num_opts; i++)
	{
	  opt = stp_string_list_param(desc.bounds.str, i);
	  stp_set_string_parameter(v, "Quality", opt->name);
	  stp_describe_resolution(v, &xdpi, &ydpi);
	  if (xdpi == -1 || ydpi == -1)
	    {
	      stp_parameter_t res_desc;
	      stp_clear_string_parameter(v, "Quality");
	      stp_describe_parameter(v, "Resolution", &res_desc);
	      stp_set_string_parameter(v, "Resolution", res_desc.deflt.str);
	      stp_describe_resolution(v, &xdpi, &ydpi);
	      stp_clear_string_parameter(v, "Resolution");
	      stp_parameter_description_destroy(&res_desc);
	    }
	  gpprintf(fp, "*%sStpQuality %s/%s:\t\"<</HWResolution[%d %d]/cupsRowFeed %d>>setpagedevice\"\n",
		   nocolor && strcmp(opt->name, desc.deflt.str) != 0 ? "?" : "",
		   opt->name, stp_i18n_lookup(po, opt->text), xdpi, ydpi, i + 1);
	}
      gpputs(fp, "*CloseUI: *StpQuality\n\n");
    }
  stp_parameter_description_destroy(&desc);
  stp_clear_string_parameter(v, "Quality");

 /*
  * Resolutions...
  */

  stp_describe_parameter(v, "Resolution", &desc);

  if (desc.p_type == STP_PARAMETER_TYPE_STRING_LIST && desc.is_active)
    {
      num_opts = stp_string_list_count(desc.bounds.str);
      if (!simplified || desc.p_level == STP_PARAMETER_LEVEL_BASIC)
	{
	  int is_color_opt =
	    stp_parameter_has_category_value(v, &desc, "Color", "Yes");
	  int nocolor = skip_color && is_color_opt;
	  stp_string_list_t *res_list = stp_string_list_create();
	  char res_name[64];	/* Plenty long enough for XXXxYYYdpi */
	  int resolution_ok;
	  int tmp_xdpi, tmp_ydpi;

	  if (is_color_opt)
	    gpprintf(fp, "*ColorKeyWords: \"Resolution\"\n");
	  gpprintf(fp, "*OpenUI *Resolution/%s: PickOne\n", _("Resolution"));
	  if (num_opts > 3)
	    gpputs(fp, "*OPOptionHints Resolution: \"resolution radiobuttons\"\n");
	  else
	    gpputs(fp, "*OPOptionHints Resolution: \"resolution dropdown\"\n");
	  gpputs(fp, "*OrderDependency: 10 AnySetup *Resolution\n");
	  gpprintf(fp, "*StpStp%s: %d %d %d %d %d %.3f %.3f %.3f\n",
		   desc.name, desc.p_type, desc.is_mandatory,
		   desc.p_class, desc.p_level, desc.channel, 0.0, 0.0, 0.0);
	  if (has_quality_parameter)
	    {
	      stp_parameter_t desc1;
	      stp_clear_string_parameter(v, "Resolution");
	      stp_describe_parameter(v, "Quality", &desc1);
	      stp_set_string_parameter(v, "Quality", desc1.deflt.str);
	      stp_parameter_description_destroy(&desc1);
	      stp_describe_resolution(v, &xdpi, &ydpi);
	      stp_clear_string_parameter(v, "Quality");
	      tmp_xdpi = xdpi;
	      while (tmp_xdpi > MAXIMUM_SAFE_PPD_X_RESOLUTION)
		tmp_xdpi /= 2;
	      tmp_ydpi = ydpi;
	      while (tmp_ydpi > MAXIMUM_SAFE_PPD_Y_RESOLUTION)
		tmp_ydpi /= 2;
	      if (tmp_ydpi < tmp_xdpi)
		tmp_xdpi = tmp_ydpi;
	      /*
		Make the default resolution look like an almost square resolution
		so that applications using it will be less likely to generate
		excess resolution.  However, make the hardware resolution
		match the printer default.
	      */
	      (void) snprintf(res_name, 63, "%dx%ddpi", tmp_xdpi + 1, tmp_xdpi);
	      default_resolution = stp_strdup(res_name);
	      stp_string_list_add_string(res_list, res_name, res_name);
	      gpprintf(fp, "*DefaultResolution: %s\n", res_name);
	      gpprintf(fp, "*StpDefaultResolution: %s\n", res_name);
	      gpprintf(fp, "*Resolution %s/%s:\t\"<</HWResolution[%d %d]>>setpagedevice\"\n",
		       res_name, _("Automatic"), xdpi, ydpi);
	      gpprintf(fp, "*StpResolutionMap: %s %s\n", res_name, "None");
	    }
	  else
	    {
	      stp_set_string_parameter(v, "Resolution", desc.deflt.str);
	      stp_describe_resolution(v, &xdpi, &ydpi);

	      if (xdpi == ydpi)
		(void) snprintf(res_name, 63, "%ddpi", xdpi);
	      else
		(void) snprintf(res_name, 63, "%dx%ddpi", xdpi, ydpi);
	      gpprintf(fp, "*DefaultResolution: %s\n", res_name);
	      gpprintf(fp, "*StpDefaultResolution: %s\n", res_name);
	      /*
	       * We need to add this to the resolution list here so that
	       * some non-default resolution won't wind up with the
	       * default resolution name
	       */
	      stp_string_list_add_string(res_list, res_name, res_name);
	    }

	  stp_clear_string_parameter(v, "Quality");
	  for (i = 0; i < num_opts; i ++)
	    {
	      /*
	       * Strip resolution name to its essentials...
	       */
	      opt = stp_string_list_param(desc.bounds.str, i);
	      stp_set_string_parameter(v, "Resolution", opt->name);
	      stp_describe_resolution(v, &xdpi, &ydpi);

	      /* This should only happen with a "None" resolution */
	      if (xdpi == -1 || ydpi == -1)
		continue;

	      resolution_ok = 0;
	      tmp_xdpi = xdpi;
	      while (tmp_xdpi > MAXIMUM_SAFE_PPD_X_RESOLUTION)
		tmp_xdpi /= 2;
	      tmp_ydpi = ydpi;
	      while (tmp_ydpi > MAXIMUM_SAFE_PPD_Y_RESOLUTION)
		tmp_ydpi /= 2;
	      do
		{
		  if (tmp_xdpi == tmp_ydpi)
		    (void) snprintf(res_name, 63, "%ddpi", tmp_xdpi);
		  else
		    (void) snprintf(res_name, 63, "%dx%ddpi", tmp_xdpi, tmp_ydpi);
		  if ((!has_quality_parameter &&
		       strcmp(opt->name, desc.deflt.str) == 0) ||
		      !stp_string_list_is_present(res_list, res_name))
		    {
		      resolution_ok = 1;
		      stp_string_list_add_string(res_list, res_name, opt->text);
		    }
		  else if (tmp_ydpi > tmp_xdpi &&
			   tmp_ydpi < MAXIMUM_SAFE_PPD_Y_RESOLUTION)
		    /* Note that we're incrementing the *higher* resolution.
		       This will generate less aliasing, and apps that convert
		       down to a square resolution will do the right thing. */
		    tmp_ydpi++;
		  else if (tmp_xdpi < MAXIMUM_SAFE_PPD_X_RESOLUTION)
		    tmp_xdpi++;
		  else
		    tmp_xdpi /= 2;
		} while (!resolution_ok);
	      stp_string_list_add_string(resolutions, res_name, opt->text);
	      gpprintf(fp, "*%sResolution %s/%s:\t\"<</HWResolution[%d %d]/cupsCompression %d>>setpagedevice\"\n",
		       nocolor && strcmp(opt->name, desc.deflt.str) != 0 ? "?" : "",
		       res_name, stp_i18n_lookup(po, opt->text), xdpi, ydpi, i + 1);
	      if (strcmp(res_name, opt->name) != 0)
		gpprintf(fp, "*StpResolutionMap: %s %s\n", res_name, opt->name);
	    }

	  stp_string_list_destroy(res_list);
	  stp_clear_string_parameter(v, "Resolution");
	  gpputs(fp, "*CloseUI: *Resolution\n\n");
	}
    }

  stp_parameter_description_destroy(&desc);

  stp_describe_parameter(v, "OutputOrder", &desc);
  if (desc.p_type == STP_PARAMETER_TYPE_STRING_LIST)
    {
      gpprintf(fp, "*OpenUI *OutputOrder/%s: PickOne\n", _("Output Order"));
      gpputs(fp, "*OPOptionHints OutputOrder: \"radiobuttons\"\n");
      gpputs(fp, "*OrderDependency: 10 AnySetup *OutputOrder\n");
      gpprintf(fp, "*DefaultOutputOrder: %s\n", desc.deflt.str);
      gpprintf(fp, "*StpDefaultOutputOrder: %s\n", desc.deflt.str);
      gpprintf(fp, "*OutputOrder Normal/%s: \"\"\n", _("Normal"));
      gpprintf(fp, "*OutputOrder Reverse/%s: \"\"\n", _("Reverse"));
      gpputs(fp, "*CloseUI: *OutputOrder\n\n");
    }
  stp_parameter_description_destroy(&desc);

 /*
  * Duplex
  * Note that the opt->name strings MUST match those in the printer driver(s)
  * else the PPD files will not be generated correctly
  */

  stp_describe_parameter(v, "Duplex", &desc);
  if (desc.is_active && desc.p_type == STP_PARAMETER_TYPE_STRING_LIST)
    {
      num_opts = stp_string_list_count(desc.bounds.str);
      if (num_opts > 0)
      {
	int is_color_opt =
	  stp_parameter_has_category_value(v, &desc, "Color", "Yes");
	if (is_color_opt)
	  gpprintf(fp, "*ColorKeyWords: \"InputSlot\"\n");
        gpprintf(fp, "*OpenUI *Duplex/%s: PickOne\n", _("2-Sided Printing"));
	gpputs(fp, "*OPOptionHints Duplex: \"radiobuttons\"\n");
        gpputs(fp, "*OrderDependency: 10 AnySetup *Duplex\n");
	gpprintf(fp, "*StpStp%s: %d %d %d %d %d %.3f %.3f %.3f\n",
		 desc.name, desc.p_type, desc.is_mandatory,
		 desc.p_class, desc.p_level, desc.channel, 0.0, 0.0, 0.0);
        gpprintf(fp, "*DefaultDuplex: %s\n", desc.deflt.str);
        gpprintf(fp, "*StpDefaultDuplex: %s\n", desc.deflt.str);

        for (i = 0; i < num_opts; i++)
          {
            opt = stp_string_list_param(desc.bounds.str, i);
            if (strcmp(opt->name, "None") == 0)
              gpprintf(fp, "*Duplex %s/%s: \"<</Duplex false>>setpagedevice\"\n", opt->name, stp_i18n_lookup(po, opt->text));
            else if (strcmp(opt->name, "DuplexNoTumble") == 0)
              gpprintf(fp, "*Duplex %s/%s: \"<</Duplex true/Tumble false>>setpagedevice\"\n", opt->name, stp_i18n_lookup(po, opt->text));
            else if (strcmp(opt->name, "DuplexTumble") == 0)
              gpprintf(fp, "*Duplex %s/%s: \"<</Duplex true/Tumble true>>setpagedevice\"\n", opt->name, stp_i18n_lookup(po, opt->text));
           }
        gpputs(fp, "*CloseUI: *Duplex\n\n");
      }
    }
  stp_parameter_description_destroy(&desc);

  gpprintf(fp, "*OpenUI *StpiShrinkOutput/%s: PickOne\n",
	   _("Shrink Page If Necessary to Fit Borders"));
  gpputs(fp, "*OPOptionHints StpiShrinkOutput: \"radiobuttons\"\n");
  gpputs(fp, "*OrderDependency: 10 AnySetup *StpiShrinkOutput\n");
  gpputs(fp, "*DefaultStpiShrinkOutput: Shrink\n");
  gpputs(fp, "*StpDefaultStpiShrinkOutput: Shrink\n");
  gpprintf(fp, "*StpiShrinkOutput %s/%s: \"\"\n", "Shrink", _("Shrink (print the whole page)"));
  gpprintf(fp, "*StpiShrinkOutput %s/%s: \"\"\n", "Crop", _("Crop (preserve dimensions)"));
  gpprintf(fp, "*StpiShrinkOutput %s/%s: \"\"\n", "Expand", _("Expand (use maximum page area)"));
  gpputs(fp, "*CloseUI: *StpiShrinkOutput\n\n");

  param_list = stp_get_parameter_list(v);

  for (j = 0; j <= STP_PARAMETER_CLASS_OUTPUT; j++)
    {
      for (k = 0; k <= maximum_level; k++)
	{
	  int printed_open_group = 0;
	  size_t param_count = stp_parameter_list_count(param_list);
	  for (l = 0; l < param_count; l++)
	    {
	      const stp_parameter_t *lparam =
		stp_parameter_list_param(param_list, l);
	      if (lparam->p_class != j || lparam->p_level != k ||
		  is_special_option(lparam->name) || lparam->read_only ||
		  (lparam->p_type != STP_PARAMETER_TYPE_STRING_LIST &&
		   lparam->p_type != STP_PARAMETER_TYPE_BOOLEAN &&
		   lparam->p_type != STP_PARAMETER_TYPE_DIMENSION &&
		   lparam->p_type != STP_PARAMETER_TYPE_INT &&
		   lparam->p_type != STP_PARAMETER_TYPE_DOUBLE))
		  continue;
	      stp_describe_parameter(v, lparam->name, &desc);
	      if (desc.is_active)
		{
		  if (!printed_open_group)
		    {
		      print_group_open(fp, j, k, language, po);
		      printed_open_group = 1;
		    }
		  print_one_option(fp, v, po, ppd_type, lparam, &desc);
		}
	      stp_parameter_description_destroy(&desc);
	    }
	  if (printed_open_group)
	    print_group_close(fp, j, k, language, po);
	}
    }
  stp_parameter_list_destroy(param_list);
  stp_describe_parameter(v, "ImageType", &desc);
  if (desc.is_active && desc.p_type == STP_PARAMETER_TYPE_STRING_LIST)
    {
      num_opts = stp_string_list_count(desc.bounds.str);
      if (num_opts > 0)
	{
	  for (i = 0; i < num_opts; i++)
	    {
	      opt = stp_string_list_param(desc.bounds.str, i);
	      if (strcmp(opt->name, "None") != 0)
		gpprintf(fp, "*APPrinterPreset %s/%s: \"*StpImageType %s\"\n",
			 opt->name, stp_i18n_lookup(po, opt->text), opt->name);
	    }
	  gpputs(fp, "\n");
	}
    }
  stp_parameter_description_destroy(&desc);

  /* Constraints */
  stp_describe_parameter(v, "PPDUIConstraints", &desc);
  if (desc.is_active && desc.p_type == STP_PARAMETER_TYPE_STRING_LIST)
    {
      num_opts = stp_string_list_count(desc.bounds.str);
      if (num_opts > 0)
	{
          gpputs(fp, "*% ===== Constraints ===== \n");
	  for (i = 0; i < num_opts; i++)
	    {
	      char *opt1, *opt2;
	      opt = stp_string_list_param(desc.bounds.str, i);
	      opt1 = stp_strdup(opt->text);
	      opt2 = strrchr(opt1, '*');
	      if (opt2)
	        {
		  opt2[-1] = 0;
		  gpprintf(fp, "*%s: %s %s\n", opt->name, opt1, opt2);
		  gpprintf(fp, "*%s: %s %s\n", opt->name, opt2, opt1);
		}
	      stp_free(opt1);
	    }
	  gpputs(fp, "\n");      
	}
    }  
  stp_parameter_description_destroy(&desc);

  if (!language)
    {
      /*
       * Generate globalized PPDs when POSIX language is requested...
       */

      const char *lang;
      const stp_string_list_t *savepo = po;
      int langnum;

      for (langnum = 0; all_langs[langnum]; langnum ++)
	{
	  lang = all_langs[langnum];

	  if (!strcmp(lang, "C") || !strcmp(lang, "en"))
	    continue;

	  if ((po = stp_i18n_load(lang)) == NULL)
	    continue;

	  /*
	   * Get the page sizes from the driver...
	   */

	  if (printer_is_color)
	    stp_set_string_parameter(v, "PrintingMode", "Color");
	  else
	    stp_set_string_parameter(v, "PrintingMode", "BW");
	  stp_set_string_parameter(v, "ChannelBitDepth", "8");
	  stp_describe_parameter(v, "PageSize", &desc);
	  num_opts = stp_string_list_count(desc.bounds.str);

	  gpprintf(fp, "*%s.Translation PageSize/%s: \"\"\n", lang, _("Media Size"));
	  gpprintf(fp, "*%s.Translation PageRegion/%s: \"\"\n", lang, _("Media Size"));

	  for (i = 0; i < num_opts; i++)
	    {
	      const stp_papersize_t *papersize;
	      opt = stp_string_list_param(desc.bounds.str, i);
	      papersize = stp_get_papersize_by_name(opt->name);

	      if (!papersize)
		continue;

	      /*
		if (strcmp(opt->name, "Custom") == 0)
		continue;
	      */

	      if (simplified && num_opts >= 10 &&
		  (papersize->paper_unit == PAPERSIZE_ENGLISH_EXTENDED ||
		   papersize->paper_unit == PAPERSIZE_METRIC_EXTENDED))
		continue;

	      if ((papersize->width <= 0 || papersize->height <= 0) &&
		  strcmp(opt->name, "Custom") != 0)
		continue;

	      gpprintf(fp, "*%s.PageSize %s/%s: \"\"\n", lang, opt->name, stp_i18n_lookup(po, opt->text));
	      gpprintf(fp, "*%s.PageRegion %s/%s: \"\"\n", lang, opt->name, stp_i18n_lookup(po, opt->text));
	    }

	  stp_parameter_description_destroy(&desc);

	  /*
	   * Do we support color?
	   */

	  gpprintf(fp, "*%s.Translation ColorModel/%s: \"\"\n", lang, _("Color Model"));
	  gpprintf(fp, "*%s.ColorModel Gray/%s: \"\"\n", lang, _("Grayscale"));
	  gpprintf(fp, "*%s.ColorModel Black/%s: \"\"\n", lang, _("Inverted Grayscale"));

	  if (printer_is_color)
	    {
	      gpprintf(fp, "*%s.ColorModel RGB/%s: \"\"\n", lang, _("RGB Color"));
	      gpprintf(fp, "*%s.ColorModel CMY/%s: \"\"\n", lang, _("CMY Color"));
	      gpprintf(fp, "*%s.ColorModel CMYK/%s: \"\"\n", lang, _("CMYK"));
	      gpprintf(fp, "*%s.ColorModel KCMY/%s: \"\"\n", lang, _("KCMY"));
	    }

	  if (!simplified)
	    {
	      /*
	       * 8 or 16 bit color (16 bit is slower)
	       */
	      gpprintf(fp, "*%s.Translation StpColorPrecision/%s: \"\"\n", lang, _("Color Precision"));
	      gpprintf(fp, "*%s.StpColorPrecision Normal/%s: \"\"\n", lang, _("Normal"));
	      gpprintf(fp, "*%s.StpColorPrecision Best/%s: \"\"\n", lang, _("Best"));
	    }

	  /*
	   * Media types...
	   */

	  stp_describe_parameter(v, "MediaType", &desc);
	  if (desc.p_type == STP_PARAMETER_TYPE_STRING_LIST && desc.is_active &&
	      stp_string_list_count(desc.bounds.str) > 0)
	    {
	      num_opts = stp_string_list_count(desc.bounds.str);
	      gpprintf(fp, "*%s.Translation MediaType/%s: \"\"\n", lang, _("Media Type"));

	      for (i = 0; i < num_opts; i ++)
		{
		  opt = stp_string_list_param(desc.bounds.str, i);
		  gpprintf(fp, "*%s.MediaType %s/%s: \"\"\n", lang, opt->name, stp_i18n_lookup(po, opt->text));
		}
	    }
	  stp_parameter_description_destroy(&desc);

	  /*
	   * Input slots...
	   */

	  stp_describe_parameter(v, "InputSlot", &desc);

	  if (desc.p_type == STP_PARAMETER_TYPE_STRING_LIST && desc.is_active &&
	      stp_string_list_count(desc.bounds.str) > 0)
	    {
	      num_opts = stp_string_list_count(desc.bounds.str);
	      gpprintf(fp, "*%s.Translation InputSlot/%s: \"\"\n", lang, _("Media Source"));

	      for (i = 0; i < num_opts; i ++)
		{
		  opt = stp_string_list_param(desc.bounds.str, i);
		  gpprintf(fp, "*%s.InputSlot %s/%s: \"\"\n", lang, opt->name, stp_i18n_lookup(po, opt->text));
		}
	    }
	  stp_parameter_description_destroy(&desc);
	  
	  /*
	   * Quality settings
	   */

	  stp_describe_parameter(v, "Quality", &desc);
	  if (desc.p_type == STP_PARAMETER_TYPE_STRING_LIST && desc.is_active)
	    {
	      gpprintf(fp, "*%s.Translation StpQuality/%s: \"\"\n", lang, stp_i18n_lookup(po, desc.text));
	      num_opts = stp_string_list_count(desc.bounds.str);
	      for (i = 0; i < num_opts; i++)
		{
		  opt = stp_string_list_param(desc.bounds.str, i);
		  gpprintf(fp, "*%s.StpQuality %s/%s: \"\"\n", lang, opt->name, stp_i18n_lookup(po, opt->text));
		}
	    }
	  stp_parameter_description_destroy(&desc);

	  /*
	   * Resolution
	   */

	  stp_describe_parameter(v, "Resolution", &desc);

	  if (!simplified || desc.p_level == STP_PARAMETER_LEVEL_BASIC)
	    {
	      gpprintf(fp, "*%s.Translation Resolution/%s: \"\"\n", lang, _("Resolution"));
	      if (has_quality_parameter)
		gpprintf(fp, "*%s.Resolution %s/%s: \"\"\n", lang,
			 default_resolution, _("Automatic"));

	      num_opts = stp_string_list_count(resolutions);
	      for (i = 0; i < num_opts; i ++)
		{
		  opt = stp_string_list_param(resolutions, i);
		  gpprintf(fp, "*%s.Resolution %s/%s: \"\"\n", lang,
			   opt->name, stp_i18n_lookup(po, opt->text));
		}
	    }

	  stp_parameter_description_destroy(&desc);

	  /*
	   * OutputOrder
	   */

	  stp_describe_parameter(v, "OutputOrder", &desc);
	  if (desc.p_type == STP_PARAMETER_TYPE_STRING_LIST)
	    {
	      gpprintf(fp, "*%s.Translation OutputOrder/%s: \"\"\n", lang, _("Output Order"));
	      gpprintf(fp, "*%s.OutputOrder Normal/%s: \"\"\n", lang, _("Normal"));
	      gpprintf(fp, "*%s.OutputOrder Reverse/%s: \"\"\n", lang, _("Reverse"));
	    }
	  stp_parameter_description_destroy(&desc);

	  /*
	   * Duplex
	   * Note that the opt->name strings MUST match those in the printer driver(s)
	   * else the PPD files will not be generated correctly
	   */

	  stp_describe_parameter(v, "Duplex", &desc);
	  if (desc.is_active && desc.p_type == STP_PARAMETER_TYPE_STRING_LIST)
	    {
	      num_opts = stp_string_list_count(desc.bounds.str);
	      if (num_opts > 0)
		{
		  gpprintf(fp, "*%s.Translation Duplex/%s: \"\"\n", lang, _("2-Sided Printing"));

		  for (i = 0; i < num_opts; i++)
		    {
		      opt = stp_string_list_param(desc.bounds.str, i);
		      if (strcmp(opt->name, "None") == 0)
			gpprintf(fp, "*%s.Duplex %s/%s: \"\"\n", lang, opt->name, stp_i18n_lookup(po, opt->text));
		      else if (strcmp(opt->name, "DuplexNoTumble") == 0)
			gpprintf(fp, "*%s.Duplex %s/%s: \"\"\n", lang, opt->name, stp_i18n_lookup(po, opt->text));
		      else if (strcmp(opt->name, "DuplexTumble") == 0)
			gpprintf(fp, "*%s.Duplex %s/%s: \"\"\n", lang, opt->name, stp_i18n_lookup(po, opt->text));
		    }
		}
	    }
	  stp_parameter_description_destroy(&desc);

	  gpprintf(fp, "*%s.Translation StpiShrinkOutput/%s: \"\"\n", lang,
		   _("Shrink Page If Necessary to Fit Borders"));
	  gpprintf(fp, "*%s.StpiShrinkOutput %s/%s: \"\"\n", lang, "Shrink", _("Shrink (print the whole page)"));
	  gpprintf(fp, "*%s.StpiShrinkOutput %s/%s: \"\"\n", lang, "Crop", _("Crop (preserve dimensions)"));
	  gpprintf(fp, "*%s.StpiShrinkOutput %s/%s: \"\"\n", lang, "Expand", _("Expand (use maximum page area)"));

	  param_list = stp_get_parameter_list(v);

	  for (j = 0; j <= STP_PARAMETER_CLASS_OUTPUT; j++)
	    {
	      for (k = 0; k <= maximum_level; k++)
		{
		  size_t param_count = stp_parameter_list_count(param_list);
		  for (l = 0; l < param_count; l++)
		    {
		      const stp_parameter_t *lparam =
			stp_parameter_list_param(param_list, l);
		      if (lparam->p_class != j || lparam->p_level != k ||
			  is_special_option(lparam->name) || lparam->read_only ||
			  (lparam->p_type != STP_PARAMETER_TYPE_STRING_LIST &&
			   lparam->p_type != STP_PARAMETER_TYPE_BOOLEAN &&
			   lparam->p_type != STP_PARAMETER_TYPE_DIMENSION &&
			   lparam->p_type != STP_PARAMETER_TYPE_INT &&
			   lparam->p_type != STP_PARAMETER_TYPE_DOUBLE))
			continue;
		      stp_describe_parameter(v, lparam->name, &desc);
		      if (desc.is_active)
			print_one_localization(fp, po, simplified, lang,
					       lparam, &desc);
		      stp_parameter_description_destroy(&desc);
		    }
		}
	    }
	  stp_parameter_list_destroy(param_list);
	  stp_describe_parameter(v, "ImageType", &desc);
	  if (desc.is_active && desc.p_type == STP_PARAMETER_TYPE_STRING_LIST)
	    {
	      num_opts = stp_string_list_count(desc.bounds.str);
	      if (num_opts > 0)
		{
		  for (i = 0; i < num_opts; i++)
		    {
		      opt = stp_string_list_param(desc.bounds.str, i);
		      if (strcmp(opt->name, "None") != 0)
			gpprintf(fp, "*%s.APPrinterPreset %s/%s: \"*StpImageType %s\"\n",
				 lang, opt->name, opt->text, opt->name);
		    }
		}
	    }
	  stp_parameter_description_destroy(&desc);
	}
      po = savepo;
    }
  if (has_quality_parameter)
    stp_free(default_resolution);
  stp_string_list_destroy(resolutions);

 /*
  * Fonts...
  */

  print_standard_fonts(fp);
  gpprintf(fp, "\n*%% End of %s\n", filename);

  stp_vars_destroy(v);

  return (0);
}


/*
 * End of "$Id: genppd.c,v 1.205 2015/10/17 16:27:18 rlk Exp $".
 */
