/* pcc_math.c -- int div, mod, xor, or, and
 * Adapted from pcc-tests/tests/c/codegen/math0.c */

test1(b)
int b;
{
	int z, y, x, w, v, u, t, s;
	z = b + 3;	/* 4 */
	y = z - 3;	/* 1 */
	x = y / 3;	/* 0 */
	w = x * 3;	/* 0 */
	v = w % 3;	/* 0 */
	u = v ^ 3;	/* 3 */
	t = u | 3;	/* 3 */
	s = t & 3;	/* 3 */
	return s;
}

test2(b)
int b;
{
	unsigned int z, y, x, w, v, u, t, s;
	z = b + 3;	/* 4 */
	y = z - 3;	/* 1 */
	x = y / 3;	/* 0 */
	w = x * 3;	/* 0 */
	v = w % 3;	/* 0 */
	u = v ^ 3;	/* 3 */
	t = u | 3;	/* 3 */
	s = t & 3;	/* 3 */
	return s;
}

main()
{
	if (test1(1) != 3) return 1;
	if (test2(1) != 3) return 2;

	/* additional div/mod checks */
	if (7 / 2 != 3) return 3;
	if (7 % 2 != 1) return 4;
	if (100 / 10 != 10) return 5;
	if (100 % 10 != 0) return 6;

	/* xor/or/and */
	if ((5 ^ 3) != 6) return 7;
	if ((5 | 3) != 7) return 8;
	if ((5 & 3) != 1) return 9;

	return 0;
}
