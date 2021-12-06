/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    MAIN = 258,
    VAR = 259,
    PRIMITIVO = 260,
    PYC = 261,
    INPUT = 262,
    OUTPUT = 263,
    CADENA = 264,
    RETURN = 265,
    IF = 266,
    ELSE = 267,
    REPEAT = 268,
    UNTIL = 269,
    WHILE = 270,
    OPERLISTA = 271,
    CONSTANTE = 272,
    INICIOBLOQUE = 273,
    FINBLOQUE = 274,
    CORCHETEIZQ = 275,
    CORCHETEDER = 276,
    COMA = 277,
    ASIGN = 278,
    PARDER = 279,
    PARIZQ = 280,
    ESTRUCTURA = 281,
    ID = 282,
    OPERMASMAS = 283,
    ARROBA = 284,
    ORLOG = 285,
    ANDLOG = 286,
    XOR = 287,
    EQN = 288,
    REL = 289,
    PORPOR = 290,
    PORPOT = 291,
    MASMENOS = 292,
    MULDIV = 293,
    MENOSMENOS = 294,
    NOT = 295,
    INT = 296,
    DOLLAR = 297,
    BAR = 298
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */