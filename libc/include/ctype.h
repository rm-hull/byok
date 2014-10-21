#ifndef _CTYPE_H
#define _CTYPE_H 1

#ifdef __cplusplus
extern "C" {
#endif

#define __INLINE inline static unsigned short

// NOTE: not locale specific
__INLINE isdigit(char c) { return (c >= '0' && c <= '9'); }
__INLINE isupper(char c) { return (c >= 'A' && c <= 'Z'); }
__INLINE islower(char c) { return (c >= 'a' && c <= 'z'); }
__INLINE isalpha(char c) { return isupper(c) || islower(c); }
__INLINE isalnum(char c) { return isupper(c) || islower(c) || isdigit(c); }
__INLINE ispunct(char c) { return !isalnum(c); }
__INLINE isprint(char c) { return (c >= 0x20 && c <= 0x7E); }
__INLINE isgraph(char c) { return (c >= 0x21 && c <= 0x7E); }
__INLINE iscntrl(char c) { return (c >= 0x00 && c <= 0x1F) || c == 0x7F; }
__INLINE isblank(char c) { return (c == 0x20 || c == 0x09); }
__INLINE isspace(char c) { return (c == 0x20 || (c >= 0x09 && c <= 0x0D)); }
__INLINE isxdigit(char c) { return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); }

__INLINE toupper(char c) { return islower(c) ? (c - 0x20) : c; }
__INLINE tolower(char c) { return isupper(c) ? (c + 0x20) : c; }

#ifdef __cplusplus
}
#endif

#endif
