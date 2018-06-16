/*-------------------------------------------------------------------*
 *  options.c
 *  Nov 04 (PJW)
 *
 *  Allow output options to be set or tested.  Completely opaque 
 *  to both front and backend routines.  All routines are int 
 *-------------------------------------------------------------------*/

#include "options.h"

#include "error.h"
#include "lists.h"
#include "sym.h"

enum eqntype { eqn_scalar, eqn_vector, eqn_unknown };
enum sumtype { sum_scalar, sum_vector, sum_unknown };
enum eqnform { eqf_norm,   eqf_lvalue, eqf_unknown };

static enum eqntype myeqn  = eqn_unknown ;
static enum sumtype mysum  = sum_unknown ;
static enum eqnform myform = eqf_unknown ;

static int linelength=0;
static int alpha_elements=0;
static int explicit_time=0;
static List *reserved=0;

//
//  set 
//

int set_eqn_scalar()     { myeqn = eqn_scalar;  return 1; }
int set_eqn_vector()     { myeqn = eqn_vector;  return 1; }

int set_eqn_normalized() { myform = eqf_norm;   return 1; }
int set_eqn_lvalue()     { myform = eqf_lvalue; return 1; }

int set_sum_scalar()     { mysum = sum_scalar;  return 1; }
int set_sum_vector()     { mysum = sum_vector;  return 1; }

int set_line_length(int n) { linelength=n;     return 1; }
int set_alpha_elements()   { alpha_elements=1; return 1; }

int set_reserved_word(char *word)
{
   if( word==0  )FAULT("Null pointer passed to set_reserved_word");
   if( *word==0 )FAULT("Empty string passed to set_reserved_word");
   if( reserved==0 )reserved = newlist();
   addlist( reserved, word );
   return 1;
}
   
int set_explicit_time() { explicit_time=1; return 1; }

//
//  test
//

int is_eqn_set()        { return myeqn != eqn_unknown; }
int is_eqn_scalar()     { return myeqn == eqn_scalar;  }
int is_eqn_vector()     { return myeqn == eqn_vector;  }

int is_eqn_normalized() { return myform == eqf_norm;   }
int is_eqn_lvalue()     { return myform == eqf_lvalue; }

int is_sum_set()        { return mysum != sum_unknown; }
int is_sum_scalar()     { return mysum == sum_scalar;  }
int is_sum_vector()     { return mysum == sum_vector;  }

int is_alpha_elements() { return alpha_elements;       }

int is_reserved_word(char *word)
{
   if( word==0  )FAULT("Null pointer passed to is_reserved_word");
   if( *word==0 )FAULT("Empty string passed to is_reserved_word");
   if( reserved==0 )return 0;
   return ismember(word,reserved);
}

int is_explicit_time() { return explicit_time; }

//
//  get
//

int get_line_length()   { return linelength; }
