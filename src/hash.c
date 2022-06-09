#include "hash.h"

#include <stdlib.h>

// typedef struct entrada {
// 	const char *clave;
// 	void *elemento;
// 	struct entrada *siguiente;
// } entrada_t;

// struct hash {
// 	entrada_t **tabla;
// 	size_t capacidad;
// 	size_t ocupados;
// };

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	hash->capacidad = capacidad;

	entrada_t **tabla = calloc(capacidad, sizeof(entrada_t *));
	hash->tabla = tabla;
	
	return hash;
}

size_t funcion_hash(const char *clave) {
	return (size_t)clave[0];
}

// Recibe la cabeza de una nueva lista. A medida que recorra toda la lista, verifica
// que la clave no coincida.
// Si no coincide, inserta al final, normal como lista.
// Si coincide, apunta anterior al elemento. Luego actualiza el elemento al nuevo elemento.
entrada_t *lista_insertar(entrada_t *head, entrada_t *nueva_entrada, void ***anterior)
{
	if (!head) return nueva_entrada;

	entrada_t *iterador = head;

	while (iterador->siguiente != NULL && iterador->clave != nueva_entrada->clave) {
		iterador = iterador->siguiente;
	}

	if (!iterador->siguiente) {
		iterador->siguiente = nueva_entrada;
		nueva_entrada->siguiente = NULL;
	}

	if (iterador->clave == nueva_entrada->clave) {
		anterior = iterador->elemento;
		iterador->elemento = nueva_entrada->elemento;
	}

	return head;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento, void **anterior)
{
	size_t posicion = funcion_hash(clave) % hash->capacidad;

	entrada_t *nueva_entrada = malloc(sizeof(entrada_t));
	nueva_entrada->clave = clave;
	nueva_entrada->elemento = elemento;
	nueva_entrada->siguiente = NULL;

	// funcion de lista pero le doy el head de la lista, no la lista.
	entrada_t *lista = lista_insertar(hash->tabla[posicion], nueva_entrada, &anterior);

	if (!lista) return NULL;

	hash->tabla[posicion] = lista;

	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	return 0;
}

void hash_destruir(hash_t *hash)
{
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
}

size_t hash_con_cada_clave(hash_t *hash,
						   bool (*f)(const char *clave,
						   void *valor, void *aux),
						   void *aux)
{
	return 0;
}