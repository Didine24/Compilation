/* ppascal.l */
%{
/*
NE PAS  ecrire
#include "pppascal.tab.h"

*/
 extern int ligcour;
%}
%option nounput noinput

%x COMMENT  
debcom            "/*"
endcom            "*/"
sep               [ \t]+
sign              [\+-]
lettre            [A-Za-z]
caractere         [A-Za-z0-9\-\_]  
%%
<INITIAL>{debcom}          {BEGIN COMMENT;}
<COMMENT>{endcom}          {BEGIN INITIAL;}
<COMMENT>.                 {;}
skip                       {yylval.NO=Nalloc();
                           (yylval.NO)->codop = Sk;
                           (yylval.NO)->FG = NULL;
                           (yylval.NO)->FD = NULL;
                           (yylval.NO)->ETIQ = malloc(yyleng + 1);
                           strcpy((yylval.NO)->ETIQ,yytext);
                           type_copy(&((yylval.NO)->typno),creer_type(0,T_com));
                                                      return(Sk);}
new{sep}array{sep}of       {return(NewAr);}
array{sep}of               {return(T_ar);}
integer                    {type_copy(&(yylval.TYP),creer_type(0,T_int));
                                                       return(T_int);
                            }
boolean                    {type_copy(&(yylval.TYP),creer_type(0,T_boo));
                           return(T_boo);}
true                       {yylval.NO=Nalloc();
                           (yylval.NO)->codop = true;  /* cte true */
                           type_copy(&((yylval.NO)->typno),creer_type(0,T_boo)); 
                           (yylval.NO)->FG = NULL;
                           (yylval.NO)->FD = NULL;
                           (yylval.NO)->ETIQ = malloc(yyleng + 1);
                           strcpy((yylval.NO)->ETIQ,yytext);
                           return(true);} 
false                      {yylval.NO=Nalloc();
                           (yylval.NO)->codop = false;  /* cte false */
                           type_copy(&((yylval.NO)->typno),creer_type(0,T_boo)); 
                           (yylval.NO)->FG = NULL;
                           (yylval.NO)->FD = NULL;
                           (yylval.NO)->ETIQ = malloc(yyleng + 1);
                           strcpy((yylval.NO)->ETIQ,yytext);
                           return(false);}
if                         {return(If);}
then                       {return(Th);}
else                       {return(El);}
var                        {return(Var);}
while                      {return(Wh);}
do                         {return(Do);}
"+"                        {return(Pl);}
"-"                        {return(Mo);}
"*"                        {return(Mu);}
"and"                      {return(And);}
"or"                       {return(Or);}
"not"                      {return(Not);}
"<"                        {return(Lt);}
"="                        {return(Eq);}
{lettre}{caractere}*       {yylval.NO=Nalloc();
                           (yylval.NO)->codop = V; /* variable (type inconnu) */
                           (yylval.NO)->FG = NULL;
                           (yylval.NO)->FD = NULL;
                           (yylval.NO)->ETIQ = malloc(yyleng + 1);
                           strcpy((yylval.NO)->ETIQ,yytext);
                                                      return(V);}   /* variable */
0|[1-9][0-9]*              {yylval.NO=Nalloc(); /* entier positif */
                           (yylval.NO)->codop = I;  /* entier */
                                                      type_copy(&((yylval.NO)->typno),creer_type(0,T_int));
                                                      (yylval.NO)->FG = NULL;
                           (yylval.NO)->FD = NULL;
                           (yylval.NO)->ETIQ = malloc(yyleng + 1);
                           strcpy((yylval.NO)->ETIQ,yytext);
                                                      (yylval.NO)->codop = I;
                                                      return(I);}     /* entier */
:=                         {return(Af);}
";"                        {return(Se);}
{sep}                      {;}
\n                         {ligcour++;}
.                          {return(yytext[0]);}
%%
int yywrap()
{return(1);}

