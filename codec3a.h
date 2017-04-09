/* codec3a.h */
#ifndef CODEC3A_H
#define CODEC3A_H 
/* ----------------------------types--------------------------------------------*/
/* biliste de quadruplets etiquetes (stocke C3A ou Y86 )*/
typedef struct cellquad{
  char *ETIQ;
  int  OP;
  char *ARG1, *ARG2, *RES;
  struct cellquad *SUIV;} *QUAD;

typedef struct{
  QUAD debut;
  QUAD fin;}BILQUAD;


char *matching[TAILLEADR];  
/*---------------------quadruplets ----------------------------------------------*/
extern char *gensym(char *prefix); /* fabrique de nouvelles chaines              */
extern QUAD creer_quad(char *etiq,int op,char *arg1,char *arg2,char *res);
extern BILQUAD bilquad_vide() ;                    /* retourne une biliste vide  */
extern BILQUAD creer_bilquad(QUAD qd); /* retourne une biliste  a un element     */
extern QUAD rechbq(char *chaine, BILQUAD bq);/*ret le quad etiquete par chaine   */
extern BILQUAD normal(BILQUAD bq);/* retourne la f.n. de bq: skip a la fin       */ 
extern BILQUAD concatq(BILQUAD bq1, BILQUAD bq2);/* retourne la concatenation    */
extern void ecrire_quad(QUAD qd); /* affiche le quadruplet                       */
extern void secrire_quad(char *tquad,QUAD qd); /* ecrit le quadruplet dans tquad */
extern void ecrire_bilquad(BILQUAD bq); /* affiche la biliste de quadruplets     */
extern void ecrire_sep_bilquad(BILQUAD bq);/* affiche bq , avec separateurs ":"  */
extern void test_tradc3a(int n, NOE c);/* teste la traduction imp --> c3a        */
extern void test_constantes();     /* teste le generateur de noms de constantes  */
/*---------------------de-imp-vers-C3A-------------------------------------------*/
extern BILQUAD imp2quad(NOE ec); /* traduit une commande en quadruplets          */
/*---------------------test------------------------------------------------------*/
extern void test_tradc3a(int n, NOE c);/* teste la traduction imp --> c3a        */
extern int NameToId(char *etiq);
#endif
