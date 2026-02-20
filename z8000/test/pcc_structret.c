/* pcc_structret.c -- struct return from function
 * Adapted from pcc-tests/tests/c/codegen/struct2.c */

struct str {
	int i;
};

struct str init(v)
int v;
{
	struct str s;
	s.i = v;
	return s;
}

main()
{
	struct str r;

	r = init(10);
	if (r.i != 10) return 1;

	r = init(99);
	if (r.i != 99) return 2;

	return 0;
}
