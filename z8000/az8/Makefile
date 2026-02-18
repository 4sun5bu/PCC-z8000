FILES = mical.h inst.h scan.c error.c init.c ins.c ioz8.c\
	ps.c rel.c sdi.c sym.c ../b.out.h

OBJECTS = error.o init.o ins.o ioz8.o ps.o rel.o sdi.o sym.o scan.o

CFLAGS = -O -w -I.. -Wno-implicit-int -Wno-implicit-function-declaration -Wno-return-mismatch -Wno-return-type

az8: $(OBJECTS)
	cc -o az8 $(OBJECTS)

$(OBJECTS): mical.h

init.o ins.o: mical.h inst.h

rel.c:	mical.h ../b.out.h

sym.c:	mical.h ../b.out.h

clean:
	rm -f *.o az8
