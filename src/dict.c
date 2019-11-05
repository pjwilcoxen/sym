/*-------------------------------------------------------------------*
 *  dict.c
 *  Jun 06 (PJW)
 *
 *  Build and manage a hash table that works more or less like
 *  a Python dictionary.
 *-------------------------------------------------------------------*/

#include "error.h"
#include "assoc.h"
#include "str.h"
#include "xmalloc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OBJ_entry 1272
#define OBJ_table 1273


typedef unsigned long Hashcode;

typedef struct Entry {
   int obj;
   char *key;
   void *object;
   Array *list;
   } Entry;

typedef struct Table
   {
   int obj;
   int size;
   Entry *table;
   int objects;
   int nlists;
   } Table;

//
//  Private methods
//

/*-------------------------------------------------------------------*
 *  hash_code
 *
 *  Loosely derived from lookup2.c by Bob Jenkins.  Maps about 
 *  18,000 IGEM parameters into 11,000 distinct hash codes.
 *-------------------------------------------------------------------*/
static int hash_code( Table *table, char *str )
{
   Hashcode a,b,c,code;
   int slen,i;

   validate( table, OBJ_table, "hash_code" );

   slen = strlen(str);

   a = 0x9e3779b9;
   b = 0;
   c = 0;

   for( i=0 ; i<slen ; i=i+8 )
      {
      b = str[i];
      if( i+1 < slen )b = b<<8 | str[i+1];
      if( i+2 < slen )b = b<<8 | str[i+2];
      if( i+3 < slen )b = b<<8 | str[i+3];
      if( i+4 < slen )c =        str[i+4];
      if( i+5 < slen )c = c<<8 | str[i+5];
      if( i+6 < slen )c = c<<8 | str[i+6];
      if( i+7 < slen )c = c<<8 | str[i+7];
      a = (a-b-c) ^ (c>>13);
      b = (b-c-a) ^ (a<<8 );
      c = (c-a-b) ^ (b>>13);
      a = (a-b-c) ^ (c>>12);
      b = (b-c-a) ^ (a<<16);
      c = (c-a-b) ^ (b>>5 );
      a = (a-b-c) ^ (c>>3 );
      b = (b-c-a) ^ (a<<10);
      c = (c-a-b) ^ (b>>15);
      }

   code = c % table->size;

   return code;
}


/*-------------------------------------------------------------------*
 *  qcmp
 *
 *  Shim for using strcasecmp with qsort in getkeys
 *-------------------------------------------------------------------*/
static int qcmp( const void *v1, const void *v2 )
{
   return strcasecmp( *(char **) v1, *(char **) v2 );
}


//
//  Public methods
//

/*-------------------------------------------------------------------*
 *  newdict
 *-------------------------------------------------------------------*/
void *newdict( int size )
{
   Table *new;
   Entry *table;
   int i;

   new = (Table *) xmalloc( sizeof(Table) );
   new->obj       = OBJ_table;
   new->size      = size;
   new->table     = (Entry *) xmalloc( size*sizeof(Entry) );
   new->objects   = 0;
   new->nlists    = 0;

   table = new->table;
   for( i=0 ; i < size ; i++ )
      {
      table[i].obj     = OBJ_entry;
      table[i].key     = 0;
      table[i].object  = 0;
      table[i].list    = 0;
      }

   return new;
}


/*-------------------------------------------------------------------*
 *  putdict
 *  
 *  hash a string and store the associated value in the table.  
 *  note that collisions cause the value to be converted to -1.
 *-------------------------------------------------------------------*/
void putdict( Table *tab, char *key, void *object )
{
   Hashcode hash;
   char *curkey;
   void *curobj;
   void *list;

//  
//  check for problems
//  

   validate( tab, OBJ_table, "putdict" );
   validate( key, 0 , "putdict" );

   tab->objects++;

//
//  compute the hash code
//

   hash = hash_code(tab,key);
      
//
//  if no object is there, store this one
//

   curkey = tab->table[hash].key;
   curobj = tab->table[hash].object;
   if( curkey == 0 )
      {
      tab->table[hash].key = xstrdup(key);
      tab->table[hash].object = object;
      return;
      }
    
//
//  there was an object; check for a list and build one if necessary
//

   list = tab->table[hash].list;
   if( list == 0 )
      {
      list = newarray();
      addvalue(list,curkey,curobj);
      tab->table[hash].list = list;
      tab->nlists++;
      }

//
//  add this object to the list for this code
//

   addvalue(list,key,object);
}


/*-------------------------------------------------------------------*
 *  getdict
 *  
 *  Find a string in the table.
 *-------------------------------------------------------------------*/
void *getdict( Table *tab, char *key )
{
   Hashcode hash;
   void *list;
   char *curkey;
   void *curobj;

   validate( tab, OBJ_table, "getdict" );
   validate( key, 0 , "getdict" );

//
//  generate the hash code
//  
      
   hash = hash_code(tab,key);

//
//  look for a list
//

   list = tab->table[hash].list;
   if( list )return getvalue(list,key);

//
//  no list; look for a single entry
//

   curkey = tab->table[hash].key;
   if( curkey == 0 )return 0;

//
//  did it match?
//

   curobj = tab->table[hash].object;
   if( strcmp(key,curkey)==0 )return curobj;
   
//
//  no match at all
//

   return 0;
}


/*-------------------------------------------------------------------*
 *  getkeys
 *
 *  Return an alphabetical list of keys in the dictionary. Use
 *  qsort for efficiency but then return them via a sequence for
 *  external convenience.
 *-------------------------------------------------------------------*/
List *getkeys( Table *tab )
{
   List *keys_out;
   Entry cur;
   Element *item;
   int i,k;
   char **keys_vec;

   validate( tab, OBJ_table, "getkeys" );

   keys_out = newsequence();

   //
   //  collect the keys into a vector of pointers
   //

   keys_vec = (char **) xmalloc( tab->objects * sizeof(char *) );

   for( i=0,k=0 ; i < tab->size ; i++ )
      {
      cur = tab->table[i];
      if( cur.list )
         {
         for( item=cur.list->first ; item ; item=item->next )
            keys_vec[k++] = item->name;
         continue;
         }
      if( cur.key )
         keys_vec[k++] = cur.key;
      }

   if( k != tab->objects )
      FAULT("inconsistent number of keys in getkeys");

   //
   //  sort the pointers
   //

   qsort( keys_vec, k, sizeof(char *), qcmp ); 

   //
   //  load them into a list but without resorting
   //

   for( i=0 ; i<k ; i++ )
      addlist(keys_out,keys_vec[i]);

   // 
   //  clean up and return
   //

   xfree( keys_vec );

   return keys_out;
}


/*-------------------------------------------------------------------*
 *  dictinfo
 *-------------------------------------------------------------------*/
void dictinfo( void *table )
{
   Table *tab;
   tab = (Table *) table;
   printf( "Hash size:        %d\n",tab->size    ); 
   printf( "Hash entries:     %d\n",tab->objects ); 
   printf( "Hash collisions:  %d\n",tab->nlists  );
}

