/* pcc_struct.c -- struct assignment from local, global, static
 * Adapted from pcc-tests/tests/c/codegen/struct1.c */

struct st {
	int v1;
	int v2;
};

struct st t3;
static struct st t4;

main()
{
	struct st t1;
	struct st t2;

	t1.v1 = 11;
	t1.v2 = 12;
	t2.v1 = 21;
	t2.v2 = 22;
	t3.v1 = 31;
	t3.v2 = 32;
	t4.v1 = 41;
	t4.v2 = 42;

	/* verify initial values */
	if (t1.v1 != 11 || t1.v2 != 12) return 1;

	/* assign from local */
	t1 = t2;
	if (t1.v1 != 21 || t1.v2 != 22) return 2;

	/* assign from global */
	t1 = t3;
	if (t1.v1 != 31 || t1.v2 != 32) return 3;

	/* assign from static */
	t1 = t4;
	if (t1.v1 != 41 || t1.v2 != 42) return 4;

	/* assign to global */
	t3 = t2;
	if (t3.v1 != 21 || t3.v2 != 22) return 5;

	return 0;
}
