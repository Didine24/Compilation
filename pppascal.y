/* ppascal.y */
%{
 #include <stdio.h>
 #include <ctype.h>
 #include <string.h>
 #include "arbre.h"
 #include "anasem.h"
 #include "interp.h"
/* ------------------VARIABLES GLOBALES -------------------------*/
  NOE syntree;          /* commande  globale                     */
  BILENVTY benvty;      /* environnement global                  */
  int ligcour=1;        /* ligne  courante                       */
  char ident[MAXIDENT]; /* identificateur courant                */
  ENVTY vtycour;        /* var typee courante                    */
/* -------------------------------- -----------------------------*/  
%}
%start MP
%union{NOE NO; type TYP; BILENVTY LARGT; }
/* attributs NOE: noeud binaire (IfThEl est "binarise")                                */
/* attributs TYP: contient un type                                                     */
/* attributs LARGT: liste d'arguments types(parametres ou var locales ou var globales) */
%type <NO> MP Ca C E T F Et
%type <TYP> TP
%type <LARGT> Argt L_vart L_vartnn
/* Non-terminaux MP Ca C E T F Et TP Argt  L_vart L_vartnn*/
/* P:main_program; Ca:commande atomique; C:commande; E:expression; T:terme; F:facteur;*/
/* Et: expr tableau; */
/* TP:TyPe; Argt:argument_type; */
/* L_vart: Liste_variables_typees L_vartnn: Liste_variables_typee non-nulle */
%token <NO> I V Def Dep Mp Af Sk NewAr T_ar T_com true false Se Ind If Th El Var Wh Do Pl Mo Mu And Or Not Lt Eq
%token <TYP> T_int T_boo T_err T_bot

/* Unités lexicales<NO>: Integer Variable Main_prog                            */
/* Affectation Skip NewArrayOf Type_array Type_commande                        */
/* true false                                                                  */
/* Sequence Index If Then Else Var While Do                                    */
/* Plus Moins Mult And Or Not Lessthan Equal                                   */
/* Unités lexicales<TYP>:Type_int Type_bool Type_erreur Type_indefini          */

%%
MP :  L_vart C            {benvty=$1;/* en fait deja affecte par L_vart */
                           syntree=$2;
			   YYACCEPT;}
   ;

E : E Pl T                {$$=Nalloc();
                           $$->codop=Pl;
                           $$->FG=$1;
                           $$->FD=$3;
                           $$->ETIQ=malloc(2);
                           strcpy($$->ETIQ,"+");
			   /* calcul_type */
                           calcul_type(benvty, $$ , ligcour);
			   }
  | E Mo T               {$$=Nalloc();
                          $$->codop=Mo;
                          $$->FG=$1;
                          $$->FD=$3;
		          $$->ETIQ=malloc(2);
                          strcpy($$->ETIQ,"-");
			  /* calcul_type */
                          calcul_type(benvty, $$ , ligcour);}
  | E Or T               {$$=Nalloc();
                          $$->codop=Or;
                          $$->FG=$1;
                          $$->FD=$3;
                          $$->ETIQ=malloc(2);
                          strcpy($$->ETIQ,"Or");
			  /* calcul_type */
                          calcul_type(benvty, $$ , ligcour);}
  | E Lt T               {$$=Nalloc();
                          $$->codop=Lt;
                          $$->FG=$1;
                          $$->FD=$3;
                          $$->ETIQ=malloc(2);
                          strcpy($$->ETIQ,"Lt");
			  /* calcul_type */
                          calcul_type(benvty, $$ , ligcour);}
  | E Eq T                {$$=Nalloc();
                          $$->codop=Eq;
                          $$->FG=$1;
                          $$->FD=$3;
                          $$->ETIQ=malloc(2);
                          strcpy($$->ETIQ,"Eq");
			  /* calcul_type */
                          calcul_type(benvty, $$ , ligcour);}
  | T                    {$$=$1;}
  ;

T : T Mu  F               {$$=Nalloc();
                           $$->codop=Mu;
                           $$->FG=$1;
                           $$->FD=$3;
                           $$->ETIQ=malloc(2);
                           strcpy($$->ETIQ,"*");
			   /* calcul_type */
                           calcul_type(benvty, $$ , ligcour);}
  | T And F               {$$=Nalloc();
                           $$->codop=And;
                           $$->FG=$1;
                           $$->FD=$3;
                           $$->ETIQ=malloc(2);
                           strcpy($$->ETIQ,"And");
			   /* calcul_type */
                           calcul_type(benvty, $$ , ligcour);}
  | Not F                  {$$=Nalloc();
                            $$->codop=Not;
			    $$->FG=$2;
                            $$->FD=NULL;
			    $$->ETIQ=malloc(2);
                            strcpy($$->ETIQ,"Not");
			    /* calcul_type */
                            calcul_type(benvty, $$ , ligcour);}
  | F                      {$$=$1;}
  ;

F : '(' E ')'                  {$$=$2;}
  | I                          {$$=$1;}/* attribut type [0,0,T_int]                   */ 
  | V                          {$$=$1;/* calcul du  type                              */
				calcul_type(benvty, $$ , ligcour);
                                }
  | true                       {$$=$1;}/* attribut type [0,0,T_boo]                   */
  | false                      {$$=$1;}/* attribut type [0,0,T_boo]                   */
  | NewAr TP '[' E ']'         {$$=Nalloc();
                                $$->codop=NewAr;
				/* calcul_type :                                       */
      				type_copy(&($$->typno),$2); /* DIM,TYPEF sont connus   */
				($$->typno).DIM++; /* mise a jour DIM                  */
      				$$->FG=NULL;
				$$->FD=$4;
        calcul_type(benvty, $$ , ligcour);
        /* TAILLE a calculer a partir de E */}
  | Et                          {$$=$1;}
  ;

Et: V  '[' E ']'                {$$=Nalloc();/* un seul indice                        */
                                $$->codop=Ind;
				$$->FG=$1;
				$$->FD=$3;/* verif des types */
				/* calcul du type de V puis de Et */
				calcul_type(benvty, $1, ligcour);
				calcul_type(benvty, $$ , ligcour);
				}
  | Et '[' E ']'                {$$=Nalloc();/* plusieurs indices                     */
                                $$->codop=Ind;
				$$->FG=$1;
				$$->FD=$3;/* verif des types */
				calcul_type(benvty, $$ , ligcour);}
  ;

C : C Se Ca                     {$$=Nalloc();      /* sequence */
                                 $$->codop=Se;
                                 $$->FG=$1;
                                 $$->FD=$3;
                                 $$->ETIQ=malloc(2);
                                 strcpy($$->ETIQ,"Se");
				 /* calcul type */
				 calcul_type(benvty, $$ , ligcour);
                                 }    
  | Ca                          {$$=$1;}
  ;

Ca : Et Af E            {$$=Nalloc();
                        $$->codop=Af;
                        $$->FG=$1;
                        $$->FD=$3;
                        $$->ETIQ=malloc(2);
                        strcpy($$->ETIQ,"Af");
			/* verif des types */
                         calcul_type(benvty, $$ , ligcour);
                        } 
  | V Af E              {$$=Nalloc();
                        $$->codop=Af;
                        $$->FG=$1;
                        $$->FD=$3;
                        $$->ETIQ=malloc(2);
                        strcpy($$->ETIQ,"Af");
			/* verif des types */
			calcul_type(benvty, $1 , ligcour);
                        calcul_type(benvty, $$ , ligcour);
			}
  | Sk                  {$$=$1;
                        /* calcul_type: ana_lex */}      
  | '{' C '}'           {$$=$2;}     
  | If E Th C  El Ca    {$$=Nalloc();
                        $$->codop=If;
                        $$->FG=$2;         /* booleen       */
                        $$->FD=Nalloc();   /* alternative   */
			$$->FD->ETIQ="";   /* champ inutile */
			$$->FD->FG=$4;     /* branche true  */
			$$->FD->FD=$6;     /* branche false */
                        $$->ETIQ=malloc(2);
                        strcpy($$->ETIQ,"IfThEl");/* calcul_type */
			calcul_type(benvty, $$ , ligcour);}
  | Wh E Do Ca         {$$=Nalloc();
                        $$->codop=Wh;
                        $$->FG=$2;         /* booleen d'entree dans le while */
                        $$->FD=$4;         /* corps du while                 */
                        $$->ETIQ=malloc(2);
                        strcpy($$->ETIQ,"Wh");/* calcul_type */
			calcul_type(benvty, $$ , ligcour);}
  ;

/* un (ident, type) */ 
Argt:  V ':' TP         {vtycour=creer_envty($1->ETIQ, $3, 0);/* default_val($3)=0*/
                         $$=creer_bilenvty(vtycour);}
    ;
    
/* un type */
TP: T_boo               {$$=$1;}
  | T_int               {$$=$1;}
  | T_ar TP             {$$=$2;$$.DIM++;}
  ;

/* table des variables  globales  */
L_vart: %empty           {$$=bilenvty_vide();benvty=$$;
                         }
| L_vartnn               {$$=$1;benvty=$$;}

;

L_vartnn: Var Argt                {$$=$2;}
        | L_vartnn ',' Var Argt   {$$=concatty($1,$4);}
        ;

%%

#include "arbre.h"
#include "lex.yy.c"

/*  pour tester l'analyse 
int main(int argn, char **argv)
{yyparse();
  ecrire_prog(benvty,blfonctions,syntree);
  return(1);
}
*/

/*  pour tester l'analyse semantique */
int main(int argn, char **argv)
{yyparse();
  ecrire_prog(benvty,syntree);
  type terr=creer_type(0,T_err);
  type tcom= creer_type(0,T_com);
  if (type_eq(syntree->typno,terr))
    printf("erreur de typage\n");
  else if (type_eq(syntree->typno,tcom))
    printf("programme bien type\n");
  else
    printf("attention: typage incomplet");
  return(1);
}

/*  pour tester l'interpreteur 
int main(int argn, char **argv)
{//ligcour=0;
  yyparse();
  ecrire_prog(benvty,syntree);
  //init_memoire();
  printf("Les variables globales avant exec:\n");
  printf("------------------------:\n");
  ecrire_bilenvty(benvty); printf("\n");
  //ecrire_memoire(5,5,20);
  semop_gp(benvty,syntree);
  printf("Les variables globales apres exec:\n");
  printf("------------------------:\n");
  ecrire_bilenvty(benvty); printf("\n");
  //ecrire_memoire(5,5,20);
  return(1);
}*/

int yyerror(s)
     char *s;
{
  fprintf(stderr, "%s: ligne %d\n", s, ligcour);
    return EXIT_FAILURE;
}

