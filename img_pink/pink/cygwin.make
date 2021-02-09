#!/bin/bash
export PINK=.
# Version CYGWIN
  export XLIB=-L/usr/X11R6/lib -lX11 -lXext
  export XINCL=/usr/include/X11R6 
  export CCFLAGS=-g -DPC -DUNIXIO -O3 -DCYGWIN -pg
  export ODIR=$(PINK)/linux/obj
  export BDIR=$(PINK)/linux/bin
  export LIBS=-lm
  OBJ_COMMON = $(ODIR)/mcimage.o $(ODIR)/mcchrono.o
