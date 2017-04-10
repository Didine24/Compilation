/* arbre.c */
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre.h"
#include "pppascal.tab.h"
#include "interp.h"
/*-------------------------------------------------------------------*/
/* ----------------------------types---------------------------------*/
/*  type, NOE,ENVTY,LFON,BILENVTY,BILFON : definis dans arbre.h      */
/*-------------------------------------------------------------------*/
/*---------------------allocation memoire----------------------------*/
char *Idalloc()
{
  return((char *)malloc(MAXIDENT*sizeof(char)));
}
NOE Nalloc()
{
  return((NOE)malloc(sizeof(struct noeud)));
}
ENVTY Envtalloc()
{
  return((ENVTY)malloc(sizeof(struct cellenvty)));
}
type *talloc()
{
  return((type *)malloc(sizeof(type)));
}

/*-------------------------------------------------------------------*/
/*--------------------------------arbres-----------------------------*/

void prefix(NOE n)
/* ecrit l'expression n en notation prefixe*/
{ if(n != NULL)
    {if (n->ETIQ)
	printf("%s ",n->ETIQ);
      else
	printf("%s ",nomop(n->codop));
      prefix(n->FG);
      prefix(n->FD);
    };
}

/* 1 si est une feuille, 0 sinon */
int est_feuille(NOE n)
{if (n ==NULL)
    return(0);
 else
   return ((n->FG==NULL)&&(n->FD==NULL));
}

NOE creer_noe(int ncodop,type ntypno,char *NETIQ,NOE NFG,NOE NFD )
{NOE res=Nalloc();
  res->codop=ncodop;
  res->typno=ntypno;
  strcpy(res->ETIQ,NETIQ);
  res->FG=NFG;
  res->FD=NFD;
  return(res);
}

/*-------------------------------------------------------------------*/
/*-----------------------------environnements------------------------*/
/* 1 si t1 ==t2 , 0 sinon                   */
int type_eq(type t1, type t2)
{return((t1.DIM==t2.DIM) && (t1.TYPEF==t2.TYPEF));
}

/* copie torig vers *tcop  */
void type_copy(type *tcop,type torig)
{tcop->DIM=torig.DIM;
 tcop->TYPEF=torig.TYPEF;
 return;
}

/* affecte le type  de *prho      */
void type_affect(ENVTY rho,type tvar)
{(rho->TYPE).DIM=tvar.DIM;
 (rho->TYPE).TYPEF=tvar.TYPEF;
 return;
}

/* retourne le type                  */
type creer_type(int dm, int tf)
{type TT;
  TT.DIM=dm;
  TT.TYPEF=tf;
  return(TT);
}

/* retourne le type resultat de op               */
extern type type_res_op(int op)
{ type t;
  switch(op)
    {case true:case false:case And:case Or:case Lt:case Eq:case Not:
       t=creer_type(0,T_boo);
       break;
     case Pl:case Mo:case Mu:
       t=creer_type(0,T_int);
       break;
     default:
	break;
    }
  return(t);
}
     
/* pointe vers cette var typee */
ENVTY creer_envty(char *etiq, type tau, int val)
{ENVTY ety;
  ety= Envtalloc();
  if (etiq !=NULL)
    {ety->ID=Idalloc();
      strcpy(ety->ID,etiq);}
  ety->TYPE=tau;
  ety->VAL=val;
  ety->SUIV=NULL;
  return(ety);
}

/* pointe vers copie  */
ENVTY copier_envty(ENVTY env)
{ENVTY ety = NULL;
  if (env != NULL)
  {
  ety= Envtalloc();
  if (env->ID!=NULL)
    {ety->ID=Idalloc();
     strcpy(ety->ID,env->ID);}
  type_copy(&(ety->TYPE),env->TYPE);
  ety->VAL=env->VAL;
  ety->SUIV= copier_envty(env->SUIV);
  }
return(ety);
}

/* retourne (arg1 op arg2) ou bien (op arg1) pour op "de base "*/
int eval(int op, int arg1, int arg2)
{switch(op)
    {case Pl:
  return(arg1 + arg2);
    case Mo:
      return(arg1 - arg2);
    case Mu:
      return(arg1 * arg2);
    case Ind:
      return(TAS[ADR[arg1]+arg2]);
    case And:
      return(arg1 * arg2);
    case Or:
      if (arg1 * arg2 == 0)
  return(arg1 + arg2);
      else
  return(1);
    case Not:
      return(1-arg1);/* l'argument est en position 1 */
    case Lt:
      if (arg1 < arg2)
  return(1);
      else
  return(0);
    case Eq:
      if (arg1 == arg2)
  return(1);
      else
  return(0);
    default:
      return(0);
    }
  return(0);
}

 
/* retourne l'adresse de la cellule contenant chaine. NULL si la chaine est absente */
ENVTY rechty(char *chaine, ENVTY listident)
{if (listident!=NULL)
    {if (strcmp(listident->ID,chaine)==0)
        {	printf("trouve %s en position %p \n",chaine,listident);  
          return listident;}
      else
	return rechty(chaine,listident->SUIV);
    }
  else
    return NULL;
}



/* affecte val a la variable var, dans rho */
/* NB: le type n'est pas utilise           */
int affectty(ENVTY rho, char *var, type tpvar, int val)
{ENVTY pos;
  pos=rechty(var,rho);/* adresse de la cellule contenant var */
  printf("ancienne valeur vaut %d \n",pos->VAL);
  if (pos != NULL)
    {(pos->VAL)=val;
      (pos->TYPE)=tpvar;
      return(EXIT_SUCCESS);
    }
  else
    return(EXIT_FAILURE);
}

/* traduit entier (= codop) vers chaine (= nom operation)  */
/* utile pour les fonctions d'ecriture */
/* traduit entier (= codop) vers chaine (= nom operation)  */
char *nomop(int codop)
{switch(codop)
    {case(I):return("I");
    case(V):return("V");
    case(Mp): return("Mp");
    case(Af): return("Af");
    case(AfInd) : return("AfInd");
    case(Sk): return("Sk");
    case(NewAr): return("NewAr ");
    case(T_ar): return("array_of ");
    case(T_int): return("integer");
    case(T_boo): return("boolean");
    case(T_com): return("commande");
    case(T_bot): return("typ_indefini");
    case(T_err): return("typ_erreur");  
    case(true): return("true");
    case(false):return("false");
    case(Se): return("Se");
    case(Ind): return("Ind");  
    case(If): return("If");
    case(Th): return("Th");
    case(El): return("El");
    case(Wh): return("Wh");
    case(Do): return("Do");
    case(Pl): return("Pl");
    case(Mo): return("Mo");
    case(Mu): return("Mu");
    case(Afc): return("Afc");
    case(St): return("St");
    case(Jp): return("Jp");
    case(Jz): return("Jz"); 
    case(And):return("And");
    case(Or): return("Or");
    case(Not): return("Not");
    case(Lt): return("Lt");
    case(Eq): return("Eq");
    case(0): return("");                   /* code 0: directive assembleur y86 */
    default: return(NULL);
    };
}


/* ecrit le type */
void ecrire_type(type tp)
{printf("DIM:%d,TYPEF:%d",tp.DIM,tp.TYPEF);
}

/* affiche l'environnement type      */  
int ecrire_envty(ENVTY rho)
{ if (rho==NULL)
    {printf("fin d'environnement \n");
      return(EXIT_SUCCESS);}
  else
    {printf("variable %s ",rho->ID);
      ecrire_type(rho->TYPE);
      printf("valeur %d \n",rho->VAL);
      ecrire_envty(rho->SUIV); 
      return(EXIT_SUCCESS);
    };
}

/* valeur de var dans rho  (UTILISEE ??)                     */
/* NB: la valeur d'un tableau est un index du tas            */
int valchty(ENVTY rho, char *var)
{ENVTY pos;
  pos=rechty(var,rho);/* adresse de la cellule contenant var */
  if (pos != NULL)
    return(pos->VAL);
  else
    return(0);
}

/* initialise var dans *prho */
/* le couple  (var,tvar)  est copie dans l'environnement */
void inbilenvty(BILENVTY *prho,char *var,type tvar)
{ENVTY erho, pos, newcell;
  erho=prho->debut;
      pos=rechty(var,erho);/* adresse de la cellule contenant var */
  if (pos == NULL)
    /*on insere var en tete de envrnt*/
    {       newcell=Envtalloc();
      newcell->ID=Idalloc();
      strcpy(newcell->ID,var);
      /* newcell->VAL=intalloc(); */
      newcell->VAL=0;
      type_affect(newcell,tvar);
      newcell->SUIV=erho;
            prho->debut=newcell;
    };
  return;
}

/* retourne une biliste vide  */
BILENVTY bilenvty_vide()
{BILENVTY bty;
  bty.debut=NULL;bty.fin=NULL;
  return(bty);
}

/* retourne une biliste a un element */
BILENVTY creer_bilenvty(ENVTY varty)
{BILENVTY bty;
  bty.debut=varty;bty.fin=varty;
  return(bty);
}

/* pointe vers copie      */
BILENVTY copier_bilenvty(BILENVTY bty)
{ENVTY aty,ctycour; BILENVTY bcty;
  aty=copier_envty(bty.debut);
  bcty.debut=aty;
  ctycour=aty;
  while(ctycour && ctycour->SUIV)
      ctycour=ctycour->SUIV;
  bcty.fin=ctycour;
  return(bcty);
}

/* retourne la concatenation                       */
/* copie les deux arguments: pas de factorisation  */
BILENVTY concatty(BILENVTY bty1, BILENVTY bty2)
{BILENVTY bty,nbty1,nbty2;
  nbty1=copier_bilenvty(bty1);
  nbty2=copier_bilenvty(bty2);
  if (nbty1.fin!= NULL)
    if (nbty2.debut!=NULL)
       { nbty1.fin->SUIV=nbty2.debut;
        bty.debut=nbty1.debut;
        bty.fin=nbty2.fin;
        return(bty);}
    else
      return(nbty1);  
  else
    return(nbty2);
}

/* affiche la biliste de variables typees */
void ecrire_bilenvty(BILENVTY bty)
{ecrire_envty(bty.debut);
}

/* affecte  la valeur rhs a la variable lhs */
void affectb(BILENVTY rho_gb, char *lhs, int rhs)
{ENVTY pos;
    pos=rechty(lhs,rho_gb.debut);  
    if (pos!=NULL)
	pos->VAL=rhs;                   /* lhs est une var enregistree           */
    else
      printf("erreur: variable %s non declaree", lhs);
}

/* ajoute la variable typee (nomvar,tp) dans rho,en prem position, rho est modifie*/
extern void ajout_var(BILENVTY rho, char *nomvar, type tp)
{ ENVTY vty=creer_envty(nomvar,tp,0);
  BILENVTY bvty=creer_bilenvty(vty);
  rho= concatty(bvty,rho);
  return;}


/*-------------------------------------------------------------------------------*/
/*---------------------programmes -----------------------------------------------*/
void ecrire_prog(BILENVTY argby,NOE argno)
{printf("Les variables globales:\n");
 printf("------------------------:\n");
 ecrire_bilenvty(argby);printf("\n");
  printf("Le programme principal:\n");
 printf("------------------------:\n");
 prefix(argno);printf("\n");
 return;
}
