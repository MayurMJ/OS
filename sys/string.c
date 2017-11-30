
// TODO: change this function
int kstrcmp(const char *p, const char *q) {
    while (*p || *q) {
        if (*p != *q)
            return -1;
        p++, q++;
    }
    return 0;
}
