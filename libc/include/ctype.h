#ifndef _CTYPE_H
#define _CTYPE_H 1

#ifdef __cplusplus
extern "C" {
#endif

inline static int isspace(char c)
{
    return c == 0x20 || (c >= 0x09 && c <= 0x0D);
}

inline static int isdigit(char c)
{
    return c >= '0' && c <= '9';
}

#ifdef __cplusplus
}
#endif

#endif
