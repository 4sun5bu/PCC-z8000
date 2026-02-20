/* arith.c -- function calls, recursion, multiply */
fact(n)
int n;
{
	if (n <= 1)
		return 1;
	return n * fact(n - 1);
}

main()
{
	return fact(5);	/* 120 */
}
