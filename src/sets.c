/*-------------------------------------------------------------------*
 *  SETS.C 1.00
 *  Mar 04 (PJW)
 *
 *  Manage list of sets
 *-------------------------------------------------------------------*/

#include "sets.h"

#include "error.h"
#include "lists.h"
#include "options.h"
#include "str.h"
#include "sym.h"
#include "symtable.h"
#include "wprint.h"
#include "xmalloc.h"
#include <stdio.h>
#include <string.h>

#define  myDEBUG 0

extern FILE *info;

//====================================================================
//  
//  Private data structures and routines
//  
//====================================================================

struct setinfo
   {
   char *name;
   List *elements;
   int  size;
   int  subset;
   int  imp;
   char *aliasof;
   char *subsetof;
   char *rootset;
   List *immsups;
   struct setinfo *next;
   };

struct setinfo *sethead=0;
struct setinfo *aliases=0;
struct setinfo *subsets=0;

static int issameset(char*,char*);

static int sups_built=0;

/*-------------------------------------------------------------------*
 *  newset
 *
 *  Create a new set and add it to the list.
 *-------------------------------------------------------------------*/
static struct setinfo *newset(char *name, List *ele)
{
   struct setinfo *new,*pre,*cur;
   int check;
   
   validate( ele, LISTOBJ, "newset" );

   new = (struct setinfo *) xmalloc( sizeof(struct setinfo) );
   new->name     = name;
   new->elements = ele;
   new->size     = ele->n;
   new->subset   = 0;
   new->imp      = 0;
   new->aliasof  = 0;
   new->subsetof = 0;
   new->rootset  = 0;
   new->immsups  = 0;
   new->next     = 0;

   //  first set of all?
   
   if( sethead==0 )
      {
      sethead = new;
      return new;
      }
   
   //  before first in the list?
   
   check = strcasecmp(name,sethead->name);
   if( check==0 )FAULT("Duplicate set name");
   if( check <0 )
      {
      new->next = sethead;
      sethead = new;
      return new;
      }
   
   //  before some other set?
   
   for( pre=sethead ; pre->next ; pre=pre->next )
      {
      cur = pre->next;
      check = strcasecmp(name,cur->name);
      if( check==0 )FAULT("Duplicate set name");
      if( check <0 )
         {
         new->next = cur;
         pre->next = new;
         return new;
         }
      }

   //  at the end

   pre->next = new;
   return new;
   }
   
   
/*-------------------------------------------------------------------*
 *  lookupset
 *
 *  Look up a set in the list by name and return the set or null.
 *-------------------------------------------------------------------*/
static struct setinfo *lookupset(char *name)
{
   struct setinfo *cur;
   for( cur=sethead ; cur ; cur=cur->next )
      if( isequal(name,cur->name) )
         return cur;
   return 0;
}


/*-------------------------------------------------------------------*
 *  findset
 *
 *  Look up a set in the list by name. Abort if the set doesn't 
 *  exist.
 *-------------------------------------------------------------------*/
struct setinfo *findset(char *name)
{
   struct setinfo *cur;
   cur = lookupset(name);
   if(cur==0)
      fatal_error("Set '%s' does not exist",name);
   return cur;
}


/*-------------------------------------------------------------------*
 *  build_time_sets
 *
 *  Create time subsets when needed.
 *-------------------------------------------------------------------*/
static void build_time_sets()
{
   List *timevals,*timesets,*gettimesets();
   List *newset;
   Item *ele,*cur;
   void *time;
   char *name,desc[40];
   int  check,off_start,off_end,first,last,i;
      
   
   if( is_explicit_time()==0 )return;
   
   // 
   // find the time set
   // 
   
   time = lookup("time");
   if( time==0 )
      {
      error_back("Explicit set 'time' is required.");
      return;
      }
   validatetype( time, set, "build_time_sets" );

   // 
   // what subsets do we need to build?  cross 'first' or 'last' off the list
   // if they were declared explicitly.  don't have to any error checking
   // because the syntax of sym files and the code in decset will ensure that
   // any declarations of first or last will be singleton sets that pick out
   // the correct elements of time.
   // 

   timesets = gettimesets(); 
   if( timesets==0 )return;

   if( ismember("first",timesets) && lookup("first") )freeitem(timesets,"first");
   if( ismember("last", timesets) && lookup("last" ) )freeitem(timesets,"last" );

   if( timesets->n == 0 )
      {
      freelist(timesets);
      return;
      }

   // 
   // get the elements of time
   // 

   timevals = symvalue(time);
   
   // 
   // build first and last, if needed
   // 
   
   if( ismember("first",timesets) )
      {
      newset = addlist( newsequence(), timevals->first->str );
      symdeclare(set,"first",newset,"First time period",0);
      newset = freelist( newset );
      freeitem(timesets,"first");
      }

   if( ismember("last",timesets) )
      {
      for( ele=timevals->first ; ele->next ; ele=ele->next );
      newset = addlist( newsequence(), ele->str );
      symdeclare(set,"last",newset,"Last time period",0);
      newset = freelist( newset );
      freeitem(timesets,"last");
      }

   if( timesets->n == 0 )
      {
      freelist(timesets);
      freelist(timevals);
      return;
      }

   // 
   // anything left should be sets of the form 'time_p#m#'.  go through 
   // them one by one.
   // 

   for( ele=timesets->first ; ele ; ele=ele->next )
      {
      name  = ele->str;
      check = sscanf(name,"time_p%dm%d",&off_start,&off_end);
      if( check != 2 )FAULT("Improper time subset name");
      
      first = off_start;
      last  = timevals->n - 1 - off_end; 

      if( first > last )
         error_front("Not enough elements in time to build set %s",name);

      newset = newsequence();

      for( i=0, cur=timevals->first ; i <= last ; i++, cur=cur->next )
         if( i >= first )addlist(newset,cur->str);
         
      if( off_start    && off_end==0 )
         sprintf(desc,"where lag=%d exists",off_start);
      if( off_start==0 && off_end    )
         sprintf(desc,"where lead=%d exists",off_end);
      if( off_start    && off_end    )
         sprintf(desc,"where lag=%d and lead=%d both exist",off_start,off_end);
      if( off_start==0 && off_end==0 )
         FAULT("Implicit time subset is improper");
         
      symdeclare(set,name,newset,desc,0);
      setsubset(name,"time");

      newset = freelist( newset );
      }

   freelist( timesets );
   freelist( timevals );
}


/*-------------------------------------------------------------------*
 *  build_immediate_sups
 *
 *  For each set, build a list of the supersets one tier above.
 *  For example, if A is a subset of B and C, and B is a subset 
 *  of D, A's immediate subset list will be [B,C] but won't 
 *  include D. Convenient for avoiding redundancy in subset
 *  declarations for target languages. 
 *-------------------------------------------------------------------*/
static void build_immediate_sups() 
{
   struct setinfo *cur_set,*alt_set;
   Item *cur,*other;
   List *all,*sel;
   int keep_cur;

   if( sups_built > 0 )
      FAULT("attempted rebuild of immediate sups"); 

   //
   //  find root set for every set in the list by following
   //  explicit subset declarations backward
   //

   for( cur_set=sethead ; cur_set ; cur_set=cur_set->next )
      cur_set->rootset = findbase( cur_set->name );

   //
   //  walk through all the sets again to set their 
   //  superset lists
   //

   for( cur_set=sethead ; cur_set ; cur_set=cur_set->next )
      {

      //  find all supersets of the current set

      all = newlist();
      for( alt_set=sethead ; alt_set ; alt_set=alt_set->next )
         if( strcmp(alt_set->name,cur_set->name) != 0 )
            if( strcmp(alt_set->rootset,cur_set->rootset) == 0 )
               if( issubset(cur_set->name,alt_set->name) )
                  addlist( all, alt_set->name );

      //  trim out those that are supersets of other sets
      //  in the list to avoid redundant declarations

      sel = newlist();
      if( all->n )
         for( cur=all->first ; cur ; cur=cur->next )
            {
            keep_cur = 1;
            for( other=all->first ; other ; other=other->next )
               if( issubset(other->str,cur->str) )
                  keep_cur = 0;
            if( keep_cur )
               addlist( sel, cur->str );
            }
            
      //  attach it to the right place in the master set list and 
      //  tidy up
      
      cur_set->immsups = sel;
      freelist(all);
      }

   // 
   //  set the flag indicating that the relationships have been built
   //

   sups_built = 1;
}

//====================================================================
//  
//  Public routines
//  
//====================================================================


/*-------------------------------------------------------------------*
 *  build_set_relationships
 *
 *  Identify the sets which have no supersets and build the list of 
 *  implicit sets from the corresponding elements.  Also, build time
 *  subsets, if needed.
 *-------------------------------------------------------------------*/
void build_set_relationships()
{
   void *cur;
   List *tmplist,*implist,*timesets,*gettimesets();
   Item *ele;
   int count;
   struct setinfo *this,*that,*a,*s,*sub;
   
   //
   //  build implicit time sets, if needed
   //

   build_time_sets();

   //  
   //  add all the explicitly declared sets to the list
   //  
   
   for( cur=firstsymbol(set) ; cur ; cur=nextsymbol(cur) )
      newset( symname(cur), symvalue(cur) );      
      
   //
   //  incorporate information about aliases
   //

   for( a=aliases ; a ; a=a->next )
      for( s=sethead ; s ; s=s->next )
         if( isequal(a->name,s->name) )
            {
            s->subset   = 1;
            s->aliasof  = a->aliasof;
            s->subsetof = a->aliasof; 
            }
   
   //
   //  incorporate information about subsets
   //

   for( sub=subsets ; sub ; sub=sub->next )
      for( s=sethead ; s ; s=s->next )
         if( isequal(sub->name,s->name) )
            {
            s->subset   = 1;
            s->subsetof = sub->subsetof;
            }
   
   // 
   //  identical sets are not allowed except as aliases; enforce 
   //  that rule now
   //
      
   for( this=sethead ; this ; this=this->next )
      for( that=this->next ; that ; that=that->next )
         {
         if( this->aliasof || that->aliasof   )continue;
         if( issameset(this->name,that->name) )
            error_front("Set '%s' is identical to an earlier set",that->name);
         }
               
   //  
   //  set subset flag for proper subsets 
   //  

   for( this=sethead ; this ; this=this->next )
      for( that=this->next ; that ; that=that->next )
         {
         if( this->aliasof || issubset(this->name,that->name) )this->subset = 1;
         if( that->aliasof || issubset(that->name,this->name) )that->subset = 1;
         }

   //
   //  for the time being, disallow any explicit subsets of time other than 
   //  first and last; enforce the rule now
   //

   timesets = gettimesets();
   if( lookup("time") )
      for( this=sethead ; this ; this=this->next )
         {
         if( timesets && ismember(this->name,timesets) )continue;
         if( isequal(this->name,"first") || isequal(this->name,"last") )continue;
         if( issubset(this->name,"time") || isaliasof(this->name,"time") )
            error_front("Set '%s' is a subset of time but explicit time subsets are not allowed yet.",this->name);
         }
   timesets = freelist(timesets);

   //  
   //  build a list of all element names
   //  
 
   tmplist = newlist();

   for( this=sethead ; this ; this=this->next )
      if( this->subset == 0 )
         catlist( tmplist, this->elements );

   //
   //  check for name conflicts between sets and elements, and 
   //  weed out elements that appear in multiple top sets from 
   //  the implicit set list
   //  

   implist = newlist();

   for( ele=tmplist->first ; ele ; ele=ele->next )
      {
      count = 0;
      for( this=sethead ; this ; this=this->next )
         {
         if( isequal(this->name,ele->str) )
            fatal_error("Name used for set and element: %s\n",ele->str);
         if( this->subset )
            continue;
         if( ismember(ele->str,this->elements) )
            count++;
         }
      if( count == 1 )
         addlist( implist, ele->str );
      }

   freelist(tmplist);
   
   //
   //  create the implicit sets
   //

   for( ele=implist->first ; ele ; ele=ele->next )
      {
      tmplist = newlist();
      addlist( tmplist, ele->str );
      this = newset( strdup(ele->str), tmplist );
      this->subset = 1;
      this->imp    = 1;
      }

   freelist(implist);
}


/*-------------------------------------------------------------------*
 *  listelements
 *
 *  Print a list of unique elements
 *-------------------------------------------------------------------*/
void listelements()
{
   struct setinfo *this;
   void *wp;
   List *ele;

   if( sethead==0 )return;
   
   fprintf(info,"\nUnique Elements:\n\n");

   wp = new_wprint(info,72);
   wprint_start(wp,"   ");

   ele = newlist();
   for( this=sethead ; this ; this=this->next )
      if( this->imp )addlist(ele,this->name);

   wprint_list(wp,ele,",");

   if( ele->n==0 )wprint_add(wp,"none");

   wprint_end(wp);

   freelist(ele);
   free_wprint(wp);
}


/*-------------------------------------------------------------------*
 *  setelements()
 *
 *  Return a duplicate of the list of the set's elements.
 *-------------------------------------------------------------------*/
List *setelements( char *name )
{
   return duplist( findset(name)->elements );
}


/*-------------------------------------------------------------------*
 *  setsize
 *
 *  Return the number of elements in the set with the given name
 *-------------------------------------------------------------------*/
int setsize( char *name )
{
   return findset(name)->size;
}


/*-------------------------------------------------------------------*
 *  setindex
 *
 *  Return the index of a given element within the set.  Index is
 *  based at 0.  Setindex aborts if the element does not occur in
 *  the set.
 *-------------------------------------------------------------------*/
int setindex( char *setname, char *element )
{
   List *elist;
   int n;

   elist = findset(setname)->elements;
   n = ismember(element,elist);
   
   if( n==0 )
      FAULT("Invalid subscript in setindex");

   return n-1;
}


/*-------------------------------------------------------------------*
 *  issubset
 *
 *  Is the first argument a proper subset or an alias of the second?
 *-------------------------------------------------------------------*/
int issubset(char *small, char *large)
{
   struct setinfo *sm;
   List *sm_set,*lg_set;
   Item *sm_ele;
   int result;

   sm = lookupset(small);
   if(sm==0)
      fatal_error("Element is a member of multiple master sets: %s\n",small);

   if( sm->aliasof && isequal(sm->aliasof,large) )
      return 1;
      
   sm_set = sm->elements;
   lg_set = findset(large)->elements;
   
   if( sm_set->n >= lg_set->n )return 0;

   result = 1;
   for( sm_ele = sm_set->first ; sm_ele ; sm_ele=sm_ele->next )
      if( ismember( sm_ele->str, lg_set )==0 )
         result = 0;

   return result;
}


/*-------------------------------------------------------------------*
 *  issameset
 *
 *  Do these sets have identical elements?  Note that the comparison
 *  is invariant to the order of the elements.
 *-------------------------------------------------------------------*/
static int issameset(one,two)
char *one,*two;
{
   struct setinfo *s1,*s2;
   List *alpha1,*alpha2;
   Item *e1,*e2;
   
   s1 = findset(one);
   s2 = findset(two);
   
   if( s1->size != s2->size )return 0;

   alpha1 = newlist();
   alpha2 = newlist();

   catlist( alpha1, s1->elements );
   catlist( alpha2, s2->elements );

   e1 = alpha1->first;
   e2 = alpha2->first;

   while( e1 )
      {
      if( ! isequal(e1->str,e2->str) )
         {
         freelist( alpha1 );
         freelist( alpha2 );
         return 0;
         }
      e1 = e1->next;
      e2 = e2->next;
      }

   freelist( alpha1 );
   freelist( alpha2 );
   return 1;
}


/*-------------------------------------------------------------------*
 *  setalias
 *
 *  Record the fact that one name is an alias of another set.
 *-------------------------------------------------------------------*/
void setalias(char *alias, char *aliasof )
{
   struct setinfo *new,*last;
   
   new = (struct setinfo *) xmalloc( sizeof(struct setinfo) );
   new->name    = strdup(alias  );
   new->aliasof = strdup(aliasof);
   new->next    = 0;
   
   if( aliases==0 )
      {
      aliases = new;
      return;
      }
      
   for( last=aliases ; last->next ; last=last->next );
   last->next = new;
}


/*-------------------------------------------------------------------*
 *  isaliasof
 *
 *  Is the first argument and alias of the second?
 *-------------------------------------------------------------------*/
int isaliasof(char *set, char *aliasof )
{
   char *set_aliasof;
   set_aliasof = findset(set)->aliasof;
   if( set_aliasof && isequal(set_aliasof,aliasof) )return 1;
   return 0;
}


/*-------------------------------------------------------------------*
 *  isimplicit
 *
 *  Is this an implicit set?
 *-------------------------------------------------------------------*/
int isimplicit(char *setname)
{
   return findset(setname)->imp;
}


/*-------------------------------------------------------------------*
 *  setsubset
 *
 *  Record the fact that one set is a proper subset of another.
 *  Needed because the information has to be recorded before the
 *  actual set objects have been built, which is not done until
 *  build_set_relationships is called.
 *-------------------------------------------------------------------*/
void setsubset(char *subname, char *supname )
{
   struct setinfo *new,*last;
   
   new = (struct setinfo *) xmalloc( sizeof(struct setinfo) );
   new->name     = strdup(subname);
   new->subsetof = strdup(supname);
   new->next     = 0;
 
   if( subsets==0 )
      {
      subsets = new;
      return;
      }

   for( last=subsets ; last->next ; last=last->next );
   last->next = new;
}


/*-------------------------------------------------------------------*
 *  getsupset
 *
 *  Find the immediate superset of the current set.
 *-------------------------------------------------------------------*/
char *getsupset(char *subname)
{
   struct setinfo *s;
   s = findset(subname);
   if( s == 0 || s->subsetof == 0)return 0;
   return strdup(s->subsetof);
}


/*-------------------------------------------------------------------*
 *  findbase
 *
 *  Find the ultimate superset set of a given set by following 
 *  subsetof statements upward until reaching a set that isn't 
 *  a subset. Needed to prevent potential problems when inferring
 *  other subset relationships in cases where multiple root sets 
 *  use elements that have the same names.
 *-------------------------------------------------------------------*/
char *findbase(char *setname) 
{
   struct setinfo *s;

   validate( setname, 0, "findbase" );
   
   s = findset(setname);
   if( s==0 )
      FAULT("failed to find set in findbase");
   
   if( s->subsetof == 0 )
      return( strdup(s->name) );
   
   return findbase( s->subsetof );
}


/*-------------------------------------------------------------------*
 *  find_immediate_sups
 *
 *  Return the immediate supersets of the current set.
 *-------------------------------------------------------------------*/
List *find_immediate_sups(char *setname) 
{
   struct setinfo *s;

   validate( setname, 0, "find_immediate_sups" );
   
   if( sups_built == 0 )
      build_immediate_sups();
   
   s = findset(setname);
   if( s==0 )
      FAULT("could not find set in find_immediate_sets");
   if( s->immsups == 0 )
      FAULT("no immediate sup list");
   
   return( s->immsups );
}
