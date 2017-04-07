/* arbre.h */
#ifndef ARBRE_H
#define ARBRE_H 
/* ----------------------------types--------------------------------------------*/

/* type: tableau */
typedef struct {
  int DIM;   /* dimension ; vaut 0 ssi type de base                           */
  int TYPEF; /* type des cellules de plus petite dim: T_int ou T_boo ou T_com */
}type;

/* commande := arbre abstrait */
typedef struct noeud{
  int codop;  /* I,V, operateur */
  type typno; /*[0,T_com](si commande) ou [k,(T_int,T_boo)] (si expression)   */
  char *ETIQ;
  struct noeud *FG, *FD;} *NOE;

/* environnement_type := liste de var ou param (identificateur, type, valeur)  */
typedef struct cellenvty{
  char *ID;
  type TYPE;
  int  VAL;
  struct cellenvty *SUIV;} *ENVTY;

/* biliste de var ou param */
typedef struct{
  ENVTY debut;
  ENVTY fin;}BILENVTY;

/*------------------FONCTIONS ---------------------------------------------------*/
/*---------------------parcours d'arbres-----------------------------------------*/
extern int yylex();          /* fonction generee par flex                        */
extern int yyerror();        /* fonction generee par flex/bison                  */
/*---------------------allocation memoire----------------------------------------*/
extern char *Idalloc();      /* retourne un tableau de MAXIDENT char             */
extern NOE Nalloc();         /* retourne un NOE                                  */
extern ENVTY Envtalloc();    /* retourne un ENVTY                                */
extern type *talloc();       /* retourne un pointeur sur type *                  */
/*---------------------------------arbres----------------------------------------*/
extern void prefix(NOE n);   /* ecrit l'expression n en notation prefixe         */
extern int est_feuille(NOE n);/* 1 si est une feuille, 0 sinon                   */
/* retourne un NOE pointant vers une cellule qui a ces champs                    */
extern NOE creer_noe(int ncodop,type ntypno,char *NETIQ,NOE NFG,NOE NFD );       
/*---------------------environnements--------------------------------------------*/
extern int  type_eq(type t1, type t2);/* 1 si t1 ==t2 , 0 sinon                  */
extern void type_copy(type *tcop, type torig);/* copie torig vers tcop           */
extern void type_affect(ENVTY rho, type tvar);/* affecte le type  de   *rho      */
extern type creer_type(int dm, int tf);/* retourne le type                       */
extern type type_res_op(int op);/* retourne le type resultat de op               */
extern ENVTY creer_envty(char *etiq, type tau,int val);/*pointe vers cette var typee*/
extern ENVTY copier_envty(ENVTY env);/*pointe vers une copie                     */
extern char *nomop(int codop);/* traduit entier vers chaine (= nom operation)    */
extern void ecrire_type(type tp);      /* ecrit le type                          */
extern int ecrire_envty(ENVTY rho);/* affiche l'environnement type               */
extern ENVTY rechty(char *chaine, ENVTY listident);/* retourne la position de chaine*/
/* retourne la position de chaine */
extern int eval(int op, int arg1, int arg2); /* retourne (arg1 op arg2)          */
extern int affectty(ENVTY rho, char *var, type tpvar, int val);/*   affecte var  */
extern int valchty(ENVTY rho, char *var); /* valeur de var dans envirnt-type rho */
/* affecte  la valeur rhs a la variable lhs                                      */
extern void affectb(BILENVTY rho_gb, char *lhs, int rhs);   
/*---------------------bilistes-de-var-typees------------------------------------*/
extern void inbilenvty(BILENVTY *prho,char *var,type tvar);   /* initialise var  */
extern BILENVTY bilenvty_vide() ;                  /* retourne une biliste vide  */
extern BILENVTY creer_bilenvty(ENVTY varty);/* retourne une biliste a un element */
extern BILENVTY copier_bilenvty(BILENVTY bty);/*pointe vers copie                */
extern BILENVTY concatty(BILENVTY bty1, BILENVTY bty2);/* retourne la concatenation*/
extern void ecrire_bilenvty(BILENVTY bty); /* affiche la biliste de quadruplets  */
/*---------------------programmes -----------------------------------------------*/
void ecrire_prog(BILENVTY argby,NOE argno);             /* affiche le programme  */

/* --------------------CONSTANTES -----------------------------------------------*/
#define MAXIDENT 16          /* long max d'un identificateur de variable         */
#define TAILLEADR 1000       /* nbe max adresses dans le tas                     */
#define TAILLEMEM 10000      /* taille du tas                                    */
/*---------------------VARIABLES globales ---------------------------------------*/
extern NOE syntree;          /* arbre syntaxique                        (y.tab.c)*/
#endif

