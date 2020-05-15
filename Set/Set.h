/*
 * Set.h
 *
 *  Created on: 15 may. 2020
 *      Author: Miguel Aguilar
 */

#ifndef SET_H_
#define SET_H_

#include <stdbool.h>

typedef struct stSet* Set;

typedef void* Type;
typedef int (*CompareFunc) (Type, Type);

//methods
Set set_create(CompareFunc);
int set_size(Set s);
bool set_add(Set s, Type t);
bool set_containsValue(Type X, Set s);
bool set_removeValue(Type t, Set s);
void set_printPreOrder(Set s);
void set_printInOrder(Set s);
void set_printPostOrder(Set s);
bool set_destroy(Set s);

int compareInts(Type, Type);

#endif /* SET_H_ */
