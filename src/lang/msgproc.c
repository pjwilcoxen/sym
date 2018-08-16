/*--------------------------------------------------------------------*
 * msgproc.c
 * Jul 04 (PJW)
 *--------------------------------------------------------------------*
.. ### msgproc
..
.. Support the Ox implementation of the MSG/G-Cubed solution algorithm.
 *
 * + Each variable must have exactly one of the following attribute
 *   tags indicating its type: end = endogenous, exo = exogenous,
 *   sta = state variable, cos = costate variable.
 *
 * + Leads and lags are only allowed on some variable types:
 *   lead(cos), lead(sta), lead(end), lag(end).
 *--------------------------------------------------------------------*
 *
 *  Each variable in the model file must be given exactly one of the 
 *  following attribute tags, where "LHS vec" and "RHS vec" show 
 *  how contemporaneous values of the variables (no lag or lead) are 
 *  mapped into the algorithm's vectors, and "Suffix" shows the 
 *  variable suffixes used in the Gauss version.
 *
 *  Vector in Warwick's algorithm:
 *     LHS:
 *        Z1L : endogenous except PRCT, PRID
 *        ZEL : PRCT, PRID
 *        J1L : lead(costate)
 *        X1L : lead(state) plus PRCT, PRID appended due to lag() reference 
 *                ( X1L = ZER )
 *     RHS:
 *        Z1R : endogenous except PRCT, PRID
 *        ZER : PRCT, PRID
 *        EXO : exogenous
 *        YXR : state, lag(PRCT), lag(PRID)
 *        YJR : costate
 *        EXZ : expected endog: lead(PRCT), lead(PRID)
 *        X1R : not used? PRCT..YC, PRID..YC
 *        
 *    Attrib   LHS vec  RHS vec  Suffix   Description
 *    ------   -------  -------  ------   -----------
 *     end       Z1L      Z1R      NC     Normal endogenous variables
 *     end       ZEL      ZER      DC     Endogenous involved in lead() or lag()
 *     exo       ---      EXO      XC     Normal exogenous variables
 *     sta       ---      YXR      SC     State variables (stocks) 
 *     cos       ---      YJR      JC     Costate variables 
 *
 *  Lead and lag operators are only allowed on some of the variable 
 *  types.  They are translated into vectors as follows:
 *
 *     Usage    LHS vec  RHS vec  Suffix   Description
 *   ---------  -------  -------  ------   -----------
 *   lead(cos)    J1L      ---      JL
 *   lead(sta)    X1L      ---      YL
 *   lead(end)    X1L      EXZ      DL
 *   lag(end)     ---      YXR      SB
 *    
 *  PRCT
 *    +LHS: PRCTUUDC = PRCT       : ZEL
 *    +RHS: PRCTUUDC = PRCT       : ZER
 *    +RHS: PRCTUUEL = lead(PRCT) : EXZ
 *     RHS: PRCTUUSB = lag(PRCT)  : YXR
 *     RHS: PRCTUUYC = PRCT       : X1R // contemporaneous PRCT
 *     
 *    +LHS: PRCTUUYC = PRCT       : X1L
 *    +PRCTUUYC=PRCTUUDC : X1L = ZER
 *
 * PRID
 *     LHS: PRIDUUDC = PRID       : ZEL // contemporaneous PRID
 *     RHS: PRIDUUDC = PRID       : ZER // contemporaneous PRID
 *     RHS: PRIDUUDL = lead(PRID) : EXZ
 *     RHS: PRIDUUSB = lag(PRID)  : YXR
 *     RHS: PRIDUUYC = PRID       : X1R 
 *
 *     LHS: PRIDUUYC =            : X1L 
 *     PRIDUUYC=PRIDUUDC : X1L = ZER
 *
 *  Notes:
 *
 *     Do we need special syntax to handle xxxNC = xxxJC, or should
 *     msgproc automatically generate the corresponding equations
 *     and variables?  Why not use xxxJC everwhere xxxNC appears?
 *     Needed for: WELH, REXC:
 *        
 *        WELH..NC = WELH..JC
 *        REXC..NC = REXC..JC
 *
 *     Not needed for TOB because the TOB is now a pure endogenous
 *     variable.  Not needed for LAM because only the JC version 
 *     appears on the RHS.  Would it hurt to use it anyway?
 *
 *     PRCT and PRID: add auxiliary equation as follows to map
 *     YC RHS varible into DC:
 *
 *        PRCT..DC = PRCT..YC
 *        PRCT = ?? : ZEL = X1R
 *--------------------------------------------------------------------*/

#include "../cart.h"
#include "../eqns.h"
#include "../error.h"
#include "../lang.h"
#include "../options.h"
#include "../output.h"
#include "../sets.h"
#include "../str.h"
#include "../sym.h"
#include "../symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  myDEBUG 1

extern char *strdup();

// 
//  Flag for checking local MSGPROC objects for corruption
//

#define MSGVAROBJ 3001

//
//  Subscript origin for output array references
//

#define MSG_ORIGIN 1

//
//  Internal variables
//

static int MSGPROC_block =1;
static int MSGPROC_scalar=1;
static int MSGPROC_vars  =1;

//
//  Debugging files
//

FILE *varmap;
FILE *varinfo;
FILE *vars;
FILE *optmap;

//
//  MSGPROC vectors
//

#define NUL  0
#define Z1L  1
#define ZEL  2
#define J1L  3
#define X1L  4
#define Z1R  5
#define ZER  6
#define YJR  7
#define YXR  8
#define EXO  9
#define EXZ 10
#define PAR 11
#define X1R 12
#define UNK 13

static int   vecinfo[UNK+1] ;
static char *vecname[UNK+1];

//
//  MSGPROC variable types and mapping into vectors
//
//     Output vector names in the vlist array are organized as follows: LHS
//     name when lagged, LHS name for the contemporaneous value, LHS name when
//     a lead, RHS name when lagged, RHS name for contemporaneous value, RHS
//     name when a lead.  
//     
//     A null value indicates a semantic error: that type of variable is not
//     allowed in the corresponding context (eg, lead(exog) = something).
//
//     Order of vector names: left-lag, left-cur, left-lead, 
//     right-lag, right-cur, right-lead.
//     

struct vartype_info 
   {
   char *type;
   int  vecid[6];
   };

struct vartype_info vlist[] =
   {
   { "end",   0, Z1L,   0,   0, Z1R,   0 },
   { "ets",   0, ZEL,   0,   0, ZER, EXZ },
   { "exo",   0,   0,   0,   0, EXO,   0 },
   { "cos",   0,   0, J1L,   0, YJR,   0 },
   { "sta",   0,   0, X1L,   0, YXR,   0 },
   { "stl",   0, X1L,   0, YXR, X1R,   0 },
   { "par",   0,   0,   0,   0, PAR,   0 },
// { "ttp",   0, ZEL,   0, YXR, ZER, EXZ },
   {     0,   0,   0,   0,   0,   0,   0 }
   };

//
//  Units
//
//  The us_units list is used for setting the region of variables 
//  in the vars.csv file.  It is used for variables that are normalized
//  relative to US rather than own GDP.

char *units[] = 
   { "del", "pct", "gdp", "usgdp", "cent", "gwh", "gwhgdp", 
     "btu", "mmt", "btugdp", "mmtgdp", "btuusgdp", "mmtusgdp", 0 };

char *us_units[] =
   { "usgdp", "btuusgdp", "mmtusgdp", "gwhusgdp", 0 };

//
//  Special set names needed to generate MSG 8-character names.
//
//  If these are not present in the input file, setup_gaussname
//  will crash.

char *setname_regions = "regions";
char *setname_sectors = "sectors";
char *setname_goods   = "goods";

//
//  US region code.  Ugly hack: really should be handled via
//  syntax in the sym file.
//

char *USA_CODE = "UU";

//
//  Rules for translating between new and old notation in the
//  varmap file
//

enum oldform { o_unk, o_err, o_none, 
               o_cc, o_cu, o_a, o_g,
               o_acc, o_gcc, o_ag, o_do, 
               o_agcc, o_gdo };

//
//  Define the variable object and initialize the linked list
//

struct variable
   {
   int  obj;               // fidelity check
   char *str;              // variable's name
   char *type;             // variable's type
   char *unit;             // variable's unit
   int  vecid[6];          // vector id number for each context
   int  vecoff[6];         // offset from start of vector
   Symboltype stype;       // symbol type
   enum oldform of_type;   // old variable name form
   int  of_a;              // position of agent subscript
   int  of_g;              // position of commod subscript
   int  of_c;              // position of country subscript
   int  of_d;              // position of dest subscript
   int  of_o;              // position of orig subscript
   int  varsnum;           // first element number in VARS list
   struct variable *next;  // next variable
   };

typedef struct variable Variable;
static Variable *v_head=0;


//----------------------------------------------------------------------//
//  Function prototypes
//----------------------------------------------------------------------//

char *get_msgname(char*, List*, Context);
void msg_error(char*, char*);
static void write_gaussname( FILE*, Variable*, List* );


//----------------------------------------------------------------------//
//  msg_error()
//
//  Print an error message and crash.  Use this routine instead of
//  msg_error in order to indicate that the problem was with the
//  MSGPROC back end.
//----------------------------------------------------------------------//

void msg_error(char *fmt, char *str)
{
   show_error("Fatal Error Writing MSGPROC File",fmt,str);
}


//----------------------------------------------------------------------//
//  write_varmap()
//  
//  Write information about a variable or parameter to the varmap 
//  file.  The real work is done by cart_build(), cart_first() and 
//  cart_next(), which create and step through the cartesian product
//  of the subscript sets.
//----------------------------------------------------------------------//

void write_varmap(Variable *thisvar, List *setlist)
{
   Context mycontext;
   List *cur;
   char *subs,*name;
   int j,n;

   cart_build(setlist);
         
   for( j=0 ; j<6 ; j++ )
      if( thisvar->vecid[j] )
         {
         
         mycontext.lhs = (j < 3) ? 1 : 0 ;
         mycontext.dt  = 0;
         if( j == 0 || j == 3 )mycontext.dt = -1;
         if( j == 2 || j == 5 )mycontext.dt =  1;

         n = thisvar->varsnum ;

         for( cur=cart_first() ; cur ; cur=cart_next() )
            {
            name = get_msgname(thisvar->str,cur,mycontext);
            subs = strchr(name,']');
            *subs = '\0';
            subs = strchr(name,'[');
            *subs++ = '\0';

            fprintf(varmap,"\"%s(%s)\",",thisvar->str,slprint(cur));
            fprintf(varmap,"\"%s[%s]\",%s,%s,",name,subs,name,subs);
            write_gaussname( varmap, thisvar, cur );
            fprintf(varmap,"\n");

            //
            //  write a modified version to Yiyong's optmap file
            //

            if( thisvar->stype == par )n = 0;
            fprintf(optmap,"%d,",n++);
            fprintf(optmap,"\"%s[%s]\",%s,%s,",name,subs,name,subs);
            write_gaussname( optmap, thisvar, cur );
            if( thisvar->stype == par )fprintf(optmap,"0");
            fprintf(optmap,"\n");

            free(name);
            }
         }
}


//----------------------------------------------------------------------//
//  write_vars()
//  
//  Write information about a varible to the vars.csv file.  Driven
//  by cart_build().
//----------------------------------------------------------------------//

void write_vars(Variable *thisvar, List *setlist, char *desc )
{
   List *cur;
   Item *sub;
   char *region;
   int region_index;
   int sub_index;
   int i;

   //
   // figure out which subscript contains the region code for the variable.
   // will be the main region determined by the setup_gaussname (subscript
   // at index of_c) unless there's a destination set as well (subscript
   // at index of_d).
   //

   region_index = thisvar->of_c;
   if( thisvar->of_d >= 0 )region_index = thisvar->of_d;
   if( region_index >= setlist->n )
      msg_error("internal error when determining region of variable %s",thisvar->str);
 
   // 
   // force region to US for variables with units defined in terms of US GDP.  
   // forcing is done by setting region_index to -1.  That causes them to 
   // be treated as variables with no region index, which default to US.
   //

   for( i=0 ; us_units[i] ; i++ )
      if( strcasecmp(thisvar->unit,us_units[i])==0 )
         {
         region_index = -1;
         break;
         }
      
   //
   // default is the US ...
   //

   region = USA_CODE;

   //
   // do the actual work ...
   //

   cart_build(setlist);

   thisvar->varsnum = MSGPROC_vars;

   for( cur=cart_first() ; cur ; cur=cart_next() )
      {
      fprintf(vars,"%d,",MSGPROC_vars++);
      fprintf(vars,"\"%s(%s)\",",thisvar->str,slprint(cur));
      fprintf(vars,"\"%s\",",desc);
      fprintf(vars,"\"%s\",",thisvar->unit);
      
      if( region_index >= 0 )
         {
         for( sub=cur->first, sub_index=0 ; sub_index < region_index ; sub_index++, sub=sub->next );
         region = sub->str;
         }
      fprintf(vars,"\"%s\",",region);

      fprintf(vars,"\n");
      }
}


//----------------------------------------------------------------------//
//  get_msgname()
//
//  Figure out the vector and element number for this variable given
//  the context in which it appears.
//----------------------------------------------------------------------//

char *get_msgname(char *str, List *sublist, Context context)
{
   int sel=1;
   int vecid;
   int check;
   char buf[1024],*ptr;
   Variable *var;
   List *numsubs;
   
   if( v_head == 0 )
      FAULT("Variable list is blank in get_msgname");

   for( var = v_head ; var ; var=var->next )
      {
      validate( var, MSGVAROBJ, "get_msgname" );

      check = strcasecmp(str,var->str);
      if( check == 0 )break;
      if( check <  0 )
         FAULT("Name not in variable list in get_msgname");
      }

   if( var==0 )
      FAULT("Name not in variable list in get_msgname");

   //
   //  check that lead and lag structure is OK for msgproc
   //
   
   if( context.dt < -1 )
      msg_error("%s","lag(lag(var)) cannot be used with msgproc");

   if( context.dt >  1 )
      msg_error("%s","lead(lead(var)) cannot be used with msgproc");

   //
   //  given the context, look up the MSGPROC vector id
   //

   sel += context.dt;
   if( context.lhs == 0 )sel += 3;

   vecid = var->vecid[sel];
   
   //
   //  if vecid is zero, this type of variable is not allowed
   //  in the current context.  figure out what the problem was
   //  and print an appropriate message.
   //

   if( vecid==0 )
      {
      char *side;
      switch( sel )
         {
         case 0: side = "LHS in lag()"               ; break;
         case 1: side = "LHS without lag() or lead()"; break;
         case 2: side = "LHS in lead()"              ; break;
         case 3: side = "RHS in lag()"               ; break;
         case 4: side = "RHS without lag() or lead()"; break;
         case 5: side = "RHS in lead()"              ; break;
         }
      sprintf(buf,"%s\n   Type '%s' on %s",str,var->type,side);
      msg_error("Invalid context for variable %s",buf);
      }
      
   //
   //  context is OK; now convert the subscripts
   //

   numsubs = sub_offset( str, sublist, var->vecoff[sel] );

   sprintf(buf,"%s[%s]",vecname[vecid],slprint(numsubs));
   
   freelist(numsubs);

   ptr = strdup(buf);
   if( ptr == 0 )FAULT("Could not allocate memory in get_msgname");

   return ptr;
}


//----------------------------------------------------------------------//
//  setup_gaussname()
//
//  Figure out information needed to write this identifier out in 
//  the original notation used in gauss.
//----------------------------------------------------------------------//

static void setup_gaussname( Variable *v, Symboltype type, List *sets ) 
{
   Item *c;
   int  i,flags;
   char *name;
   
   v->stype   = type;
   v->of_type = o_unk;
   v->of_a    = -1;
   v->of_g    = -1;
   v->of_c    = -1;
   v->of_o    = -1;
   v->of_d    = -1;

   if( sets->n == 0 ) {
      v->of_type = o_none;
      return;
   }
  
   //
   //  look through the list of sets and see if we can deduce what the
   //  gauss name would have looked like
   //

   flags = 0;
   
   for( c=sets->first, i=0 ; c ; c=c->next, i++ ) {
      
      name = c->str;

      if( strcasecmp(name,setname_regions)==0 || issubset(name,setname_regions) ) {
         v->of_c = i;
         flags |= 0x01;
         if( strcasecmp(name,"orig")==0 || strcasecmp(name,"currency")==0 )v->of_o = i;
         if( strcasecmp(name,"dest")==0 || strcasecmp(name,"owner"   )==0 )v->of_d = i;
      }

      if( strcasecmp(name,setname_sectors)==0 || issubset(name,setname_sectors) ) { 
         v->of_a = i;
         flags |= (flags & 0x02) ? 0xFF : 0x02;
      }

      if( strcasecmp(name,setname_goods)==0 || issubset(name,setname_goods) ) { 
         v->of_g = i;
         flags |= (flags & 0x04) ? 0xFF : 0x04;
      }

   }
   
   switch( flags ) {
      case 0x01: v->of_type = o_cc  ; break;
      case 0x02: v->of_type = o_a   ; break;
      case 0x04: v->of_type = o_g   ; break;
      case 0x03: v->of_type = o_acc ; break;
      case 0x05: v->of_type = o_gcc ; break;
      case 0x07: v->of_type = o_agcc; break;
      case 0x0F: v->of_type = o_err ; break;
      default:   v->of_type = o_err ; break;
   }

   if( v->of_o >= 0 && v->of_d >= 0 ) {
      if( v->of_g <  0 )v->of_type = o_do;
      if( v->of_g >= 0 )v->of_type = o_gdo;
   }

   // REXC and EXCH are a special case
    
   if( strcasecmp(v->str,"REXC")==0 || strcasecmp(v->str,"EXCH")==0 )
      v->of_type = o_cu ;

   // check for errors
   
   if( v->of_type == o_err || v->of_type == o_unk )
      msg_error("Variable has unexpected type: %s",v->str);

}


//----------------------------------------------------------------------//
//  write_gaussname
//
//  Write this identifier out in the original notation used in gauss.
//  Most names follow one of these rules:
//
//     STEM
//     STEMc   - country c
//     STEMcU  - country c (rexc and exch only)
//     STEMgc  - good g in country c
//     STEMac  - agent a in country c
//     STEMagc - agent a, good g, country c
//     STEMdo  - origin o, destination d
//     STEMgdo - good g, origin o, destination d
//     
//  Also, agent 'aRAW' and good 'aRAW' are mapped to 'Y' and agent  
//  'HH' is mapped to 'Z'.
//----------------------------------------------------------------------//

static void write_gaussname( FILE *ofile, Variable *v, List *s ) 
{
   int i,isec;
   Item *sub;
   enum oldform ofmt;
   char *cc=0,*agent=0,*good=0,*dest=0,*orig=0,asec,gsec,d,o;
   char *subs[5];
   static char *secs = "123456789ABCDEFGHIJKLMNOPQRSTUVWX";

   if( v->stype == par )
      return;

   ofmt = v->of_type;

   switch( ofmt ) {
      case o_none:
         fprintf(ofile,"%s",v->str);
         return;

      case o_cc:
         cc = s->first->str;
         fprintf(ofile,"%s%s",v->str,cc);
         break;

      case o_cu:
         cc = s->first->str;
         fprintf(ofile,"%s%cU",v->str,*cc);
         break;

      case o_acc:
      case o_gcc:
      case o_agcc:
         for( sub=s->first, i=0 ; sub ; sub=sub->next, i++ ) 
            subs[i] = sub->str;
            
         if( v->of_c >= 0 )cc    = subs[v->of_c];
         if( v->of_a >= 0 )agent = subs[v->of_a];
         if( v->of_g >= 0 )good  = subs[v->of_g];

         if( agent ) {
            isec = setindex( setname_sectors , agent );
            asec = secs[isec];
            if( strcasecmp(agent,"aRAW")==0 )FAULT("Saw obsolete reference to aRAW");
            if( strcasecmp(agent,"HH"  )==0 )FAULT("Saw obsolete reference to HH");
         }

         if( good  ) {
            isec = setindex( setname_goods  , good  );
            gsec = secs[isec];
            if( strcasecmp(good,"aRAW")==0 )FAULT("Saw obsolete reference to aRAW");
         }

         if( ofmt == o_acc  )fprintf(ofile,"%s%c%s",v->str,asec,cc);
         if( ofmt == o_gcc  )fprintf(ofile,"%s%c%s",v->str,gsec,cc);
         if( ofmt == o_agcc )fprintf(ofile,"%s%c%c%s",v->str,asec,gsec,cc);

         break;

      case o_do:
      case o_gdo:
         for( sub=s->first, i=0 ; sub ; sub=sub->next, i++ ) 
            subs[i] = sub->str;
            
         if( v->of_d >= 0 )dest = subs[v->of_d];
         if( v->of_o >= 0 )orig = subs[v->of_o];
         if( v->of_g >= 0 )good = subs[v->of_g];

         if( dest==0 || orig==0 )
            FAULT("Incorrect region subscripts in write_gaussname");

         d = *dest;
         o = *orig;
         
         if( good  ) {
            isec = setindex( setname_goods , good  );
            gsec = secs[isec];
            if( strcasecmp(good ,"aRAW")==0 )gsec = 'Y';
         }

         if( ofmt == o_do )
            fprintf(ofile,"%s%c%c",v->str,d,o);
         else
            fprintf(ofile,"%s%c%c%c",v->str,gsec,d,o);

         break;
         
      default:
         return;
   }
}


//----------------------------------------------------------------------//
//  MSGPROC_begin_file()
//
//  Begin processing the file
//----------------------------------------------------------------------//

void MSGPROC_begin_file(char *basename)
{
   int i;
   char *fname;
   
   fname  = concat(2,basename,"_varmap.csv");
   varmap = fopen(fname,"w");
   if( varmap == 0 )
      msg_error("Could not create file: %s",fname);
   free( fname );
   
   fname  = concat(2,basename,"_optmap.csv");
   optmap = fopen(fname,"w");
   if( optmap == 0 )
      msg_error("Could not create file: %s",fname);
   free( fname );
   
   fname   = concat(2,basename,"_varinfo.csv");
   varinfo = fopen(fname,"w");
   if( varinfo == 0 )
      msg_error("Could not create file: %s",fname);
   free( fname );

   fname = concat(2,basename,"_vars.csv");
   vars  = fopen(fname,"w");
   if( vars == 0 )
      msg_error("Could not create file: %s",fname);
   free( fname );

   for( i=NUL ; i <= UNK ; i++ )vecinfo[i] = MSG_ORIGIN;

   vecname[NUL] = ""    ;
   vecname[Z1L] = "z1l" ;
   vecname[ZEL] = "zel" ; 
   vecname[J1L] = "j1l" ;
   vecname[X1L] = "x1l" ;
   vecname[Z1R] = "z1r" ;
   vecname[ZER] = "zer" ;
   vecname[YJR] = "yjr" ;
   vecname[YXR] = "yxr" ;
   vecname[EXO] = "exo" ;
   vecname[EXZ] = "exz" ;
   vecname[PAR] = "par" ;
   vecname[X1R] = "x1r" ;
   vecname[UNK] = ""    ;

   fprintf(code,"#include <oxstd.h>\n\n");
   fprintf(code,"#include \"declGcubedVarsSYM.ox\"\n");
   fprintf(code,"msgproc(z1r,zer,yjr,yxr,exo,exz)\n");
   fprintf(code,"{\n\n");
}


//----------------------------------------------------------------------//
//
//  End processing the file
//
//----------------------------------------------------------------------//

void MSGPROC_end_file()
{
   int i,ecount,vcount;
   int ucount;
   void *cur;
   char *err;

   fprintf(code,"\n}\n");

   fclose( varmap  );
   fclose( varinfo );
   fclose( vars    );
   fclose( optmap  );

   ecount = MSGPROC_scalar-1;
   vcount = vecinfo[Z1L]+vecinfo[ZEL]+vecinfo[J1L]+vecinfo[X1L]
          - 4*MSG_ORIGIN ;

   fprintf(info,"\nLength of MSGPROC Vectors:\n\n");
   for( i=NUL+1 ; i < UNK ; i++ )
      switch( i )
         {
         case Z1R:
         case YJR:
         case ZER:
         case EXZ:
         case YXR:
         case X1R:
            break;
         
         default:
            fprintf(info,"   %s has %d elements\n",vecname[i],vecinfo[i]-MSG_ORIGIN);
         }
      
   //
   //  count unused endogenous variables
   //

   ucount = 0;
   for( cur=firstsymbol(var) ; cur ; cur=nextsymbol(cur) ) 
      if( ismember("end",symattrib(cur)) && !isused(cur) )
         ucount += symsize(cur);

   fprintf(info,"\n");
   fprintf(info,"Equation Count: %d\n",ecount);
   fprintf(info,"Endogenous Variables, Used:   %d\n",vcount-ucount);
   fprintf(info,"Endogenous Variables, Total:  %d\n",vcount);

   //
   //  crash loudly if there's a mismatch
   //
   
   if( ecount != vcount-ucount )
      {
      err = "Counts of equations and endogenous variables do not match.";
      fprintf(info,"\nFatal Error:\n   %s\n",err);
      msg_error("%s",err);
      }
}

//----------------------------------------------------------------------//
//  MSGPROC_declare
//
//  Add a new variable or parameter to the internal list.  Reserves 
//  space in the relevant MSGPROC vectors.
//----------------------------------------------------------------------//

void MSGPROC_declare(void *sym)
{
   char     *name;
   Variable *newvar;
   Variable *cur,*nxt;
   List     *attlist;
   List     *vallist;
   char     *curtype,setlist[1024],*desc;
   int      vecid,start,do_inc;
   int      my_Z1L, my_J1L, my_ZEL, my_X1L;
   
   int i,j,check,found,count,vi;
   validate( sym, SYMBOBJ, "MSGPROC_declare" );

   if( istype(sym,set) )return;
   
   if( ! isident(sym) )
      FAULT("Invalid symbol type passed to MSGPROC_declare");

   name    = symname(sym);
   desc    = symdescrip(sym);
   vallist = symvalue(sym);
   count   = symsize(sym);

   if( count<1 )
      FAULT("Symbol has no element count in MSGPROC_declare");

   newvar = (Variable *) malloc( sizeof(Variable) );
   newvar->obj  = MSGVAROBJ;
   newvar->str  = name;
   newvar->next = 0;

   //
   //  figure out how to write the old-style variable name in 
   //  the varmap file
   //
   
   if( istype(sym,par) )setup_gaussname( newvar, par, vallist );
   if( istype(sym,var) )setup_gaussname( newvar, var, vallist );

   //
   //  if this is a variable, figure out which variable type 
   //  was given.  there must be exactly one match.  if it 
   //  is a parameter, default.
   //

   found   = 0;
   vi      = 0;
      
   attlist = symattrib(sym);

   for( i=0 ; curtype = vlist[i].type ; i++ )
      {
      if( *curtype == 0 )
         FAULT("Corrupted vlist in MSGPROC_declare");
         
      if( istype(sym,par) && strcmp(curtype,"par")==0 )
         {
         found = 1;
         vi = i;
         break;
         }

      if( istype(sym,var) && ismember(curtype,attlist) )
         {
         if( found )msg_error("Multiple variable types for variable: %s",name);
         found = 1;
         vi = i;
         }
      }

   if( istype(sym,var) && found==0 )
      msg_error("No MSGPROC type declared for variable %s",name);
   
   if( istype(sym,par) && found==0 )
      FAULT("Failed to find parameter in vlist in MSGPROC_declare");
      
   newvar->type = vlist[vi].type;

   //
   //  if this is a variable, figure out its units
   //

   newvar->unit = "";
   if( istype(sym,var) )
      {
      newvar->unit = 0;
      for( i=0 ; units[i] ; i++ )
        if( ismember(units[i],attlist) )
           {
           newvar->unit = units[i];
           break;
           }
      if( newvar->unit == 0 )
         {
         char *msg;
         msg = concat(3,name," with attributes ",slprint(attlist));
         msg_error("No units given for variable %s",msg);
         }
      }
  
   //
   //  get the current offset into each vector and then reserve space
   //  for all of this variable's elements.  use my_* variables to synchronize
   //  subscripts between vectors; set them to -1 initially to allow error
   //  checks.
   //

   my_Z1L = -1;
   my_J1L = -1;
   my_ZEL = -1;
   my_X1L = -1;

   for( j=0 ; j<6 ; j++ )
      {
      vecid = vlist[vi].vecid[j];
      newvar->vecid[j] = vecid;
      
      if( vecid==0 )
         {
         newvar->vecoff[j] = 0;
         continue;
         }
         
      //  look up this vecid in the vecinfo table

      if( vecid <= NUL || vecid >= UNK )
         FAULT("Unrecognized vector id in MSGPROC_declare");

      //  figure out which counter to use.  this is a bit tricky because
      //  we need to synchronize subscripts across certain sets of vectors.

      start  = vecinfo[vecid];
      do_inc = 1;

      switch( vecid ) 
         {
         // 
         // Z1L drives Z1R
         // 
         case Z1L: 
            my_Z1L = start ; 
            break;
            
         case Z1R: 
            if( my_Z1L < 0 )FAULT("Z1R without Z1L");
            start  = my_Z1L ; 
            do_inc = 0;
            break;

         // 
         // J1L drives YJR
         // 
         case J1L:
            my_J1L = start;
            break;
            
         case YJR:
            if( my_J1L < 0 )FAULT("YJR without J1L");
            start  = my_J1L ; 
            do_inc = 0;
            break;

         //
         // ZEL drives ZER and EXZ
         //  
         case ZEL:
            my_ZEL = start;
            break;
            
         case ZER:
         case EXZ:
            if( my_ZEL < 0 )FAULT("ZER or EXZ without ZEL");
            start  = my_ZEL;
            do_inc = 0;
            break;

         //
         // X1L drives YXR and X1R
         //
         case X1L:
            my_X1L = start;
            break;

         case YXR:
         case X1R:
            if( my_X1L < 0 )FAULT("YXR or X1R without X1L");
            start  = my_X1L;
            do_inc = 0;
            break;

         //
         // everything else drives itself
         // 
         default:
            break;
         }
         
      newvar->vecoff[j] = start ;
      if( do_inc )vecinfo[vecid] += count;

      }

   //
   //  entry is now complete
   //

   if( vallist->n )
      sprintf(setlist,"(%s)",slprint(vallist) );
   else
      strcpy(setlist,"");
      
   fprintf(varinfo,"\"%s%s\",%d,%s,%s,\"%s\",\"%s\"\n",
      name, setlist, count, newvar->type, newvar->unit, desc, slprint(attlist) );

   if( DBG )
      {
      printf("MSGPROC_declare: %s, type %s, has %d elements -> ",name,vlist[vi].type,count);
      for( j=0 ; j<6 ; j++ )
         {
         vecid = newvar->vecid[j];
         if( vecid > 0 )
            printf("%s[%d] ",vecname[vecid],newvar->vecoff[j]);
         else
            printf("-- ");
         }
      printf("\n");
      }

   //
   //  if this is a variable, write the vars line
   //

   if( istype(sym,var) )
      write_vars(newvar,vallist,desc);

   //
   //  free some variables that are no longer needed
   //

   freelist(attlist);
   free(desc);

   //
   //  insert it into the list in alphabetical order
   //

   //  case 1: list is empty
   
   if( v_head == 0 ) 
      {
      v_head = newvar;
      write_varmap(newvar,vallist);
      freelist(vallist);
      return;
      }

   cur = v_head;
   check = strcasecmp(name,cur->str);

   if( check == 0 )
      FAULT("Multiple definitions of variable in MSGPROC_declare");

   //  case 2: insert before first existing entry

   if( check < 0 ) 
      {
      newvar->next = v_head;
      v_head = newvar;
      write_varmap(newvar,vallist);
      freelist(vallist);
      return;
      }

   //  case 3: anywhere else except the end

   for( nxt=cur->next ; nxt ; nxt=nxt->next )
      {
      check = strcasecmp(name,nxt->str);
      if( check == 0 )
         FAULT("Multiple definitions of variable in MSGPROC_declare");
      if( check  < 0 )
         {
         newvar->next = nxt;
         cur->next = newvar;
         write_varmap(newvar,vallist);
         freelist(vallist);
         return;
         }
      cur=nxt; 
      }

   //  case 4: append after last element

   cur->next = newvar;
   write_varmap(newvar,vallist);
   freelist(vallist);
}


//----------------------------------------------------------------------//
//
//  Begin an equation block
//
//----------------------------------------------------------------------//

void MSGPROC_begin_block(void *eq)
{
   List *esets,*eqnsets();
   int nblk,nstart,nscalar,nend;

   nblk    = MSGPROC_block++;
   nstart  = MSGPROC_scalar;
   nscalar = eqncount(eq);
   nend    = nstart+nscalar-1;

   MSGPROC_scalar = nend+1;
   
   esets = eqnsets(eq);
   
   fprintf(code,"// Equation block %d\n",nblk);

   if( islvalue(eq)==0 )
      msg_error("%s","LHS of an equation is not a variable");

   if( esets->n )
      fprintf(code,"//    Defined over sets (%s)\n",slprint(esets) );

   if( nscalar )
      fprintf(code,"//    Scalar equations %d-%d (%d total)\n\n",nstart,nend,nscalar);
   else
      fprintf(code,"//    Contains undeclared symbols\n");
}

//----------------------------------------------------------------------//
//
//  Show a symbol
//  
//----------------------------------------------------------------------//

char *MSGPROC_show_symbol(char *str, List *sublist, Context context)
{
   char *ptr;

   ptr = get_msgname(str,sublist,context) ;

   if( DBG )printf("MSGPROC_show_symbol: %s\n",ptr);
   
   return ptr;
}

//----------------------------------------------------------------------//
//
//  Connect up the public routines.
//  
//----------------------------------------------------------------------//

void Msgproc_setup(void)
{
   lang_begin_file ( MSGPROC_begin_file  );
   lang_end_file   ( MSGPROC_end_file    );
   lang_declare    ( MSGPROC_declare     ); 
   lang_begin_block( MSGPROC_begin_block ); 
   lang_show_symbol( MSGPROC_show_symbol );

   set_eqn_scalar();
   set_sum_scalar();
}

char *Msgproc_version = "$Revision: 57 $";

