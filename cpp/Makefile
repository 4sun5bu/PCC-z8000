YACC=yaccV7
CFLAGS=-O0 -g3 -Wno-implicit-function-declaration -Wno-implicit-int -Wno-return-type

all: cpp

cpp: cpp.o cpy.o
	cc $(CFLAGS) cpp.o cpy.o -o cppV7
cpp.o: cpp.c
	cc $(CFLAGS) -c -Dunix=1 -Dpdp11=1 cpp.c
cpy.o: cpy.y yylex.c
	$(YACC) cpy.y
	cc $(CFLAG) -c y.tab.c -o cpy.o

install:
	cp cppV7 /usr/local/bin

clean:
	rm -f *.o y.tab.c cppV7
