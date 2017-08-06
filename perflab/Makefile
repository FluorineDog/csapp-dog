# Student's Makefile for the CS:APP Performance Lab
TEAM = dog
VERSION = 1
HANDINDIR = build

CC = gcc
CFLAGS = -Wall -Ofast
LIBS = -lm

OBJS = clock.o kernels.o fcyc.o driver.o

all: driver

driver: $(OBJS) fcyc.h clock.h defs.h config.h Makefile
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o driver

handin:
	cp kernels.c $(HANDINDIR)/$(TEAM)-$(VERSION)-kernels.c

clean: 
	-rm -f $(OBJS) driver core *~ *.o


