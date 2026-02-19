/* test shift operations for word and long types */

long x;

long shl(v, n) long v; int n; {
	return v << n;
}

long shr_signed(v, n) long v; int n; {
	return v >> n;
}

unsigned long shr_unsigned(v, n) unsigned long v; int n; {
	return v >> n;
}

long shl_const(v) long v; {
	return v << 4;
}

long shr_const(v) long v; {
	return v >> 8;
}

int word_shl(v, n) int v; int n; {
	return v << n;
}

int word_shr(v, n) int v; int n; {
	return v >> n;
}

main() {
	x = shl(100L, 3);
	x = shr_signed(-1000L, 2);
	x = shr_unsigned(0xFFFF0000, 16);
	x = shl_const(1L);
	x = shr_const(0x12340000);
	word_shl(1, 5);
	word_shr(256, 3);
	return 0;
}
