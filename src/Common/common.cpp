#include "common.h"

char* strtok_t(char* instr, char* delimit, char** saveptr)
{
    char* sstr = instr;
    if (sstr == NULL)
    {
        sstr = *saveptr;
        if (sstr == NULL)
            return NULL;
    }
    unsigned char dmap[32];
    memset(dmap, 0, sizeof(dmap));
    while (*delimit)
    {
        unsigned char c = *delimit;
        dmap[c >> 3] |= (1 << (c & 7));
        delimit++;
    }
    char *estr = sstr;
    while (*estr)
    {
        unsigned char c = *estr;
        if (dmap[c >> 3] & (1 << (c & 7)))
        {
            goto FOUND;
        }
        estr++;
    }
FOUND:
    if (*estr != '\0')
    {
        *estr = '\0';
        *saveptr = estr + 1;
    }
    else
    {
        *saveptr = NULL;
    }
    return sstr;
}


char* hs_strncpy(char* dest, const char* src, size_t size)
{
    if (dest == NULL)
        return 0;
    if (src == NULL)
    {
        dest[0] = 0;
        return 0;
    }
    char* ret = strncpy(dest, src, size - 1);
    dest[size - 1] = 0;
    return ret;
}