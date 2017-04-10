/* interp.h */
#ifndef INTERP_H
#define INTERP_H

/*----------------------interpéteur ----------------------------------------------*/
/* initialisation  memoire                                                       */
void init_memoire();/* initialise le tas: TAS, ADR, TAL                          */
/* decrit la memoire: ADR, TAL, TAS */
void ecrire_memoire(int maxadr, int maxtal, int maxtas);
/* interpréteur op a grands pas des expressions */  
extern int interpval(BILENVTY rho_gb, NOE e);
/*interpréteur op a grdspas des commandes                                          */
extern void interp_gp(BILENVTY rho_gb, NOE c);
/*---------------------VARIABLES globales ---------------------------------------*/
extern NOE syntree;          /* arbre syntaxique                      (y.tab.c)  */
extern int TAS [TAILLEMEM];  /* le tas; (NIL=0); "vraies" adresses >=1(y.tab.c)  */
extern int ADR[TAILLEADR];   /* ADR[i]=adresse dans le tas du tab i              */
extern int TAL[TAILLEADR];   /* TAL[i]=taille du tab i                           */
extern int ptasl;            /* premiere place libre dans TAS[]                  */
extern int padrl;            /* premiere place libre dans ADR[]                  */
#endif