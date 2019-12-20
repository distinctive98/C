.SUFFIXES: .v .V .m .M .pc .c .o .sh

CC=gcc

PROCFLAGS= include=jansson userid=bo/bo 1>err

CFLAGS= -I$(ORACLE_HOME)/precomp/public -I$(ORACLE_HOME)/rdbms/public -Ijansson -I. 2>err

OBJSTRS	=	trs.o netlib.o tr_clib.o tr_lib.o  tr1.o tr2.o tr4.o tr5.o tr6.o

.pc.c:
	$(ORABINDIR)/proc $(PROCFLAGS) iname=$*.pc

.pc.o:
	$(ORABINDIR)/proc $(PROCFLAGS) iname=$*.pc
	$(CC) $(CFLAGS) -c $*.c

.c.o:
	$(CC) $(CFLAGS) -c $*.c

all:    t1 clean

trs:$(OBJSTRS)
	$(CC) $(CFLAGS) -o $@ $(OBJSTRS) -L./jansson -ljansson  -L$(ORALIBD) -lclntsh -lm

clean:
	rm -f err *.lis *.o
	rm -f oracle.c

###
### end of makefile






