/* test bitfield read and write code generation */

struct flags {
	int a:3;
	int b:5;
	int c:8;
};

int read_a(p) struct flags *p; {
	return p->a;
}

int read_b(p) struct flags *p; {
	return p->b;
}

int read_c(p) struct flags *p; {
	return p->c;
}

write_a(p, v) struct flags *p; int v; {
	p->a = v;
}

write_b(p, v) struct flags *p; int v; {
	p->b = v;
}

clear_a(p) struct flags *p; {
	p->a = 0;
}

main() {
	struct flags f;
	f.a = 3;
	f.b = 10;
	f.c = 255;
	read_a(&f);
	read_b(&f);
	read_c(&f);
	write_a(&f, 5);
	write_b(&f, 20);
	clear_a(&f);
	return 0;
}
