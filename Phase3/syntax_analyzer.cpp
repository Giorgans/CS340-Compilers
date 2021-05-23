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
     INTCONST = 258,
     REALCONST = 259,
     ID = 260,
     STRING = 261,
     IF = 262,
     ELSE = 263,
     WHILE = 264,
     FOR = 265,
     FUNCTION = 266,
     RETURN = 267,
     BREAK = 268,
     CONTINUE = 269,
     AND = 270,
     NOT = 271,
     OR = 272,
     LOCAL = 273,
     TRUE = 274,
     FALSE = 275,
     NIL = 276,
     ASSIGN = 277,
     PLUS = 278,
     MINUS = 279,
     MULTIPLY = 280,
     DIV = 281,
     MOD = 282,
     PLUS_PLUS = 283,
     MINUS_MINUS = 284,
     LESS_THAN = 285,
     GREATER_THAN = 286,
     LESS_EQUAL = 287,
     GREATER_EQUAL = 288,
     EQUAL = 289,
     NOT_EQUAL = 290,
     L_BRACE = 291,
     R_BRACE = 292,
     L_PARENTHESIS = 293,
     R_PARENTHESIS = 294,
     L_BRACKET = 295,
     R_BRACKET = 296,
     SEMICOLON = 297,
     COMMA = 298,
     COLON = 299,
     DOUBLE_COLON = 300,
     DOT = 301,
     DOUBLE_DOT = 302,
     UMINUS = 303
   };
#endif
/* Tokens.  */
#define INTCONST 258
#define REALCONST 259
#define ID 260
#define STRING 261
#define IF 262
#define ELSE 263
#define WHILE 264
#define FOR 265
#define FUNCTION 266
#define RETURN 267
#define BREAK 268
#define CONTINUE 269
#define AND 270
#define NOT 271
#define OR 272
#define LOCAL 273
#define TRUE 274
#define FALSE 275
#define NIL 276
#define ASSIGN 277
#define PLUS 278
#define MINUS 279
#define MULTIPLY 280
#define DIV 281
#define MOD 282
#define PLUS_PLUS 283
#define MINUS_MINUS 284
#define LESS_THAN 285
#define GREATER_THAN 286
#define LESS_EQUAL 287
#define GREATER_EQUAL 288
#define EQUAL 289
#define NOT_EQUAL 290
#define L_BRACE 291
#define R_BRACE 292
#define L_PARENTHESIS 293
#define R_PARENTHESIS 294
#define L_BRACKET 295
#define R_BRACKET 296
#define SEMICOLON 297
#define COMMA 298
#define COLON 299
#define DOUBLE_COLON 300
#define DOT 301
#define DOUBLE_DOT 302
#define UMINUS 303




/* Copy the first part of user declarations.  */
#line 1 "syntax_an_v2.y"

/*** --- SYNTAX ANALYSIS YACC/BISON FILE ---
     Georgios Zervos AM:3384
	 Stylianos Michelakakis AM:3524
	 Iasonas Filippos Ntagiannis AM:3540 
     Compiled and run in Mac OS Big Sur 11.3.1 , x86 chip***/
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <list>
#include <stack>
#include "parser.h"
#include "SymbolTable_v2.h"
#include "iCode.h"

#define YY_DECL int alpha_yylex (void* yylval)
using namespace std;

int yyerror(const char *yaccProvidedMessage);
int yylex(void);
extern int yylineno;
extern char *yytext; 
extern FILE *yyin;

SymbolTable table;
extern contbreaklists *BClist;
extern stack <unsigned> LoopCounterStack;
extern stack <unsigned> functionLocalStack;
unsigned int scope=0,loop_open=0,func_open=0,func_id_num=0;
unsigned loopcounter = 0;
bool isMember=false;
bool partial=false;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 43 "syntax_an_v2.y"
{
    char *stringValue; 
    int intValue; 
    unsigned labelValue;
    double realValue;
    class Symbol *sym;
    class expr *expression;
    class forprefix *fprefix;
    class contbreaklists *BCLists;
    class calls *Call;
    class elists *elistV;
    class indexedelements *elem;
    class indexedlist *indlist;
}
/* Line 193 of yacc.c.  */
#line 243 "syntax_analyzer.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 256 "syntax_analyzer.cpp"

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
#define YYLAST   608

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNRULES -- Number of states.  */
#define YYNSTATES  189

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   303

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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    12,    14,    15,    18,
      19,    22,    24,    27,    30,    32,    34,    36,    38,    42,
      46,    50,    54,    58,    62,    66,    70,    74,    79,    84,
      88,    92,    94,    98,   101,   104,   107,   110,   113,   116,
     118,   122,   124,   126,   128,   132,   134,   136,   139,   142,
     144,   148,   153,   157,   162,   167,   170,   177,   179,   181,
     185,   191,   193,   197,   198,   202,   206,   208,   212,   218,
     219,   224,   225,   226,   228,   229,   232,   233,   238,   239,
     244,   248,   250,   252,   254,   256,   258,   260,   262,   266,
     267,   270,   275,   280,   282,   283,   284,   288,   292,   294,
     298,   306,   314,   317,   321,   322
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      50,     0,    -1,    51,    -1,    51,    52,    -1,    -1,    55,
      42,    -1,    82,    -1,    -1,    53,    88,    -1,    -1,    54,
      92,    -1,    93,    -1,    13,    42,    -1,    14,    42,    -1,
      69,    -1,    79,    -1,    42,    -1,    57,    -1,    55,    23,
      55,    -1,    55,    24,    55,    -1,    55,    25,    55,    -1,
      55,    26,    55,    -1,    55,    27,    55,    -1,    55,    30,
      55,    -1,    55,    31,    55,    -1,    55,    32,    55,    -1,
      55,    33,    55,    -1,    55,    15,    95,    55,    -1,    55,
      17,    95,    55,    -1,    55,    34,    55,    -1,    55,    35,
      55,    -1,    56,    -1,    38,    55,    39,    -1,    24,    55,
      -1,    16,    55,    -1,    28,    59,    -1,    59,    28,    -1,
      29,    59,    -1,    59,    29,    -1,    58,    -1,    59,    22,
      55,    -1,    59,    -1,    61,    -1,    66,    -1,    38,    79,
      39,    -1,    80,    -1,     5,    -1,    18,     5,    -1,    45,
       5,    -1,    60,    -1,    59,    46,     5,    -1,    59,    40,
      55,    41,    -1,    61,    46,     5,    -1,    61,    40,    55,
      41,    -1,    61,    38,    65,    39,    -1,    59,    62,    -1,
      38,    79,    39,    38,    65,    39,    -1,    63,    -1,    64,
      -1,    38,    65,    39,    -1,    47,     5,    38,    65,    39,
      -1,    55,    -1,    65,    43,    55,    -1,    -1,    40,    67,
      41,    -1,    40,    65,    41,    -1,    68,    -1,    67,    43,
      68,    -1,    36,    55,    44,    55,    37,    -1,    -1,    36,
      70,    51,    37,    -1,    -1,    -1,     5,    -1,    -1,    11,
      73,    -1,    -1,    38,    76,    81,    39,    -1,    -1,    78,
      71,    69,    72,    -1,    74,    75,    77,    -1,     3,    -1,
       4,    -1,     6,    -1,    21,    -1,    19,    -1,    20,    -1,
       5,    -1,    81,    43,     5,    -1,    -1,    83,    52,    -1,
      83,    52,    84,    52,    -1,     7,    38,    55,    39,    -1,
       8,    -1,    -1,    -1,    85,    52,    86,    -1,    89,    90,
      87,    -1,     9,    -1,    38,    55,    39,    -1,    10,    38,
      65,    42,    95,    55,    42,    -1,    91,    94,    65,    39,
      94,    87,    94,    -1,    12,    42,    -1,    12,    55,    42,
      -1,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   104,   104,   106,   113,   115,   136,   142,   142,   143,
     143,   144,   145,   151,   157,   158,   159,   162,   163,   168,
     173,   178,   183,   188,   199,   210,   221,   232,   303,   370,
     380,   390,   392,   393,   399,   413,   435,   457,   478,   499,
     501,   545,   546,   547,   548,   552,   554,   583,   598,   606,
     608,   609,   615,   616,   618,   619,   628,   634,   635,   637,
     639,   643,   644,   645,   647,   657,   671,   672,   674,   676,
     676,   678,   680,   682,   683,   686,   719,   719,   724,   724,
     726,   737,   741,   745,   750,   753,   757,   762,   775,   789,
     791,   796,   802,   834,   839,   841,   843,   845,   853,   855,
     884,   914,   925,   929,   934,   936
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTCONST", "REALCONST", "ID", "STRING",
  "IF", "ELSE", "WHILE", "FOR", "FUNCTION", "RETURN", "BREAK", "CONTINUE",
  "AND", "NOT", "OR", "LOCAL", "TRUE", "FALSE", "NIL", "ASSIGN", "PLUS",
  "MINUS", "MULTIPLY", "DIV", "MOD", "PLUS_PLUS", "MINUS_MINUS",
  "LESS_THAN", "GREATER_THAN", "LESS_EQUAL", "GREATER_EQUAL", "EQUAL",
  "NOT_EQUAL", "L_BRACE", "R_BRACE", "L_PARENTHESIS", "R_PARENTHESIS",
  "L_BRACKET", "R_BRACKET", "SEMICOLON", "COMMA", "COLON", "DOUBLE_COLON",
  "DOT", "DOUBLE_DOT", "UMINUS", "$accept", "program", "stmts", "stmt",
  "@1", "@2", "exp", "term", "assign_exp", "primary", "lvalue", "member",
  "call", "callsuffix", "normcall", "methodcall", "elist", "obj_def",
  "indexed", "index_el", "block", "@3", "funcblockstart", "funcblockend",
  "funcname", "funcprefix", "funcargs", "@4", "funcbody", "@5", "f_def",
  "const", "idlist", "if_stmt", "if_prefix", "else_prefix", "loopstart",
  "loopend", "loopstmt", "while_stmt", "whilestart", "whilecond",
  "for_prefix", "for_stmt", "ret_stmt", "N", "M", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,   303
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    49,    50,    51,    51,    52,    52,    53,    52,    54,
      52,    52,    52,    52,    52,    52,    52,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    56,    56,    56,    56,    56,    56,    56,    56,
      57,    58,    58,    58,    58,    58,    59,    59,    59,    59,
      60,    60,    60,    60,    61,    61,    61,    62,    62,    63,
      64,    65,    65,    65,    66,    66,    67,    67,    68,    70,
      69,    71,    72,    73,    73,    74,    76,    75,    78,    77,
      79,    80,    80,    80,    80,    80,    80,    81,    81,    81,
      82,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    93,    94,    95
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     1,     0,     2,     0,
       2,     1,     2,     2,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     3,
       3,     1,     3,     2,     2,     2,     2,     2,     2,     1,
       3,     1,     1,     1,     3,     1,     1,     2,     2,     1,
       3,     4,     3,     4,     4,     2,     6,     1,     1,     3,
       5,     1,     3,     0,     3,     3,     1,     3,     5,     0,
       4,     0,     0,     1,     0,     2,     0,     4,     0,     4,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       2,     4,     4,     1,     0,     0,     3,     3,     1,     3,
       7,     7,     2,     3,     0,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     2,     1,    81,    82,    46,    83,     0,    74,
       0,     0,     0,     0,     0,    85,    86,    84,     0,     0,
       0,    69,     0,    63,    16,     0,     3,     0,     0,     0,
      31,    17,    39,    41,    49,    42,    43,    14,     0,    15,
      45,     6,     7,    11,     0,    73,    75,   102,     0,    12,
      13,    34,    47,    33,     0,    35,     0,    37,     4,     0,
       0,     0,    61,     0,     0,    66,    48,    98,     8,     0,
       0,   104,    10,   105,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     0,    36,    38,
      63,     0,     0,     0,    55,    57,    58,    63,     0,     0,
      76,    78,    90,     0,   103,     0,     7,    32,    44,     0,
      65,     0,    64,     0,     0,    94,    63,    63,     0,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    29,
      30,    40,     0,     0,    50,     0,     0,     0,    52,    89,
      80,    71,    93,     7,    92,     0,    70,    63,     0,    62,
      67,     0,     7,    97,     0,     0,    27,    28,    59,    51,
      63,    54,    53,    87,     0,     0,    91,     0,     0,    99,
      95,   105,   104,     0,    77,     0,    72,    56,    68,    96,
       0,    94,    60,    88,    79,     0,   104,   100,   101
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    26,    27,    28,    62,    30,    31,    32,
      33,    34,    35,    94,    95,    96,    63,    36,    64,    65,
      37,    58,   165,   184,    46,    38,   101,   139,   140,   141,
      39,    40,   164,    41,    42,   143,   152,   179,   153,    68,
      69,   115,    71,    72,    43,   117,   118
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -168
static const yytype_int16 yypact[] =
{
    -168,     6,   115,  -168,  -168,  -168,  -168,  -168,   -24,    34,
      27,   -16,    10,   288,    36,  -168,  -168,  -168,   288,    -1,
      -1,  -168,   232,   260,  -168,    58,  -168,    14,    56,   334,
    -168,  -168,  -168,   273,  -168,   -28,  -168,  -168,    30,  -168,
    -168,  -168,   204,  -168,   288,  -168,  -168,  -168,   355,  -168,
    -168,  -168,  -168,  -168,    83,   -25,   -28,   -25,  -168,   439,
      59,   288,   523,   -34,    -5,  -168,  -168,  -168,  -168,    57,
      62,  -168,  -168,  -168,  -168,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,  -168,   288,  -168,  -168,
     288,   288,    96,    98,  -168,  -168,  -168,   288,   288,   100,
    -168,  -168,    99,   460,  -168,    67,   161,  -168,    70,   312,
    -168,   288,  -168,    74,   288,  -168,   288,   288,   288,   288,
      66,    66,  -168,  -168,  -168,   575,   575,   575,   575,   549,
     549,   523,   -15,   397,  -168,    75,   -14,   418,  -168,   109,
    -168,  -168,  -168,   204,  -168,    70,  -168,   288,   288,   523,
    -168,   481,   204,  -168,     7,    18,   562,   536,  -168,  -168,
     288,  -168,  -168,  -168,    19,    79,  -168,    21,   502,  -168,
    -168,  -168,  -168,    47,  -168,   118,  -168,  -168,  -168,  -168,
     288,  -168,  -168,  -168,  -168,   376,  -168,  -168,  -168
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -168,  -168,    72,   -41,  -168,  -168,    -2,  -168,  -168,  -168,
      51,  -168,    68,  -168,  -168,  -168,   -63,  -168,  -168,    24,
     -33,  -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,
     -19,  -168,  -168,  -168,  -168,  -168,  -168,  -168,   -43,  -168,
    -168,  -168,  -168,  -168,  -168,  -167,   -72
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -10
static const yytype_int16 yytable[] =
{
      29,   102,   119,    60,     6,   181,     3,   110,    48,   111,
      97,    51,    98,    90,    44,    91,    53,    14,    99,   188,
      59,    92,    93,    67,   158,   161,    49,   132,   111,   111,
       4,     5,     6,     7,   136,   105,   112,    54,   113,    45,
      29,    52,   103,    13,    25,    14,    15,    16,    17,   171,
     111,    18,    50,   154,   155,    19,    20,   172,   174,   109,
     177,   111,   175,    66,   111,    22,    70,    23,   100,    47,
      55,    57,    25,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   167,   131,   182,    56,    56,   133,
     111,    77,    78,    79,     9,   114,   137,   173,   108,   180,
     116,   134,   166,   135,    29,   138,   145,   142,   147,   149,
      61,   170,   151,   160,   163,    21,   156,   157,     4,     5,
       6,     7,     8,   183,    -7,    -9,     9,    10,    11,    12,
     106,    13,   176,    14,    15,    16,    17,   150,   186,    18,
       0,    29,     0,    19,    20,     0,   168,     0,     0,     0,
      29,    21,     0,    22,     0,    23,     0,    24,     0,     0,
      25,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    -9,     9,    10,    11,    12,     0,    13,   185,    14,
      15,    16,    17,     0,     0,    18,     0,     0,     0,    19,
      20,     0,     0,     0,     0,     0,     0,    21,   146,    22,
       0,    23,     0,    24,     0,     0,    25,     4,     5,     6,
       7,     8,     0,     0,    -9,     9,    10,    11,    12,     0,
      13,     0,    14,    15,    16,    17,     0,     0,    18,     0,
       0,     0,    19,    20,     0,     4,     5,     6,     7,     0,
      21,     0,    22,     9,    23,     0,    24,     0,    13,    25,
      14,    15,    16,    17,     0,     0,    18,     0,     0,     0,
      19,    20,     0,     4,     5,     6,     7,     0,     0,     0,
      22,     0,    23,     0,     0,     0,    13,    25,    14,    15,
      16,    17,     0,     0,    18,     0,     0,     0,    19,    20,
       0,     4,     5,     6,     7,    87,    61,     0,    22,     0,
      23,    88,    89,     0,    13,    25,    14,    15,    16,    17,
       0,    90,    18,    91,     0,     0,    19,    20,     0,    92,
      93,     0,     0,     0,     0,     0,    22,    73,    23,    74,
       0,     0,     0,    25,     0,    75,    76,    77,    78,    79,
       0,     0,    80,    81,    82,    83,    84,    85,     0,    73,
       0,    74,     0,     0,     0,     0,   148,    75,    76,    77,
      78,    79,     0,     0,    80,    81,    82,    83,    84,    85,
      73,     0,    74,     0,     0,     0,    86,     0,    75,    76,
      77,    78,    79,     0,     0,    80,    81,    82,    83,    84,
      85,    73,     0,    74,     0,     0,     0,   104,     0,    75,
      76,    77,    78,    79,     0,     0,    80,    81,    82,    83,
      84,    85,    73,     0,    74,     0,     0,     0,   187,     0,
      75,    76,    77,    78,    79,     0,     0,    80,    81,    82,
      83,    84,    85,    73,     0,    74,     0,     0,   159,     0,
       0,    75,    76,    77,    78,    79,     0,     0,    80,    81,
      82,    83,    84,    85,    73,     0,    74,     0,     0,   162,
       0,     0,    75,    76,    77,    78,    79,     0,     0,    80,
      81,    82,    83,    84,    85,    73,     0,    74,   107,     0,
       0,     0,     0,    75,    76,    77,    78,    79,     0,     0,
      80,    81,    82,    83,    84,    85,    73,     0,    74,   144,
       0,     0,     0,     0,    75,    76,    77,    78,    79,     0,
       0,    80,    81,    82,    83,    84,    85,    73,     0,    74,
     169,     0,     0,     0,     0,    75,    76,    77,    78,    79,
       0,     0,    80,    81,    82,    83,    84,    85,    73,   178,
      74,     0,     0,     0,     0,     0,    75,    76,    77,    78,
      79,    73,     0,    80,    81,    82,    83,    84,    85,    75,
      76,    77,    78,    79,     0,     0,    80,    81,    82,    83,
      84,    85,    75,    76,    77,    78,    79,     0,     0,    80,
      81,    82,    83,   -10,   -10,    75,    76,    77,    78,    79,
       0,     0,    80,    81,    82,    83,    84,    85,    75,    76,
      77,    78,    79,     0,     0,   -10,   -10,   -10,   -10
};

static const yytype_int16 yycheck[] =
{
       2,    42,    74,    22,     5,   172,     0,    41,    10,    43,
      38,    13,    40,    38,    38,    40,    18,    18,    46,   186,
      22,    46,    47,     9,    39,    39,    42,    90,    43,    43,
       3,     4,     5,     6,    97,    54,    41,    38,    43,     5,
      42,     5,    44,    16,    45,    18,    19,    20,    21,    42,
      43,    24,    42,   116,   117,    28,    29,    39,    39,    61,
      39,    43,    43,     5,    43,    38,    10,    40,    38,    42,
      19,    20,    45,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,   147,    87,    39,    19,    20,    91,
      43,    25,    26,    27,    11,    38,    98,   160,    39,   171,
      38,     5,   143,     5,   106,     5,    39,     8,    38,   111,
      36,   152,   114,    38,     5,    36,   118,   119,     3,     4,
       5,     6,     7,     5,     9,    10,    11,    12,    13,    14,
      58,    16,   165,    18,    19,    20,    21,   113,   181,    24,
      -1,   143,    -1,    28,    29,    -1,   148,    -1,    -1,    -1,
     152,    36,    -1,    38,    -1,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,   180,    18,
      19,    20,    21,    -1,    -1,    24,    -1,    -1,    -1,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    -1,    42,    -1,    -1,    45,     3,     4,     5,
       6,     7,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    -1,    18,    19,    20,    21,    -1,    -1,    24,    -1,
      -1,    -1,    28,    29,    -1,     3,     4,     5,     6,    -1,
      36,    -1,    38,    11,    40,    -1,    42,    -1,    16,    45,
      18,    19,    20,    21,    -1,    -1,    24,    -1,    -1,    -1,
      28,    29,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      38,    -1,    40,    -1,    -1,    -1,    16,    45,    18,    19,
      20,    21,    -1,    -1,    24,    -1,    -1,    -1,    28,    29,
      -1,     3,     4,     5,     6,    22,    36,    -1,    38,    -1,
      40,    28,    29,    -1,    16,    45,    18,    19,    20,    21,
      -1,    38,    24,    40,    -1,    -1,    28,    29,    -1,    46,
      47,    -1,    -1,    -1,    -1,    -1,    38,    15,    40,    17,
      -1,    -1,    -1,    45,    -1,    23,    24,    25,    26,    27,
      -1,    -1,    30,    31,    32,    33,    34,    35,    -1,    15,
      -1,    17,    -1,    -1,    -1,    -1,    44,    23,    24,    25,
      26,    27,    -1,    -1,    30,    31,    32,    33,    34,    35,
      15,    -1,    17,    -1,    -1,    -1,    42,    -1,    23,    24,
      25,    26,    27,    -1,    -1,    30,    31,    32,    33,    34,
      35,    15,    -1,    17,    -1,    -1,    -1,    42,    -1,    23,
      24,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    35,    15,    -1,    17,    -1,    -1,    -1,    42,    -1,
      23,    24,    25,    26,    27,    -1,    -1,    30,    31,    32,
      33,    34,    35,    15,    -1,    17,    -1,    -1,    41,    -1,
      -1,    23,    24,    25,    26,    27,    -1,    -1,    30,    31,
      32,    33,    34,    35,    15,    -1,    17,    -1,    -1,    41,
      -1,    -1,    23,    24,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    35,    15,    -1,    17,    39,    -1,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    -1,    -1,
      30,    31,    32,    33,    34,    35,    15,    -1,    17,    39,
      -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,    -1,
      -1,    30,    31,    32,    33,    34,    35,    15,    -1,    17,
      39,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      -1,    -1,    30,    31,    32,    33,    34,    35,    15,    37,
      17,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,
      27,    15,    -1,    30,    31,    32,    33,    34,    35,    23,
      24,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    35,    23,    24,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    35,    23,    24,    25,    26,    27,
      -1,    -1,    30,    31,    32,    33,    34,    35,    23,    24,
      25,    26,    27,    -1,    -1,    30,    31,    32,    33
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    50,    51,     0,     3,     4,     5,     6,     7,    11,
      12,    13,    14,    16,    18,    19,    20,    21,    24,    28,
      29,    36,    38,    40,    42,    45,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    66,    69,    74,    79,
      80,    82,    83,    93,    38,     5,    73,    42,    55,    42,
      42,    55,     5,    55,    38,    59,    61,    59,    70,    55,
      79,    36,    55,    65,    67,    68,     5,     9,    88,    89,
      10,    91,    92,    15,    17,    23,    24,    25,    26,    27,
      30,    31,    32,    33,    34,    35,    42,    22,    28,    29,
      38,    40,    46,    47,    62,    63,    64,    38,    40,    46,
      38,    75,    52,    55,    42,    79,    51,    39,    39,    55,
      41,    43,    41,    43,    38,    90,    38,    94,    95,    95,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    65,    55,     5,     5,    65,    55,     5,    76,
      77,    78,     8,    84,    39,    39,    37,    38,    44,    55,
      68,    55,    85,    87,    65,    65,    55,    55,    39,    41,
      38,    39,    41,     5,    81,    71,    52,    65,    55,    39,
      52,    42,    39,    65,    39,    43,    69,    39,    37,    86,
      95,    94,    39,     5,    72,    55,    87,    42,    94
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
        case 3:
#line 106 "syntax_an_v2.y"
    {
    if((yyval.BCLists)==NULL){
        (yyval.BCLists) = new contbreaklists();
        (yyvsp[(1) - (2)].BCLists) = new contbreaklists();
    }
    //$$->setBreakList(merge($1->getBreakList(),$2->getBreakList()));
    //$$->setContList(merge($1->getContList(),$2->getContList()));
}
    break;

  case 5:
#line 115 "syntax_an_v2.y"
    {
            resettemp();
        expr *temp=new expr(constbool_e);
        temp->setboolConst(false);
        if((yyvsp[(1) - (2)].expression)->getType()==boolexp_e){
            /*Backpatching falselist*/
            for(int i=0 ;i<(yyvsp[(1) - (2)].expression)->getFalseList().size() ; i++)
                backpatch(nextQuad(),(yyvsp[(1) - (2)].expression)->getFalseList().at(i));
        
            emit(assign,temp,NULL,(yyvsp[(1) - (2)].expression),0,yylineno);
            emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);
        
            /*Backpatching truelist*/
            for(int i=0 ;i<(yyvsp[(1) - (2)].expression)->getTrueList().size() ; i++)
                backpatch(nextQuad(),(yyvsp[(1) - (2)].expression)->getTrueList().at(i));
            
            temp=new expr(constbool_e);
            temp->setboolConst(true);
            emit(assign,temp,NULL,(yyvsp[(1) - (2)].expression),0,yylineno); 
        }
    }
    break;

  case 6:
#line 136 "syntax_an_v2.y"
    {
            (yyval.BCLists) = new contbreaklists();
            //$$->setBreakList(merge($$->getBreakList(),$1->getBreakList()));
            //$$->setContList(merge($$->getContList(),$1->getContList()));

      }
    break;

  case 7:
#line 142 "syntax_an_v2.y"
    {loop_open++;}
    break;

  case 8:
#line 142 "syntax_an_v2.y"
    {loop_open--;}
    break;

  case 9:
#line 143 "syntax_an_v2.y"
    {loop_open++;}
    break;

  case 10:
#line 143 "syntax_an_v2.y"
    {loop_open--;}
    break;

  case 12:
#line 145 "syntax_an_v2.y"
    {
          if(!loop_open) cout << "[Syntax Analysis] ERROR: Cannot use BREAK statement while not within loop, in line " << yylineno << endl;
          (yyval.BCLists) = BClist;
          (yyval.BCLists)->insertBreakList(nextQuad());
          emit(jump,NULL,NULL,NULL,0,yylineno);
      }
    break;

  case 13:
#line 151 "syntax_an_v2.y"
    {
          if(!loop_open) cout << "[Syntax Analysis] ERROR: Cannot use CONTINUE statement while not within loop, in line " << yylineno << endl;
          (yyval.BCLists) = BClist;
          (yyval.BCLists)->insertContList(nextQuad());
          emit(jump,NULL,NULL,NULL,0,yylineno);
      }
    break;

  case 14:
#line 157 "syntax_an_v2.y"
    {  (yyval.BCLists) = (yyvsp[(1) - (1)].BCLists);}
    break;

  case 17:
#line 162 "syntax_an_v2.y"
    {(yyval.expression)=(yyvsp[(1) - (1)].expression);}
    break;

  case 18:
#line 163 "syntax_an_v2.y"
    {
       (yyval.expression) = new expr(arithexp_e);
       (yyval.expression)->insertSymbol(newtemp());
       emit(add, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression), (yyval.expression),0,yylineno);
    }
    break;

  case 19:
#line 168 "syntax_an_v2.y"
    {
       (yyval.expression) = new expr(arithexp_e);
       (yyval.expression)->insertSymbol(newtemp());
       emit(sub, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression), (yyval.expression),0,yylineno);
    }
    break;

  case 20:
#line 173 "syntax_an_v2.y"
    {
       (yyval.expression) = new expr(arithexp_e);
       (yyval.expression)->insertSymbol(newtemp());
       emit(mul, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression), (yyval.expression),0,yylineno);
    }
    break;

  case 21:
#line 178 "syntax_an_v2.y"
    {
       (yyval.expression) = new expr(arithexp_e);
       (yyval.expression)->insertSymbol(newtemp());
       emit(divide, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression), (yyval.expression),0,yylineno);
    }
    break;

  case 22:
#line 183 "syntax_an_v2.y"
    {
       (yyval.expression) = new expr(arithexp_e);
       (yyval.expression)->insertSymbol(newtemp());
       emit(mod, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression), (yyval.expression), 0, yylineno);
    }
    break;

  case 23:
#line 188 "syntax_an_v2.y"
    {
        (yyval.expression) = new expr(boolexp_e);
        (yyval.expression)->insertSymbol(newtemp());
        
        (yyval.expression)->insertTrueLabel(nextQuad());
        emit(if_less,(yyvsp[(1) - (3)].expression),(yyvsp[(3) - (3)].expression),NULL ,0, yylineno);
 
        (yyval.expression)->insertFalseLabel(nextQuad());
        emit(jump,NULL,NULL,NULL,0,yylineno);

    }
    break;

  case 24:
#line 199 "syntax_an_v2.y"
    {
        (yyval.expression) = new expr(boolexp_e);
        (yyval.expression)->insertSymbol(newtemp());
       
        (yyval.expression)->insertTrueLabel(nextQuad());
        emit(if_greater,(yyvsp[(1) - (3)].expression),(yyvsp[(3) - (3)].expression),NULL ,0, yylineno);
        
        (yyval.expression)->insertFalseLabel(nextQuad());
        emit(jump,NULL,NULL,NULL,0,yylineno);

    }
    break;

  case 25:
#line 210 "syntax_an_v2.y"
    {
        (yyval.expression) = new expr(boolexp_e);
        (yyval.expression)->insertSymbol(newtemp());
        
        (yyval.expression)->insertTrueLabel(nextQuad());
        emit(if_lesseq,(yyvsp[(1) - (3)].expression),(yyvsp[(3) - (3)].expression),NULL ,0, yylineno);
        
        (yyval.expression)->insertFalseLabel(nextQuad());
        emit(jump,NULL,NULL,NULL,0,yylineno);

    }
    break;

  case 26:
#line 221 "syntax_an_v2.y"
    {
        (yyval.expression) = new expr(boolexp_e);
        (yyval.expression)->insertSymbol(newtemp());
        
        (yyval.expression)->insertTrueLabel(nextQuad());
        emit(if_greatereq,(yyvsp[(1) - (3)].expression),(yyvsp[(3) - (3)].expression),NULL ,0, yylineno);
        
        (yyval.expression)->insertFalseLabel(nextQuad());
        emit(jump,NULL,NULL,NULL,0,yylineno);

    }
    break;

  case 27:
#line 232 "syntax_an_v2.y"
    {
 
        
        if((yyvsp[(1) - (4)].expression)->getType()!=boolexp_e){
            expr *temp = new expr(constbool_e);
            temp->setboolConst(true);
            emit(if_eq,(yyvsp[(1) - (4)].expression),temp,NULL ,0, yylineno);
        
            emit(jump,NULL,NULL,NULL,0,yylineno);

            if((yyvsp[(1) - (4)].expression)->getNotGate()){
                (yyvsp[(1) - (4)].expression)->insertTrueLabel(nextQuad()-1);
                (yyvsp[(1) - (4)].expression)->insertFalseLabel(nextQuad()-2);
            }
            else{
                (yyvsp[(1) - (4)].expression)->insertTrueLabel(nextQuad()-2);
                (yyvsp[(1) - (4)].expression)->insertFalseLabel(nextQuad()-1);
            }
            (yyvsp[(3) - (4)].labelValue) = nextQuad();
        }
        
        if((yyvsp[(4) - (4)].expression)->getType()!=boolexp_e){
            expr *temp = new expr(constbool_e);
            temp->setboolConst(true);
            emit(if_eq,(yyvsp[(4) - (4)].expression),temp,NULL ,0, yylineno);
            
            emit(jump,NULL,NULL,NULL,0,yylineno);
            
            if((yyvsp[(4) - (4)].expression)->getNotGate()){
                (yyvsp[(4) - (4)].expression)->insertTrueLabel(nextQuad()-1);
                (yyvsp[(4) - (4)].expression)->insertFalseLabel(nextQuad()-2);
            }
            else{
                (yyvsp[(4) - (4)].expression)->insertTrueLabel(nextQuad()-2);
                (yyvsp[(4) - (4)].expression)->insertFalseLabel(nextQuad()-1);
            }        
        }



       cout << "\nM value: " << (yyvsp[(3) - (4)].labelValue) ; 
        cout << "\nAND E1 TRUE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<(yyvsp[(1) - (4)].expression)->getTrueList().size() ; i++ )
            cout <<  (yyvsp[(1) - (4)].expression)->getTrueList().at(i);
        cout << "\nAND E2 TRUE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<(yyvsp[(4) - (4)].expression)->getTrueList().size() ; i++ )
            cout <<  (yyvsp[(4) - (4)].expression)->getTrueList().at(i);
	    
        cout << "\nAND E1 FALSE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<(yyvsp[(1) - (4)].expression)->getFalseList().size() ; i++ )
            cout <<  (yyvsp[(1) - (4)].expression)->getFalseList().at(i);
        cout << "\nAND E2 FALSE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<(yyvsp[(4) - (4)].expression)->getFalseList().size() ; i++ )
            cout <<  (yyvsp[(4) - (4)].expression)->getFalseList().at(i);
        cout << "\n";        

        for(int i=0 ;i<(yyvsp[(1) - (4)].expression)->getTrueList().size() ; i++)
            backpatch((yyvsp[(3) - (4)].labelValue),(yyvsp[(1) - (4)].expression)->getTrueList().at(i));
        
        (yyval.expression) = new expr(boolexp_e);
        (yyval.expression)->insertSymbol(newtemp());
        (yyval.expression)->setTrueList((yyvsp[(4) - (4)].expression)->getTrueList());
        (yyval.expression)->setFalseList(merge((yyvsp[(1) - (4)].expression)->getFalseList(),(yyvsp[(4) - (4)].expression)->getFalseList() ));
        
        cout << "\nAND E TRUE LIST" << "\t" ;
	    for(unsigned i=0 ; i<(yyval.expression)->getTrueList().size() ; i++ )
            cout <<  (yyval.expression)->getTrueList().at(i) << " ";
        cout << "\nAND E FALSE LIST" << "\t" ;
	    for(unsigned i=0 ; i<(yyval.expression)->getFalseList().size() ; i++ )
            cout <<  (yyval.expression)->getFalseList().at(i) << " ";
    }
    break;

  case 28:
#line 303 "syntax_an_v2.y"
    {

        
        if((yyvsp[(1) - (4)].expression)->getType()!=boolexp_e){
            expr *temp = new expr(constbool_e);
            temp->setboolConst(true);
            emit(if_eq,(yyvsp[(1) - (4)].expression),temp,NULL ,0, yylineno);
        
            emit(jump,NULL,NULL,NULL,0,yylineno);

            if((yyvsp[(1) - (4)].expression)->getNotGate()){
                (yyvsp[(1) - (4)].expression)->insertTrueLabel(nextQuad()-1);
                (yyvsp[(1) - (4)].expression)->insertFalseLabel(nextQuad()-2);
            }
            else{
                (yyvsp[(1) - (4)].expression)->insertTrueLabel(nextQuad()-2);
                (yyvsp[(1) - (4)].expression)->insertFalseLabel(nextQuad()-1);
            }
            (yyvsp[(3) - (4)].labelValue) = nextQuad();
        }
        
        if((yyvsp[(4) - (4)].expression)->getType()!=boolexp_e){
            expr *temp = new expr(constbool_e);
            temp->setboolConst(true);
            emit(if_eq,(yyvsp[(4) - (4)].expression),temp,NULL ,0, yylineno);
            
            emit(jump,NULL,NULL,NULL,0,yylineno);
            
            if((yyvsp[(4) - (4)].expression)->getNotGate()){
                (yyvsp[(4) - (4)].expression)->insertTrueLabel(nextQuad()-1);
                (yyvsp[(4) - (4)].expression)->insertFalseLabel(nextQuad()-2);
            }
            else{
                (yyvsp[(4) - (4)].expression)->insertTrueLabel(nextQuad()-2);
                (yyvsp[(4) - (4)].expression)->insertFalseLabel(nextQuad()-1);
            }        
        }
        /*cout << "\n\nM value: " << $3 ; 
        cout << "\nOR E1 TRUE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$1->getTrueList().size() ; i++ )
            cout <<  $1->getTrueList().at(i);
        cout << "\nOR E2 TRUE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$4->getTrueList().size() ; i++ )
            cout <<  $4->getTrueList().at(i);
	    
        cout << "\nOR E1 FALSE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$1->getFalseList().size() ; i++ )
            cout <<  $1->getFalseList().at(i);
        cout << "\nOR E2 FALSE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$4->getFalseList().size() ; i++ )
            cout <<  $4->getFalseList().at(i);*/

        (yyval.expression) = new expr(boolexp_e);
        (yyval.expression)->insertSymbol(newtemp());
        for(int i=0 ;i<(yyvsp[(1) - (4)].expression)->getFalseList().size() ; i++)
            backpatch((yyvsp[(3) - (4)].labelValue),(yyvsp[(1) - (4)].expression)->getFalseList().at(i));
        
        (yyval.expression)->setTrueList(merge((yyvsp[(1) - (4)].expression)->getTrueList(),(yyvsp[(4) - (4)].expression)->getTrueList()));
        (yyval.expression)->setFalseList((yyvsp[(4) - (4)].expression)->getFalseList());
        
        /*cout << "\nOR E TRUE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$$->getTrueList().size() ; i++ )
            cout <<  $$->getTrueList().at(i) << " ";
        cout << "\nOR E FALSE LIST:" << "\t" ;
	    for(unsigned i=0 ; i<$$->getFalseList().size() ; i++ )
            cout <<  $$->getFalseList().at(i) << " \n";*/
    }
    break;

  case 29:
#line 370 "syntax_an_v2.y"
    {
        (yyval.expression) = new expr(boolexp_e);
        (yyval.expression)->insertSymbol(newtemp());
       
        (yyval.expression)->insertTrueLabel(nextQuad());
        emit(if_eq,(yyvsp[(1) - (3)].expression),(yyvsp[(3) - (3)].expression),NULL ,0, yylineno);
        
        (yyval.expression)->insertFalseLabel(nextQuad());
        emit(jump,NULL,NULL,NULL,0,yylineno);
    }
    break;

  case 30:
#line 380 "syntax_an_v2.y"
    {
        (yyval.expression) = new expr(boolexp_e);
        (yyval.expression)->insertSymbol(newtemp());
       
        (yyval.expression)->insertTrueLabel(nextQuad());
        emit(if_noteq,(yyvsp[(1) - (3)].expression),(yyvsp[(3) - (3)].expression),NULL ,0, yylineno);
        
        (yyval.expression)->insertFalseLabel(nextQuad());
        emit(jump,NULL,NULL,NULL,0,yylineno);
    }
    break;

  case 31:
#line 390 "syntax_an_v2.y"
    {(yyval.expression)=(yyvsp[(1) - (1)].expression);}
    break;

  case 32:
#line 392 "syntax_an_v2.y"
    {(yyval.expression)=(yyvsp[(2) - (3)].expression);}
    break;

  case 33:
#line 393 "syntax_an_v2.y"
    {
        checkuminus((yyvsp[(2) - (2)].expression));
        (yyval.expression)=new expr(arithexp_e);
        (yyval.expression)->insertSymbol(newtemp());
        emit(uminus,(yyvsp[(2) - (2)].expression),NULL,(yyval.expression),0,yylineno);
    }
    break;

  case 34:
#line 399 "syntax_an_v2.y"
    {
        (yyval.expression)=(yyvsp[(2) - (2)].expression);
        vector <unsigned> templist = (yyvsp[(2) - (2)].expression)->getTrueList();
        (yyval.expression)->setTrueList((yyvsp[(2) - (2)].expression)->getFalseList());
        (yyval.expression)->setFalseList(templist);
        (yyval.expression)->setNotGate(true);

        cout << "\nnot E TRUE LIST" << "\t" ;
	    for(unsigned i=0 ; i<(yyval.expression)->getTrueList().size() ; i++ )
            cout <<  (yyval.expression)->getTrueList().at(i) << " ";
        cout << "\nnot E FALSE LIST" << "\t" ;
	    for(unsigned i=0 ; i<(yyval.expression)->getFalseList().size() ; i++ )
            cout <<  (yyval.expression)->getFalseList().at(i) << " ";
    }
    break;

  case 35:
#line 413 "syntax_an_v2.y"
    {
        Symbol *symbol=table.Lookup((yyvsp[(2) - (2)].expression)->getSymbol()->getName());
        if(symbol->getType()==programfunc_s || symbol->getType()==libraryfunc_s)
            cout <<"[Syntax Analysis] ERROR: Trying to left increase the function \"" << (yyvsp[(2) - (2)].expression) << "\" by 1, in line " <<  yylineno << endl;

        if((yyvsp[(2) - (2)].expression)->getType() == tableitem_e){
            (yyval.expression) =  emit_iftableitem((yyvsp[(2) - (2)].expression));
            expr *temp = new expr(costnum_e);
            temp->setnumconst(1);
            emit(add,(yyval.expression),temp,(yyval.expression),0,yylineno);
            emit(tablesetelem,(yyvsp[(2) - (2)].expression)->getIndex(),(yyval.expression),(yyvsp[(2) - (2)].expression),0,yylineno);
        }
        else{
            expr *temp = new expr(costnum_e);
            temp->setnumconst(1);
            emit(add,(yyvsp[(2) - (2)].expression),temp,(yyvsp[(2) - (2)].expression),0,yylineno);
            (yyval.expression) = new expr(arithexp_e);
            (yyval.expression)->insertSymbol(newtemp());
            emit(assign,(yyvsp[(2) - (2)].expression),NULL,(yyval.expression),0,yylineno);
        }

    }
    break;

  case 36:
#line 435 "syntax_an_v2.y"
    {
        Symbol *symbol=table.Lookup((yyvsp[(1) - (2)].expression)->getSymbol()->getName());
        if(symbol->getType()==programfunc_s || symbol->getType()==libraryfunc_s)
            cout <<"[Syntax Analysis] ERROR: Trying to right increase the function \"" << (yyvsp[(1) - (2)].expression) << "\" by 1, in line " <<yylineno << endl;
    
        (yyval.expression) = new expr(var_e);
        (yyval.expression)->insertSymbol(newtemp());
        if((yyvsp[(1) - (2)].expression)->getType() == tableitem_e){
            expr *value = emit_iftableitem((yyvsp[(1) - (2)].expression));
            expr *temp = new expr(costnum_e);
            temp->setnumconst(1);
            emit(assign,value,NULL,(yyval.expression),0,yylineno);
            emit(add,value,temp,value,0,yylineno);
            emit(tablesetelem,(yyvsp[(1) - (2)].expression)->getIndex(),value,(yyvsp[(1) - (2)].expression),0,yylineno);
        }
        else{
            emit(assign,(yyvsp[(1) - (2)].expression),NULL,(yyval.expression),0,yylineno);
            expr *temp = new expr(costnum_e);
            temp->setnumconst(1);
            emit(add,(yyvsp[(1) - (2)].expression),temp,(yyvsp[(1) - (2)].expression),0,yylineno);
        }
    }
    break;

  case 37:
#line 457 "syntax_an_v2.y"
    {
        Symbol *symbol=table.Lookup((yyvsp[(2) - (2)].expression)->getSymbol()->getName());
        if(symbol->getType()==programfunc_s || symbol->getType()==libraryfunc_s)
            cout <<"[Syntax Analysis] ERROR: Trying to left decrease the function \"" << (yyvsp[(2) - (2)].expression) << "\" by 1, in line" << yylineno << endl;
    
        if((yyvsp[(2) - (2)].expression)->getType() == tableitem_e){
            (yyval.expression) =  emit_iftableitem((yyvsp[(2) - (2)].expression));
            expr *temp = new expr(costnum_e);
            temp->setnumconst(-1);
            emit(add,(yyval.expression),temp,(yyval.expression),0,yylineno);
            emit(tablesetelem,(yyvsp[(2) - (2)].expression)->getIndex(),(yyval.expression),(yyvsp[(2) - (2)].expression),0,yylineno);
        }
        else{
            expr *temp = new expr(costnum_e);
            temp->setnumconst(-1);
            emit(add,(yyvsp[(2) - (2)].expression),temp,(yyvsp[(2) - (2)].expression),0,yylineno);
            (yyval.expression) = new expr(arithexp_e);
            (yyval.expression)->insertSymbol(newtemp());
            emit(assign,(yyvsp[(2) - (2)].expression),NULL,(yyval.expression),0,yylineno);
        }    
    }
    break;

  case 38:
#line 478 "syntax_an_v2.y"
    {
        Symbol *symbol=table.Lookup((yyvsp[(1) - (2)].expression)->getSymbol()->getName());
        if(symbol->getType()==programfunc_s || symbol->getType()==libraryfunc_s)
            cout <<"[Syntax Analysis] ERROR: Trying to right decrease the function \"" << (yyvsp[(1) - (2)].expression) << "\" by 1, in line " << yylineno << endl;
            
        (yyval.expression) = new expr(var_e);
        (yyval.expression)->insertSymbol(newtemp());
        if((yyvsp[(1) - (2)].expression)->getType() == tableitem_e){
            expr *value = emit_iftableitem((yyvsp[(1) - (2)].expression));
            expr *temp = new expr(costnum_e);
            temp->setnumconst(-1);
            emit(assign,value,NULL,(yyval.expression),0,yylineno);
            emit(add,value,temp,value,0,yylineno);
            emit(tablesetelem,(yyvsp[(1) - (2)].expression)->getIndex(),value,(yyvsp[(1) - (2)].expression),0,yylineno);
        }
        else{
            emit(assign,(yyvsp[(1) - (2)].expression),NULL,(yyval.expression),0,yylineno);
            expr *temp = new expr(costnum_e);
            temp->setnumconst(-1);
            emit(add,(yyvsp[(1) - (2)].expression),temp,(yyvsp[(1) - (2)].expression),0,yylineno);
        }    }
    break;

  case 39:
#line 499 "syntax_an_v2.y"
    {(yyval.expression)=(yyvsp[(1) - (1)].expression);}
    break;

  case 40:
#line 501 "syntax_an_v2.y"
    {
    Symbol *symbol;
        symbol=table.LookupScope((yyvsp[(1) - (3)].expression)->getSymbol()->getName(),0);
        if(symbol!=NULL && !isMember && symbol->getType()==libraryfunc_s)
            cout << "[Syntax Analysis] ERROR: Trying to assign a value to \"" << (yyvsp[(1) - (3)].expression) << "\" library function, in line "<< yylineno << endl;
        for(int i=scope ; i>=0 ; i--){
            symbol=table.LookupScope((yyvsp[(1) - (3)].expression)->getSymbol()->getName(),i);
            if(symbol!=NULL && !isMember && scope==i && symbol->getType()==programfunc_s)
                cout << "[Syntax Analysis] ERROR: Trying to assign a value to \"" << (yyvsp[(1) - (3)].expression) << "\" user function, in line "<< yylineno << endl;
        }  
        if(isMember) isMember=false;
        
        if((yyvsp[(3) - (3)].expression)->getType()==boolexp_e){
            expr *temp=new expr(constbool_e);
            temp->setboolConst(false);
        
            /*Backpatching falselist*/
            for(int i=0 ;i<(yyvsp[(3) - (3)].expression)->getFalseList().size() ; i++)
                backpatch(nextQuad(),(yyvsp[(3) - (3)].expression)->getFalseList().at(i));
        
            emit(assign,temp,NULL,(yyvsp[(3) - (3)].expression),0,yylineno);
            emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);
        
            /*Backpatching truelist*/
            for(int i=0 ;i<(yyvsp[(3) - (3)].expression)->getTrueList().size() ; i++)
                backpatch(nextQuad(),(yyvsp[(3) - (3)].expression)->getTrueList().at(i));

            temp=new expr(constbool_e);
            temp->setboolConst(true);
            emit(assign,temp,NULL,(yyvsp[(3) - (3)].expression),0,yylineno); 
        }

        if((yyvsp[(1) - (3)].expression)->getType()==tableitem_e){
            emit(tablesetelem,(yyvsp[(1) - (3)].expression)->getIndex(),(yyvsp[(3) - (3)].expression),(yyvsp[(1) - (3)].expression),0,yylineno);
            (yyval.expression)=emit_iftableitem((yyvsp[(1) - (3)].expression));
        }
        else{
            emit(assign,(yyvsp[(3) - (3)].expression),NULL,(yyvsp[(1) - (3)].expression),0,yylineno);
            (yyval.expression) = new expr(assignexp_e);
            (yyval.expression)->insertSymbol(newtemp());
            emit(assign,(yyvsp[(1) - (3)].expression),NULL,(yyval.expression),0,yylineno);
        }
}
    break;

  case 41:
#line 545 "syntax_an_v2.y"
    { (yyval.expression) = emit_iftableitem((yyvsp[(1) - (1)].expression));}
    break;

  case 44:
#line 548 "syntax_an_v2.y"
    {
            (yyval.expression) = new expr(programfunc_e);
            (yyval.expression)->insertSymbol((yyvsp[(2) - (3)].sym));
        }
    break;

  case 45:
#line 552 "syntax_an_v2.y"
    {(yyval.expression)=(yyvsp[(1) - (1)].expression);}
    break;

  case 46:
#line 554 "syntax_an_v2.y"
    {

            Symbol *symbol,*temp;
            expr *expression;
            unsigned int temp_scope;
            bool found = false;
            symbol = table.LookupScope((yyvsp[(1) - (1)].stringValue),scope);
            for(int i=scope ; i>=0 ; i--){
                temp=table.LookupScope((yyvsp[(1) - (1)].stringValue),i);
                if(temp!=NULL && temp->IsActive() ){
                    found=true;
                    temp_scope=i;
                    break;
                }
            }

            if(!found){
                symbol = new Symbol(var_s,(yyvsp[(1) - (1)].stringValue),currscopespace(),scope,yylineno,currscopeoffset());
                table.Insert(symbol);
                (yyval.expression) = lvalue_exp(symbol);
            }
            else{
                symbol=table.LookupScope((yyvsp[(1) - (1)].stringValue),temp_scope);
                if(temp!=NULL && temp->IsActive() && !loop_open && func_open && scope>temp_scope && !isMember && temp->getType()==var_s)
                    cout << "[Syntax Analysis] ERROR: Cannot access \"" << (yyvsp[(1) - (1)].stringValue) << "\", in line " << yylineno << endl;
                (yyval.expression) = lvalue_exp(symbol);
            }
            
}
    break;

  case 47:
#line 583 "syntax_an_v2.y"
    {
            Symbol *symbol,*temp;
            symbol = table.LookupScope((yyvsp[(2) - (2)].stringValue),scope);
            if(symbol==NULL){
                temp=table.LookupScope((yyvsp[(2) - (2)].stringValue),0);
                if(temp!=NULL && temp->getType()==libraryfunc_s)
                    cout << "[Syntax Analysis] ERROR: Collision with library function \""<< (yyvsp[(2) - (2)].stringValue) << "\", in line " << yylineno << endl;                
                else{
                    table.Insert(new Symbol(var_s,(yyvsp[(2) - (2)].stringValue),currscopespace(),scope,yylineno,currscopeoffset()));
                }   
            }
            symbol=table.LookupScope((yyvsp[(2) - (2)].stringValue),scope);
            (yyval.expression) = lvalue_exp(symbol);

    }
    break;

  case 48:
#line 598 "syntax_an_v2.y"
    { 
            Symbol *symbol=table.LookupScope((yyvsp[(2) - (2)].stringValue),0);
            if(symbol==NULL)
                cout << "[Syntax Analysis] ERROR: There is no declaration of global var \"" << (yyvsp[(2) - (2)].stringValue) <<"\", in line " << yylineno << endl;  
            (yyval.expression) = lvalue_exp(symbol); 
    }
    break;

  case 49:
#line 606 "syntax_an_v2.y"
    {isMember=true;}
    break;

  case 50:
#line 608 "syntax_an_v2.y"
    {(yyval.expression) = member_item((yyvsp[(1) - (3)].expression),(yyvsp[(3) - (3)].stringValue));}
    break;

  case 51:
#line 609 "syntax_an_v2.y"
    {
        (yyvsp[(1) - (4)].expression) = emit_iftableitem((yyvsp[(1) - (4)].expression));
        (yyval.expression) = new expr(tableitem_e);
        (yyval.expression)->insertSymbol((yyvsp[(1) - (4)].expression)->getSymbol());
        (yyval.expression)->setIndex((yyvsp[(3) - (4)].expression));
    }
    break;

  case 54:
#line 618 "syntax_an_v2.y"
    { (yyval.expression) = make_call((yyvsp[(1) - (4)].expression),(yyvsp[(3) - (4)].elistV)); }
    break;

  case 55:
#line 619 "syntax_an_v2.y"
    {
        if((yyvsp[(2) - (2)].Call)->getMethod()){
            expr *self = (yyvsp[(1) - (2)].expression);
            (yyvsp[(1) - (2)].expression) = emit_iftableitem(member_item(self,(yyvsp[(2) - (2)].Call)->getName()));  // TO DO!
            (yyvsp[(2) - (2)].Call)->getElist()->pushfrontElistItem(self);
        }

        (yyval.expression) = make_call((yyvsp[(1) - (2)].expression),(yyvsp[(2) - (2)].Call)->getElist());
    }
    break;

  case 56:
#line 628 "syntax_an_v2.y"
    {
        expr *func = new expr(programfunc_e);
        func->insertSymbol((yyvsp[(2) - (6)].sym));
        (yyval.expression) = make_call(func,(yyvsp[(5) - (6)].elistV));
    }
    break;

  case 57:
#line 634 "syntax_an_v2.y"
    {(yyval.Call)=(yyvsp[(1) - (1)].Call);}
    break;

  case 58:
#line 635 "syntax_an_v2.y"
    {(yyval.Call)=(yyvsp[(1) - (1)].Call);}
    break;

  case 59:
#line 637 "syntax_an_v2.y"
    { (yyval.Call) = new calls("nil",false,(yyvsp[(2) - (3)].elistV));  }
    break;

  case 60:
#line 639 "syntax_an_v2.y"
    {
    (yyval.Call) = new calls((yyvsp[(2) - (5)].stringValue),true,(yyvsp[(4) - (5)].elistV));
}
    break;

  case 61:
#line 643 "syntax_an_v2.y"
    {(yyval.elistV)=new elists((yyvsp[(1) - (1)].expression));}
    break;

  case 62:
#line 644 "syntax_an_v2.y"
    {(yyval.elistV)=(yyvsp[(1) - (3)].elistV); (yyval.elistV)->insertElistItem((yyvsp[(3) - (3)].expression));}
    break;

  case 63:
#line 645 "syntax_an_v2.y"
    {(yyval.elistV)=new elists();}
    break;

  case 64:
#line 647 "syntax_an_v2.y"
    {
        expr *temp = new expr(newtable_e),*tempi;
        temp->insertSymbol(newtemp());
        emit(tablecreate,NULL,NULL,temp,0,yylineno);
        for(list <indexedelements>::iterator i = (yyvsp[(2) - (3)].indlist)->getIndexedList()->begin() ;  i!=(yyvsp[(2) - (3)].indlist)->getIndexedList()->end() ; i++)
            emit(tablesetelem,i->getIndexElement(),i->getValueElement(),temp,0,yylineno);
        
        (yyval.expression) = temp;

}
    break;

  case 65:
#line 657 "syntax_an_v2.y"
    {
        expr *temp = new expr(newtable_e),*tempi;
        temp->insertSymbol(newtemp());
        emit(tablecreate,NULL,NULL,temp,0,yylineno);
        int count = 0;
        for(list <expr>::iterator i = (yyvsp[(2) - (3)].elistV)->getElist()->begin() ; i!=(yyvsp[(2) - (3)].elistV)->getElist()->end() ; i++){
            tempi = new expr(costnum_e);
            tempi->setnumconst(count);
            emit(tablesetelem,tempi,&*i,temp,0,yylineno);
            count++;
        }
        (yyval.expression) = temp;
    }
    break;

  case 66:
#line 671 "syntax_an_v2.y"
    {(yyval.indlist) = new indexedlist(); (yyval.indlist)->insertElements((yyvsp[(1) - (1)].elem));}
    break;

  case 67:
#line 672 "syntax_an_v2.y"
    { (yyval.indlist) = (yyvsp[(1) - (3)].indlist); (yyval.indlist)->insertElements((yyvsp[(3) - (3)].elem)); }
    break;

  case 68:
#line 674 "syntax_an_v2.y"
    {(yyval.elem)=new indexedelements((yyvsp[(2) - (5)].expression),(yyvsp[(4) - (5)].expression));}
    break;

  case 69:
#line 676 "syntax_an_v2.y"
    {scope++;}
    break;

  case 70:
#line 676 "syntax_an_v2.y"
    {table.Hide(scope); scope--; (yyval.BCLists) = (yyvsp[(3) - (4)].BCLists);}
    break;

  case 71:
#line 678 "syntax_an_v2.y"
    {LoopCounterStack.push(loopcounter); loopcounter=0; }
    break;

  case 72:
#line 680 "syntax_an_v2.y"
    { loopcounter = LoopCounterStack.top(); LoopCounterStack.pop(); }
    break;

  case 73:
#line 682 "syntax_an_v2.y"
    { (yyval.stringValue) = (yyvsp[(1) - (1)].stringValue); }
    break;

  case 74:
#line 683 "syntax_an_v2.y"
    { string fname = "n$" + to_string(func_id_num); func_id_num++; (yyval.stringValue) = strcpy(new char[fname.length() + 1], fname.c_str()); }
    break;

  case 75:
#line 686 "syntax_an_v2.y"
    {
        Symbol *symbol;
        symbol=table.LookupScope((yyvsp[(2) - (2)].stringValue),scope);
        if(symbol!=NULL && symbol->IsActive()){
            if(symbol->getType()==programfunc_s)
             {
                cout<< "[Syntax Analysis] ERROR: Redefinition of \"" << (yyvsp[(2) - (2)].stringValue) << "\" as function, in line " << yylineno << endl;
                cout<< "\tNote: Previous definition of \""<< (yyvsp[(2) - (2)].stringValue) << "\" in line " << symbol->getLine() << endl;
             }   
            if(!scope && symbol->getType()==libraryfunc_s){
                //$$ = new expr(libraryfunc_e);
                //$$->insertSymbol(symbol);
                cout << "[Syntax Analysis] ERROR: Collision with library function \"" << (yyvsp[(2) - (2)].stringValue) << "\", in line " << yylineno << endl;                
            }
            if(symbol!=NULL  && symbol->getType()==var_s){
                cout << "[Syntax Analysis] ERROR: Redefinition of \"" << (yyvsp[(2) - (2)].stringValue) << "\" as function, in line " << yylineno << endl;
                cout<< "\tNote: Previous definition of \""<< (yyvsp[(2) - (2)].stringValue) << "\" in line " << symbol->getLine() << endl;
            }
        }
        else{
            symbol = new Symbol(programfunc_s,(yyvsp[(2) - (2)].stringValue),currscopespace(),scope,yylineno,currscopeoffset());
            table.Insert(symbol);
            symbol->setiAddress(nextQuad());
            (yyval.sym) = symbol;
            emit(funcstart,NULL,NULL,lvalue_exp(symbol),0,yylineno);
            functionLocalStack.push(getfunctionLocalOffset());
            enterscopespace();
            resetformalargoffset();

        }

    }
    break;

  case 76:
#line 719 "syntax_an_v2.y"
    {scope++;}
    break;

  case 77:
#line 719 "syntax_an_v2.y"
    {
    enterscopespace();
    resetfunctionlocaloffset();
}
    break;

  case 78:
#line 724 "syntax_an_v2.y"
    { scope--; func_open++;}
    break;

  case 79:
#line 724 "syntax_an_v2.y"
    {func_open--; exitscopespace(); }
    break;

  case 80:
#line 726 "syntax_an_v2.y"
    { 
    exitscopespace(); 
    (yyval.sym)->setTotalLocals(getfunctionLocalOffset());
    setfunctionLocalOffset(functionLocalStack.top());
    functionLocalStack.pop();

    (yyval.sym)=(yyvsp[(1) - (3)].sym);
    emit(funcend,NULL,NULL,lvalue_exp((yyvsp[(1) - (3)].sym)),0,yylineno);

}
    break;

  case 81:
#line 737 "syntax_an_v2.y"
    { 
                    (yyval.expression) = new expr(costnum_e);
                    (yyval.expression)->setnumconst((yyvsp[(1) - (1)].intValue));
                }
    break;

  case 82:
#line 741 "syntax_an_v2.y"
    {
                    (yyval.expression) = new expr(costnum_e);
                    (yyval.expression)->setnumconst((yyvsp[(1) - (1)].realValue));
                }
    break;

  case 83:
#line 745 "syntax_an_v2.y"
    {
                    (yyval.expression) = new expr(conststring_e);
                    (yyval.expression)->setstrConst((yyvsp[(1) - (1)].stringValue));

                }
    break;

  case 84:
#line 750 "syntax_an_v2.y"
    {
                    (yyval.expression) = new expr(nil_e);
                }
    break;

  case 85:
#line 753 "syntax_an_v2.y"
    {
                    (yyval.expression) = new expr(constbool_e);
                    (yyval.expression)->setboolConst(true);
                }
    break;

  case 86:
#line 757 "syntax_an_v2.y"
    {
                    (yyval.expression) = new expr(constbool_e);
                    (yyval.expression)->setboolConst(false);
                }
    break;

  case 87:
#line 762 "syntax_an_v2.y"
    {
    Symbol *symbol = table.LookupScope((yyvsp[(1) - (1)].stringValue),scope),*temp;
    if(symbol!=NULL){
            cout<< "[Syntax Analysis] ERROR: Redefinition of \"" << (yyvsp[(1) - (1)].stringValue) << "\" as variable, in line: " << yylineno << endl;
            cout<< "\tNote: Previous definition of \""<< (yyvsp[(1) - (1)].stringValue) << "\" in line: " << symbol->getLine() << endl;
    }
    else  {
         temp=table.LookupScope((yyvsp[(1) - (1)].stringValue),0);
         if(temp!=NULL && temp->getType()==libraryfunc_s)
            cout << "[Syntax Analysis] ERROR: Collision with library function \""<< (yyvsp[(1) - (1)].stringValue) << "\", in line " << yylineno << endl;                
          else table.Insert(new Symbol(var_s,(yyvsp[(1) - (1)].stringValue),currscopespace(),scope,yylineno,currscopeoffset()));
    }
}
    break;

  case 88:
#line 775 "syntax_an_v2.y"
    {
    Symbol *symbol = table.LookupScope((yyvsp[(3) - (3)].stringValue),scope),*temp;
    if(symbol!=NULL){
        cout<< "[Syntax Analysis] ERROR: Redefinition of \"" << (yyvsp[(3) - (3)].stringValue) << "\" as formal, in line: " << yylineno << endl;
        cout<< "\tNote: Previous definition of \""<< (yyvsp[(3) - (3)].stringValue) << "\" in line: " << symbol->getLine() << endl;
    }
    else  {
        temp=table.LookupScope((yyvsp[(3) - (3)].stringValue),0);
        if(temp!=NULL && temp->getType()==libraryfunc_s)
            cout << "[Syntax Analysis] ERROR: Collision with library function \""<< (yyvsp[(3) - (3)].stringValue) << "\", in line " << yylineno << endl;                
        else 
            table.Insert(new Symbol(var_s,(yyvsp[(3) - (3)].stringValue),currscopespace(),scope,yylineno,currscopeoffset()));
    }
    }
    break;

  case 90:
#line 791 "syntax_an_v2.y"
    {
        cout << "IFPREFIX ONLY: " << (yyvsp[(1) - (2)].labelValue) << endl;
        patchlabel((yyvsp[(1) - (2)].labelValue)-2, (yyvsp[(1) - (2)].labelValue)+1);
        patchlabel((yyvsp[(1) - (2)].labelValue)-1, nextQuad()+1);
    }
    break;

  case 91:
#line 796 "syntax_an_v2.y"
    {
        patchlabel((yyvsp[(1) - (4)].labelValue)-2,(yyvsp[(3) - (4)].labelValue)-1); //if eq if_prefix
        patchlabel((yyvsp[(1) - (4)].labelValue)-1,(yyvsp[(3) - (4)].labelValue)+2); //jmp if_prefix
        patchlabel((yyvsp[(3) - (4)].labelValue), nextQuad()+1); // jmp to end     
}
    break;

  case 92:
#line 802 "syntax_an_v2.y"
    {
    expr *temp = new expr(constbool_e);
    temp->setboolConst(true);
    if((yyvsp[(3) - (4)].expression)->getType()==boolexp_e){
        //////////////////////////////
        expr *temp=new expr(constbool_e);
        temp->setboolConst(false);
        
        /*Backpatching falselist*/
        for(int i=0 ;i<(yyvsp[(3) - (4)].expression)->getFalseList().size() ; i++)
            backpatch(nextQuad(),(yyvsp[(3) - (4)].expression)->getFalseList().at(i));
        
        emit(assign,temp,NULL,(yyvsp[(3) - (4)].expression),0,yylineno);
        emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);
        
        /*Backpatching truelist*/
        for(int i=0 ;i<(yyvsp[(3) - (4)].expression)->getTrueList().size() ; i++)
            backpatch(nextQuad(),(yyvsp[(3) - (4)].expression)->getTrueList().at(i));

        temp=new expr(constbool_e);
        temp->setboolConst(true);
        emit(assign,temp,NULL,(yyvsp[(3) - (4)].expression),0,yylineno); 
        //////////////////////////////
        emit(if_eq, (yyvsp[(3) - (4)].expression), temp, NULL, 0, yylineno);
        emit(jump,NULL,NULL,NULL,0,yylineno);
    }

    (yyval.labelValue) = nextQuad();

}
    break;

  case 93:
#line 834 "syntax_an_v2.y"
    {
    (yyval.labelValue) = nextQuad();   
    emit(jump,NULL,NULL,NULL,0,yylineno);
}
    break;

  case 94:
#line 839 "syntax_an_v2.y"
    {++loopcounter;}
    break;

  case 95:
#line 841 "syntax_an_v2.y"
    {--loopcounter;}
    break;

  case 96:
#line 843 "syntax_an_v2.y"
    { (yyval.BCLists) = (yyvsp[(2) - (3)].BCLists); }
    break;

  case 97:
#line 845 "syntax_an_v2.y"
    {
    emit(jump,NULL,NULL,NULL,(yyvsp[(1) - (3)].labelValue),yylineno); 
    patchlabel((yyvsp[(2) - (3)].labelValue), nextQuad()); 
    patchlabelBC((yyval.BCLists)->getBreakList(), nextQuad()); 
    patchlabelBC((yyval.BCLists)->getContList(), (yyvsp[(1) - (3)].labelValue));
}
    break;

  case 98:
#line 853 "syntax_an_v2.y"
    {(yyval.labelValue)=nextQuad();}
    break;

  case 99:
#line 855 "syntax_an_v2.y"
    {
    expr *temp = new expr(constbool_e);
    temp->setboolConst(true);
    
    if((yyvsp[(2) - (3)].expression)->getType()==boolexp_e){
        expr *temp=new expr(constbool_e);
        temp->setboolConst(false);
        
        /*Backpatching falselist*/
        for(int i=0 ;i<(yyvsp[(2) - (3)].expression)->getFalseList().size() ; i++)
            backpatch(nextQuad(),(yyvsp[(2) - (3)].expression)->getFalseList().at(i));
        
        emit(assign,temp,NULL,(yyvsp[(2) - (3)].expression),0,yylineno);
        emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);
        
        /*Backpatching truelist*/
        for(int i=0 ;i<(yyvsp[(2) - (3)].expression)->getTrueList().size() ; i++)
            backpatch(nextQuad(),(yyvsp[(2) - (3)].expression)->getTrueList().at(i));

        temp=new expr(constbool_e);
        temp->setboolConst(true);
        emit(assign,temp,NULL,(yyvsp[(2) - (3)].expression),0,yylineno); 
    }
    
    emit(if_eq, (yyvsp[(2) - (3)].expression), temp, NULL, nextQuad()+2,yylineno); 
    (yyval.labelValue) = nextQuad();
    emit(jump,NULL,NULL,NULL,0,yylineno);
}
    break;

  case 100:
#line 884 "syntax_an_v2.y"
    {
    expr *temp = new expr(constbool_e);
    temp->setboolConst(true);

    if((yyvsp[(6) - (7)].expression)->getType()==boolexp_e){
        expr *temp=new expr(constbool_e);
        temp->setboolConst(false);
        
        /*Backpatching falselist*/
        for(int i=0 ;i<(yyvsp[(6) - (7)].expression)->getFalseList().size() ; i++)
            backpatch(nextQuad(),(yyvsp[(6) - (7)].expression)->getFalseList().at(i));
        
        emit(assign,temp,NULL,(yyvsp[(6) - (7)].expression),0,yylineno);
        emit(jump,NULL,NULL,NULL,nextQuad()+3,yylineno);
        
        /*Backpatching truelist*/
        for(int i=0 ;i<(yyvsp[(6) - (7)].expression)->getTrueList().size() ; i++)
            backpatch(nextQuad(),(yyvsp[(6) - (7)].expression)->getTrueList().at(i));

        temp=new expr(constbool_e);
        temp->setboolConst(true);
        emit(assign,temp,NULL,(yyvsp[(6) - (7)].expression),0,yylineno); 
    }

    (yyval.fprefix) = new forprefix((yyvsp[(5) - (7)].labelValue),nextQuad());
    emit(if_eq,(yyvsp[(6) - (7)].expression),temp,NULL,0,yylineno);


}
    break;

  case 101:
#line 914 "syntax_an_v2.y"
    {
    cout<< (yyvsp[(1) - (7)].fprefix)->getEnter() <<endl;
    patchlabel((yyvsp[(1) - (7)].fprefix)->getEnter(),(yyvsp[(5) - (7)].labelValue)+2); // true jump
    patchlabel((yyvsp[(2) - (7)].labelValue),nextQuad()+1); // false jump
    patchlabel((yyvsp[(5) - (7)].labelValue),(yyvsp[(1) - (7)].fprefix)->getTest()+1); // loop jump
    patchlabel((yyvsp[(7) - (7)].labelValue),(yyvsp[(2) - (7)].labelValue)+3); // closure jump
    
    //patchlabelBC($$->getBreakList(),nextQuad());  //false jump
    //patchlabelBC($$->getContList(),$2+1);  //closure jump
}
    break;

  case 102:
#line 925 "syntax_an_v2.y"
    {
        if(!func_open) cout << "[Syntax Analysis] ERROR: Cannot use RETURN statement while not within function, in line " << yylineno << endl;
        emit(ret,NULL,NULL,NULL,0,yylineno);
    }
    break;

  case 103:
#line 929 "syntax_an_v2.y"
    {
        if(!func_open) cout << "[Syntax Analysis] ERROR: Cannot use RETURN statement while not within function, in line "<< yylineno << endl;
        emit(ret,NULL,NULL,(yyvsp[(2) - (3)].expression),0,yylineno);
    }
    break;

  case 104:
#line 934 "syntax_an_v2.y"
    {(yyval.labelValue)=nextQuad(); emit(jump,NULL,NULL,NULL,0,yylineno);}
    break;

  case 105:
#line 936 "syntax_an_v2.y"
    {(yyval.labelValue)=nextQuad();}
    break;


/* Line 1267 of yacc.c.  */
#line 2870 "syntax_analyzer.cpp"
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


#line 937 "syntax_an_v2.y"


int yyerror(const char* yaccProvidedMessage){
	fprintf(stderr, "[Syntax Analysis] ERROR: %s before token \"%s\", in line %d\n", yaccProvidedMessage,yytext, yylineno);
  return 0;
}

int main(int argc, char **argv) { 
    if(argc>1){
        if(!(yyin = fopen(argv[1],"r"))){
        fprintf(stderr, "Cannot read file %s\n",argv[1]);
        return 1;
        }
    }
    else yyin = stdin;    
    cout << "\n--------------------------- Errors/Warnings ---------------------------" << endl;
    yyparse();
    cout << "-----------------------------------------------------------------------" << endl;

  //table.printSymbols();
  print_quads();


  return 0; 
}

