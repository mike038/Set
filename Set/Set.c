/*
 * Set.c
 *
 *  Created on: 15 may. 2020
 *      Author: Miguel Aguilar
 */

#include "Set.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

struct stNode {
	Type data;
	struct stNode *left;
	struct stNode *right;
	struct stNode *father;
};

typedef struct stNode *Node;

struct stSet {
	Node root;
	int size;
	CompareFunc comparefunc;
};

//prototipos de funciones locales, funciones privadas (recursivas).
bool set_contains(Type X, Node n, Set s);
bool set_remove(Type t, Node n, Set s);
Type set_remove_min(Node n, Set s);
void set_printPreO(Node n);
void set_printInO(Node n);
void set_printPostO(Node n);

//variables globales
int verbose = 1; //en 1 para imprimir, en 0 para no imprimir

Set set_create(CompareFunc cf) {
	Set s = malloc(sizeof(struct stSet));
	s->root = NULL;
	s->size = 0;
	s->comparefunc = cf;

	return s;
}

int set_size(Set s) {
	return s->size;
}

bool set_add(Set s, Type t) {
	//creando el nodo, y almacenando su información
	Node new_node = malloc(sizeof(struct stNode));
	new_node->data = t;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->father = NULL;

	int nivel = 0;

	if (s->root == NULL) {
		s->root = new_node;
		s->size++;
		if (verbose)
			printf("Dato agregado en el nivel %d. ROOT.\n", nivel);
		return true;
	} else {
		Node current = s->root;

		int cmp;
		cmp = s->comparefunc(new_node->data, current->data);
		while (cmp != 0) {
			nivel++; //aquí cambiamos de nivel

			//algoritmo para insertar el nodo, current se tiene que mover
			if (cmp < 0) //vamos por la izquierda
					{
				if (current->left) //si hay algo por la izquierda, avanzo el current

					current = current->left;
				else //(if current->left == NULL) si no, ya encontramos donde va el nodo, agregarlo
				{
					current->left = new_node;
					new_node->father = current;
					s->size++;
					if (verbose)
						printf("Dato agregado en el nivel %d. LEFT.\n", nivel);
					return true;
				}
			} else //vamos por la derecha
			{
				if (current->right) //si hay algo por la derecha, avanzo el current
					current = current->right;
				else //(if current->left == NULL) si no, ya encontramos donde va el nodo, agregarlo
				{
					current->right = new_node;
					new_node->father = current;
					s->size++;
					if (verbose)
						printf("Dato agregado en el nivel %d. RIGHT.\n", nivel);
					return true;
				}
			}

			cmp = s->comparefunc(new_node->data, current->data);
		}
		if (cmp == 0) {
			free(new_node);
			return false;
		}
	}
	return false;
}

int compareInts(Type t1, Type t2) {
	int *i1, *i2;
	i1 = (int*) t1;
	i2 = (int*) t2;
	return *i1 - *i2;
}

bool set_contains(Type X, Node n, Set s) {
	if (n == NULL)
		return false;
	else if (s->comparefunc(X, n->data) == 0)
		return true;
	else if (s->comparefunc(X, n->data) < 0)
		return set_contains(X, n->left, s);
	else if (s->comparefunc(X, n->data) > 0)
		return set_contains(X, n->right, s);
	return false;
}

bool set_containsValue(Type X, Set s) {
	return set_contains(X, s->root, s);
}

Type set_remove_min(Node n, Set s) {
	if (n->left == NULL) {
		Type data;
		data = n->data;
		if (n->right) {
			//presento al padre con el hijo antes de eliminar
			n->right->father = n->father;
			//ver n qué hijo es de su padre
			if (s->comparefunc(n->data, n->father->data) < 0) // es hijo izq
				n->father->left = n->right;
			else
				n->father->right = n->right;
		} else {
			//ver qué hijo es, para decirte al padre que se queda sin hijo
			if (s->comparefunc(n->data, n->father->data) < 0) // es hijo izq
				n->father->left = NULL;
			else
				n->father->right = NULL;

		}
		free(n);
		return data;
	} else
		return set_remove_min(n->left, s);
}

bool set_remove(Type t, Node n, Set s) {
	if (n != NULL) {
		if (s->comparefunc(t, n->data) > 0)
			return set_remove(t, n->right, s);
		else if (s->comparefunc(t, n->data) < 0)
			return set_remove(t, n->left, s);
		else {

			//si llegó a este punto, significa que encontró el valor
			//caso 1, no tiene hijos
			if (n->left == NULL && n->right == NULL) {
				//ver qué hijo es
				if (s->comparefunc(n->data, n->father->data) < 0)// es hijo izq
						{
					n->father->left = NULL;	//avisarle al padre que se queda sin hijo
					free(n);
				} else {
					n->father->right = NULL;//avisarle al padre que se queda sin hijo
					free(n);
				}
				s->size--;
				return true;
			}
			//caso donde tenga hijo derecho
			else if (n->left == NULL) {
				//presentar al hijo a su nuevo padre
				n->right->father = n->father;

				//ver n qué hijo es de su padre
				if (s->comparefunc(n->data, n->father->data) < 0)// es hijo izq
					//presentar al padre con el hijo izq
					n->father->left = n->right;
				else
					// es hijo der
					n->father->right = n->right;
				free(n);
				s->size--;
				return true;

			}
			//caso donde tenga hijo izquierdo
			else if (n->right == NULL) {
				//presentar al hijo a su nuevo padre
				n->left->father = n->father;

				//ver n qué hijo es de su padre
				if (s->comparefunc(n->data, n->father->data) < 0) // es hijo izq
					//presentar al padre con el hijo izq
					n->father->left = n->left;
				else
					// es hijo der
					n->father->right = n->left;

				free(n);

				s->size--;
				return true;
			}
			//caso donde tenga dos hijos
			else {
				n->data = set_remove_min(n->right, s);
				s->size--;
				return true;
			}
		}
	} else {
		return false;
	}
}

bool set_removeValue(Type t, Set s) {
	return set_remove(t, s->root, s);
}

void set_printPreO(Node n) {
	if (n != NULL) {
		printf("%d, ", *(int*) n->data);
		if (n->left)
			set_printPreO(n->left);
		if (n->right)
			set_printPreO(n->right);
	}
}

void set_printPreOrder(Set s) {
	set_printPreO(s->root);
}

void set_printInO(Node n) {
	if (n == NULL)
		return;
	set_printInO(n->left);
	printf("%d, ", *(int*) n->data);
	set_printInO(n->right);
}

void set_printInOrder(Set s) {
	set_printInO(s->root);
}

void set_printPostO(Node n) {
	if (n == NULL)
		return;
	set_printPostO(n->left);
	set_printPostO(n->right);
	printf("%d ", *(int*) n->data);
}
void set_printPostOrder(Set s){
	set_printPostO(s->root);
}
//bool set_destroy(Set s);


