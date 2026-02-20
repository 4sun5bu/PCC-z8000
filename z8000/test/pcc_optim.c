/* pcc_optim.c -- constant folding, dead code, loop with multiply
 * Adapted from pcc-tests/tests/c/codegen/optim0.c */

test()
{
	int a, b, i, j, r, p, q, s;

	a = 10;
	b = 0;
	i = 1;
	j = 2;
	r = 0;
	p = i;
	q = j;

	r = i + j;		/* 3 */
	if (r == 3)
		s = 1 * 4;	/* 4 */
	else
		s = 4;
	s = 2;			/* overwrite */
	s = s + 10;		/* 12 */
	s = s - 10;		/* 2 */
	if (0)
		s = s + 4;	/* dead code */
	if (p != q)
		s = s - 4;	/* -2 */

	for (i = 0; i < (j * 7); i++) {
		s = s + r;	/* s += 3 each iter, 14 iterations */
		b = b + (p * q);	/* b += 2 each iter */
	}
	/* s = -2 + 14*3 = 40 */
	/* b = 14*2 = 28 */
	return (s + b);		/* 68 */
}

main()
{
	if (test() != 68) return 1;
	return 0;
}
