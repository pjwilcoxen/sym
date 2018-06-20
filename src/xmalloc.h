/*--------------------------------------------------------------------*
 *  xmalloc.h
 *
 *  $Id: xmalloc.h 49 2018-06-16 17:15:39Z wilcoxen $
 *--------------------------------------------------------------------*/

#ifndef XMALLOC_H
#define XMALLOC_H

char* xstrdup(char*);
long  xmark();
void  xcheck(char*);
void  xfree(void*);
void  xleak(long,char*);
void* xmalloc(int);

#endif /* XMALLOC_H */
