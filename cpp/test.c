add(a, b) int a, b;
{
#ifdef ADD
    return a+b;
#else
    return a-b;
#endif
}

test() {
    add(2, 3);
}
