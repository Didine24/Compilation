/* arbre.c */
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre.h"
#include "codec3a.h"
#include "pppascal.tab.h"
#include "interp.h"
/* pppascal.tab.h APRES arbre,h, sinon le type NOE est inconnu de gcc    */
/*-------------------------------------------------------------------*/
/* ----------------------------types---------------------------------*/
/* NOE,PILCOM,ENV : definis dans arbre.h                             */
/* QUAD,BILQUAD: definis dans codec3a.h                              */
/*-------------------------------------------------------------------*/
/*---------------------quadruplets-----------------------------------*/




int NameToId(char *etiq){
  for (int i = 0; i < TAILLEADR; i++)
  {
    if(strcmp(matching[i],etiq)==0){
      return i;
    }
  }
  return -1;
}

/* retourne une nouvelle chaine */
char *gensym(char *prefix)
{static int counter=0;
  char *chaine;char *chcounter;
  chcounter=Idalloc();
  chaine=Idalloc();
  strcpy(chaine,prefix); 
  sprintf(chcounter,"%d",counter);   /* prefix+chaine(counter)*/
  counter=counter+1;
  strcat(chaine,chcounter);
  return(chaine);
}

/* test de gensym */
void test_constantes(int n)
{char *ident;int i=0;
  for(i=0;i<n;i++)
    {ident=gensym("CONS");
      printf("%s \n",ident);
    }
}

/* retourne un quadruplet (avec etiquette etiq) */
QUAD creer_quad(char *etiq,int op,char *arg1,char *arg2,char *res)
{QUAD qd;int lres;
  qd=(struct cellquad *)malloc(sizeof(struct cellquad));
  if (etiq !=NULL)
    {qd->ETIQ=Idalloc();
      strcpy(qd->ETIQ,etiq);}
  qd->OP=op;
  if (arg1 !=NULL)
    {qd->ARG1=Idalloc();
      strcpy(qd->ARG1,arg1);}
  if (arg2 !=NULL)
    {qd->ARG2=Idalloc();
      strcpy(qd->ARG2, arg2);}
  if (res!= NULL)
    {lres=strlen(res);
      qd->RES=(char *)malloc(lres*sizeof(char));
      strcpy(qd->RES,res);}
  return(qd);
}

/* retourne une biliste vide  */
BILQUAD bilquad_vide() 
{BILQUAD bq;
  bq.debut=NULL;bq.fin=NULL;
  return(bq);
}

/* retourne une biliste a un element  */
BILQUAD creer_bilquad(QUAD qd) 
{BILQUAD bq;
  bq.debut=qd;bq.fin=qd;
  return(bq);
}

/* fonction aux  pour la fonction rechbq */
QUAD rechq(char *chaine, QUAD qd)
{QUAD qcour;
  qcour=qd;
  if (qcour!=NULL)
    {if (strcmp(qcour->ETIQ,chaine)==0)
        {printf("trouve %s en position %p \n",chaine,qcour);
	  return qcour;}
      else
	return rechq(chaine,qcour->SUIV);
    }
  else
    return NULL;
}

/*retourne le quad etiquete par chaine, NULL s'il n'y en a pas */
QUAD rechbq(char *chaine, BILQUAD bq)
{return(rechq(chaine,bq.debut));}


BILQUAD concatq(BILQUAD bq1, BILQUAD bq2)
/* retourne la concatenation; ne detruit pas bqi; ne copie pas *bqi */
/* peut creer une boucle ! */
{BILQUAD bq;
  if (bq1.fin!= NULL)
    if (bq2.debut!=NULL)
       { bq1.fin->SUIV=bq2.debut;
        bq.debut=bq1.debut;
        bq.fin=bq2.fin;
        return(bq);}
    else
      return(bq1);  
  else
    return(bq2);
}

/* retourne bq +  skip */
BILQUAD ajouterskip(BILQUAD bq)
{QUAD nq; BILQUAD nbq;
  nq=creer_quad(gensym("ET"),Sk,NULL,NULL,NULL);/* instruction skip */
  nbq=creer_bilquad(nq);
  return(concatq(bq,nbq));
}

/* retourne la "forme normale" de bq: dernier quad = skip */  
BILQUAD normal(BILQUAD bq)
{if (bq.fin== NULL)
    {return(ajouterskip(bq));}
  else
    {if (bq.fin->OP!=Sk)        /* pas normal-> on normalise */
	return(ajouterskip(bq));
      else                      /* deja normal */
	return(bq);}
}


/* affiche le quadruplet v1 (pour tests) */
void ecrire_quad1(QUAD qd)
{printf("etiq:%s,op:%s,arg1:%s,arg2:%s,res:%s \n",qd->ETIQ,nomop(qd->OP),qd->ARG1,qd->ARG2,qd->RES);
}

/* affiche le quadruplet (pour generer code); puis saute a la ligne */
void ecrire_quad(QUAD qd)
{ if(strcmp(qd->ETIQ,"") == 0)       /* etiquette= mot vide */
    {printf("%-10s ","");}
  else
    {printf("%-10s:",qd->ETIQ);}
  printf("%-6s ",nomop(qd->OP));
  if (qd->ARG1!=NULL)
    {printf("%-10s",qd->ARG1);}
  else
    {printf("%-10s","");}
  if (qd->ARG2!=NULL)
    {printf("%-10s",qd->ARG2);}
  else
    {printf("%-10s","");}
  if (qd->RES!=NULL)
    {printf("%-10s\n",qd->RES);}
  else
    {printf("\n");}
  }
/* affiche le quadruplet (pour generer code) avec separateur":" ; 
puis saute a la ligne */
void ecrire_sep_quad(QUAD qd)
{ if(strcmp(qd->ETIQ,"") == 0)       /* etiquette= mot vide */
    {printf("%-10s: ","");}
  else
    {printf("%-10s:",qd->ETIQ);}
  printf("%-6s:",nomop(qd->OP));
  if (qd->ARG1!=NULL)
    {printf("%-10s:",qd->ARG1);}
  else
    {printf("%-10s:","");}
  if (qd->ARG2!=NULL)
    {printf("%-10s:",qd->ARG2);}
  else
    {printf("%-10s:","");}
  if (qd->RES!=NULL)
    {printf("%-10s\n",qd->RES);}
  else
    {printf("\n");}
  }

/* ecrit le quadruplet dans un string; pas de newline             */
void secrire_quad(char *tquad, QUAD qd)
{ char mot[MAXIDENT];
  sprintf(tquad,"%-6s ",nomop(qd->OP));
  if (qd->ARG1!=NULL)
    {sprintf(mot,"%-10s",qd->ARG1);
      strcat(tquad,mot);}
  else
    {sprintf(mot,"%-10s"," ");
      tquad=strcat(tquad,mot);};
    if (qd->ARG2!=NULL)
    {sprintf(mot,"%-10s",qd->ARG2);
      strcat(tquad,mot);}
  else
    {sprintf(mot,"%-10s"," ");
      strcat(tquad,mot);};
    if (qd->RES!=NULL)
    {sprintf(mot,"%-10s",qd->RES);
      strcat(tquad,mot);}
  else
    {sprintf(mot," ");
      strcat(tquad,mot);};
    }

/* affiche la biliste de quad */
void ecrire_bilquad(BILQUAD bq)
{QUAD qcour;
  qcour=bq.debut;
  while(qcour!=NULL)
    {ecrire_quad(qcour);
      qcour=qcour->SUIV;}
}
/* affiche la biliste de quad (avec separateurs) */
void ecrire_sep_bilquad(BILQUAD bq)
{QUAD qcour;
  qcour=bq.debut;
  while(qcour!=NULL)
    {ecrire_sep_quad(qcour);
      qcour=qcour->SUIV;}
}
/*-------------------------------------------------------------------*/
/*------------------------imp-vers-quad------------------------------*/

/* traduit une (expression ou commande) en biliste de quadruplets */
/* met a jour l'environnement (var globale)                      */
BILQUAD imp2quad(NOE ec)
{ extern BILENVTY benvty;
  type tint;
  tint=creer_type(0,T_int);
  type tboo;
  tboo=creer_type(0,T_boo);
  int codop;

  BILQUAD bilq1, bilq2, bilexp, bilres;/* trad de: fg, fd, expression, resultat */
  int newop; char *netiq, *netiqf, *nres;        /* nouveaux ingredients */
  char *narg1=NULL;char  *narg2=NULL; 
  QUAD nquad;                         /* nouveau quadruplet   */
    assert(ec != NULL);
  switch(ec->codop)
    {/* CAS: ec est une EXPRESSION */

    case NewAr:
      netiq=gensym("ET");
      newop=ec->codop;
      narg1=Idalloc();
      sprintf(narg1, "TAB%d", padrl);
      ADR[padrl]=ptasl;
      ptasl += atoi(ec->FD->ETIQ);
      TAL[padrl]=atoi(ec->FD->ETIQ);
      padrl++;
      nres=gensym("VA");
      narg2=Idalloc();
      narg2 = ec->FD->ETIQ;
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      bilres=creer_bilquad(nquad);
      break;
    case Ind:
      netiq=gensym("ET");
      int index=NameToId(ec->FG->ETIQ);
      newop=ec->codop;
      int indice = atoi(ec->FD->ETIQ);     
      int val=TAS[ADR[index]+indice];
      narg1=Idalloc();
      sprintf(narg1, "%d",val );
      narg2=Idalloc();
      narg2=NULL;
      nres=gensym("VA");
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      bilres=creer_bilquad(nquad);
      break;
        
    case And:case Or:
      /* les ingredients */
      netiq=gensym("ET");
      newop=ec->codop;
      /* les traductions des deux arguments */
      bilq1=imp2quad(ec->FG);
      bilq2=imp2quad(ec->FD);
      /* se simplifie ? */
      if (ec->FG->codop!=V)
  {narg1=Idalloc();
    strcpy(narg1,bilq1.fin->RES);}
      else
  {narg1=Idalloc();
    strcpy(narg1,ec->FG->ETIQ);}
      if (ec->FD->codop!=V)
  {narg2=Idalloc();
    strcpy(narg2,bilq2.fin->RES);}
      else
  {narg2=Idalloc();
    strcpy(narg2,ec->FD->ETIQ);}
      nres=gensym("VA");
      /* on insere le nom de var dans l'environnement */
      inbilenvty(&benvty,nres,tint);
      /* le quadruplet: ETnum, Afc, chaineconst,-, VAnum */
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      bilres=creer_bilquad(nquad);
      /* la suite de quadruplets */
      bilq2=concatq(bilq1,bilq2);
      bilres=concatq(bilq2,bilres);
      break; 

    case Not:
      netiq=gensym("ET");
      newop=ec->codop;
      /* les traductions des deux arguments */
      bilq1=imp2quad(ec->FG);
      /* se simplifie ? */
      narg1=Idalloc();
      strcpy(narg1,bilq1.fin->RES);
      nres=gensym("VA");
      /* on insere le nom de var dans l'environnement */
      inbilenvty(&benvty,nres,tboo);
      /* le quadruplet: ETnum, Afc, chaineconst,-, VAnum */
      narg2 = NULL;
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      bilres=creer_bilquad(nquad);
      /* la suite de quadruplets */
      bilres=concatq(bilq1,bilres);
      break;

    case Lt:
      printf("%d\n",ec->codop);
      netiq=gensym("ET");
      newop=Lt;

      /* les traductions des deux arguments */
      bilq1=imp2quad(ec->FG);
      /* se simplifie ? */
      narg1=Idalloc();
      strcpy(narg1,bilq1.fin->RES);
      bilq2=imp2quad(ec->FD);
      /* se simplifie ? */
      narg2=Idalloc();
      strcpy(narg2,bilq2.fin->RES);
      nres=gensym("VA");
      /* on insere le nom de var dans l'environnement */
      inbilenvty(&benvty,nres,tboo);
      /* le quadruplet: ETnum, Afc, chaineconst,-, VAnum */
      printf("%d\n",ec->codop);
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      bilres=creer_bilquad(nquad);
      /* la suite de quadruplets */
      bilq2=concatq(bilq1,bilq2);
      bilres=concatq(bilq2,bilres);
      break;
    case true: case false:
            /* les ingredients */
      netiq=gensym("ET");newop=Afc;
      narg1=Idalloc();sprintf(narg1,"%s",ec->ETIQ);
      narg2=NULL;nres=gensym("CT");
      /* on insere le nom de const dans l' environnement */
      inbilenvty(&benvty,nres,tboo);
      /* le quadruplet: ETnum, Afc, chaineconst,-, CTnum */
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      bilres=creer_bilquad(nquad);
      break;
    case Pl:case Mo:case Mu:                   /* operation binaire */
      /* les ingredients */
      netiq=gensym("ET");
      newop=ec->codop;
      /* les traductions des deux arguments */
      bilq1=imp2quad(ec->FG);
      bilq2=imp2quad(ec->FD);
      /* se simplifie ? */
      if (ec->FG->codop!=V)
	{narg1=Idalloc();
	  strcpy(narg1,bilq1.fin->RES);}
      else
	{narg1=Idalloc();
	  strcpy(narg1,ec->FG->ETIQ);}
      if (ec->FD->codop!=V)
	{narg2=Idalloc();
	  strcpy(narg2,bilq2.fin->RES);}
      else
	{narg2=Idalloc();
	  strcpy(narg2,ec->FD->ETIQ);}
      nres=gensym("VA");
      /* on insere le nom de var dans l'environnement */
      inbilenvty(&benvty,nres,tint);
      /* le quadruplet: ETnum, Afc, chaineconst,-, VAnum */
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      bilres=creer_bilquad(nquad);
      /* la suite de quadruplets */
      bilq2=concatq(bilq1,bilq2);
      bilres=concatq(bilq2,bilres);
      break; 
    case I:
      /* les ingredients */
      netiq=gensym("ET");newop=Afc;
      narg1=Idalloc();sprintf(narg1,"%s",ec->ETIQ);
      narg2=NULL;nres=gensym("CT");
      /* on insere le nom de const dans l' environnement */
      inbilenvty(&benvty,nres,tint);
      /* le quadruplet: ETnum, Afc, chaineconst,-, CTnum */
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      bilres=creer_bilquad(nquad);
      break;
    case V:
      /* le quadruplet: skip, resultat dans chainevar */
      netiq=gensym("ET");newop=Sk;narg1=NULL;narg2=NULL;nres=ec->ETIQ;
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      bilres=creer_bilquad(nquad);
      break;
      /* CAS: ec est une COMMANDE */
    case Mp:
      bilq1=imp2quad(ec->FG);
      /* les ingredients */
      netiq=gensym("ET");newop=St;narg1=NULL;narg2=NULL;nres=NULL;
      /* le quadruplet final: stop  (pas d'adresse de resultat) */
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      bilq2=creer_bilquad(nquad);
      bilres=concatq(bilq1,bilq2);
      break;
    case Af:
      codop=ec->FG->codop;
      if (codop==271)
        {
          int index=NameToId(((ec->FG)->FG)->ETIQ); //indice tableau dans le tas
          // indice de la case c'est : (ec->FG)->FD
          int indice=atoi(((ec->FG)->FD)->ETIQ);
          // valeur a affecter c'est : ec->FD
          bilq2=imp2quad(ec->FD);
          // TAS[ADR[index]+indice] valeur de la case 
          if((ec->FD)->codop==258){
            TAS[ADR[index]+indice]=atoi((ec->FD)->ETIQ); // changer la valeur en dur si c'est une constante 
          }else{
            TAS[ADR[index]+indice]=valchty(benvty.debut,(ec->FD)->ETIQ);
          }  
          // faire l'affictation
          netiq=gensym("ET");
          newop=Af;
          narg1=strcat(((ec->FG)->FG)->ETIQ, ((ec->FG)->FD)->ETIQ);
          /* narg2= adresse res du code du rhs */
          narg2=Idalloc();
          strcpy(narg2,bilq2.fin->RES);
          nres=NULL;
          /* le quadruplet: ETnum, Af, chainevar1,chaineres2, NULL */
          nquad=creer_quad(netiq,newop,narg1,narg2,nres);
          bilres=concatq(bilq2,creer_bilquad(nquad));
          break;
        }
      else{
        if(ec->FD->codop==265){ // NewAr
          matching[padrl]=ec->FG->ETIQ;
        }
        /* les ingredients */
        netiq=gensym("ET");
        newop=Af;
        /* narg1= chaine en lhs */
        narg1=ec->FG->ETIQ;
        /* narg2= adresse res du code du rhs */
        bilq2=imp2quad(ec->FD);
        narg2=Idalloc();
        strcpy(narg2,bilq2.fin->RES);
        nres=NULL;
        /* le quadruplet: ETnum, Af, chainevar1,chaineres2, NULL */
        nquad=creer_quad(netiq,newop,narg1,narg2,nres);
        bilres=concatq(bilq2,creer_bilquad(nquad));
      }
      break;
        
        
    case Sk:
      /* les ingredients */
      netiq=gensym("ET");newop=Sk;narg1=NULL;narg2=NULL;nres=NULL; 
      /* le quadruplet: skip  (pas d'adresse de resultat) */
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      bilres=creer_bilquad(nquad);
      break;
    case Se: 
      bilq1=imp2quad(ec->FG);
      bilq2=imp2quad(ec->FD);
      bilres=concatq(bilq1,bilq2);
      break;
    case If:
      bilexp=imp2quad(ec->FG);    /* traduction de l'expression */
      bilq1=imp2quad(ec->FD->FG); /* commande (cas vrai) */
      bilq2=imp2quad(ec->FD->FD); /* commande (cas faux) */
      bilq2=normal(bilq2);
      /* les ingredients de Q1 */
      netiq=gensym("ET");newop=Jz;
      narg1=bilexp.fin->RES;
      narg2=NULL;
      nres=bilq2.debut->ETIQ;
      /* le quadruplet Q1 */
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      /* nouvelle biliste */
      bilres=concatq(bilexp,creer_bilquad(nquad));
      bilres=concatq(bilres,bilq1);
      /* les ingredients de Q2 */
      netiq=gensym("ET");newop=Jp;
      narg1=NULL;
      narg2=NULL;
      nres=bilq2.fin->ETIQ;
      /* le quadruplet Q2 */
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      /* nouvelle biliste */
      bilres=concatq(bilres,creer_bilquad(nquad));
      bilres=concatq(bilres,bilq2);
      break;
    case Wh:
      bilexp=imp2quad(ec->FG);    /* traduction de l'expression */
      bilq1=imp2quad(ec->FD);     /* traduction du corps        */
      bilq1=normal(bilq1);
      /* les ingredients de Q1 */
      netiq=gensym("ET");newop=Jz;  /* etiquette de Q1            */
      narg1=bilexp.fin->RES;
      narg2=NULL;
      netiqf=gensym("ET");        /* etiquette fin de traduction */
      nres=netiqf;
      /* le quadruplet Q1 */
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      /* nouvelle biliste */
      bilres=concatq(bilexp,creer_bilquad(nquad));
      bilres=concatq(bilres,bilq1);
      /* les ingredients de Q2 */
      newop=Jp;
      /* narg1=narg2=NULL; */
      nres=bilexp.debut->ETIQ;
      /* on substitue Q2 a la fin de bilres */
      bilres.fin->OP=newop;
      assert(bilres.fin->ARG1==NULL); /* vrai si bilq1 est normalise */
      assert(bilres.fin->ARG2==NULL);
      bilres.fin->RES=nres;
      /* les ingredients de Q3 */
      netiq=netiqf;                   /* etiquette de Q3            */
      newop=Sk; narg1=NULL; narg2=NULL; nres=NULL;
      /* le quadruplet Q3 */
      nquad=creer_quad(netiq,newop,narg1,narg2,nres);
      /* nouvelle biliste */
      bilres=concatq(bilres,creer_bilquad(nquad));
      break;
    default: break;
    };
    return(bilres);
} 

/* teste la traduction imp --> c3a*/
void test_tradc3a(int n, NOE c)
{/* char *etiq;int op;char *arg1; char *arg2;char *res; */
  int i;QUAD qcour;
  BILQUAD bq,bqcour;
  bq.debut=NULL;bq.fin=NULL;
    for(i=0;i<n;i++)
    {qcour=creer_quad(gensym("ET"),Af,gensym("ARG"),gensym("ARG"),gensym("RES"));
      ecrire_quad(qcour);
    };
  for(i=0;i<n;i++)
    {qcour=creer_quad(gensym("ET"),Af,gensym("ARG"),gensym("ARG"),gensym("RES"));
      bqcour=creer_bilquad(qcour);
      bq=concatq(bq,bqcour);
    };
  printf("la biliste des chaines engendrees \n");
  ecrire_bilquad(bq);
  printf("\n");
  bq=imp2quad(c);
  printf("le code a 3 adresses de la commande: \n");
  ecrire_bilquad(bq);
}
