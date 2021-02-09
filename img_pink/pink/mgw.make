PINK = .
# Version MinGW
  CC = c:/MinGW/bin/gcc
  CPP = c:/MinGW/bin/g++
  CCFLAGS = -g -DDOSIO -O3
  ODIR = $(PINK)/obj
  BDIR = $(PINK)/bin
  OBJ_COMMON = $(ODIR)/mcimage.o $(ODIR)/mcchrono.o
  OBJ = $(ODIR)/mcliste.o $(ODIR)/mcset.o $(ODIR)/mckhalimsky2d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mckhalimskyNd.o
