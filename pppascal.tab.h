/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_PPPASCAL_TAB_H_INCLUDED
# define YY_YY_PPPASCAL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    I = 258,
    V = 259,
    Def = 260,
    Dep = 261,
    Mp = 262,
    Af = 263,
    Sk = 264,
    NewAr = 265,
    T_ar = 266,
    T_com = 267,
    true = 268,
    false = 269,
    Se = 270,
    Ind = 271,
    If = 272,
    Th = 273,
    El = 274,
    Var = 275,
    Wh = 276,
    Do = 277,
    Pl = 278,
    Mo = 279,
    Mu = 280,
    And = 281,
    Or = 282,
    Not = 283,
    Lt = 284,
    Eq = 285,
    Afc = 286,
    St = 287,
    Jp = 288,
    Jz = 289,
    T_int = 290,
    T_boo = 291,
    T_err = 292,
    T_bot = 293
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 20 "pppascal.y" /* yacc.c:1909  */
NOE NO; type TYP; BILENVTY LARGT; 

#line 96 "pppascal.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PPPASCAL_TAB_H_INCLUDED  */
