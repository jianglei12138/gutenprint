
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 23 "printrcy.y"


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gutenprint/gutenprint-intl-internal.h>
#include <gutenprintui2/gutenprintui.h>
#include "gutenprintui-internal.h"
#include "printrc.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int mylineno;

extern int yylex(void);
char *quotestrip(const char *i);
char *endstrip(const char *i);

extern char* yytext;

static int yyerror( const char *s )
{
  fprintf(stderr,"stdin:%d: %s before '%s'\n", mylineno, s, yytext);
  return 0;
}

static stpui_plist_t *current_printer = NULL;



/* Line 189 of yacc.c  */
#line 107 "printrcy.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     tINT = 258,
     tDOUBLE = 259,
     tDIMENSION = 260,
     tBOOLEAN = 261,
     tSTRING = 262,
     tWORD = 263,
     tGSWORD = 264,
     CURRENT_PRINTER = 265,
     SHOW_ALL_PAPER_SIZES = 266,
     PRINTER = 267,
     DESTINATION = 268,
     SCALING = 269,
     ORIENTATION = 270,
     AUTOSIZE_ROLL_PAPER = 271,
     UNIT = 272,
     DRIVER = 273,
     LEFT = 274,
     TOP = 275,
     CUSTOM_PAGE_WIDTH = 276,
     CUSTOM_PAGE_HEIGHT = 277,
     OUTPUT_TYPE = 278,
     PRINTRC_HDR = 279,
     PARAMETER = 280,
     QUEUE_NAME = 281,
     OUTPUT_FILENAME = 282,
     EXTRA_PRINTER_OPTIONS = 283,
     CUSTOM_COMMAND = 284,
     COMMAND_TYPE = 285,
     GLOBAL_SETTINGS = 286,
     GLOBAL = 287,
     END_GLOBAL_SETTINGS = 288,
     pINT = 289,
     pSTRING_LIST = 290,
     pFILE = 291,
     pDOUBLE = 292,
     pDIMENSION = 293,
     pBOOLEAN = 294,
     pCURVE = 295
   };
#endif
/* Tokens.  */
#define tINT 258
#define tDOUBLE 259
#define tDIMENSION 260
#define tBOOLEAN 261
#define tSTRING 262
#define tWORD 263
#define tGSWORD 264
#define CURRENT_PRINTER 265
#define SHOW_ALL_PAPER_SIZES 266
#define PRINTER 267
#define DESTINATION 268
#define SCALING 269
#define ORIENTATION 270
#define AUTOSIZE_ROLL_PAPER 271
#define UNIT 272
#define DRIVER 273
#define LEFT 274
#define TOP 275
#define CUSTOM_PAGE_WIDTH 276
#define CUSTOM_PAGE_HEIGHT 277
#define OUTPUT_TYPE 278
#define PRINTRC_HDR 279
#define PARAMETER 280
#define QUEUE_NAME 281
#define OUTPUT_FILENAME 282
#define EXTRA_PRINTER_OPTIONS 283
#define CUSTOM_COMMAND 284
#define COMMAND_TYPE 285
#define GLOBAL_SETTINGS 286
#define GLOBAL 287
#define END_GLOBAL_SETTINGS 288
#define pINT 289
#define pSTRING_LIST 290
#define pFILE 291
#define pDOUBLE 292
#define pDIMENSION 293
#define pBOOLEAN 294
#define pCURVE 295




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 228 "printrcy.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  10
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   75

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  70
/* YYNRULES -- Number of states.  */
#define YYNSTATES  114

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   295

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     7,    10,    13,    16,    19,    22,    25,
      28,    31,    34,    37,    40,    43,    46,    49,    52,    53,
      58,    63,    68,    73,    78,    83,    88,    90,    92,    94,
      96,    98,   100,   102,   105,   108,   110,   112,   114,   116,
     118,   120,   122,   124,   126,   128,   130,   132,   134,   136,
     138,   140,   143,   145,   149,   152,   154,   157,   160,   162,
     164,   167,   170,   172,   174,   177,   179,   183,   185,   187,
     189
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      82,     0,    -1,    12,     7,     7,    -1,    13,     7,    -1,
      26,     7,    -1,    27,     7,    -1,    28,     7,    -1,    29,
       7,    -1,    30,     3,    -1,    14,     4,    -1,    15,     3,
      -1,    16,     3,    -1,    17,     3,    -1,    19,     3,    -1,
      20,     3,    -1,    23,     3,    -1,    21,     3,    -1,    22,
       3,    -1,    -1,     8,    34,     6,     3,    -1,     8,    35,
       6,     7,    -1,     8,    36,     6,     7,    -1,     8,    37,
       6,     4,    -1,     8,    38,     6,     3,    -1,     8,    39,
       6,     6,    -1,     8,    40,     6,     7,    -1,    59,    -1,
      60,    -1,    61,    -1,    62,    -1,    64,    -1,    65,    -1,
      63,    -1,    25,    66,    -1,    68,    67,    -1,    58,    -1,
      43,    -1,    49,    -1,    50,    -1,    51,    -1,    52,    -1,
      53,    -1,    54,    -1,    56,    -1,    57,    -1,    55,    -1,
      44,    -1,    45,    -1,    46,    -1,    47,    -1,    48,    -1,
      70,    69,    -1,    58,    -1,    42,    70,    68,    -1,    72,
      71,    -1,    58,    -1,    10,     7,    -1,    11,     6,    -1,
      73,    -1,    74,    -1,    73,    74,    -1,     8,     7,    -1,
      75,    -1,    77,    -1,    79,    78,    -1,    58,    -1,    31,
      79,    33,    -1,    80,    -1,    76,    -1,    58,    -1,    24,
      81,    72,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   100,   100,   111,   118,   128,   138,   148,   158,   165,
     172,   179,   186,   193,   200,   207,   228,   235,   242,   245,
     262,   280,   298,   315,   332,   353,   376,   376,   376,   376,
     377,   377,   377,   380,   383,   383,   386,   386,   386,   386,
     387,   387,   387,   387,   387,   388,   388,   388,   388,   389,
     389,   392,   392,   395,   398,   398,   401,   405,   415,   415,
     418,   421,   432,   432,   435,   435,   438,   441,   441,   441,
     444
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tINT", "tDOUBLE", "tDIMENSION",
  "tBOOLEAN", "tSTRING", "tWORD", "tGSWORD", "CURRENT_PRINTER",
  "SHOW_ALL_PAPER_SIZES", "PRINTER", "DESTINATION", "SCALING",
  "ORIENTATION", "AUTOSIZE_ROLL_PAPER", "UNIT", "DRIVER", "LEFT", "TOP",
  "CUSTOM_PAGE_WIDTH", "CUSTOM_PAGE_HEIGHT", "OUTPUT_TYPE", "PRINTRC_HDR",
  "PARAMETER", "QUEUE_NAME", "OUTPUT_FILENAME", "EXTRA_PRINTER_OPTIONS",
  "CUSTOM_COMMAND", "COMMAND_TYPE", "GLOBAL_SETTINGS", "GLOBAL",
  "END_GLOBAL_SETTINGS", "pINT", "pSTRING_LIST", "pFILE", "pDOUBLE",
  "pDIMENSION", "pBOOLEAN", "pCURVE", "$accept", "Printer", "Destination",
  "Queue_Name", "Output_Filename", "Extra_Printer_Options",
  "Custom_Command", "Command_Type", "Scaling", "Orientation",
  "Autosize_Roll_Paper", "Unit", "Left", "Top", "Output_Type",
  "Custom_Page_Width", "Custom_Page_Height", "Empty", "Int_Param",
  "String_List_Param", "File_Param", "Double_Param", "Dimension_Param",
  "Boolean_Param", "Curve_Param", "Typed_Param", "Parameter", "Parameters",
  "Standard_Value", "Standard_Values", "A_Printer", "Printers",
  "Current_Printer", "Show_All_Paper_Sizes", "Global", "Old_Globals",
  "New_Global_Setting", "Global_Setting", "Global_Settings",
  "Global_Subblock", "Global_Block", "Thing", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    66,    66,    66,
      66,    66,    66,    67,    68,    68,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    70,    70,    71,    72,    72,    73,    74,    75,    75,
      76,    77,    78,    78,    79,    79,    80,    81,    81,    81,
      82
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     4,
       4,     4,     4,     4,     4,     4,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     3,     2,     1,     2,     2,     1,     1,
       2,     2,     1,     1,     2,     1,     3,     1,     1,     1,
       3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    18,     0,     0,    18,    69,     0,    68,    67,    18,
       1,    56,    65,     0,     0,    60,    55,    70,     0,    66,
      58,    59,    62,    63,    64,    57,     0,    18,    54,    61,
       0,    52,    18,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
      46,    47,    48,    49,    50,    37,    38,    39,    40,    41,
      42,    45,    43,    44,    35,    53,    51,     3,     9,    10,
      11,    12,    13,    14,    16,    17,    15,     4,     5,     6,
       7,     8,     0,    34,     0,    26,    27,    28,    29,    32,
      30,    31,    33,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    19,    20,    21,
      22,    23,    24,    25
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    27,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,     5,    85,    86,
      87,    88,    89,    90,    91,    92,    83,    65,    66,    32,
      28,    17,     6,    15,    22,     7,    23,    24,    13,     8,
       9,     2
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -26
static const yytype_int8 yypact[] =
{
     -22,    -2,     6,     0,   -26,   -26,     5,   -26,   -26,   -26,
     -26,   -26,   -26,    -7,    11,   -26,   -26,     7,    13,   -26,
     -26,   -26,   -26,   -26,   -26,   -26,    14,   -26,   -26,   -26,
      15,   -26,    17,   -26,    18,    20,    24,    32,    38,    39,
      45,    46,    47,    48,    49,    50,    51,    52,    57,   -26,
     -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,
     -26,   -26,   -26,   -26,   -26,    27,   -26,   -26,   -26,   -26,
     -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,
     -26,   -26,    10,   -26,   -25,   -26,   -26,   -26,   -26,   -26,
     -26,   -26,   -26,    55,    56,    58,    59,    60,    61,    62,
      66,    63,    64,    68,    70,    69,    67,   -26,   -26,   -26,
     -26,   -26,   -26,   -26
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,
     -26,   -26,   -26,   -26,   -26,   -26,   -26,    -4,   -26,   -26,
     -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,
     -26,   -26,    40,    41,   -26,   -26,   -26,   -26,   -26,   -26,
     -26,   -26
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      12,    18,     1,     3,    14,    16,    10,    11,     3,    93,
      94,    95,    96,    97,    98,    99,    14,    25,    84,    26,
      29,    30,    33,    31,    68,    67,    19,    69,    64,     4,
      34,    35,    36,    37,    38,    70,    39,    40,    41,    42,
      43,    71,    72,    44,    45,    46,    47,    48,    73,    74,
      75,    76,    82,    20,    21,     0,    77,    78,    79,    80,
      81,   100,   101,     0,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   113,   112
};

static const yytype_int8 yycheck[] =
{
       4,     8,    24,    10,    11,     9,     0,     7,    10,    34,
      35,    36,    37,    38,    39,    40,    11,     6,     8,    12,
       7,     7,     7,    27,     4,     7,    33,     3,    32,    31,
      13,    14,    15,    16,    17,     3,    19,    20,    21,    22,
      23,     3,     3,    26,    27,    28,    29,    30,     3,     3,
       3,     3,    25,    13,    13,    -1,     7,     7,     7,     7,
       3,     6,     6,    -1,     6,     6,     6,     6,     6,     3,
       7,     7,     4,     3,     7,     6
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    24,    82,    10,    31,    58,    73,    76,    80,    81,
       0,     7,    58,    79,    11,    74,    58,    72,     8,    33,
      73,    74,    75,    77,    78,     6,    12,    42,    71,     7,
       7,    58,    70,     7,    13,    14,    15,    16,    17,    19,
      20,    21,    22,    23,    26,    27,    28,    29,    30,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    68,    69,     7,     4,     3,
       3,     3,     3,     3,     3,     3,     3,     7,     7,     7,
       7,     3,    25,    67,     8,    59,    60,    61,    62,    63,
      64,    65,    66,    34,    35,    36,    37,    38,    39,    40,
       6,     6,     6,     6,     6,     6,     6,     3,     7,     7,
       4,     3,     6,     7
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 101 "printrcy.y"
    {
	  current_printer = stpui_plist_create((yyvsp[(2) - (3)].sval), (yyvsp[(3) - (3)].sval));
	  g_free((yyvsp[(2) - (3)].sval));
	  g_free((yyvsp[(3) - (3)].sval));
	}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 112 "printrcy.y"
    {
	  if ((yyvsp[(2) - (2)].sval))
	    g_free((yyvsp[(2) - (2)].sval));
	}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 119 "printrcy.y"
    {
	  if (current_printer && (yyvsp[(2) - (2)].sval))
	    {
	      stpui_plist_set_queue_name(current_printer, (yyvsp[(2) - (2)].sval));
	      g_free((yyvsp[(2) - (2)].sval));
	    }
	}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 129 "printrcy.y"
    {
	  if (current_printer && (yyvsp[(2) - (2)].sval))
	    {
	      stpui_plist_set_output_filename(current_printer, (yyvsp[(2) - (2)].sval));
	      g_free((yyvsp[(2) - (2)].sval));
	    }
	}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 139 "printrcy.y"
    {
	  if (current_printer && (yyvsp[(2) - (2)].sval))
	    {
	      stpui_plist_set_extra_printer_options(current_printer, (yyvsp[(2) - (2)].sval));
	      g_free((yyvsp[(2) - (2)].sval));
	    }
	}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 149 "printrcy.y"
    {
	  if (current_printer && (yyvsp[(2) - (2)].sval))
	    {
	      stpui_plist_set_custom_command(current_printer, (yyvsp[(2) - (2)].sval));
	      g_free((yyvsp[(2) - (2)].sval));
	    }
	}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 159 "printrcy.y"
    {
	  if (current_printer)
	    stpui_plist_set_command_type(current_printer, (yyvsp[(2) - (2)].ival));
	}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 166 "printrcy.y"
    {
	  if (current_printer)
	    current_printer->scaling = (yyvsp[(2) - (2)].dval); 
	}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 173 "printrcy.y"
    {
	  if (current_printer)
	    current_printer->orientation = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 180 "printrcy.y"
    {
	  if (current_printer)
	    current_printer->auto_size_roll_feed_paper = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 187 "printrcy.y"
    {
	  if (current_printer)
	    current_printer->unit = (yyvsp[(2) - (2)].ival); 
	}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 194 "printrcy.y"
    {
	  if (current_printer)
	    stp_set_left(current_printer->v, (yyvsp[(2) - (2)].ival));
	}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 201 "printrcy.y"
    {
	  if (current_printer)
	    stp_set_top(current_printer->v, (yyvsp[(2) - (2)].ival));
	}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 208 "printrcy.y"
    {
	  if (current_printer)
	    {
	      switch ((yyvsp[(2) - (2)].ival))
		{
		case 0:
		  stp_set_string_parameter
		    (current_printer->v, "PrintingMode", "BW");
		  break;
		case 1:
		case 2:
		default:
		  stp_set_string_parameter
		    (current_printer->v, "PrintingMode", "Color");
		  break;
		}
	    }
	}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 229 "printrcy.y"
    {
	  if (current_printer)
	    stp_set_page_width(current_printer->v, (yyvsp[(2) - (2)].ival));
	}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 236 "printrcy.y"
    {
	  if (current_printer)
	    stp_set_page_height(current_printer->v, (yyvsp[(2) - (2)].ival));
	}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 246 "printrcy.y"
    {
	  if (current_printer)
	    {
	      stp_set_int_parameter(current_printer->v, (yyvsp[(1) - (4)].sval), (yyvsp[(4) - (4)].ival));
	      if (strcmp((yyvsp[(3) - (4)].sval), "False") == 0)
		stp_set_int_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
					     STP_PARAMETER_INACTIVE);
	      else
		stp_set_int_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
					     STP_PARAMETER_ACTIVE);
	    }
	  g_free((yyvsp[(1) - (4)].sval));
	  g_free((yyvsp[(3) - (4)].sval));
	}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 263 "printrcy.y"
    {
	  if (current_printer)
	    {
	      stp_set_string_parameter(current_printer->v, (yyvsp[(1) - (4)].sval), (yyvsp[(4) - (4)].sval));
	      if (strcmp((yyvsp[(3) - (4)].sval), "False") == 0)
		stp_set_string_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
						STP_PARAMETER_INACTIVE);
	      else
		stp_set_string_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
						STP_PARAMETER_ACTIVE);
	    }
	  g_free((yyvsp[(1) - (4)].sval));
	  g_free((yyvsp[(3) - (4)].sval));
	  g_free((yyvsp[(4) - (4)].sval));
	}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 281 "printrcy.y"
    {
	  if (current_printer)
	    {
	      stp_set_file_parameter(current_printer->v, (yyvsp[(1) - (4)].sval), (yyvsp[(4) - (4)].sval));
	      if (strcmp((yyvsp[(3) - (4)].sval), "False") == 0)
		stp_set_file_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
					      STP_PARAMETER_INACTIVE);
	      else
		stp_set_file_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
					      STP_PARAMETER_ACTIVE);
	    }
	  g_free((yyvsp[(1) - (4)].sval));
	  g_free((yyvsp[(3) - (4)].sval));
	  g_free((yyvsp[(4) - (4)].sval));
	}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 299 "printrcy.y"
    {
	  if (current_printer)
	    {
	      stp_set_float_parameter(current_printer->v, (yyvsp[(1) - (4)].sval), (yyvsp[(4) - (4)].dval));
	      if (strcmp((yyvsp[(3) - (4)].sval), "False") == 0)
		stp_set_float_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
					       STP_PARAMETER_INACTIVE);
	      else
		stp_set_float_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
					       STP_PARAMETER_ACTIVE);
	    }
	  g_free((yyvsp[(1) - (4)].sval));
	  g_free((yyvsp[(3) - (4)].sval));
	}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 316 "printrcy.y"
    {
	  if (current_printer)
	    {
	      stp_set_dimension_parameter(current_printer->v, (yyvsp[(1) - (4)].sval), (yyvsp[(4) - (4)].ival));
	      if (strcmp((yyvsp[(3) - (4)].sval), "False") == 0)
		stp_set_dimension_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
						   STP_PARAMETER_INACTIVE);
	      else
		stp_set_dimension_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
						   STP_PARAMETER_ACTIVE);
	    }
	  g_free((yyvsp[(1) - (4)].sval));
	  g_free((yyvsp[(3) - (4)].sval));
	}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 333 "printrcy.y"
    {
	  if (current_printer)
	    {
	      if (strcmp((yyvsp[(4) - (4)].sval), "False") == 0)
		stp_set_boolean_parameter(current_printer->v, (yyvsp[(1) - (4)].sval), 0);
	      else
		stp_set_boolean_parameter(current_printer->v, (yyvsp[(1) - (4)].sval), 1);
	      if (strcmp((yyvsp[(3) - (4)].sval), "False") == 0)
		stp_set_boolean_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
						 STP_PARAMETER_INACTIVE);
	      else
		stp_set_boolean_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
						 STP_PARAMETER_ACTIVE);
	    }
	  g_free((yyvsp[(1) - (4)].sval));
	  g_free((yyvsp[(3) - (4)].sval));
	  g_free((yyvsp[(4) - (4)].sval));
	}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 354 "printrcy.y"
    {
	  if (current_printer)
	    {
	      stp_curve_t *curve = stp_curve_create_from_string((yyvsp[(4) - (4)].sval));
	      if (curve)
		{
		  stp_set_curve_parameter(current_printer->v, (yyvsp[(1) - (4)].sval), curve);
		  if (strcmp((yyvsp[(3) - (4)].sval), "False") == 0)
		    stp_set_curve_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
						   STP_PARAMETER_INACTIVE);
		  else
		    stp_set_curve_parameter_active(current_printer->v, (yyvsp[(1) - (4)].sval),
						   STP_PARAMETER_ACTIVE);
		  stp_curve_destroy(curve);
		}
	    }
	  g_free((yyvsp[(1) - (4)].sval));
	  g_free((yyvsp[(3) - (4)].sval));
	  g_free((yyvsp[(4) - (4)].sval));
	}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 402 "printrcy.y"
    { stpui_printrc_current_printer = (yyvsp[(2) - (2)].sval); }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 406 "printrcy.y"
    {
	  if (strcmp((yyvsp[(2) - (2)].sval), "True") == 0)
	    stpui_show_all_paper_sizes = 1;
	  else
	    stpui_show_all_paper_sizes = 0;
	  g_free((yyvsp[(2) - (2)].sval));
	}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 422 "printrcy.y"
    {
	  if ((yyvsp[(2) - (2)].sval))
	    {
	      stpui_set_global_parameter((yyvsp[(1) - (2)].sval), (yyvsp[(2) - (2)].sval));
	      g_free((yyvsp[(2) - (2)].sval));
	    }
	  g_free((yyvsp[(1) - (2)].sval));
	}
    break;



/* Line 1455 of yacc.c  */
#line 1905 "printrcy.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 447 "printrcy.y"


