# 
# A Makefile for Pink function
#
# Hugues Talbot 2010-08-25
#

SOURCE=perona_malik.c do_nothing.c

OBJECT=${SOURCE:.c=.o}

CC=gcc
CFLAGS=-Wall -g -I/home/talboth/projects/Pink/Pink_SVN_MC/trunk/include
LDFLAGS= -L/home/talboth/projects/Pink/Build/lib  -lpink

# compiling target
.c.o:
	${CC}  ${CFLAGS} -c $< 

# linking target
.o:
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

.PHONY=test clean

all: do_nothing perona_malik

# dependencies
do_nothing: do_nothing.o
perona_malik: perona_malik.o


clean:
	-rm *~ ${OBJECT} perona_malik do_nothing

test:
	@echo "SOURCE = " ${SOURCE}
	@echo "OBJECT = " ${OBJECT}