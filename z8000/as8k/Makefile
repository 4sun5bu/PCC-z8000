FILES = mical.h inst.h scan.c error.c init.c ins.c ioz8.c\
	ps.c rel.c sdi.c sym.c ../a.out.h

OBJECTS = error.o init.o ins.o ioz8.o ps.o rel.o sdi.o sym.o scan.o

CFLAGS = -O0 -g3 -w -I.. -Wno-implicit-int -Wno-implicit-function-declaration -Wno-return-mismatch -Wno-return-type

as8k: $(OBJECTS)
	cc -o as8k $(OBJECTS)

$(OBJECTS): mical.h ../a.out.h

init.o ins.o: mical.h inst.h

rel.c:	mical.h

sym.c:	mical.h

clean:
	rm -f *.o as8k
