/*--------------------------------------------------------------------*
 *  OUTPUT.C 1.00
 *  9 Apr 90 (PJW)
 *
 *  Write out the model calling language-dependent functions
 *  implemented in external files.
 *--------------------------------------------------------------------*/

#include "output.h"

#include "cart.h"
#include "codegen.h"
#include "eqns.h"
#include "error.h"
#include "lists.h"
#include "nodes.h"
#include "options.h"
#include "sets.h"
#include "str.h"
#include "sym.h"
#include "symtable.h"
#include "wprint.h"
#include "xmalloc.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define myDEBUG 1

//
//  cached information about format of output
//

static int linelength = 0;

//
//  structures for holding information about the current equation
//

static void show_eq(void *, List *, List *);
static char *show_node(Nodetype, Node *, List *, List *);
static char *show_symbol(char *, List *, List *, List *, Context);

/*--------------------------------------------------------------------*
 *  show_symbol
 *
 *  Validate arguments and then call the language-specific routine
 *  to display a parameter or variable.  Inputs are the following:
 *
 *     sym     = symbol to be displayed
 *     vsets   = subsets of symbol's sets relevant to this reference
 *     esets   = list of sets over which the equation is defined
 *     esubs   = list of subscripts for this individual equation
 *     context = context of the current symbol
 *
 *  Key activity of this routine is to figures out the applicable
 *  subscripts for this symbol in the current equation.
 *
 *  The sets for this vector equation are "esets" and the particular
 *  subscripts for this scalar equation are "esubs"; each element of
 *  esubs is a member of the corresponding set in esets.
 *
 *  The symbol is defined over sets "vsets" (in this context; elements of
 *  vsets may be subsets of the variable's original sets).
 *
 *  This bit of code goes through vsets, finds the matching equation set in
 *  esets, and then picks out the corresponding subscript from esubs.
 *
 *  All this is a bit complicated because vsets and esets are not guaranteed
 *  to be in any particular order.  That is, parsing of the file to this
 *  point guarantees that there will be exactly one entry in esets matching
 *  each entry in vsets, but they need not be in the same order and vsets
 *  need not include all the sets in esets.
 *
 *  Example: suppose set "regions" consists of {r1, r2}, the set "goods"
 *  consists of {g1, g2}, variables p, q and v are defined over regions and
 *  goods, and variable tax is defined over regions.  Consider the following
 *  equation:
 *
 *     v = p*(1+tax)*q
 *
 *  This will generate 4 scalar equations.  For each equation, esets will be
 *  "regions,goods" but esubs will vary:
 *
 *     1) r1,g1
 *     2) r1,g2
 *     3) r2,g1
 *     4) r2,g2
 *
 *  When calling this routine on tax in scalar equation 2, the code matches the
 *  variable set "region" to the first equation set, and then picks out the
 *  corresponding element of esubs, which is "r1".
 *
 *  The finished list of subscripts for this symbol is stored in vsubs.
 *
 *--------------------------------------------------------------------*/
static char *show_symbol(name, vsets, esets, esubs, context)
char *name;
List *vsets, *esets, *esubs;
Context context;
{
   void *sym;
   List *vsubs;
   Item *vset, *eset, *esub;
   char *str;
   int n, isvec;

   sym = lookup(name);

   //
   //  check arguments
   //

   validate(sym, SYMBOBJ, "show_symbol");
   validate(vsets, LISTOBJ, "show_symbol (vsets)");
   validate(esets, LISTOBJ, "show_symbol (esets)");
   validate(esubs, LISTOBJ, "show_symbol (esubs)");

   if (!istype(sym, par) && !istype(sym, var))
      FAULT("Unexpected symbol type in show_symbol");

   if (DBG)
   {
      printf("show_symbol %s lhs=%d dt=%d\n", name, context.lhs, context.dt);
      printf("vsets %s\n", slprint(vsets));
   }

   //
   //  for vector equations, pick out the elements of esets that apply
   //  to this variable.  for scalar equations, choose the appropriate
   //  subscripts for this variable from the list for the equation
   //

   isvec = is_eqn_vector();
   vsubs = newsequence();
   n = 0;

   if (isvec)
      for (vset = vsets->first; vset; vset = vset->next)
         for (eset = esets->first; eset; eset = eset->next)
            if (strcasecmp(vset->str, eset->str) == 0 || isaliasof(eset->str, vset->str))
            {
               addlist(vsubs, eset->str);
               n++;
            }

   if (!isvec)
      for (vset = vsets->first; vset; vset = vset->next)
         for (eset = esets->first, esub = esubs->first; eset; eset = eset->next, esub = esub->next)
            if (strcasecmp(vset->str, eset->str) == 0 || isaliasof(eset->str, vset->str))
            {
               if (issubset(vset->str, "time") || isaliasof(vset->str, "time"))
                  context.tsub = strdup(esub->str);
               addlist(vsubs, esub->str);
               n++;
            }

   //
   //  if we didn't find exactly the right number of subscripts, there
   //  must be a logic error in getsets():
   //
   //   * if n is too small, the equation's domain omits one of the
   //     sets over which the variable is defined (eg, equation over
   //     regions, variable over regions,sectors);
   //
   //   * if n is too large, the equation's domain includes both a set
   //     and its alias (eg, equation defined over regions,to).
   //
   //  check here to make sure neither happened
   //

   if (n != vsets->n)
      FAULT("Inconsistent number of subscripts in show_symbol");

   //
   //  all's well: call the backend routine
   //

   str = codegen_show_symbol(name, vsubs, context);

   freelist(vsubs);
   return str;
}

/*--------------------------------------------------------------------*
 *  write_file
 *
 *  Write out a file in the selected language.  This is the only
 *  public method in the file.
 *--------------------------------------------------------------------*/
void write_file(char *basename)
{
   void *sym;
   void *eq;
   List *eqnsets(), *eqsets;
   int eqncount();

   if (DBG)
      printf("write_file\n");

   //
   //  Retrieve options relevant to file format
   //

   linelength = get_line_length();

   //
   //  Allow the language module to set options and write
   //  any introductory information to the output file.
   //

   codegen_begin_file(basename);
   if (DBG)
      xcheck("after begin_file");

   //
   //  Some options do not have defaults and MUST be set
   //  by the language module.
   //

   if (is_eqn_set() == 0)
      FAULT("Equation style has not been set");
   if (is_sum_set() == 0)
      FAULT("Summation style has not been set");

   if (DBG)
   {
      printf("   eqn style: ");
      printf("scalar=%d ", is_eqn_scalar());
      printf("vector=%d\n", is_eqn_vector());
      printf("   sum style: ");
      printf("scalar=%d ", is_sum_scalar());
      printf("vector=%d\n", is_sum_vector());
   }

   //
   //  Tell the language module about the symbols.  Pass it
   //  all the sets first, then the parameters, and then the
   //  variables.
   //

   for (sym = firstsymbol(set); sym; sym = nextsymbol(sym))
      codegen_declare(sym);

   for (sym = firstsymbol(par); sym; sym = nextsymbol(sym))
      codegen_declare(sym);

   for (sym = firstsymbol(var); sym; sym = nextsymbol(sym))
      codegen_declare(sym);

   if (DBG)
      xcheck("after declares");

   //
   //  Now generate the equations.  The language module is
   //  allowed to write a preamble to each equation block, and
   //  to write a prefix and suffix to each equation, but the
   //  the main equation-writing is done by by show_eq.
   //

   for (eq = firsteqn(); eq; eq = nexteqn(eq))
   {
      List *sublist;

      if (hasundec(eq) || !istimeok(eq))
         continue;

      eqsets = eqnsets(eq);

      codegen_begin_block(eq);

      if (is_eqn_vector())
      {
         sublist = newsequence();
         show_eq(eq, eqsets, sublist);
         freelist(sublist);
      }
      else
      {
         int neqns;
         neqns = eqncount(eq);
         cart_build(eqsets);
         while (sublist = cart_next())
         {
            show_eq(eq, eqsets, sublist);
            neqns--;
         }
         if (neqns)
            FAULT("Incorrect number of equations written. Using # with a time set?");
      }

      eqsets = freelist(eqsets);
   }

   if (DBG)
      xcheck("after equations");

   //
   //  All done; allow the language module to write a postscript
   //

   codegen_end_file();
   if (DBG)
      xcheck("after end_file");

   fclose(code);
   fclose(info);
}

/*--------------------------------------------------------------------*
 *  wrap_write
 *
 *  write a line to the code file but wrap it to keep the line from
 *  being too long.
 *--------------------------------------------------------------------*/
void wrap_write(char *line, int addcr, int commaok)
{
   char *end, op, *dup, *rest;
   int do_wrap;

   dup = strdup(line);
   rest = dup;

   while (rest)
   {

      if (strlen(rest) <= linelength)
      {
         fprintf(code, rest);
         if (addcr)
            fprintf(code, "\n");
         return;
      }

      if (strcspn(rest, "\n") <= linelength)
      {
         end = strchr(rest, '\n');
         *end++ = '\0';
         fprintf(code, "%s\n", rest);
         rest = end;
         continue;
      }

      for (end = rest + linelength; end > rest; end--)
      {
         do_wrap = isspace(*end);
         do_wrap |= (*end == '+' || *end == '-' || *end == '*' || *end == '/' || *end == '=' || *end == '^');
         do_wrap |= (commaok && *end == ',');
         if (do_wrap)
         {
            op = *end;
            *end = '\0';
            fprintf(code, "%s\n   ", rest);
            *end = op;
            break;
         }
      }

      if (rest == end)
         fatal_error("Could not wrap long line:\n%s\n", line);

      rest = end;
   }

   free(dup);
}

/*--------------------------------------------------------------------*
 *  show_eq
 *
 *  Generate and print a scalar equation by recursively descending
 *  through the node tree.
 *--------------------------------------------------------------------*/
void show_eq(void *eq, List *setlist, List *sublist)
{
   Node *getlhs(), *getrhs();
   char *lstr, *rstr, *all;
   char *head, *tail;

   lstr = show_node(nul, getlhs(eq), setlist, sublist);
   rstr = show_node(nul, getrhs(eq), setlist, sublist);

   codegen_begin_eqn(eq);

   if (is_eqn_normalized())
      all = concat(4, lstr, " - (", rstr, ")");
   else
      all = concat(3, lstr, " = ", rstr);

   free(lstr);
   free(rstr);

   if (linelength == 0)
   {
      fprintf(code, all);
      free(all);
      codegen_end_eqn(eq);
      return;
   }

   if (strlen(all) <= linelength)
      fprintf(code, all);
   else
   {
      for (head = all; tail = strchr(head, '\n'); head = tail)
      {
         *tail++ = '\0';
         wrap_write(head, 1, 0);
      }
      wrap_write(head, 0, 0);
   }

   free(all);
   codegen_end_eqn(eq);
}

/*--------------------------------------------------------------------*
 *  show_node
 *
 *  Generate the node recursively.  This allocates and frees a
 *  lot of small chunks of memory as it constructs nodes from the
 *  bottom up.  Avoids having any fixed buffer sizes but is a
 *  bit tedious as a result.
 *--------------------------------------------------------------------*/
static char *show_node(prevtype, cur, setlist, sublist)
Nodetype prevtype;
Node *cur;
List *setlist, *sublist;
{
   int parens, wrap_right;
   char *buf, *newbuf;
   List *augsets, *augsubs, *sumover;
   char *beginfunc, *endfunc;
   char *lstr, *rstr, *cr;
   char *lpar, *rpar;
   char *op, *thisop;
   int isfunc;
   char *side;
   Context mycontext;

   if (cur == 0)
      return strdup("");

   mycontext.lhs = cur->lhs;
   mycontext.dt = cur->dt;
   mycontext.tsub = 0;

   side = mycontext.lhs ? "lhs" : "rhs";
   if (DBG)
      printf("show_node (%s)\n", side);

   validate(cur, NODEOBJ, "show_node");
   validate(setlist, LISTOBJ, "show_block for setlist");
   validate(sublist, LISTOBJ, "show_block for sublist");

#define now(arg) (cur->type == arg)

   //
   //  decide whether the current node should be wrapped with
   //  parentheses.  in principle this shouldn't be necessary
   //  if all software correctly obeyed precedence rules but
   //  we'll do it just out of paranoia (and experience with
   //  gauss).
   //
   //  the choice is determined by the type of the node one
   //  step higher in the parse tree.
   //

   parens = 0;
   switch (prevtype)
   {
   case nul:
   case add:
   case sub:
      if (now(neg))
         parens = 1;
      break;

   case mul:
      if (now(add) || now(sub))
         parens = 1;
      if (now(dvd) || now(neg))
         parens = 1;
      break;

   case neg:
      parens = 1;
      if (now(nam) || now(num) || now(mul))
         parens = 0;
      if (now(log) || now(exp) || now(pow))
         parens = 0;
      if (now(lag) || now(led))
         parens = 0;
      if (now(sum) || now(prd))
         parens = 0;
      break;

   case dvd:
      parens = 1;
      if (now(nam) || now(num) || now(pow))
         parens = 0;
      if (now(sum) || now(prd))
         parens = 0;
      if (now(lag) || now(led))
         parens = 0;
      if (now(log) || now(exp))
         parens = 0;
      break;

   case pow:
      parens = 1;
      if (now(nam) || now(num) || now(log) || now(exp))
         parens = 0;
      if (now(sum) || now(prd))
         parens = 0;
      if (now(lag) || now(led))
         parens = 0;
      break;

   case log:
   case exp:
   case lag:
   case led:
   case sum:
   case prd:
   case nam:
   case num:
   case equ:
   case dom:
      break;

   default:
      FAULT("Invalid state reached in show_node");
   }

   //
   //  now construct the current node
   //

   //
   //  case 1: a few straightforward items
   //

   switch (cur->type)
   {
   case nam:
      return show_symbol(cur->str, cur->domain, setlist, sublist, mycontext);

   case lag:
   case led:
      return show_node(cur->type, cur->r, setlist, sublist);

   case dom:
      return show_node(cur->type, cur->l, setlist, sublist);

   case lst:
      FAULT("Unexpected lst state in show_node");
   }

   //
   //  case 2: sum and product, scalar form
   //

   if (cur->type == sum || cur->type == prd)
      if (is_sum_scalar())
      {
         Item *ele;

         if (DBG)
            printf("scalar sum or product: %s\n", snprint(cur));

         lstr = (cur->l)->str;

         augsets = newsequence();
         catlist(augsets, setlist);
         addlist(augsets, lstr);

         op = (cur->type == prd) ? "*" : "+";
         lpar = (cur->type == prd) ? "(" : "";
         rpar = (cur->type == prd) ? ")" : "";

         buf = strdup("(");
         thisop = " ";

         sumover = setelements(lstr);
         for (ele = sumover->first; ele; ele = ele->next)
         {
            augsubs = newsequence();
            catlist(augsubs, sublist);
            addlist(augsubs, ele->str);

            if (DBG)
            {
               printf("calling show_node for %s\n", ele->str);
               printf("augsets = %s\n", slprint(augsets));
            }

            rstr = show_node(cur->type, cur->r, augsets, augsubs);
            newbuf = concat(6, buf, "\n      ", thisop, lpar, rstr, rpar);

            thisop = op;

            free(buf);
            free(rstr);
            freelist(augsubs);

            buf = newbuf;
         }

         newbuf = concat(2, buf, ")");
         free(buf);

         freelist(augsets);
         freelist(sumover);

         return newbuf;
      }

   //
   //  case 3: sum or product in vector form
   //

   if (cur->type == sum || cur->type == prd)
   {
      if (DBG)
         printf("vector sum or product: %s\n", snprint(cur));

      lstr = (cur->l)->str;

      augsets = newsequence();
      catlist(augsets, setlist);
      addlist(augsets, lstr);

      augsubs = newsequence();
      catlist(augsubs, sublist);
      addlist(augsubs, "*");

      beginfunc = codegen_begin_func(cur->str, lstr);
      rstr = show_node(cur->type, cur->r, augsets, augsubs);
      endfunc = codegen_end_func();

      buf = concat(3, beginfunc, rstr, endfunc);

      free(beginfunc);
      free(rstr);
      free(endfunc);

      freelist(augsubs);
      freelist(augsets);

      return buf;
   }

   //
   //  case 4: everything else
   //

   switch (cur->type)
   {
   case log:
   case exp:
      isfunc = 1;
      lstr = codegen_begin_func(cur->str, 0);
      endfunc = codegen_end_func();
      op = "";
      break;

   // GCS 2022-12-15 Modified power operator to ** instead of ^
   case pow:
      isfunc = 0;
      lstr = show_node(cur->type, cur->l, setlist, sublist);
      endfunc = strdup("");
      op = "**";
      break;

   default:
      isfunc = 0;
      lstr = show_node(cur->type, cur->l, setlist, sublist);
      endfunc = strdup("");
      op = cur->str;
   }

   rstr = show_node(cur->type, cur->r, setlist, sublist);

   cr = "";
   if (strlen(lstr) + strlen(rstr) > 70 || strlen(lstr) > 40 || strlen(rstr) > 40)
      // GCS 2022-12-15 modified cr string to handle Python whitespace issues.
      cr = " \\\n        ";
   //   cr = "\n   ";

   lpar = (parens && isfunc == 0) ? "(" : "";
   rpar = (parens && isfunc == 0) ? ")" : "";

   wrap_right = 0;
   if (cur->type == sub)
      if (cur->r->type == add || cur->r->type == sub)
         wrap_right = 1;

   if (wrap_right)
      buf = concat(9, lpar, lstr, cr, op, "(", rstr, ")", rpar, endfunc);
   else
      buf = concat(7, lpar, lstr, cr, op, rstr, rpar, endfunc);

   free(lstr);
   free(rstr);
   free(endfunc);

   return buf;
}

/*-------------------------------------------------------------------*
 *  listsymbols
 *
 *  Print out a list of all entries in the symbol table.
 *-------------------------------------------------------------------*/
void listsymbols()
{
   void *cur, *wp;
   char fmt[1024];
   int namelen, max_namelen;
   char *title, *desc, *base;
   List *attr;
   Symboltype type, types[3];
   int i;

   max_namelen = 0;

   types[0] = set;
   types[1] = par;
   types[2] = var;

   for (i = 0; i < 3; i++)
   {
      type = types[i];
      for (cur = firstsymbol(type); cur; cur = nextsymbol(cur))
      {
         namelen = strlen(symname(cur));
         if (namelen > max_namelen)
            max_namelen = namelen;
      }
   }

   sprintf(fmt, "   %%-%ds  :  %%s  : ", max_namelen);

   wp = new_wprint(info, 72);

   for (i = 0; i < 3; i++)
   {

      type = types[i];
      switch (type)
      {
      case set:
         title = "Sets";
         break;
      case par:
         title = "Parameters";
         break;
      case var:
         title = "Variables";
         break;
      default:
         FAULT("Unexpected type in listsymbols");
      }
      fprintf(info, "\n%s:\n\n", title);

      for (cur = firstsymbol(type); cur; cur = nextsymbol(cur))
      {
         title = symname(cur);
         fprintf(info, "   %s\n", title);

         if (type == set)
         {
            fprintf(info, "      Base set: ");
            base = findbase(title);
            if (strcmp(base, title) == 0)
               fprintf(info, "self\n");
            else
               fprintf(info, "%s\n", base);
         }

         desc = symdescrip(cur);
         attr = symattrib(cur);
         if (strlen(desc) || attr->n)
         {
            fprintf(info, "      ");
            fprintf(info, "%s", desc);
            if (strlen(desc))
               fprintf(info, " ");
            if (attr->n > 0)
               fprintf(info, "(%s)", slprint(attr));
            fprintf(info, "\n");
         }

         wprint_start(wp, "         ");
         wprint_list(wp, symvalue(cur), ",");

         wprint_end(wp);
         fprintf(info, "\n");

         free(title);
         free(desc);
         freelist(attr);
      }
   }

   free_wprint(wp);
}
