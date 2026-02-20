/* pcc_scope.c -- variable scoping and shadowing
 * Inspired by pcc-tests/tests/c/lang/mustpass0001.c */

int g;

main()
{
	int a;

	/* outer scope */
	a = 10;
	g = 20;
	if (a != 10) return 1;

	/* inner scope shadows outer variable */
	{
		int a;
		a = 42;
		if (a != 42) return 2;

		/* modify global from inner scope */
		g = 99;
	}

	/* outer 'a' unchanged, global modified */
	if (a != 10) return 3;
	if (g != 99) return 4;

	/* nested scope two levels deep */
	{
		int a;
		a = 1;
		{
			int a;
			a = 2;
			if (a != 2) return 5;
		}
		if (a != 1) return 6;
	}

	if (a != 10) return 7;

	return 0;
}
