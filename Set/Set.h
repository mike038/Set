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
int set_size(Set);
bool set_add(Set, Type);
bool set_agregar(Set, Type);
bool set_containsValue(Type, Set);
bool set_removeValue(Type, Set);
void set_printPreOrder(Set);
void set_printInOrder(Set);
void set_printPostOrder(Set);
bool set_destroy(Set);

int compareInts(Type, Type);

#endif /* SET_H_ */
