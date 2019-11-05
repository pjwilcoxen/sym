/* dict.h
 * Nov 19 (PJW)
 *
 * Header file for dictionaries.
 */

#ifndef DICT_H
#define DICT_H

#include "lists.h"

//
//  Function prototypes
//

void* newdict(int size);
void  putdict(void* dict, char* key, void* obj);
void* getdict(void* dict, char* key);
void  dictinfo(void* dict);
List* getkeys(void* dict);

#endif /* DICT_H */

