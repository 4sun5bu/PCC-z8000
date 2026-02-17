int abs(x) int x; {
	if (x < 0) return -x;
	return x;
}

int sum(n) int n; {
	int s, i;
	s = 0;
	for (i = 1; i <= n; i++)
		s = s + i;
	return s;
}

int fib(n) int n; {
	int a, b, t, i;
	a = 0;
	b = 1;
	for (i = 0; i < n; i++) {
		t = a + b;
		a = b;
		b = t;
	}
	return a;
}

struct point {
	int x;
	int y;
};

int dist2(p) struct point *p; {
	return p->x * p->x + p->y * p->y;
}

int arr[10];

fill(n) int n; {
	int i;
	for (i = 0; i < n; i++)
		arr[i] = i * i;
}

main() {
	struct point pt;
	abs(-5);
	sum(100);
	fib(10);
	pt.x = 3;
	pt.y = 4;
	dist2(&pt);
	fill(10);
	return 0;
}
