#include "hash.h"

#include "xxhash.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FACTOR_REHASH 2
#define FACTOR_CARGA_MAXIMO 0.75
#define DIVISOR_CLAVES 123456789

typedef struct entrada {
	const char *clave;
	void *elemento;
	struct entrada *siguiente;
} entrada_t;

struct hash {
	entrada_t **tabla;
	size_t capacidad;
	size_t ocupados;
};

// FUNCIONES HASH

XXH32_hash_t hash_string(const char* string, XXH32_hash_t seed)
{
    // NULL pointers are only valid if the length is zero
    size_t length = (string == NULL) ? 0 : strlen(string);
    return XXH32(string, length, seed);
}

size_t funcion_hash(const char *clave)
{
	size_t auxiliar = 0;
	// size_t pos;
	for (int i = 0; i <= strlen(clave); i++) {
		auxiliar += ( (size_t)(clave[i])) % DIVISOR_CLAVES;
	}
	// pos = auxiliar % hash->tamanio;
	return auxiliar;
}

// static unsigned long funcion_hash(const char* clave)
// {
// 	unsigned long hash = 0;
// 	int c;

// 	while ((c = *clave++)) {
// 		hash = c + (hash << 6) + (hash << 16) - hash;
// 	}

// 	return hash;
// }

// size_t funcion_hash(const char *clave) {
// 	// return (size_t)funcion_hash_hash(clave);
// 	// return funcion_hash(clave);
// 	// return (size_t)hash_string(clave, 0);
// 	// return (size_t)clave[0];
// }

// size_t funcion_hash(const char* str){
//   int hash = 0;
//   int c;
//   while ((c = *str++))
//     hash = c + (hash << 6) + (hash << 16) - hash;
// 	return (size_t)hash;
// }

float factor_de_carga(hash_t *hash)
{
	return (float)hash->ocupados / (float)hash->capacidad;
}

/*
// hash_t *rehash(hash_t *hash)
// {
// 	printf("rehashing... \n");
// 	hash_t *nuevo_hash = hash_crear(hash->capacidad * 2);
// 	// hash_t *retorno;

// 	for (int i = 0; i < hash->capacidad; i++) {

// 		entrada_t *iterador = hash->tabla[i];

// 		while (iterador != NULL) {
// 			printf("a insertar: %s\n", iterador->clave);
// 			nuevo_hash = hash_insertar(nuevo_hash, iterador->clave, iterador->elemento, NULL);
// 			iterador = iterador->siguiente;
// 		}

// 	}

// 	nuevo_hash->ocupados = hash->ocupados;
// 	printf("ocupados nuevo hash: %i \n", (int)hash_cantidad(nuevo_hash));
// 	// hash_destruir(hash);
// 	return nuevo_hash;

// }
*/

char *duplicar_string(const char *s)
{
	if (!s)
		return NULL;

	char *p = malloc(strlen(s) + 1);
	if (!p)
		return NULL;

	strcpy(p, s);
	return p;
}

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	hash->capacidad = capacidad;

	entrada_t **tabla = calloc(capacidad, sizeof(entrada_t *));
	hash->tabla = tabla;
	
	return hash;
}

/* RECURSIVA
entrada_t *lista_insertar(entrada_t *head, const char *clave, void *elemento, void ***anterior, bool *repetido, size_t *ocupados)
{
	if (!head) {
		entrada_t *nueva_entrada = malloc(sizeof(entrada_t));
		nueva_entrada->clave = clave;
		nueva_entrada->elemento = elemento;
		nueva_entrada->siguiente = NULL;
		if (anterior != NULL && (*anterior) != NULL) **anterior = NULL;
		(*ocupados)++;
		return nueva_entrada;
	}

	// if ((char *)head->clave == (char *)clave) {
	if (strcmp(head->clave, clave) == 0) {
		free((char *)clave); // acá en vez de dupliar el strng en la entrada, debería duplicarlo acá.
		*repetido = true;
		// printf("pasó a true\n");
		// printf("head->clave = %p, clave = %p\n", head->clave, clave);
		if (anterior != NULL && (*anterior) != NULL) **anterior = head->elemento;
		head->elemento = elemento;
		return head;
	}

	head->siguiente = lista_insertar(head->siguiente, clave, elemento, anterior, repetido, ocupados);
	return head;
}
*/


// Recibe la cabeza de una nueva lista. A medida que recorra toda la lista, verifica
// que la clave no coincida.
// Si no coincide, inserta al final, normal como lista.
// Si coincide, apunta anterior al elemento. Luego actualiza el elemento al nuevo elemento.
// ITERATIVA
entrada_t *lista_insertar(entrada_t *head, const char *clave, void *elemento, void ***anterior, bool *repetido, size_t *ocupados)
{
	// SI LA LISTA ESTÁ VACÍA
	if (!head) {
		entrada_t *nueva_entrada = malloc(sizeof(entrada_t));
		nueva_entrada->clave = clave;
		nueva_entrada->elemento = elemento;
		nueva_entrada->siguiente = NULL;
		if (anterior != NULL && (*anterior) != NULL) **anterior = NULL;
		(*ocupados)++;
		return nueva_entrada;
	}

	entrada_t *iterador = head;

	while (iterador->siguiente != NULL && strcmp(iterador->clave, clave) != 0) {
		iterador = iterador->siguiente;
	}

	// SI ENCUENTRA LA CLAVE
	if (strcmp(iterador->clave, clave) == 0) {
		// free(nueva_entrada);
		free((char *)clave); // acá en vez de dupliar el strng en la entrada, debería duplicarlo acá.
		*repetido = true;
		// printf("pasó a true\n");
		// printf("head->clave = %p, clave = %p\n", head->clave, clave);
		if (anterior != NULL && (*anterior) != NULL) **anterior = head->elemento;
		iterador->elemento = elemento;
		return head;
	}

	// SI NO ENCUENTRA LA CLAVE, ENTONCES AGREGA AL FINAL
	if (!iterador->siguiente) {
		entrada_t *nueva_entrada = malloc(sizeof(entrada_t));
		nueva_entrada->clave = clave;
		nueva_entrada->elemento = elemento;
		nueva_entrada->siguiente = NULL;
		iterador->siguiente = nueva_entrada;
		if (anterior != NULL && (*anterior) != NULL) **anterior = NULL;
		(*ocupados)++;
		return head;
	}

	return head;
}



hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento, void **anterior)
{
	if (!hash) return NULL;

	if ( ((float)hash->ocupados + 1) / (float)hash->capacidad >= FACTOR_CARGA_MAXIMO) {
		// printf("\n\nfactor de carga: %f \n", ((float)hash->ocupados + 1) / (float)hash->capacidad);

		hash_t *nuevo_hash = hash_crear(hash->capacidad * FACTOR_REHASH);

		for (int i = 0; i < hash->capacidad; i++) {

			entrada_t *iterador = hash->tabla[i];

			while (iterador != NULL) {
				// printf("\na insertar recursivamente: <%s, %p>\n", iterador->clave, iterador->clave);
				nuevo_hash = hash_insertar(nuevo_hash, iterador->clave, iterador->elemento, NULL);
				iterador = iterador->siguiente;
			}

		}
		// free(hash->tabla);
		entrada_t **aux = hash->tabla;
		hash->tabla = nuevo_hash->tabla;
		nuevo_hash->tabla = aux;

		size_t aux2 = hash->capacidad;
		hash->capacidad = nuevo_hash->capacidad;
		nuevo_hash->capacidad = aux2;

		size_t aux3 = hash->ocupados;
		hash->ocupados = nuevo_hash->ocupados;
		nuevo_hash->ocupados = aux3;

		// free(nuevo_hash->tabla);
		// free(nuevo_hash);
		hash_destruir_todo(nuevo_hash, NULL);
	}
	
	size_t posicion = funcion_hash(clave) % hash->capacidad;

	bool repetido = false;
	// printf("pasó a false");
	// printf("\na insertar: <%s, %p>\n", clave, clave);
	// hash->tabla[posicion] = lista_insertar(hash->tabla[posicion], clave, elemento, &anterior, &repetido, &hash->ocupados);
	hash->tabla[posicion] = lista_insertar(hash->tabla[posicion], duplicar_string(clave), elemento, &anterior, &repetido, &hash->ocupados);

	// if (!anterior || !(*anterior)) hash->ocupados++;
	// if (!repetido) {
	// 	printf("sumo");
	// 	hash->ocupados++;
	// }

	// printf("\nocupados hash: %i\n", (int)hash_cantidad(hash));
	return hash;
}

/*
hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento, void **anterior)
{
	size_t posicion = funcion_hash(clave) % hash->capacidad;
	// printf("%i\n", (int)posicion);

	entrada_t *nueva_entrada = malloc(sizeof(entrada_t));
	nueva_entrada->clave = clave;
	nueva_entrada->elemento = elemento;
	nueva_entrada->siguiente = NULL;

	// funcion de lista pero le doy el head de la lista, no la lista.
	entrada_t *lista = lista_insertar(hash->tabla[posicion], nueva_entrada, &anterior);

	// if (!lista) return NULL;

	hash->tabla[posicion] = lista;
	if (!(*anterior)) hash->ocupados++;
	// printf("%s/n", hash->tabla[posicion]->clave);
	// hash->tabla[] = lista;

	return hash;
}
*/

// ITERATIVA
entrada_t *lista_quitar(entrada_t *head, const char *clave, void **quitado)
{
	entrada_t *iterador = head;
	entrada_t *anterior = NULL;

	while (iterador != NULL && strcmp(iterador->clave, clave) != 0) {
		anterior = iterador;
		iterador = iterador->siguiente;
	}

	if (!iterador) return head;

	// if (iterador->clave == clave) {
	if ( strcmp(iterador->clave, clave) == 0) {
		*quitado = iterador->elemento;
		if (!anterior) head = iterador->siguiente;
		else anterior->siguiente = iterador->siguiente;
		free((char *)iterador->clave);
		free(iterador);
	}

	return head;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	// printf("ACA");
	size_t posicion = funcion_hash(clave) % hash->capacidad;
	void *quitado = NULL;

	entrada_t *lista = lista_quitar(hash->tabla[posicion], clave, &quitado);

	// if (!lista) return NULL;

	hash->tabla[posicion] = lista;
	if (quitado != NULL) hash->ocupados--;

	// printf("QUITADO: %s\n", (const char *)quitado);
	return quitado;
}

entrada_t *recorrer_hasta_encontrar(entrada_t *head, const char *clave)
{
	if (!head) return NULL;

	// if (head->clave == clave) return head;
	if ( strcmp(head->clave, clave) == 0 ) return head;

	return recorrer_hasta_encontrar(head->siguiente, clave);
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	entrada_t *a = NULL;

	for (int i = 0; i < hash->capacidad; i++) {
		a = recorrer_hasta_encontrar(hash->tabla[i], clave);
		if (a != NULL) {
			return a->elemento;
		};
	}
	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	entrada_t *a = NULL;
	// printf("\nBUSCANDO CLAVE: %s\n", clave);
	
	for (int i = 0; i < hash->capacidad; i++) {
		a = recorrer_hasta_encontrar(hash->tabla[i], clave);
		if (a != NULL) {
			// printf("ENCONTRE LA CLAVE :D %s\n", a->clave);
			return true;
		};
	}
	// printf("NO ENCONTRE LA CLAVE D:");
	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	return hash->ocupados;
}

void hash_destruir(hash_t *hash)
{
	// if (!!hash) {
	// 	free(hash->tabla);
	// 	free(hash);
	// }
	hash_destruir_todo(hash, NULL);
}

entrada_t *lista_quitar_y_destruir_ultimo(entrada_t *head, void (*destructor)(void *))
{
	if (!head) return NULL;

	if (!head->siguiente) {
		if (destructor != NULL) destructor(head->elemento);
		// destructor = destructor;
		// free(head->elemento);
		free((char *)head->clave);
		free(head);
		return NULL;
	}

	head->siguiente = lista_quitar_y_destruir_ultimo(head->siguiente, destructor);
	
	return head;
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	for (int i = 0; i < hash->capacidad; i++) {

		while (hash->tabla[i] != NULL) {
			hash->tabla[i] = lista_quitar_y_destruir_ultimo(hash->tabla[i], destructor);
		}

	}
	free(hash->tabla);
	free(hash);
	// hash_destruir(hash);
}

/*
entrada_t *iterador(entrada_t *head,
					bool (*f)(const char *clave, void *valor, void *aux),
					void *aux,
					size_t *contador,
					bool *seguir)
{
	if (!head) return head;

	(*contador)++;
	if ( !f(head->clave, head->elemento, aux) ) {
		*seguir = false;
		return head;
	}

	head->siguiente = iterador(head->siguiente, f, aux, contador, seguir);
	return head;
}
*/

bool iterador(entrada_t *head,
			  bool (*f)(const char *clave, void *valor, void *aux),
			  void *aux,
			  size_t *contador)
{
	entrada_t *iterador = head;
	
	while (iterador != NULL) {
		(*contador)++;
		if ( !f(iterador->clave, iterador->elemento, aux) ) return false;
		iterador = iterador->siguiente;
	}

	return true;
}

size_t hash_con_cada_clave(hash_t *hash,
						   bool (*f)(const char *clave, void *valor, void *aux),
						   void *aux)
{
	size_t contador = 0;

	int i = 0;
	while (i < hash->capacidad && iterador(hash->tabla[i], f, aux, &contador)) {
		i++;
	}

	return contador;
}
