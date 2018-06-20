/*--------------------------------------------------------------------*
 *  cartproduct.h
 *
 *  $Id: cart.h 49 2018-06-16 17:15:39Z wilcoxen $
 *--------------------------------------------------------------------*/

#ifndef CART_H
#define CART_H

#include "lists.h"

List* cart_first(void);
List* cart_next(void);
void  cart_build(List *);

#endif /* CART_H */
