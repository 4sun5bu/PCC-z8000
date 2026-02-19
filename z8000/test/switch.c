/* test switch statement code generation */

int small_switch(x) int x; {
	switch(x) {
	case 1: return 10;
	case 2: return 20;
	case 3: return 30;
	default: return -1;
	}
}

/* dense switch — should use table jump */
int dense_switch(x) int x; {
	switch(x) {
	case 0: return 100;
	case 1: return 101;
	case 2: return 102;
	case 3: return 103;
	case 4: return 104;
	case 5: return 105;
	default: return -1;
	}
}

/* sparse switch — should use binary search */
int sparse_switch(x) int x; {
	switch(x) {
	case 1:   return 1;
	case 10:  return 2;
	case 100: return 3;
	case 1000: return 4;
	default: return 0;
	}
}

main() {
	small_switch(2);
	dense_switch(3);
	sparse_switch(100);
	return 0;
}
