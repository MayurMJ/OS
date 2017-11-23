
// TODO: change this function
int strcmp(const char *p, const char *q) {
    while (*p || *q) {
        if (*p != *q)
            return -1;
        p++, q++;
    }
    return 0;
}
