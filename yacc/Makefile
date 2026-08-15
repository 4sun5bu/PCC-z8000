CFLAGS=-O0 -g3 -Wno-implicit-function-declaration -Wno-implicit-int -Wno-return-type

all: yaccV7

yaccV7: y1.o y2.o y3.o y4.o
	cc $(CFLAGS) -o yaccV7 y?.o

y1.o y2.o y3.o y4.o: dextern files

install:
	cp yaccV7 /usr/local/bin
	cp yaccpar /usr/local/lib

clean:
	rm -f  *.o yaccV7
