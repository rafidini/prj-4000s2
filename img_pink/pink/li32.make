PINK = .
# Version LINUX
  CC = gcc
  CPP = g++

#### use next line for NO tiff interface
  CCFLAGS = -g -DUNIXIO -Wall -m32
#### use next line for TIFF interface
#  CCFLAGS = -g -DUNIXIO -DHAVE_TIFF_LIB -Wall

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
