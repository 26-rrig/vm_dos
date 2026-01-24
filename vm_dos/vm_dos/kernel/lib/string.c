int strcmp(const char *a, const char *b)
{
    while (*a && (*a == *b))
    {
        a++;
        b++;
    }
    return *(unsigned char *)a - *(unsigned char *)b;
}

int strlen(const char *s)
{
    int n = 0;
    while (*s++)
        n++;
    return n;
}
