//----------------------------------------------------------------------
//  symbol.h
//
//  Core include file for the program.  Used by both front and 
//  backends.
//  
//  $Id: sym.h 59 2018-06-16 20:56:55Z wilcoxen $
//----------------------------------------------------------------------

#ifndef SYMBOL_H
#define SYMBOL_H

//  
//  Values for obj fields in structures to help catch problems
//  

#define LISTOBJ 1993
#define ITEMOBJ 1959
#define NODEOBJ 1991
#define SYMBOBJ 2004


//----------------------------------------------------------------------
//
//  Error and debugging
//
//----------------------------------------------------------------------

//  
//  Debugging and other flags
//  

extern int debug;
extern int debugforce;
extern int intertemporal;
extern int mergeonly;
extern int only_first;
extern int only_last;
extern int do_scalars;
extern int do_calc;

#define DBG ((debug && myDEBUG)||debugforce)

#endif /* SYMBOL_H */
