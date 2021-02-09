IDIR = $(PINK)/include
ODIR = $(PINK)/linux/obj

ConnectNumbersTable:	ConnectNumbersTable.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo3d.h $(IDIR)/mctopo.h $(ODIR)/mccodimage.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o
	$(CC) -I$(IDIR) ConnectNumbersTable.c $(ODIR)/mccodimage.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o -o ConnectNumbersTable
