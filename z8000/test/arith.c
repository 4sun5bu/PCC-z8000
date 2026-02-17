int add(a, b) int a, b; { return a + b; }
int sub(a, b) int a, b; { return a - b; }
int mul(a, b) int a, b; { return a * b; }
int div2(a, b) int a, b; { return a / b; }
int mod(a, b) int a, b; { return a % b; }

int fact(n) int n; {
	if (n <= 1) return 1;
	return n * fact(n - 1);
}

int x;

main() {
	x = add(3, 4);
	x = sub(10, 5);
	x = mul(6, 7);
	x = div2(42, 6);
	x = mod(17, 5);
	x = fact(5);
	return x;
}
