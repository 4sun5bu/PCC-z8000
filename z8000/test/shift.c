/* shift.c -- word and long shifts */

main()
{
	int w;
	long l;

	/* word left shift */
	w = 1;
	w = w << 4;
	if (w != 16) return 1;

	/* word right shift (arithmetic on signed) */
	w = -16;
	w = w >> 2;
	if (w != -4) return 2;

	/* word unsigned right shift via unsigned */
	w = 1 << 15;
	w = (w >> 14) & 3;
	/* arithmetic right shift of 0x8000 >> 14 = 0xFFFE, & 3 = 2 */
	if (w != 2) return 3;

	/* long left shift */
	l = 1;
	l = l << 16;
	if (l != 65536) return 4;

	/* long right shift */
	l = 65536;
	l = l >> 8;
	if (l != 256) return 5;

	return 0;
}
