/* pcc_ptr.c -- pointer arrays, array of char pointers
 * Adapted from pcc-tests/tests/c/codegen/ptr0.c */

char p;

main()
{
	char *t1[3];
	int off;
	char *z;

	p = 65;	/* 'A' */
	t1[0] = &p;
	t1[1] = &p;
	t1[2] = &p;

	off = 2;
	z = t1[off];
	if (*z != 65) return 1;

	off = 0;
	z = t1[off];
	if (*z != 65) return 2;

	/* modify through pointer */
	*z = 66;
	if (p != 66) return 3;

	return 0;
}
