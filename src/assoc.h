/* assoc.h
 * Dec 04 (PJW)
 *
 * Header file for associative arrays.
 *
 * $Id: assoc.h 49 2018-06-16 17:15:39Z wilcoxen $
 */

#ifndef ASSOC_H
#define ASSOC_H

#include "lists.h"

#define ELEOBJ   1221
#define ARRAYOBJ 1222

typedef struct assoc_element
   {
   int obj;
   char *name;
   void *value;
   struct assoc_element *next;
   }
   Element ;
   
typedef struct assoc_array 
   {
   int obj;
   int id;
   int n;
   Element *first;
   }
   Array ;
  
//
//  Function prototypes
//

Array* newarray();
int    addvalue(Array*, char*, void*);
void*  getvalue(Array*, char*);

#endif /* ASSOC_H */
