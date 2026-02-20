/* switch.c -- dense table jump + sparse binary search */

/* dense switch: cases 0-4 contiguous */
dense(n)
int n;
{
	switch (n) {
	case 0: return 10;
	case 1: return 20;
	case 2: return 30;
	case 3: return 40;
	case 4: return 50;
	default: return -1;
	}
}

/* sparse switch: widely separated values */
sparse(n)
int n;
{
	switch (n) {
	case 1:   return 100;
	case 50:  return 200;
	case 100: return 300;
	case 500: return 400;
	default:  return -1;
	}
}

main()
{
	if (dense(0) != 10) return 1;
	if (dense(3) != 40) return 2;
	if (dense(4) != 50) return 3;
	if (dense(7) != -1) return 4;

	if (sparse(1) != 100) return 5;
	if (sparse(50) != 200) return 6;
	if (sparse(100) != 300) return 7;
	if (sparse(500) != 400) return 8;
	if (sparse(999) != -1) return 9;

	return 0;
}
