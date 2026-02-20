/* bitfield.c -- bitfield read/write */

struct bits {
	unsigned a : 3;
	unsigned b : 5;
	unsigned c : 8;
};

main()
{
	struct bits bf;

	bf.a = 5;
	bf.b = 17;
	bf.c = 200;

	if (bf.a != 5) return 1;
	if (bf.b != 17) return 2;
	if (bf.c != 200) return 3;

	/* modify one field, check others are intact */
	bf.b = 31;
	if (bf.a != 5) return 4;
	if (bf.b != 31) return 5;
	if (bf.c != 200) return 6;

	return 0;
}
