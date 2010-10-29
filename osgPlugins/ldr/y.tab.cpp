/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     tIDENT = 258,
     tGARBAGE = 259,
     tINT = 260,
     tFLOAT = 261,
     tZERO = 262,
     tONE = 263,
     tTWO = 264,
     tTHREE = 265,
     tFOUR = 266,
     tFIVE = 267,
     tSTEP = 268,
     tPAUSE = 269,
     tWRITE = 270,
     tCLEAR = 271,
     tSAVE = 272,
     tEOL = 273,
     tEOF = 274,
     tROTATE = 275,
     tTRANSLATE = 276,
     tEND = 277,
     tSCALE = 278,
     tTRANSFORM = 279,
     tCOLOR = 280,
     tALIAS = 281,
     tPOINT = 282,
     tMATRIX = 283,
     tFILE = 284
   };
#endif
/* Tokens.  */
#define tIDENT 258
#define tGARBAGE 259
#define tINT 260
#define tFLOAT 261
#define tZERO 262
#define tONE 263
#define tTWO 264
#define tTHREE 265
#define tFOUR 266
#define tFIVE 267
#define tSTEP 268
#define tPAUSE 269
#define tWRITE 270
#define tCLEAR 271
#define tSAVE 272
#define tEOL 273
#define tEOF 274
#define tROTATE 275
#define tTRANSLATE 276
#define tEND 277
#define tSCALE 278
#define tTRANSFORM 279
#define tCOLOR 280
#define tALIAS 281
#define tPOINT 282
#define tMATRIX 283
#define tFILE 284




/* Copy the first part of user declarations.  */


	
#define yyparse LDParse::LDyyparse
	
#include <stdio.h>
#include <stdlib.h>
#include "ldraw.h"
	
#undef NO_LINES
	
	using namespace LDParse;
	
	char yyfilename[256];			/* Input filename */
	char *ldraw_model=NULL;
	int tmp_i;
	void yyerror ( char * );
	int stepcount=0;
	char *mpd_subfile_name;
	
	// we don't use this anymore---Torque combines translation and transformation
	// into a single 4x4 matrix
	
	/* x y z is the x y z coordinate of the part
	 * a b c d e f g h i is a top left 3x3 matrix of a standard 4x4 homogeneous
	 * transformation matrix. This represents the rotation and scaling of the
	 * part. The entire 4x4 3D transformation matrix would then take either of
	 * the following forms:
	 *	 / a d g 0 \   / a b c x \
	 *	 | b e h 0 |   | d e f y |
	 *	 | c f i 0 |   | g h i z |
	 *	 \ x y z 1 /   \ 0 0 0 1 /
	 * The above two forms are essentially equivalent, but note the location of
	 * the transformation portion (x, y, z) relative to the other terms.
	 */
	
	//MatrixF *current_transform[MAX_INCLUDE_DEPTH];
	
	//vector3d *current_translation[MAX_INCLUDE_DEPTH];
	//matrix3d *current_transform[MAX_INCLUDE_DEPTH];
	
	S32 current_color[MAX_INCLUDE_DEPTH];
	//S32 current_type[MAX_INCLUDE_DEPTH];
	S32 is_current_file_not_cached(void);
	
	
	

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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE

{
	char *c;
	S32 i;
	F64 d;
	osg::Vec3f *v;
	osg::Matrixf *m;
}
/* Line 193 of yacc.c.  */

	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */


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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
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
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   504

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  30
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  111
/* YYNRULES -- Number of states.  */
#define YYNSTATES  214

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   284

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
      25,    26,    27,    28,    29
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    27,    29,    33,    37,    41,    45,
      47,    49,    51,    53,    55,    57,    59,    62,    65,    67,
      69,    71,    73,    75,    77,    79,    81,    86,    91,    96,
     101,   106,   111,   116,   121,   136,   151,   157,   163,   169,
     175,   181,   187,   192,   197,   202,   207,   212,   217,   222,
     227,   233,   239,   244,   249,   254,   259,   264,   269,   274,
     279,   284,   289,   291,   293,   295,   297,   299,   301,   303,
     305,   307,   309,   311,   313,   315,   317,   319,   321,   323,
     324,   327,   331,   335,   337,   339,   341,   343,   356,   358,
     362,   364,   366,   372,   378,   384,   390,   397,   404,   412,
     420,   428
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      31,     0,    -1,    32,    -1,    -1,    32,    33,    -1,    35,
      -1,    36,    -1,    37,    -1,    38,    -1,    44,    -1,    61,
      -1,    62,    -1,    63,    -1,    64,    -1,    65,    -1,    45,
      32,    46,    -1,    47,    32,    48,    -1,    49,    32,    50,
      -1,    51,    32,    52,    -1,    39,    -1,    40,    -1,    41,
      -1,    42,    -1,    43,    -1,    56,    -1,    55,    -1,     1,
      18,    -1,     1,    19,    -1,     7,    -1,     8,    -1,     9,
      -1,    10,    -1,    11,    -1,    12,    -1,     5,    -1,     6,
      -1,     7,    13,    54,    18,    -1,     7,    13,    54,    19,
      -1,     7,    14,    54,    18,    -1,     7,    14,    54,    19,
      -1,     7,    15,    54,    18,    -1,     7,    15,    54,    19,
      -1,     7,    16,    54,    18,    -1,     7,    16,    54,    19,
      -1,     7,    25,    34,     3,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    18,    -1,     7,    25,    34,     3,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    19,
      -1,     7,    26,     3,    57,    18,    -1,     7,    26,     3,
      57,    19,    -1,     7,    27,     3,    59,    18,    -1,     7,
      27,     3,    59,    19,    -1,     7,    28,     3,    58,    18,
      -1,     7,    28,     3,    58,    19,    -1,     7,    29,     3,
      18,    -1,     7,    29,     3,    19,    -1,     7,    17,    54,
      18,    -1,     7,    17,    54,    19,    -1,     7,    21,    59,
      18,    -1,     7,    21,    59,    19,    -1,     7,    21,    22,
      18,    -1,     7,    21,    22,    19,    -1,     7,    20,    34,
      59,    18,    -1,     7,    20,    34,    59,    19,    -1,     7,
      20,    22,    18,    -1,     7,    20,    22,    19,    -1,     7,
      23,    34,    18,    -1,     7,    23,    34,    19,    -1,     7,
      23,    22,    18,    -1,     7,    23,    22,    19,    -1,     7,
      24,    58,    18,    -1,     7,    24,    58,    19,    -1,     7,
      24,    22,    18,    -1,     7,    24,    22,    19,    -1,     3,
      -1,     4,    -1,    34,    -1,    13,    -1,    14,    -1,    15,
      -1,    16,    -1,    17,    -1,    21,    -1,    20,    -1,    22,
      -1,    25,    -1,    27,    -1,    23,    -1,    24,    -1,    26,
      -1,    28,    -1,    -1,    54,    53,    -1,     7,    54,    18,
      -1,     7,    54,    19,    -1,    18,    -1,    19,    -1,    34,
      -1,     3,    -1,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    -1,     3,    -1,    34,    34,
      34,    -1,     3,    -1,     3,    -1,     8,    57,    58,    60,
      18,    -1,     8,    57,    58,    60,    19,    -1,     9,    57,
      59,    59,    18,    -1,     9,    57,    59,    59,    19,    -1,
      10,    57,    59,    59,    59,    18,    -1,    10,    57,    59,
      59,    59,    19,    -1,    11,    57,    59,    59,    59,    59,
      18,    -1,    11,    57,    59,    59,    59,    59,    19,    -1,
      12,    57,    59,    59,    59,    59,    18,    -1,    12,    57,
      59,    59,    59,    59,    19,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    97,    97,   100,   101,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   129,   133,
     137,   141,   145,   149,   153,   158,   164,   171,   182,   189,
     200,   208,   218,   226,   235,   239,   246,   254,   265,   273,
     284,   292,   303,   313,   325,   333,   343,   349,   358,   362,
     369,   374,   382,   386,   393,   397,   404,   408,   415,   419,
     426,   430,   438,   447,   456,   462,   467,   472,   477,   483,
     489,   495,   501,   507,   513,   519,   525,   531,   536,   544,
     548,   559,   566,   575,   578,   584,   594,   604,   608,   618,
     628,   638,   650,   664,   682,   696,   714,   731,   752,   773,
     796,   816
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tIDENT", "tGARBAGE", "tINT", "tFLOAT",
  "tZERO", "tONE", "tTWO", "tTHREE", "tFOUR", "tFIVE", "tSTEP", "tPAUSE",
  "tWRITE", "tCLEAR", "tSAVE", "tEOL", "tEOF", "tROTATE", "tTRANSLATE",
  "tEND", "tSCALE", "tTRANSFORM", "tCOLOR", "tALIAS", "tPOINT", "tMATRIX",
  "tFILE", "$accept", "LdrawModel", "LdrawLines", "LdrawLine", "Number",
  "StepLine", "PauseLine", "WriteLine", "ClearLine", "ColorLine",
  "ColorAliasLine", "PointAliasLine", "MatrixAliasLine", "MPDFileLine",
  "SaveLine", "TranslateStartLine", "TranslateEndLine", "RotateStartLine",
  "RotateEndLine", "ScaleStartLine", "ScaleEndLine", "TransformStartLine",
  "TransformEndLine", "Word", "Words", "CommentLine", "BlankLine", "Color",
  "TransMatrix", "Position", "PartName", "ObjectLine", "LineLine",
  "TriLine", "QuadLine", "FiveLine", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    30,    31,    32,    32,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    34,    34,
      34,    34,    34,    34,    34,    34,    35,    35,    36,    36,
      37,    37,    38,    38,    39,    39,    40,    40,    41,    41,
      42,    42,    43,    43,    44,    44,    45,    45,    46,    46,
      47,    47,    48,    48,    49,    49,    50,    50,    51,    51,
      52,    52,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    54,
      54,    55,    55,    56,    56,    57,    57,    58,    58,    59,
      59,    60,    61,    61,    62,    62,    63,    63,    64,    64,
      65,    65
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     4,     4,
       4,     4,     4,     4,    14,    14,     5,     5,     5,     5,
       5,     5,     4,     4,     4,     4,     4,     4,     4,     4,
       5,     5,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       2,     3,     3,     1,     1,     1,     1,    12,     1,     3,
       1,     1,     5,     5,     5,     5,     6,     6,     7,     7,
       7,     7
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,     0,    89,     0,     0,     0,     0,
       0,    93,    94,     4,     5,     6,     7,     8,    19,    20,
      21,    22,    23,     9,     3,     3,     3,     3,    25,    24,
      10,    11,    12,    13,    14,    26,    27,    89,    89,    89,
      89,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    34,    35,    28,    29,    30,    31,    32,
      33,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    75,    76,    77,    78,    79,    91,    92,    81,    80,
      82,    85,    86,    83,    87,    84,    88,    74,    90,     0,
       0,     0,     0,     0,    89,    15,    89,    16,    89,    17,
      89,    18,    36,    37,    38,    39,    40,    41,    42,    43,
      54,    55,     0,     0,    56,    57,    64,    65,     0,    68,
      69,     0,     0,     0,     0,    52,    53,   101,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,    99,
       0,     0,    46,    47,    48,    49,    50,    51,   102,   103,
     104,   105,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   106,   107,     0,     0,    58,    59,    62,    63,    66,
      67,    70,    71,     0,     0,   108,   109,   110,   111,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,    44,    45
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    13,    78,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,   115,    25,   117,    26,
     119,    27,   121,   108,    51,    28,    29,    62,    83,    79,
     148,    30,    31,    32,    33,    34
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -59
static const yytype_int16 yypact[] =
{
     -59,    17,    13,   -59,     8,   377,   472,   472,   472,   472,
     472,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   100,   482,   100,   492,   100,    12,    16,    32,
      34,   201,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   492,   482,   482,   482,   482,   133,   152,   171,
     359,   227,   253,   279,   305,   331,   482,   -59,   100,    11,
      15,   -59,   100,    24,    41,   472,   482,   492,    59,   -59,
     -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,    48,
     482,   482,   482,   482,   394,   -59,   411,   -59,   428,   -59,
     445,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,    70,   100,   -59,   -59,   -59,   -59,   100,   -59,
     -59,   100,    72,   110,   112,   -59,   -59,   -59,   119,   128,
     482,   482,   482,    92,    52,   374,   114,   -59,   -59,   -59,
     100,   100,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   138,   482,   482,   147,   149,   157,   166,   100,
     100,   -59,   -59,   173,   343,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,   100,   100,   -59,   -59,   -59,   -59,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     345,   -59,   -59,   -59
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -59,   -59,    58,   -59,    -6,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,   -59,     9,   -59,   -59,     2,   -46,   -58,
     -59,   -59,   -59,   -59,   -59,   -59
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -3
static const yytype_int16 yytable[] =
{
      61,    61,    61,    61,    61,   110,   111,   112,   113,    63,
      64,    65,    66,    -2,     4,    85,   109,     3,   132,    86,
       5,     6,     7,     8,     9,    10,    35,    36,   143,   134,
     135,    11,    12,   136,   137,    87,    76,    88,    80,    82,
      84,   144,   139,   140,   141,   107,    71,    72,    73,    74,
      75,   147,   149,   150,   151,   152,    82,    53,    54,    55,
      56,    57,    58,    59,    60,   107,   107,   107,   107,   107,
       0,     0,   133,     0,   176,     0,   138,   145,   146,    61,
       0,    82,    67,    68,    69,    70,     0,   142,   157,   158,
     162,   163,   172,   173,   174,    77,     0,    53,    54,    55,
      56,    57,    58,    59,    60,    53,    54,    55,    56,    57,
      58,    59,    60,     0,   175,   183,   184,    81,     0,    53,
      54,    55,    56,    57,    58,    59,    60,   159,   164,   165,
     166,   167,   160,     0,     4,   161,   178,   168,   169,     0,
     114,     6,     7,     8,     9,    10,   170,   171,    76,    80,
      82,    11,    12,     4,   179,   180,   181,   182,     0,   116,
       6,     7,     8,     9,    10,   185,   186,   187,   188,     0,
      11,    12,     4,   193,   194,   189,   190,     0,   118,     6,
       7,     8,     9,    10,   191,   192,     0,   199,   200,    11,
      12,   195,   196,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,    89,    90,    53,    54,    55,    56,
      57,    58,    59,    60,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
      89,    90,    53,    54,    55,    56,    57,    58,    59,    60,
      91,    92,    93,    94,    95,   122,   123,    98,    99,   100,
     101,   102,   103,   104,   105,   106,    89,    90,    53,    54,
      55,    56,    57,    58,    59,    60,    91,    92,    93,    94,
      95,   124,   125,    98,    99,   100,   101,   102,   103,   104,
     105,   106,    89,    90,    53,    54,    55,    56,    57,    58,
      59,    60,    91,    92,    93,    94,    95,   126,   127,    98,
      99,   100,   101,   102,   103,   104,   105,   106,    89,    90,
      53,    54,    55,    56,    57,    58,    59,    60,    91,    92,
      93,    94,    95,   128,   129,    98,    99,   100,   101,   102,
     103,   104,   105,   106,    89,    90,    53,    54,    55,    56,
      57,    58,    59,    60,    91,    92,    93,    94,    95,   130,
     131,    98,    99,   100,   101,   102,   103,   104,   105,   106,
       4,   197,   198,   212,   213,     0,   120,     6,     7,     8,
       9,    10,     0,     0,     0,     0,     0,    11,    12,    53,
      54,    55,    56,    57,    58,    59,    60,     0,     0,     0,
      37,    38,    39,    40,    41,     0,   177,    42,    43,     0,
      44,    45,    46,    47,    48,    49,    50,    37,    38,    39,
      40,    41,     0,     0,    42,   153,     0,    44,    45,    46,
      47,    48,    49,    50,    37,    38,    39,    40,    41,     0,
       0,   154,    43,     0,    44,    45,    46,    47,    48,    49,
      50,    37,    38,    39,    40,    41,     0,     0,    42,    43,
       0,   155,    45,    46,    47,    48,    49,    50,    37,    38,
      39,    40,    41,     0,     0,    42,    43,     0,    44,   156,
      46,    47,    48,    49,    50,    52,     0,    53,    54,    55,
      56,    57,    58,    59,    60,    77,     0,    53,    54,    55,
      56,    57,    58,    59,    60,    81,     0,    53,    54,    55,
      56,    57,    58,    59,    60
};

static const yytype_int16 yycheck[] =
{
       6,     7,     8,     9,    10,    63,    64,    65,    66,     7,
       8,     9,    10,     0,     1,     3,    62,     0,    76,     3,
       7,     8,     9,    10,    11,    12,    18,    19,    86,    18,
      19,    18,    19,    18,    19,     3,    42,     3,    44,    45,
      46,    87,    18,    19,     3,    51,    37,    38,    39,    40,
      41,     3,   110,   111,   112,   113,    62,     5,     6,     7,
       8,     9,    10,    11,    12,    71,    72,    73,    74,    75,
      -1,    -1,    78,    -1,    22,    -1,    82,    18,    19,    85,
      -1,    87,    24,    25,    26,    27,    -1,    85,    18,    19,
      18,    19,   150,   151,   152,     3,    -1,     5,     6,     7,
       8,     9,    10,    11,    12,     5,     6,     7,     8,     9,
      10,    11,    12,    -1,    22,   173,   174,     3,    -1,     5,
       6,     7,     8,     9,    10,    11,    12,   133,    18,    19,
      18,    19,   138,    -1,     1,   141,    22,    18,    19,    -1,
       7,     8,     9,    10,    11,    12,    18,    19,   154,   155,
     156,    18,    19,     1,   160,   161,    18,    19,    -1,     7,
       8,     9,    10,    11,    12,    18,    19,    18,    19,    -1,
      18,    19,     1,   179,   180,    18,    19,    -1,     7,     8,
       9,    10,    11,    12,    18,    19,    -1,   193,   194,    18,
      19,    18,    19,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
       1,    18,    19,    18,    19,    -1,     7,     8,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    18,    19,     5,
       6,     7,     8,     9,    10,    11,    12,    -1,    -1,    -1,
      13,    14,    15,    16,    17,    -1,    22,    20,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    13,    14,    15,
      16,    17,    -1,    -1,    20,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    13,    14,    15,    16,    17,    -1,
      -1,    20,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    13,    14,    15,    16,    17,    -1,    -1,    20,    21,
      -1,    23,    24,    25,    26,    27,    28,    29,    13,    14,
      15,    16,    17,    -1,    -1,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,     3,    -1,     5,     6,     7,
       8,     9,    10,    11,    12,     3,    -1,     5,     6,     7,
       8,     9,    10,    11,    12,     3,    -1,     5,     6,     7,
       8,     9,    10,    11,    12
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    31,    32,     0,     1,     7,     8,     9,    10,    11,
      12,    18,    19,    33,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    47,    49,    51,    55,    56,
      61,    62,    63,    64,    65,    18,    19,    13,    14,    15,
      16,    17,    20,    21,    23,    24,    25,    26,    27,    28,
      29,    54,     3,     5,     6,     7,     8,     9,    10,    11,
      12,    34,    57,    57,    57,    57,    57,    32,    32,    32,
      32,    54,    54,    54,    54,    54,    34,     3,    34,    59,
      34,     3,    34,    58,    34,     3,     3,     3,     3,     3,
       4,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    34,    53,    58,
      59,    59,    59,    59,     7,    46,     7,    48,     7,    50,
       7,    52,    18,    19,    18,    19,    18,    19,    18,    19,
      18,    19,    59,    34,    18,    19,    18,    19,    34,    18,
      19,     3,    57,    59,    58,    18,    19,     3,    60,    59,
      59,    59,    59,    21,    20,    23,    24,    18,    19,    34,
      34,    34,    18,    19,    18,    19,    18,    19,    18,    19,
      18,    19,    59,    59,    59,    22,    22,    22,    22,    34,
      34,    18,    19,    59,    59,    18,    19,    18,    19,    18,
      19,    18,    19,    34,    34,    18,    19,    18,    19,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    18,    19
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
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

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
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
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
        case 28:

    {
	(yyval.d) = 0.0;
;}
    break;

  case 29:

    {
	(yyval.d) = 1.0;
;}
    break;

  case 30:

    {
	(yyval.d) = 2.0;
;}
    break;

  case 31:

    {
	(yyval.d) = 3.0;
;}
    break;

  case 32:

    {
	(yyval.d) = 4.0;
;}
    break;

  case 33:

    {
	(yyval.d) = 5.0;
;}
    break;

  case 34:

    {
	(yyval.d) = 1.0 * ((yyvsp[(1) - (1)].i));
	
;}
    break;

  case 35:

    {
	(yyval.d) =1.0 * ((yyvsp[(1) - (1)].d));
;}
    break;

  case 36:

    {
	zStep(stepcount,1);
	stepcount++;
	//dFree($3);
	delete [] (yyvsp[(3) - (4)].c);
;}
    break;

  case 37:

    {
	zStep(stepcount,1);
	stepcount++;
	
	//dFree($3);
	delete [] (yyvsp[(3) - (4)].c);
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 38:

    {
	zPause();
	
	//dFree($3);
	delete [] (yyvsp[(3) - (4)].c);
;}
    break;

  case 39:

    {
	
	zPause();
	
	//dFree($3);
	delete [] (yyvsp[(3) - (4)].c);
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 40:

    {
	
	zWrite((yyvsp[(3) - (4)].c));
	
	//dFree($3);
	delete [] (yyvsp[(3) - (4)].c);
;}
    break;

  case 41:

    {
	
	zWrite((yyvsp[(3) - (4)].c));
	//dFree($3);
	delete [] (yyvsp[(3) - (4)].c);
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 42:

    {
	
	zClear();
	//dFree($3);
	delete [] (yyvsp[(3) - (4)].c);
	
;}
    break;

  case 43:

    {
	zClear();
	//dFree($3);
	delete [] (yyvsp[(3) - (4)].c);
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 44:

    {
	zcolor_modify((int)(yyvsp[(3) - (14)].d),(yyvsp[(4) - (14)].c),(int)(yyvsp[(5) - (14)].d),(int)(yyvsp[(6) - (14)].d),(int)(yyvsp[(7) - (14)].d),(int)(yyvsp[(8) - (14)].d),(int)(yyvsp[(9) - (14)].d),(int)(yyvsp[(10) - (14)].d),(int)(yyvsp[(11) - (14)].d),(int)(yyvsp[(12) - (14)].d),(int)(yyvsp[(13) - (14)].d));
;}
    break;

  case 45:

    {
	zcolor_modify((int)(yyvsp[(3) - (14)].d),(yyvsp[(4) - (14)].c),(int)(yyvsp[(5) - (14)].d),(int)(yyvsp[(6) - (14)].d),(int)(yyvsp[(7) - (14)].d),(int)(yyvsp[(8) - (14)].d),(int)(yyvsp[(9) - (14)].d),(int)(yyvsp[(10) - (14)].d),(int)(yyvsp[(11) - (14)].d),(int)(yyvsp[(12) - (14)].d),(int)(yyvsp[(13) - (14)].d));
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 46:

    {
	zcolor_alias((int)(yyvsp[(4) - (5)].i),(yyvsp[(3) - (5)].c));
	if (is_current_file_not_cached()) {
		//dFree($3);
		delete [] (yyvsp[(3) - (5)].c);
	}
;}
    break;

  case 47:

    {
	zcolor_alias((int)(yyvsp[(4) - (5)].i),(yyvsp[(3) - (5)].c));
	if (is_current_file_not_cached()) {
		//dFree($3);
		delete [] (yyvsp[(3) - (5)].c);
	}
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 48:

    {
	zpoint_alias((yyvsp[(3) - (5)].c),(yyvsp[(4) - (5)].v));
	if (is_current_file_not_cached()) {
		//dFree($3);
		delete [] (yyvsp[(3) - (5)].c);
	}
;}
    break;

  case 49:

    {
	zpoint_alias((yyvsp[(3) - (5)].c),(yyvsp[(4) - (5)].v));
	if (is_current_file_not_cached()) {
		//dFree($3);
		delete [] (yyvsp[(3) - (5)].c);
	}
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 50:

    {
	zmatrix_alias((yyvsp[(3) - (5)].c),(yyvsp[(4) - (5)].m));
	if (is_current_file_not_cached()) {
		//dFree($3);
		delete [] (yyvsp[(3) - (5)].c);
	}
;}
    break;

  case 51:

    {
	zmatrix_alias((yyvsp[(3) - (5)].c),(yyvsp[(4) - (5)].m));
	if (is_current_file_not_cached()) {
		//dFree($3);
		delete [] (yyvsp[(3) - (5)].c);
	}
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 52:

    {
	if (cache_mpd_subfiles((yyvsp[(3) - (4)].c)) == 0) {
		mpd_subfile_name = (yyvsp[(3) - (4)].c);
	} else {
		//dFree($3);
		delete [] (yyvsp[(3) - (4)].c);
	}
	YYACCEPT;
;}
    break;

  case 53:

    {
	if (cache_mpd_subfiles((yyvsp[(3) - (4)].c)) == 0) {
		mpd_subfile_name = (yyvsp[(3) - (4)].c);
	} else {
		//dFree($3);
		delete [] (yyvsp[(3) - (4)].c);
	}
	YYACCEPT;
;}
    break;

  case 54:

    {
	
	zSave(stepcount);
	stepcount++;
	//dFree($3);
	delete [] (yyvsp[(3) - (4)].c);
;}
    break;

  case 55:

    {
	zSave(stepcount);
	stepcount++;
	//dFree($3);
	delete [] (yyvsp[(3) - (4)].c);
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 56:

    {
	osg::Matrixf tmp = osg::Matrixf::identity();
	tmp.setTrans(*(yyvsp[(3) - (4)].v));
	push_transform(&tmp);
;}
    break;

  case 57:

    {
	osg::Matrixf tmp = osg::Matrixf::identity();
	tmp.setTrans(*(yyvsp[(3) - (4)].v));
	push_transform(&tmp);
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 58:

    {
	pop_transform();
;}
    break;

  case 59:

    {
	pop_transform();
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 60:

    {
	/* axis degree representation */
	push_rotation((yyvsp[(4) - (5)].v), (yyvsp[(3) - (5)].d));
;}
    break;

  case 61:

    {
	/* axis degree representation */
	push_rotation((yyvsp[(4) - (5)].v), (yyvsp[(3) - (5)].d));
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 62:

    {
	pop_transform();
;}
    break;

  case 63:

    {
	pop_transform();
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 64:

    {
	push_transform(savemat(0, 0, 0, (float)((yyvsp[(3) - (4)].d)),0,0,0,(float)((yyvsp[(3) - (4)].d)),0,0,0,(float)((yyvsp[(3) - (4)].d))));
;}
    break;

  case 65:

    {
	push_transform(savemat(0, 0, 0, (float)((yyvsp[(3) - (4)].d)),0,0,0,(float)((yyvsp[(3) - (4)].d)),0,0,0,(float)((yyvsp[(3) - (4)].d))));
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 66:

    {
	pop_transform();
;}
    break;

  case 67:

    {
	pop_transform();
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 68:

    {
	push_transform((yyvsp[(3) - (4)].m));
;}
    break;

  case 69:

    {
	push_transform((yyvsp[(3) - (4)].m));
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 70:

    {
	pop_transform();
;}
    break;

  case 71:

    {
	pop_transform();
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 72:

    {
	if (is_current_file_not_cached()) {
		(yyval.c) = (yyvsp[(1) - (1)].c);
	} else {
		(yyval.c) = new char[128];
		strcpy((yyval.c), (yyvsp[(1) - (1)].c));
	}
;}
    break;

  case 73:

    { 
	if (is_current_file_not_cached()) {
		(yyval.c) = (yyvsp[(1) - (1)].c);
	} else {
		(yyval.c) = new char[128];
		strcpy((yyval.c), (yyvsp[(1) - (1)].c));
	}
;}
    break;

  case 74:

    { 
	
	(yyval.c) = new char[128];
	sprintf((yyval.c), "%g",(yyvsp[(1) - (1)].d)); 
;}
    break;

  case 75:

    {
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 76:

    {
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 77:

    {
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 78:

    {
	
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 79:

    {
	
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 80:

    {
	
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 81:

    {
	
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 82:

    {
	
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 83:

    {
	
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 84:

    {
	
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 85:

    {
	
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 86:

    {
	
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 87:

    {
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 88:

    {
	(yyval.c) = new char[128];
	strcpy((yyval.c), (yyvsp[(1) - (1)].c));
;}
    break;

  case 89:

    {
	(yyval.c) = new char[128];
	strcpy((yyval.c), "");
;}
    break;

  case 90:

    { 
	// any Word string should be long enough hold all concatinated Words,
	// so we do not need to malloc a new string, just strcat them.
	(yyval.c) = (yyvsp[(1) - (2)].c);
	strcat((yyvsp[(1) - (2)].c), " ");
	strcat((yyvsp[(1) - (2)].c), (yyvsp[(2) - (2)].c));
	delete [] (yyvsp[(2) - (2)].c);
;}
    break;

  case 91:

    {
	
	//std::cout << "Remark \"" << $2 << "\" at depth " << include_stack_ptr << std::endl;
	
	delete [] (yyvsp[(2) - (3)].c);
;}
    break;

  case 92:

    {
	//std::cout << "Remark \"" << $2 <<"\" at depth " << include_stack_ptr <<std::endl;
	
	delete [] (yyvsp[(2) - (3)].c);
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 93:

    {
;}
    break;

  case 94:

    {
	if (stop_include_file() == (-1)) { YYACCEPT; };
;}
    break;

  case 95:

    {
	if ((yyvsp[(1) - (1)].d) == 16.0) {
		(yyval.i) = current_color[include_stack_ptr];
	} else if ((yyvsp[(1) - (1)].d) == 24.0) {
		(yyval.i) = edge_color(current_color[include_stack_ptr]);
	} else {
		(yyval.i) = (int) (yyvsp[(1) - (1)].d);
	}
;}
    break;

  case 96:

    {
	(yyval.i) = zcolor_lookup((yyvsp[(1) - (1)].c));
	if (is_current_file_not_cached()) {
		
		delete [] (yyvsp[(1) - (1)].c);
	}
;}
    break;

  case 97:

    {
	(yyval.m) = savemat((yyvsp[(1) - (12)].d),(yyvsp[(2) - (12)].d),(yyvsp[(3) - (12)].d),(yyvsp[(4) - (12)].d),(yyvsp[(5) - (12)].d),(yyvsp[(6) - (12)].d),(yyvsp[(7) - (12)].d),(yyvsp[(8) - (12)].d),(yyvsp[(9) - (12)].d), (yyvsp[(10) - (12)].d), (yyvsp[(11) - (12)].d), (yyvsp[(12) - (12)].d));
;}
    break;

  case 98:

    {
	(yyval.m) = zmatrix_lookup((yyvsp[(1) - (1)].c));
	if (is_current_file_not_cached()) {
		
		delete [] (yyvsp[(1) - (1)].c);
	}
;}
    break;

  case 99:

    {
	float a, b, c;
	osg::Vec3f *p;
	a = (yyvsp[(1) - (3)].d);
	b = (yyvsp[(2) - (3)].d);
	c = (yyvsp[(3) - (3)].d);
	p = savept(a,b,c);
	(yyval.v) = p;
;}
    break;

  case 100:

    {
	(yyval.v) = zpoint_lookup((yyvsp[(1) - (1)].c));
	if (is_current_file_not_cached()) {
		
		delete [] (yyvsp[(1) - (1)].c);
	}
;}
    break;

  case 101:

    {
	if (is_current_file_not_cached()) {
		(yyval.c) = (yyvsp[(1) - (1)].c);
	} else {
		(yyval.c) = new char[128];
		strcpy((yyval.c), (yyvsp[(1) - (1)].c));
	}
;}
    break;

  case 102:

    {
	
	if (start_include_file((yyvsp[(4) - (5)].c)) == 0) {
		/* update transform matrices */
		push_transform((yyvsp[(3) - (5)].m));
		znamelist_push();
		current_color[include_stack_ptr] = (yyvsp[(2) - (5)].i);
	} else {
		std::cout << "Cannot find " << (yyvsp[(4) - (5)].c) << ", ignoring." << std::endl;
		}
		
		delete [] (yyvsp[(4) - (5)].c);
		;}
    break;

  case 103:

    {
			
			if (start_include_file((yyvsp[(4) - (5)].c)) == 0) {
				/* update transform matrices */
				push_transform((yyvsp[(3) - (5)].m));
				znamelist_push();
				current_color[include_stack_ptr] = (yyvsp[(2) - (5)].i);
				defer_stop_include_file();
			} else {
				std::cout << "Cannot find " << (yyvsp[(4) - (5)].c) << ", ignoring."<< std::endl;
			}
			
			delete [] (yyvsp[(4) - (5)].c);
		;}
    break;

  case 104:

    {
			*(yyvsp[(3) - (5)].v) = *(yyvsp[(3) - (5)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(4) - (5)].v) = *(yyvsp[(4) - (5)].v) * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			
			emit_line((yyvsp[(3) - (5)].v), (yyvsp[(4) - (5)].v), (yyvsp[(2) - (5)].i));
			
			
			delete (yyvsp[(3) - (5)].v);
			delete (yyvsp[(4) - (5)].v);
			
		;}
    break;

  case 105:

    {
			*(yyvsp[(3) - (5)].v) = *(yyvsp[(3) - (5)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(4) - (5)].v) = *(yyvsp[(4) - (5)].v) * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			
			emit_line((yyvsp[(3) - (5)].v), (yyvsp[(4) - (5)].v), (yyvsp[(2) - (5)].i));
			
			
			delete (yyvsp[(3) - (5)].v);
			delete (yyvsp[(4) - (5)].v);
			
			if (stop_include_file() == (-1)) { YYACCEPT; };
		;}
    break;

  case 106:

    {
			*(yyvsp[(3) - (6)].v) = *(yyvsp[(3) - (6)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(4) - (6)].v) = *(yyvsp[(4) - (6)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(5) - (6)].v) = *(yyvsp[(5) - (6)].v) * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			//current_transform[transform_stack_ptr]->mulP(*$5);
			
			emit_triangle((yyvsp[(3) - (6)].v), (yyvsp[(4) - (6)].v), (yyvsp[(5) - (6)].v), (yyvsp[(2) - (6)].i));
			
			
			delete (yyvsp[(3) - (6)].v);
			delete (yyvsp[(4) - (6)].v);
			delete (yyvsp[(5) - (6)].v);
			
		;}
    break;

  case 107:

    {
			
			*(yyvsp[(3) - (6)].v) = *(yyvsp[(3) - (6)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(4) - (6)].v) = *(yyvsp[(4) - (6)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(5) - (6)].v) = *(yyvsp[(5) - (6)].v) * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			//current_transform[transform_stack_ptr]->mulP(*$5);
			
			emit_triangle((yyvsp[(3) - (6)].v), (yyvsp[(4) - (6)].v), (yyvsp[(5) - (6)].v), (yyvsp[(2) - (6)].i));
			
			
			delete (yyvsp[(3) - (6)].v);
			delete (yyvsp[(4) - (6)].v);
			delete (yyvsp[(5) - (6)].v);
			
			if (stop_include_file() == (-1)) { YYACCEPT; };
		;}
    break;

  case 108:

    {
			
			*(yyvsp[(3) - (7)].v) = *(yyvsp[(3) - (7)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(4) - (7)].v) = *(yyvsp[(4) - (7)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(5) - (7)].v) = *(yyvsp[(5) - (7)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(6) - (7)].v) = *(yyvsp[(6) - (7)].v) * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			//current_transform[transform_stack_ptr]->mulP(*$5);
			//current_transform[transform_stack_ptr]->mulP(*$6);
			
			emit_quad((yyvsp[(3) - (7)].v), (yyvsp[(4) - (7)].v), (yyvsp[(5) - (7)].v), (yyvsp[(6) - (7)].v), (yyvsp[(2) - (7)].i));
			
			
			delete (yyvsp[(3) - (7)].v);
			delete (yyvsp[(4) - (7)].v);
			delete (yyvsp[(5) - (7)].v);
			delete (yyvsp[(6) - (7)].v);
			
		;}
    break;

  case 109:

    {
			*(yyvsp[(3) - (7)].v) = *(yyvsp[(3) - (7)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(4) - (7)].v) = *(yyvsp[(4) - (7)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(5) - (7)].v) = *(yyvsp[(5) - (7)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(6) - (7)].v) = *(yyvsp[(6) - (7)].v) * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			//current_transform[transform_stack_ptr]->mulP(*$5);
			//current_transform[transform_stack_ptr]->mulP(*$6);
			
			emit_quad((yyvsp[(3) - (7)].v), (yyvsp[(4) - (7)].v), (yyvsp[(5) - (7)].v), (yyvsp[(6) - (7)].v), (yyvsp[(2) - (7)].i));
			
			
			delete (yyvsp[(3) - (7)].v);
			delete (yyvsp[(4) - (7)].v);
			delete (yyvsp[(5) - (7)].v);
			delete (yyvsp[(6) - (7)].v);
			
			if (stop_include_file() == (-1)) { YYACCEPT; };
		;}
    break;

  case 110:

    {
			*(yyvsp[(3) - (7)].v) = *(yyvsp[(3) - (7)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(4) - (7)].v) = *(yyvsp[(4) - (7)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(5) - (7)].v) = *(yyvsp[(5) - (7)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(6) - (7)].v) = *(yyvsp[(6) - (7)].v) * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			//current_transform[transform_stack_ptr]->mulP(*$5);
			//current_transform[transform_stack_ptr]->mulP(*$6);
			
			emit_five((yyvsp[(3) - (7)].v), (yyvsp[(4) - (7)].v), (yyvsp[(5) - (7)].v), (yyvsp[(6) - (7)].v), (yyvsp[(2) - (7)].i));
			
			
			delete (yyvsp[(3) - (7)].v);
			delete (yyvsp[(4) - (7)].v);
			delete (yyvsp[(5) - (7)].v);
			delete (yyvsp[(6) - (7)].v);
			
		;}
    break;

  case 111:

    {
			*(yyvsp[(3) - (7)].v) = *(yyvsp[(3) - (7)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(4) - (7)].v) = *(yyvsp[(4) - (7)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(5) - (7)].v) = *(yyvsp[(5) - (7)].v) * *current_transform[transform_stack_ptr];
			*(yyvsp[(6) - (7)].v) = *(yyvsp[(6) - (7)].v) * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			//current_transform[transform_stack_ptr]->mulP(*$5);
			//current_transform[transform_stack_ptr]->mulP(*$6);
			
			emit_five((yyvsp[(3) - (7)].v), (yyvsp[(4) - (7)].v), (yyvsp[(5) - (7)].v), (yyvsp[(6) - (7)].v), (yyvsp[(2) - (7)].i));
			
			
			delete (yyvsp[(3) - (7)].v);
			delete (yyvsp[(4) - (7)].v);
			delete (yyvsp[(5) - (7)].v);
			delete (yyvsp[(6) - (7)].v);
			
			if (stop_include_file() == (-1)) { YYACCEPT; };
		;}
    break;


/* Line 1267 of yacc.c.  */

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
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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

  if (yyn == YYFINAL)
    YYACCEPT;

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
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




		
		void yyerror(char *s)
		{
			std::cerr << "Syntax error on line \"" << s << "\""<< std::endl;
		}
		
		osg::Vec3f * LDParse::savept(float x, float y, float z)
		{
			osg::Vec3f *tmp = new osg::Vec3f;
			tmp->set(x,y,z);
			return tmp;
		}
		
		osg::Vec3f * LDParse::copypt(osg::Vec3f *pt)
		{
			osg::Vec3f *tmp;
			tmp = new osg::Vec3f;
			*tmp = *pt;
			return tmp;
		}
		
		
		void LDParse::transform_mat_inplace(osg::Matrixf *m)
		{
			osg::Matrixf newm;
			osg::Matrixf *oldm;
			
			oldm = current_transform[transform_stack_ptr];
			
			(&newm)->mult(*oldm, *m);
			*m = newm;
			return;
		}
		
		S32 LDParse::print_transform(osg::Matrixf *m)
		{
			//m->dumpMatrix();
			std::cout << "We don't print matrices now";
			return 0;
		}
		
		void LDParse::pop_transform()
		{
			AssertFatal(transform_stack_ptr >= 0, "Error in transform stack");
			delete current_transform[transform_stack_ptr];
			transform_stack_ptr--;
		}
		
		S32 LDParse::push_rotation(osg::Vec3f *axis, F64 degrees)
		{
			// convert axis - degrees into rotation matrix
			osg::Matrixf *m = new osg::Matrixf;
			/*
			F64 a,b,c,s, veclen_inv, sin2a;
			
			// convert axis and degrees into a quaternion
			veclen_inv = 1.0/sqrt(axis->x*axis->x + axis->y*axis->y + axis->z*axis->z);
			sin2a = sin((3.1415927*degrees)/360.0);
			a = sin2a * axis->x * veclen_inv;
			b = sin2a * axis->y * veclen_inv;
			c = sin2a * axis->z * veclen_inv;
			s = cos((3.1415927*degrees)/360.0);
			// convert quaternion into a rotation matrix.
			//*/
			m->makeIdentity();
			m->rotate(degrees * M_PI/180, osg::Vec3d(*axis));
			//m->setRow(0, Point3F((float)(1 - 2*b*b-2*c*c), (float)(2*a*b - 2*s*c), (float)(2*a*c + 2*s*b)));
			//m->setRow(1, Point3F((float)(2*a*b+2*s*c), (float)(1 - 2*a*a - 2*c*c), (float)(2*b*c - 2*s*a)));
			//m->setRow(2, Point3F((float)(2*a*c - 2*s*b), (float)(2*b*c + 2*s*a), (float)(1 - 2*a*a - 2*b*b)));
			
			push_transform(m);
			
			return 0;
		}
		
		S32 LDParse::push_transform(osg::Matrixf *m)
		{
			osg::Matrixf *oldm; 
			
			AssertFatal(transform_stack_ptr < (MAX_TRANSFORM_DEPTH-1),"Transform stack overflow");
			if (transform_stack_ptr < 0) {
				if (m == NULL) {
					m = new osg::Matrixf;
					m->makeIdentity();
				}
			} else {
				oldm = current_transform[transform_stack_ptr];
				if (m == NULL) {
					m = new osg::Matrixf;
					
					*m = *oldm;
				} else {
					osg::Matrixf newm;
					newm.mult(*oldm, *m);
					*m = newm;
				}
			}
			transform_stack_ptr++;
			current_transform[transform_stack_ptr] = m;
			return 0;
		}
		
		inline void LDParse::zStep(S32 step, S32 pause){
			return;
		}
		
		inline void LDParse::zPause(){
			return;
		}
		
		inline void LDParse::zWrite(char *message){
			std::cout << "LDraw Write: " << message << std::endl;
		}
		
		inline void LDParse::zClear(){ ; // Do nothing for now
			//Con::warnf("LDraw encountered CLEAR. Ignoring");
		}
		
		inline void LDParse::zSave(S32 step){ ; // Do nothing for now
			//Con::warnf("Generating LDraw step imagery is best done with other software");
		}
		
		osg::Matrixf * LDParse::savemat(F32 x, F32 y, F32 z, 
								   F32 a, F32 b, F32 c,
								   F32 d, F32 e, F32 f,
								   F32 g, F32 h, F32 i)
		{
			osg::Matrixf *tmp = new osg::Matrixf;
			tmp->set(a, b, c, x, d, e, f, y, g, h, i, z, 0, 0, 0, 1);
			
			return tmp;
		}
		
		osg::Matrixf * LDParse::copymat(osg::Matrixf *mat)
		{
			osg::Matrixf *tmp;
			tmp = new osg::Matrixf;
			*tmp = *mat;
			return tmp;
		}
		

