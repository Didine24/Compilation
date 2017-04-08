#Makefile 
CC = gcc 
CPPFLAGS=
CFLAGS=-g -Wall -std=c99
LDFLAGS=-lm 
FLEX = flex 
BISON = bison
BFLAGS=-d --report=all
HEADERS= $(NOM).tab.h arbre.h anasem.h interp.h codec3a.h

NOM = pppascal

$(NOM):	$(NOM).tab.o arbre.o anasem.o interp.o codec3a.o
	$(CC) $^ $(LDFLAGS) -o $@

$(NOM).tab.c: $(NOM).y lex.yy.c arbre.h anasem.h interp.h codec3a.h
	$(BISON) $(BFLAGS) $(NOM).y

lex.yy.c: $(NOM).l
	$(FLEX) $(NOM).l

$(NOM).tab.h: $(NOM).tab.c

lex.yy.o: lex.yy.c $(NOM).tab.h

arbre.o: arbre.c arbre.h $(NOM).tab.h
anasem.o: anasem.c anasem.h $(NOM).tab.h 
interp.o: interp.c interp.h $(NOM).tab.h
codec3a.o: codec3a.c codec3a.h $(NOM).tab.h	

.PHONY: clean 

clean:
	rm -f *.tab.? lex.*.c *.o *~ \#*\#
	rm -f *.output $(NOM)
