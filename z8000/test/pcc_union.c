/* pcc_union.c -- union passing and address-of union parameter
 * Adapted from pcc-tests/tests/c/codegen/struct3.c */

union cmp {
	int i;
	int j;
};

int got_ptr;

test1(t)
union cmp *t;
{
	got_ptr = t->i;
}

test(t1)
union cmp t1;
{
	test1(&t1);
}

main()
{
	union cmp t;

	got_ptr = 0;
	t.i = 77;
	test(t);

	if (got_ptr != 77) return 1;

	t.j = 123;
	if (t.i != 123) return 2;

	return 0;
}
