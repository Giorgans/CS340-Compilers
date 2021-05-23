/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
/* Line 1529 of yacc.c.  */
#line 160 "parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

