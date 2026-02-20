/* pcc_cmp.c -- systematic signed and unsigned comparisons
 * Adapted from pcc-tests/tests/c/codegen/cmp0.c + cmp1.c */

test_signed()
{
	int x, y;
	x = 1;
	y = 2;

	/* compare against const */
	if (!(x == 1)) return 1;
	if (x != 1) return 2;
	if (x < 1) return 3;
	if (!(x <= 1)) return 4;
	if (x > 1) return 5;
	if (!(x >= 1)) return 6;

	/* compare against zero */
	if (x == 0) return 7;
	if (!(x != 0)) return 8;
	if (x < 0) return 9;
	if (x <= 0) return 10;
	if (!(x > 0)) return 11;
	if (!(x >= 0)) return 12;

	/* compare register vs register */
	if (x == y) return 13;
	if (!(x != y)) return 14;
	if (!(x < y)) return 15;
	if (!(x <= y)) return 16;
	if (x > y) return 17;
	if (x >= y) return 18;

	return 0;
}

test_unsigned()
{
	unsigned int x, y;
	x = 1;
	y = 2;

	/* compare against const */
	if (!(x == 1)) return 21;
	if (x != 1) return 22;
	if (x < 1) return 23;
	if (!(x <= 1)) return 24;
	if (x > 1) return 25;
	if (!(x >= 1)) return 26;

	/* compare against zero */
	if (x == 0) return 27;
	if (!(x != 0)) return 28;
	/* x < 0 is always false for unsigned; skip */
	if (x <= 0) return 30;
	if (!(x > 0)) return 31;
	/* x >= 0 is always true for unsigned; skip */

	/* compare register vs register */
	if (x == y) return 33;
	if (!(x != y)) return 34;
	if (!(x < y)) return 35;
	if (!(x <= y)) return 36;
	if (x > y) return 37;
	if (x >= y) return 38;

	return 0;
}

main()
{
	int r;
	r = test_signed();
	if (r) return r;
	r = test_unsigned();
	if (r) return r;
	return 0;
}
