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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE

{
	char *c;
	S32 i;
	F64 d;
	osg::Vec3f *v;
	osg::Matrixf *m;
}
/* Line 1529 of yacc.c.  */

	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

