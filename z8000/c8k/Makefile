CFLAGS=-O -w -Wno-implicit-int -Wno-implicit-function-declaration -Wno-return-mismatch -Wno-return-type -Wno-int-conversion

c8k: cgram.o xdefs.o scan.o pftn.o trees.o optim.o code.o local.o reader.o \
	local2.o order.o match.o allo.o comm1.o table.o
	cc $(CFLAGS) cgram.o xdefs.o scan.o pftn.o trees.o optim.o code.o \
	local.o  reader.o local2.o order.o match.o allo.o comm1.o table.o \
	-o $@

trees.o: manifest macdefs mfile1 trees.c
	cc -c $(CFLAGS) trees.c

optim.o: manifest macdefs mfile1 optim.c
	cc -c $(CFLAGS) optim.c

pftn.o: manifest macdefs mfile1 pftn.c
	cc -c $(CFLAGS) pftn.c

code.o: manifest macdefs mfile1 code.c
	cc -c $(CFLAGS) code.c

local.o: manifest macdefs mfile1 local.c
	cc -c $(CFLAGS) local.c

scan.o: manifest macdefs mfile1 scan.c
	cc -c $(CFLAGS) scan.c

xdefs.o: manifest mfile1 macdefs xdefs.c
	cc -c $(CFLAGS) xdefs.c

cgram.o: manifest mfile1 macdefs cgram.c
	cc -c $(CFLAGS) cgram.c

cgram.c: cgram.y
	yacc cgram.y
	mv y.tab.c cgram.c

comm1.o: manifest mfile1 common macdefs comm1.c
	cc -c $(CFLAGS) comm1.c

table.o: manifest mfile2 mac2defs macdefs table.c
	cc -c $(CFLAGS) table.c

reader.o: manifest mfile2 mac2defs macdefs reader.c
	cc -c $(CFLAGS) reader.c

local2.o: manifest mfile2 mac2defs macdefs local2.c
	cc -c $(CFLAGS) local2.c

order.o: manifest mfile2 mac2defs macdefs order.c
	cc -c $(CFLAGS) order.c

match.o: manifest mfile2 mac2defs macdefs match.c
	cc -c $(CFLAGS) match.c

allo.o: manifest mfile2 mac2defs macdefs allo.c
	cc -c $(CFLAGS) allo.c

clean:
	rm -f *.o c8k
