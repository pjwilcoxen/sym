/*-------------------------------------------------------------------*
 *  EQNS.C 1.00
 *  Mar 04 (PJW)
 *
 *  Manage list of equations
 *-------------------------------------------------------------------*/

#include "eqns.h"

#include "error.h"
#include "lists.h"
#include "nodes.h"
#include "options.h"
#include "sets.h"
#include "spprint.h"
#include "str.h"
#include "sym.h"
#include "symtable.h"
#include "xmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOTSET 12345
#define EQSIG 1812
#define myDEBUG 1

extern FILE *info;

struct equation
   {
   int obj;
   int n;
   char *name;
   Node *qual;
   Node *eq;
   Node *lhs;
   Node *rhs;
   List *qsets;
   List *domain;
   int count;
   int lvalue;
   List *attr;
   int min_dt;
   int max_dt;
   int timeok;
   struct equation *next;
   };

typedef struct equation Equation;

static Equation *first=0;
static int neqn = 0;
static int min_dt=0,max_dt=0;

static int node_lvalue(Node *);

List *refinesets(List*,List*,int);
List *conform(Node*);
List *timesets=0;


/*-------------------------------------------------------------------*
 *  neweqn
 *-------------------------------------------------------------------*/
void neweqn(qual,lhs,rhs,attr,name)
Node *qual,*lhs,*rhs,*attr,*name;
{
   Equation *eq,*new;
   void resetlastnode();

   new = (Equation *) xmalloc( sizeof(Equation) );
   new->obj    = EQSIG;
   new->name   = 0;
   new->qual   = qual;
   new->eq     = newnode(equ,"=",lhs,rhs);
   new->lhs    = lhs;
   new->rhs    = rhs;
   new->qsets  = newlist();
   new->domain = newlist();
   new->lvalue = 0;
   new->attr   = newlist();
   new->min_dt = NOTSET;
   new->max_dt = NOTSET;
   new->next   = 0;
   new->timeok = 1;
   new->n      = ++neqn;

   for( ; qual ; qual=qual->r )
      if( qual->type == nam || qual->type == num )
         addlist( new->qsets, qual->str );

   for( ; attr ; attr=attr->r )
      if( attr->type == nam || attr->type == num )
         addlist( new->attr, attr->str );

   if( first )
      {
      for( eq = first ; eq->next ; eq = eq->next );
      eq->next = new;
      }
   else
      first = new ; 

   if( name )
      {
      validate( name, NODEOBJ, "neweqn" );
      new->name = strdup( name->str );
      }

   resetlastnode();
}


/*-------------------------------------------------------------------*
 *  firsteqn
 *-------------------------------------------------------------------*/
void *firsteqn()
{
   validate( first, EQSIG, "firsteqn");
   return first; 
}


/*-------------------------------------------------------------------*
 *  nexteqn
 *-------------------------------------------------------------------*/
void *nexteqn(void *vcur)
{
   if( vcur==0 )return 0;
   validate( vcur, EQSIG, "getnode");
   return ((Equation *) vcur)->next;
}


/*-------------------------------------------------------------------*
 *  getnode
 *-------------------------------------------------------------------*/
Node *getnode(void *vcur)
{
   validate( vcur, EQSIG, "getnode");
   return ((Equation *) vcur)->eq;
}


/*-------------------------------------------------------------------*
 *  getlhs
 *-------------------------------------------------------------------*/
Node *getlhs(void *vcur)
{
   validate( vcur, EQSIG, "getlhs");
   return ((Equation *) vcur)->lhs;
}


/*-------------------------------------------------------------------*
 *  getrhs
 *-------------------------------------------------------------------*/
Node *getrhs(void *vcur)
{
   validate( vcur, EQSIG, "getrhs");
   return ((Equation *) vcur)->rhs;
}


/*-------------------------------------------------------------------*
 *  eqname
 *-------------------------------------------------------------------*/
char *eqname(void *vcur)
{
   validate( vcur, EQSIG, "eqnsets" );
   return ((Equation *) vcur)->name;
}


/*-------------------------------------------------------------------*
 *  eqnsets
 *-------------------------------------------------------------------*/
List *eqnsets(void *vcur)
{
   validate( vcur, EQSIG, "eqnsets" );
   return ((Equation *) vcur)->domain;
}


/*-------------------------------------------------------------------*
 *  eqncount
 *-------------------------------------------------------------------*/
int eqncount(void *vcur)
{
   validate( vcur, EQSIG, "eqncount");
   return ((Equation *) vcur)->count;
}


/*-------------------------------------------------------------------*
 *  iseqnattr
 *-------------------------------------------------------------------*/
int iseqnattr(void *vcur, char *attr)
{
   validate( vcur, EQSIG, "iseqnattr");
   return ismember( attr, ((Equation *) vcur)->attr );
}


/*-------------------------------------------------------------------*
 *  num_eqns
 *-------------------------------------------------------------------*/
int num_eqns(void)
{
   return neqn ;
}


/*-------------------------------------------------------------------*
 *  islvalue
 *
 *  Figure out if an equation is in the form lvalue = something.
 *  An lvalue is a name or a lead() or lag() applied to a name (or 
 *  another lead or lag). 
 *-------------------------------------------------------------------*/
int islvalue(void *vcur)
{
   validate( vcur, EQSIG, "islvalue");
   return node_lvalue( ((Equation *) vcur)->lhs );
}

static int node_lvalue(Node *cur)
{
  switch( cur->type )
      {
      case nam: 
         return 1;

      case lag:
      case led: 
         return node_lvalue( cur->r );
      }
   return 0;
}


/*-------------------------------------------------------------------*
 *  hasundec
 *
 *  Say whether an equation has undeclared variables.
 *-------------------------------------------------------------------*/
int hasundec(void *vcur)
{
   validate( vcur, EQSIG, "iseqnattr");
   return ((Equation *) vcur)->eq->undec;
}


/*-------------------------------------------------------------------*
 *  istimeok()
 *-------------------------------------------------------------------*/
int istimeok(void *vcur)
{
   validate( vcur, EQSIG, "istimeok");
   return ((Equation *) vcur)->timeok ;
}


/*-------------------------------------------------------------------*
 *  istimeset()
 *-------------------------------------------------------------------*/
int istimeset(char *str)
{
   validate( str, 0, "istimeset");
   if( timesets==0 )return 0;
   if( isequal(str,"time") )return 1;
   return ismember(str,timesets);
}


/*-------------------------------------------------------------------*
 *  eqcontext
 *
 *  Recursively descend through an equation and set the context
 *  variables for each node.  Also adjust the equation's min_dt
 *  and max_dt attributes accordingly.
 *-------------------------------------------------------------------*/
static void eqcontext( Equation *eq, Node *cur, int lhs, int dt )
{
   if( cur==0 )return;

   cur->lhs = lhs;
   cur->dt  = dt;

   switch( cur->type )
      {
      case nam:
         if( eq->min_dt == NOTSET )eq->min_dt = dt;
         if( eq->max_dt == NOTSET )eq->max_dt = dt;
         if( dt < eq->min_dt )eq->min_dt = dt;
         if( dt > eq->max_dt )eq->max_dt = dt;
         return;
         
      case equ:
         eqcontext( eq, cur->l, 1, dt );
         eqcontext( eq, cur->r, 0, dt );
         return;
         
      case lag:
         eqcontext( eq, cur->r, lhs, dt-1 );
         return;

      case led:
         eqcontext( eq, cur->r, lhs, dt+1 );
         return;

      case sum:
      case prd:
         eqcontext( eq, cur->r, lhs, dt );
         return;

      case dom:
         eqcontext( eq, cur->l, lhs, dt );
         return;

      case lst:
         return;
         
      default:
         eqcontext( eq, cur->l, lhs, dt );
         eqcontext( eq, cur->r, lhs, dt );
      }
}


/*-------------------------------------------------------------------*
 *  build_context
 *
 *  Build the context information for all equations.  In the
 *  process, set the intertemporal flag.
 *-------------------------------------------------------------------*/
void build_context(void)
{
   Equation *eq;
   Item *eqs;
   int has_first, has_last, has_time;
   int has_timequal;
   int this_min_dt;
   int this_max_dt;
   char buf[30];
   int exptime;

   exptime = is_explicit_time();
   
   if( exptime )
      timesets = newlist();

   for( eq = first ; eq ; eq = eq->next )
      {

      // find any explicit time qualifiers
      
      has_first = 0;
      has_last  = 0;
      has_time  = 0;
      
      for( eqs=eq->qsets->first ; eqs ; eqs=eqs->next )
         {
         if( isequal(eqs->str,"first") )has_first = 1;  
         if( isequal(eqs->str,"last" ) )has_last  = 1;  
         if( isequal(eqs->str,"time" ) )has_time  = 1;  
         }

      // add first or last to timesets

      if( timesets )
         {
         if( has_first )addlist( timesets, "first" );
         if( has_last  )addlist( timesets, "last"  );
         }
         
      has_timequal = has_first || has_last || has_time ;
      if( has_timequal )intertemporal = 1;

      // build context and check for leads and lags
      
      eqcontext( eq, eq->eq, 0, 0 );

      this_min_dt = eq->min_dt;
      this_max_dt = eq->max_dt;
      
      if( this_min_dt || this_max_dt )
         {
         intertemporal = 1;
         if( this_min_dt < min_dt )min_dt = this_min_dt;
         if( this_max_dt > max_dt )max_dt = this_max_dt;
         }

      // check for inconsistencies between explicit qualifiers
      // and leads and lags.  do this first so we can flag any 
      // problems as front end errors before worrying about the
      // rules of the target language.
      
      if( has_timequal )
         {
         if( has_first && this_min_dt )
            error_front("Equation qualified by first contains a lag");
         
         if( has_last && this_max_dt )
            error_front("Equation qualified by last contains a lead");
         
         if( has_time && (this_min_dt || this_max_dt) )
            error_front("Equation qualified by time contains a lead or a lag");
         }
      
      // we now know that any time qualifier is consistent with the leads
      // and lags in this equation.  figure out what to do if the target
      // language doesn't use an explicit time set.

      if( exptime==0 )
         {
         if( has_timequal==0 )continue;
         if( has_timequal && this_min_dt==0 && this_max_dt==0 )continue;
         FAULT("unfinished code for time subsets with exptime==0 targets");
         }
         
      // ok, the target language uses an explicit time set. if a time 
      // qualifier was given with this equation, we're done because
      // we've already made sure the qualifier is consistent with the
      // lead and lag structure
     
      if( has_timequal )
         continue;

      // no time qualifier was given; however, if there are no lead or 
      // lags, we're done because 'time' itself will be generated 
      // automatically via standard conformability calculations since
      // variables will be subscripted by time.
      
      if( this_min_dt==0 && this_max_dt==0 )
         continue;
         
      // need a special time subset; figure out what set should be used 
      // with this equation based on the lead and lag structure.  then 
      // make sure the set is included in the master list and then add 
      // the qualifier to this equation.
      
      sprintf(buf,"time_p%dm%d",-this_min_dt,this_max_dt);
      addlist( timesets  , buf );
      addlist( eq->qsets , buf );
      }

   if( intertemporal==0 )
      return;

   fprintf(info,"Intertemporal Details:\n\n");
   fprintf(info,"   Longest lag is %d; longest lead is %d.\n",min_dt,max_dt);

   if( exptime==0 )
      return;

   fprintf(info,"   Target language requires time sets.\n");

   if( timesets )
      fprintf(info,"   Time subsets used: %s.\n",slprint(timesets));
      
}     


/*-------------------------------------------------------------------*
 *  findundeclared
 *
 *  Recursively descend through an equation finding all undeclared
 *  identifiers (parameters and variables).  Along the way, set
 *  the undec flag on each node and mark symbols as being used.
 *-------------------------------------------------------------------*/
static void findundeclared( List *undec, Node *cur, int eq_num, int is_lhs )
{
   void *sym;
   void setused();
   
   if( cur==0 )return;

   validate( undec, LISTOBJ, "findundeclared" );
   validate( cur  , NODEOBJ, "findundeclared" );

   switch( cur->type )
      {
      case num:
      case lst:
         break;

      //
      //  got a name; does it exist?
      //

      case nam:
         sym = lookup(cur->str) ;
         if( isident(sym)==0 )
            {
            addlist(undec,cur->str);
            cur->undec = 1;
            }
         else
            setused(sym, eq_num, is_lhs);
         break;

      // 
      //  the left branch is a set name so don't check it
      //

      case sum:
      case prd:
         findundeclared( undec, cur->r, eq_num, is_lhs );
         cur->undec = cur->r->undec;
         break;

      //
      //  right branch is a set list so don't check it
      //

      case dom:
         findundeclared( undec, cur->l, eq_num, is_lhs );
         cur->undec = cur->l->undec;
         break;

      //
      //  set the lhs flag for equation nodes and then descend
      //

      case equ:
         findundeclared( undec, cur->l, eq_num, 1 );
         findundeclared( undec, cur->r, eq_num, 0 );
         if( cur->l && cur->l->undec )cur->undec = 1;
         if( cur->r && cur->r->undec )cur->undec = 1;
         break;
         
      //
      //  descend both branches of all other nodes
      //
      
      default:
         findundeclared( undec, cur->l, eq_num, is_lhs );
         findundeclared( undec, cur->r, eq_num, is_lhs );
         if( cur->l && cur->l->undec )cur->undec = 1;
         if( cur->r && cur->r->undec )cur->undec = 1;
      }

   return;
}


/*-------------------------------------------------------------------*
 *  build_domain()
 *
 *  Recursively build the domain of a given node from the 
 *  domains of its left and right nodes.
 *
 *  Arguments are used as follows:
 *
 *     qual
 *        List of qualifiers restricting the domain in a particular
 *        context.  Initially the equation qualifiers but may be
 *        augmented inside sums and products when the operation is being
 *        carried out over a subset.  Eg, if variable X is defined over
 *        set A and a sum is taken over a subset B, such as sum(B,X),
 *        qual will include B when examining node X inside the sum.
 *        
 *     cur 
 *        Node whose domain is to be set.
 *
 *  The node's domain attribute is used for two purposes: 
 *
 *     (1) The equation's top level domain is the domain over which 
 *         the equation will be generated.
 *
 *     (2) An identifier's domain is used in show_symbol find the 
 *         subscripts relevant to the corresponding identifier when 
 *         a scalar equation is being printed.
 *  
 *  For purpose (2), the domain lists for 'nam' nodes must be in the
 *  same order as the sets appear in the variable's declaration.  All
 *  other domain lists are in alphabetical order for convenience in 
 *  comparing them.
 *-------------------------------------------------------------------*/
static int build_domain( List *qual, Node *cur )
{
   void *sym;
   List *varsets;
   Node *item,*left,*right;
   Nodetype type;
   List *explsets,*refsets;
   List *qualsets;
   char *indexset; 
   List *innerlist,*submatch; 
   Item *ele;
   int exact,numsubset;

   validate( qual, LISTOBJ, "build_domain (qual)" );
   validate( cur,  NODEOBJ, "build_domain (node)" );

   type  = cur->type;
   left  = cur->l;
   right = cur->r;
   
   // 
   //  discard the existing list if it has already been built.  this will
   //  occur when build_domain is being rerun to adjust the lower node set lists
   //  to: (1) propagate 'subset' down to node X in an expression like
   //  this 'sum(subset,X)' where X is defined over a larger set; or (2) 
   //  to propagate aliases from one side of the equation to the other, such
   //  as when 'X=Y' appears and the domain of Y is an alias of the domain 
   //  of X (example: X(regions) = Y(to)).
   //
   
   if( cur->domain )
      {
      validate( cur->domain, LISTOBJ, "build_domain" );
      cur->domain = freelist( cur->domain );
      }

   //
   //  abort if this node or those below it contain undeclared 
   //  identifiers (the undec flag has already been built)
   //

   if( cur->undec )return 0;
      
   //
   //  If this is a name, it must be a variable or parameter.
   //  Grab its information and stop descending.
   //

   if( type == nam )
      {

      sym = lookup( cur->str );

      if( isident(sym)==0 )
         {
         if( DBG )printf("undeclared symbol: %s\n",cur->str);
         return 0;
         }

      //
      //  get the identifier's declared domain
      //
      
      varsets  = symvalue(sym) ;
       
      //
      //  apply the equation-wide qualifier
      //

      qualsets = refinesets(varsets,qual,0);
      varsets  = freelist( varsets );

      //
      //  build a list of restrictions mentioned explicitly with the variable
      //
      
      explsets = newlist();
      item = right;
      if( item && item->type == lst )
         for( item=item->r ; item && (item->type == nam || item->type == num) ; item=item->r )
            addlist(explsets,item->str);

      //
      //  combine the lists
      //

      refsets  = refinesets(qualsets,explsets,1);
      explsets = freelist( explsets );

      if( refsets == 0 )
         {
         error_front("Subscripts not compatible with variable declaration");
         error_more ("Declaration: %s(%s)\n",cur->str,slprint(symvalue(sym)));
         error_more ("Usage: %s\n",node2string(cur));
         exit(0);
         }
               
      cur->domain = refsets;
      
      if( DBG )
         {
         printf("symbol %s ",cur->str);
         printf("domain in current context: %s\n",slprint(cur->domain));
         }

      return 1;
      }

   //
   //  If this node is a domain extension, expand the domain accordingly
   //
   
   if( type == dom )
      {
      
      if( build_domain( qual, left )==0 )return 0;
      validate( left->domain, LISTOBJ, "build_domain sum" );

      varsets = newlist();
      for( item=right ; item ; item=item->r )
         addlist( varsets, item->str );

      qualsets = refinesets(varsets,qual,0);
      varsets  = freelist( varsets );

      cur->domain = catlist( newlist(), left->domain );
      catlist( cur->domain, qualsets );

      qualsets = freelist( qualsets );

      if( DBG )
         {
         printf("domain expansion\n");
         printf("   from: %s\n",slprint(cur->l->domain));
         printf("   to: %s\n",slprint(cur->domain));
         }

      return 1;
      }
      
   //
   //  If this node is a sum or product, need to deduct out the set over
   //  which the sum is being taken.
   //
   
   if( type == sum || type == prd )
      {

      if( build_domain( qual, right )==0 )return 0;

      validate( right->domain, LISTOBJ, "build_domain sum" );

      // 
      //  get the inner list and make sure it is alphabetical
      //

      innerlist = catlist( newlist(), right->domain );
      indexset  = left->str;

      //
      //  figure out how many of the inner sets are matched by the index set;
      //  hopefully the answer is 1.  note that because set lists are sorted,
      //  there will never be more than one inner set exactly matching the
      //  index set.
      //
      
      exact = 0;
      submatch = newlist();
      for( ele=innerlist->first ; ele ; ele=ele->next )
         {
         if( isequal(indexset,ele->str) )
            {
            exact = 1;
            continue;
            }
         if( issubset(indexset,ele->str) )
            addlist( submatch, ele->str );
         }
      numsubset = submatch->n;

      if( DBG )
         {
         printf("build_domain on sum: %s",node2string(cur));
         printf("exact=%d numsubset=%d\n",exact,numsubset);
         }

      //
      //  there was an exact match; remove the index set from the inner list and
      //  attach the result to the current node.  also print a warning message
      //  if the index set is a subset of other inner sets.
      //
      
      if( exact )
         {
         if( numsubset )
            {
            fprintf(info,"Warning: subscript ambiguity in sum or product\n" );
            fprintf(info,"   Expression is: %s\n",node2string(cur));
            fprintf(info,"   Index set '%s' is a subset of '%s'\n",indexset,slprint(submatch));
            fprintf(info,"   Ambiguities will default to outer values rather than '%s'\n",indexset);
            }
            
         freeitem( innerlist, indexset );
         cur->domain = innerlist;
         
         submatch = freelist( submatch );
         return 1;
         }

      //  
      //  unambiguous subscript match; add the index set to the equation qualifier 
      //  and rerun build_domain in order to set the variable context-specific subscript 
      //  list properly. then remove the index set from the list and attach the
      //  result to this node.
      //
      
      if( numsubset == 1 )
         {
         List *requal;

         requal = catlist( newlist(), qual );
         addlist( requal, indexset );
         build_domain( requal, right );
         requal = freelist( requal );
         
         cur->domain = catlist( newlist(), right->domain );
         freeitem( cur->domain, indexset );
         
         innerlist = freelist( innerlist );
         submatch  = freelist( submatch );
         return 1;
         }
         
      //
      //  error: subset doesn't match any of the inner sets at all...
      //
      
      if( numsubset == 0 )
         {
         error_front("Sum or product over '%s' has no terms defined over that set.\n",indexset);
         error_more ("Expression: %s\n",node2string(cur));
         exit(0);
         }
      
      //
      //  ack! no exact matches and multiple subscript matches; dang, must be
      //  an error
      //
      
      error_front("Subscript ambiguity in sum or product.\n" );
      error_more ("Expression is: %s\n",node2string(cur));
      error_more ("Index set '%s' is a subset of: '%s'\n",indexset,slprint(submatch));
      exit(0);
      }
   
   //
   //  All other nodes.  Collect the subsets on the left and right 
   //  sides and combine them appropriately.
   //
 
   switch( type )
      {
      case num:
         cur->domain = newlist();
         break;

      case neg:
      case log:
      case exp:
      case lag:
      case led:
         if( build_domain( qual, right )==0 )return 0;
         cur->domain = catlist( newlist(), right->domain );
         break;

      case add:
      case sub:
      case mul:
      case dvd:
      case pow:
      case equ:
         if( build_domain( qual, left  )==0 )return 0;
         if( build_domain( qual, right )==0 )return 0;
         cur->domain = conform( cur );
         if( cur->domain == 0 )return 0;
         break;

      default:
         FAULT("Unexpected node type in build_domain");
      }
      
   return 1;
}


/*-------------------------------------------------------------------*
 *  set_timeok
 *
 *  Decide whether to omit an equation based on the capabilities
 *  of the target language, the time domain of the equation, and
 *  the settings of the optional /first and /last flags.
 *-------------------------------------------------------------------*/
int set_timeok(Equation *eq)
{
   int exptime,isfirst,islast,isall,isinter;
   
   exptime = is_explicit_time();
      
   isfirst = ismember("first",eq->qsets) ;
   islast  = ismember("last" ,eq->qsets) ;
   isinter = eq->min_dt || eq->max_dt ;
   isall   = isinter==0 && isfirst==0 && islast==0 ;

   //
   //  with exptime, everything is ok unless /first or /last was given
   //
   
   if( exptime )
      {
      if( only_first )return (isfirst || isall) ;
      if( only_last  )return (islast  || isall) ;
      return 1;
      }
      
   //
   //  without exptime, /first or /last, everything is ok except 
   //  first and last
   //

   if( only_first==0 && only_last==0 )return !(isfirst || islast) ;

   //
   //  no exptime and either /first or /last; dump all intertemporal
   //  equations 
   //

   if( isinter )return 0;
   
   //  
   //  if only_first was given, accept this equation based
   //  on islast (drop if true, keep if false)
   //
   
   if( only_first )return !islast;
 
   //
   //  must have been only_last; accept based on isfirst
   //

   return !isfirst;
}


/*-------------------------------------------------------------------*
 *  check_equations 
 *-------------------------------------------------------------------*/
void check_equations()
{
   Equation *eq;
   Item *c;
   int  n,neq;
   int  nstart;
   List *undec;
   char title[30],under[30];
   int  len_title,len_under;
   void *sym;
   int  isused();
   char *name,*where;

   //
   //  set the timeok flag for each equation
   //
   
   for( eq = first ; eq ; eq = eq->next )
      eq->timeok = set_timeok(eq);

   //  
   //  go through the equations looking for undeclared variables
   //  and print an appropriate list
   //
   
   undec = newlist();

   for( eq = first ; eq ; eq = eq->next )
      if( eq->timeok )findundeclared( undec, eq->eq, eq->n, 0 );

   if( undec->n )
      {
      fprintf(info,"\nUndefined Symbols:\n\n" );
      for( c=undec->first, n=0 ; c ; c=c->next, n++ )
         fprintf(info,"   %s\n",c->str);
      if( n == 1 )
         error_front("There is 1 undefined symbol.");
      else
         error_front("There are %d undefined symbols.",n);
      }
  
   undec = freelist( undec );

   //
   //  list symbols and where they're used
   //

   fprintf(info,"\nEquations Using Each Parameter:\n\n");
   for( sym=firstsymbol(par) ; sym ; sym=nextsymbol(sym) )
      {
      name = symname(sym);
      where = symall(sym);
      fprintf(info,"   %s: %s\n",name,where);
      free(name);
      }

   fprintf(info,"\nEquations Using Each Variable:\n\n");
   for( sym=firstsymbol(var) ; sym ; sym=nextsymbol(sym) )
      {
      name = symname(sym);
      where = symdef(sym);
      fprintf(info,"   %s:\n      LHS %s\n",name,where);
      where = symuse(sym);
      fprintf(info,"      RHS %s\n",where);
      free(name);
      }

   //
   //  list any unused symbols, if necessary
   //

   for( sym=firstsymbol(par), n=0 ; sym ; sym=nextsymbol(sym) )
      if( isused(sym)==0 )
         {
         if( n++ == 0 )fprintf(info,"\nUnused Parameters:\n\n" );
         name = symname(sym);
         fprintf(info,"   %s\n",name);
         free(name);
         }
   
   for( sym=firstsymbol(var), n=0 ; sym ; sym=nextsymbol(sym) )
      if( isused(sym)==0 )
         {
         if( n++ == 0 )fprintf(info,"\nUnused Variables:\n\n" );
         name = symname(sym);
         fprintf(info,"   %s\n",name);
         free(name);
         }
   
   //
   //  print each equation
   //
      
   fprintf(info,"\n");

   n = 1;
   nstart = 1;
   
   for( eq = first ; eq ; eq = eq->next )
      {

      //
      //  print basic information
      //

      if( eq->timeok )
         sprintf(title,"Equation %d",eq->n);
      else
         sprintf(title,"Equation %d Omitted",eq->n);

      strcpy(under,"-------------------------");
      len_title = strlen(title);
      len_under = strlen(under);
      if( len_title < len_under )under[len_title] = '\0';

      fprintf(info,"\n%s\n%s\n",title,under);
      if( DBG )printf("%s\n",title);
      
      if( eq->name )
         fprintf(info,"Name: %s\n",eq->name);
         
      if( eq->qsets->n )
         fprintf(info,"Qualifiers: %s\n",slprint(eq->qsets));

      if( (eq->eq)->undec )
         {
         fprintf(info,"Domain: indeterminate, undeclared symbol(s) on %d\n",n);
         if( (eq->lhs)->undec )fprintf(info,"LHS ");
         if( (eq->rhs)->undec )fprintf(info,"RHS");
         fprintf(info,"\n");
         }

      if( ! eq->timeok )
         {
         fprintf(info,"Time domain is incompatible with target\n");
         fprintf(info,"Will be omitted when generating code\n");
         }
         
      if( eq->min_dt || eq->max_dt )
         if( eq->min_dt == NOTSET )
            error_front("Equation %d contains no variables or parameters",n);
         else
            fprintf(info,"Relative Time: [%d,%d]\n",eq->min_dt,eq->max_dt);

      if( eq->attr->n )
         fprintf(info,"Attributes: %s\n",slprint(eq->attr));

      //
      //  figure out the domain of each node in the equation, including
      //  the overall equation itself.  do this relatively late because
      //  the conformability calculations may generate warnings and they
      //  should print out in a reasonable place.
      //

      if( eq->eq->undec == 0 )
         {
         if( build_domain( eq->qsets, eq->eq )==0 )
            {
            error_front("Domain of equation %d is invalid.\nCheck for undefined variables.",n);
            continue;
            }

         catlist(eq->domain,(eq->eq)->domain);
      
         for( neq=1, c=((eq->lhs)->domain)->first ; c ; c=c->next )
            neq *= setsize(c->str);

         eq->count = neq;
         }

      //
      //  domain and count information
      //

      if( eq->eq->undec == 0 && eq->eq->domain->n && eq->timeok )
         {
         fprintf(info,"Domain: %s\n",slprint((eq->eq)->domain));
         fprintf(info,"Count: %d (%d to %d)\n",neq,nstart,nstart+neq-1);
         }

      //
      //  print the equation itself
      //

      fprintf(info,"\n   %s\n",sniprint(eq->eq,"   ") );

      //
      //  all done
      //

      fprintf(info,"\n");

      if( eq->timeok )
         nstart += neq;
      }

}

/*-------------------------------------------------------------------*
 *  impose_aliases
 *
 *  Look through the target list and impose any unambiguous 
 *  aliases from the aliases list.
 *-------------------------------------------------------------------*/

static List *impose_aliases(List *target, List *aliases)
{
   Item *t,*a;
   List *result;
   char *alias;
   int match;
   
   result = newlist();
   
   for( t=target->first ; t ; t=t->next )
      {

      //
      //  count number of matches for this target set in the 
      //  alias list
      //
      
      match = 0;
      for( a=aliases->first ; a ; a=a->next )
         if( isaliasof(a->str,t->str) )
            {
            match++;
            alias = a->str;
            }
 
      //
      //  if no matches, stash this set name and go on to the next 
      //  element
      //  
      
      if( match==0 )
         {
         addlist( result, t->str );
         continue;
         }

      //
      //  if multiple matches, we've got an ambiguity.  abort and let
      //  conform tell the user to disambiguate; can't sort out 
      //  A(regions,goods)*B(to,from,goods) by ourselves: does regions
      //  match to or from?
      //
      
      if( match > 1 )
         return 0;

      //
      //  exactly one match; don't need to check reverse relationship
      //  since the master name can only appear once in the target
      //  list.
      //

      addlist( result, alias );
      }

   return result;
}
      

/*-------------------------------------------------------------------*
 *  conform()
 *
 *  Determines conformability of the left and right arguments of 
 *  binary operations.  The left and right arguments must exist 
 *  and have their domain attributes set.
 *
 *  Each argument will fall into one of three classes: 
 *
 *     - scalar if it has no domain; 
 *     - singleton if it has a domain of size 1; 
 *     - matrix otherwise.
 *  
 *  Conformability rules are as follows (see note on aliases 
 *  at the end):
 *
 *     1) any operation on two scalars is OK and returns
 *        an empty list: 
 *
 *           null x null = null
 *
 *     2) any operation on one scalar and a nonscalar is OK 
 *        and returns the domain of the other argument: 
 *
 *           null x X = X
 *
 *  Conformability rules for two nonscalars depend on the 
 *  nonsingleton dimensions of their domains.  The singleton
 *  components from both sides are collected and conformability
 *  depends on the remaining "net" dimensions of the domains.
 *  
 *     3) any operation on two nonscalars is OK if their net
 *        domains are identical or if one of the the domains 
 *        is a subspace of the other.  Returns the larger of
 *        the net domains, plus the union of the singletons.
 *
 *        Singleton x singleton:
 *
 *           X(1)xY(3) x X(1)xZ(4) = X(1)xY(3)xZ(4) 
 *
 *        Singleton x matrix:
 *        
 *           X(1)xY(3) x XxZ = X(1)xY(3)xXxZ
 *        
 *        Matrix x matrix
 *
 *           X(1)xY x YxZ(3) = X(1)xZ(3)xY
 *
 *           XxY x XxYxZ = XxYxZ
 *
 *        Example: A(x)*B(x,y) produces C(x,y) where C(2,3) is 
 *        A(2)*B(2,3).
 *
 *        In short: Unambiguous promotion is OK.  Equivalent
 *        to duplicating the smaller object for each element 
 *        of the new dimension(s).
 *
 *     4) Anything else is a fatal error!
 *
 *  About aliases:
 *
 *     When comparing two lists of sets, any places where there is 
 *     an unambiguous match between a set in one list and an alias
 *     of the set in the other will be treated in the following way:
 *     the sets will be considered to match and the alias will be 
 *     returned.  For example, suppose 'to' and 'from' are both 
 *     aliases of 'regions', and the following expression appears
 *     in the model:
 *
 *        pexport(regions,goods)*sum(to,imports(from,to))
 *
 *     The conformability calculation will go as follows:
 *  
 *        sum(to,imports(from,to)) -> vector(from)
 *
 *        pexports(regions,goods)*vector(from) -> matrix(from,goods)
 *     
 *     That is, 'regions' will be matched to 'from' and be demoted 
 *     to it for subsequent comparisions.
 *-------------------------------------------------------------------*/
List *conform(Node *cur)
{
   List *left,*right,*lalias,*ralias,*result;
   List *lnet,*rnet,*sing,*both;
   Item *ele;
   Nodetype type;
   int llong,rlong;

   validate( cur,    NODEOBJ, "conform" );
   validate( cur->l, NODEOBJ, "conform" ); 
   validate( cur->r, NODEOBJ, "conform" ); 

   type  = cur->type;
   left  = cur->l->domain;
   right = cur->r->domain;
   
   validate( left,  LISTOBJ, "conform" );
   validate( right, LISTOBJ, "conform" );

   if( DBG )
      {
      printf("checking conformability on: %s\n",node2string(cur));
      printf("   type: %d\n",type);
      printf("   left: %s\n",slprint(left));
      printf("   right: %s\n",slprint(right));
      }
      
   //
   //  case 1: two scalars
   //
   
   if( left->n == 0 && right->n == 0 )
      return newlist();

   //
   //  case 2: a scalar and anything else (ok except if the 
   //  lhs is scalar and the rhs is not)
   //

   if( cur->type != equ || left->n > 0 )
      {
      if( left ->n == 0 )return catlist( newlist(), right );
      if( right->n == 0 )return catlist( newlist(), left  );
      }

   //
   //  ok, we have two nonscalars.  apply alias rules in case
   //  any set on one side can be replaced by a matching alias 
   //  on the other side.
   //
   
   lalias = impose_aliases( left , right );
   ralias = impose_aliases( right, left  );

   //
   //  check to see if anything broke during the alias check; that would 
   //  happen if there were ambiguities.
   //
   
   if( lalias==0 || ralias==0 )
      {
      error_front("Arguments are not conformable: %s\n",node2string(cur));
      error_more ("Left  sets: %s\n",slprint(left));
      error_more ("Right sets: %s\n",slprint(right));
      error_more ("At least one ambiguous match between left and right sets\n");
      exit(0);
      }

   //  
   //  separate out the singleton sets from the others; the singletons 
   //  need to be retained for the return value but they don't affect 
   //  the rest of the conformability calculation. ok to collect them 
   //  in a single merged list.
   // 
   //  handle the time set the same way: it's ok for one argument to 
   //  vary over time without the other one having to.
   //  
 
   sing = newlist();
   lnet = newlist();
   rnet = newlist();
   
   for( ele=lalias->first ; ele ; ele=ele->next )
      if( setsize(ele->str) > 1 && ! istimeset(ele->str) )
         addlist( lnet, ele->str );
      else
         addlist( sing, ele->str );
   
   for( ele=ralias->first ; ele ; ele=ele->next )
      if( setsize(ele->str) > 1 && ! istimeset(ele->str) )
         addlist( rnet, ele->str );
      else
         addlist( sing, ele->str );

   lalias = freelist( lalias );
   ralias = freelist( ralias );

   //
   //  Case 3: any nonscalars with identical net domains or
   //  the domain of one is a subspace of the other.   
   //  
   //  One exception is that '=' must match exactly
   //
   
   both = intersect( lnet, rnet );

   result = 0;
   
   llong = lnet->n > both->n ;
   rlong = rnet->n > both->n ;

   if( llong==0 && rlong==0 )
      {
      lnet = freelist( lnet );
      result = rnet;
      }
      
   if( llong && rlong==0 && type != equ )
      {
      rnet = freelist( rnet );
      result = lnet;
      }

   if( rlong && llong==0 && type != equ )
      {
      lnet = freelist( lnet );
      result = rnet;
      }

   if( result )
      {
      catlist( result, sing );
      sing = freelist( sing );
      both = freelist( both );
      return result;
      }
      
   //
   //  got a problem; punt
   //
   
   error_front("Arguments not Conformable");
   error_more ("Expression: %s",node2string(cur));
   error_more ("Left side:  %s",node2string(cur->l));
   error_more ("Domain:     %s",slprint(left));
   error_more ("Right side: %s",node2string(cur->r));
   error_more ("Domain:     %s",slprint(right));
   exit(0);

   return 0;
}


/*-------------------------------------------------------------------*
 *  gettimesets()
 *
 *  Used by build_set_relationships to create any on-the-fly time
 *  sets.
 *-------------------------------------------------------------------*/
List *gettimesets()
{
   if( timesets==0 )return 0;
   return duplist(timesets);
}
