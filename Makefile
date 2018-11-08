CC=gcc
CFLAGS=-lWarn -pedantic

ifttt.o: ifttt.c ifttt.h
	cc -c -ansi $<

temp: temp.o ifttt.c
	cc temp.o ifttt.c -L. -lcurl -o temp

temp.o: temp.c ifttt.h
	cc -c -ansi $<
