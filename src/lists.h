#ifndef LISTS_H
#define LISTS_H

//
//  Structure of a string list item
//  

typedef struct item_struct
   {
   int obj;
   char *str;
   struct item_struct *next;
   } 
   Item ;

//
//  Structure of a list
//

typedef struct
   {
   int obj ;
   int id ;
   int n ;
   int sort ;
   Item *first ;
   } 
   List ;

//
//  Function prototypes
//  

List* addlist (List*, char*);
List* catlist (List*, List*);
List* duplist(List *);
List* freelist(List*);
List* intersect(List*,List*);
List* newlist(void);
List* newsequence(void);
char* slprint(List*);
int   ismember(char*, List*);
void  freeitem(List*,char*);

#endif /* LISTS_H */
