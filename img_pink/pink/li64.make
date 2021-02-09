PINK = .
# Version LINUX
#  XLIB = -L/usr/X11R6/lib -lX11 -lXext
  XINCL = /usr/include/X11R6
  CC = gcc
#  CC = icc
  CPP = g++
  CCFLAGS = -g -DUNIXIO -DMC_64_BITS -Wall

  ODIR = $(PINK)/linux/obj
  BDIR = $(PINK)/linux/bin
  IDIR = $(PINK)/include
  CDIR = $(PINK)/src/com
  LDIR = $(PINK)/src/lib
  LIARDIR = $(PINK)/src/liar
  DDIR = $(PINK)/src/bdd
  TDIR = $(PINK)/src/tables

#### use next 2 lines for NO tiff interface
  LIBS = -lm
  OBJ_COMMON = $(ODIR)/mcimage.o $(ODIR)/mcchrono.o
#### use next 2 lines for TIFF interface
#  LIBS = -lm -ltiff
#  OBJ_COMMON = $(ODIR)/mcimage.o $(ODIR)/liarwrap.o $(ODIR)/mcchrono.o

  OBJ = $(ODIR)/mcliste.o $(ODIR)/mcset.o $(ODIR)/mckhalimsky2d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mckhalimskyNd.o $(ODIR)/mcchrono.o $(ODIR)/mctopo3d_table.o $(ODIR)/mcdrawps.o $(ODIR)/mcsegment.o
