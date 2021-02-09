EXE= bddsurfsing bddtest

ALLEXE= point2plus bdd1 bdd2 bddsurfend bddsurfsing

#XLIB = -L/usr/X11R6/lib -lX11 -lXext
#XINCL = /usr/include/X11R6
#CCFLAGS = -g -DPC -DPROTOARGS -DPCMAISON
#CCFLAGS = -g -DPC -DPROTOARGS -DPCTRAVAIL

XLIB = -L/usr/lib/X11R5 -lX11 -lXext
XINCL = /usr/include/X11R5
CCFLAGS = -g -DHP -Aa -O

IDIR = $(PINK)/include
ODIR = $(PINK)/obj
CC = cc

courant:	$(EXE)

all:	$(ALLEXE)

bdd1:	bdd1.c $(IDIR)/mclifo.h $(IDIR)/mcimage.h $(IDIR)/mckhalimsky3d.h $(ODIR)/mclifo.o $(ODIR)/mcimage.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) -I$(XINCL) bdd1.c $(ODIR)/mclifo.o $(ODIR)/mcimage.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o -o bdd1

bdd2:	bdd2.c $(IDIR)/mclifo.h $(IDIR)/mcimage.h $(IDIR)/mckhalimsky3d.h $(ODIR)/mclifo.o $(ODIR)/mcimage.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) -I$(XINCL) bdd2.c $(ODIR)/mclifo.o $(ODIR)/mcimage.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o -o bdd2

bddsurfend:	bddsurfend.c $(IDIR)/mclifo.h $(IDIR)/mcimage.h $(IDIR)/mckhalimsky3d.h $(ODIR)/mclifo.o $(ODIR)/mcimage.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) -I$(XINCL) bddsurfend.c $(ODIR)/mclifo.o $(ODIR)/mcimage.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o -o bddsurfend

bddsurfsing:	bddsurfsing.c $(IDIR)/mclifo.h $(IDIR)/mcimage.h $(IDIR)/mckhalimsky3d.h $(ODIR)/mclifo.o $(ODIR)/mcimage.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) -I$(XINCL) bddsurfsing.c $(ODIR)/mclifo.o $(ODIR)/mcimage.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o -o bddsurfsing

bddtest:	bddtest.c $(IDIR)/mclifo.h $(IDIR)/mcimage.h $(IDIR)/mckhalimsky3d.h $(ODIR)/mclifo.o $(ODIR)/mcimage.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) -I$(XINCL) bddtest.c $(ODIR)/mclifo.o $(ODIR)/mcimage.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o -o bddtest

point2plus:	point2plus.l
	lex point2plus.l ; cc lex.yy.c -ll -o point2plus ; rm lex.yy.c

clean:
	rm -f *.o
	rm -f $(ALLEXE)
	rm -f *~
