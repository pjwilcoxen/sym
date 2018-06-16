#ifndef STR_H
#define STR_H

char *concat(int,char*,...);
char *strlower(char*);

#ifdef __WATCOMC__

#define strcasecmp(a,b) stricmp(a,b)
#define strncasecmp(a,b,n) strnicmp(a,b,n)

#endif /* __WATCOMC__ */

#define isequal(a,b) (strcasecmp(a,b)==0)

#endif /* STR_H */
