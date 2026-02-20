/* control.c -- loops, pointers, arrays, structs */
struct point {
	int x;
	int y;
};

sum_array(a, n)
int *a;
int n;
{
	int s;
	s = 0;
	while (n-- > 0)
		s = s + *a++;
	return s;
}

main()
{
	int arr[5];
	struct point p;
	int i;
	int total;

	/* fill array with 1..5 using a for loop */
	for (i = 0; i < 5; i++)
		arr[i] = i + 1;

	total = sum_array(arr, 5);	/* 15 */
	if (total != 15)
		return 1;

	/* struct access */
	p.x = 10;
	p.y = 20;
	if (p.x + p.y != 30)
		return 2;

	/* pointer to struct member */
	i = *(&p.y);
	if (i != 20)
		return 3;

	return 0;
}
