#include "hash.h"

#include <stdlib.h>
#include <stdio.h>

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

hash_t *rehash(hash_t *hash)
{
	return NULL;
}

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
		**anterior = iterador->elemento;
		iterador->elemento = nueva_entrada->elemento;
	}

	return head;
}

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
	// printf("%s/n", hash->tabla[posicion]->clave);
	// hash->tabla[] = lista;

	return hash;
}

entrada_t *lista_quitar(entrada_t *head, const char *clave, void **quitado)
{
	entrada_t *iterador = head;
	entrada_t *anterior = NULL;

	while (iterador != NULL && iterador->clave != clave) {
		anterior = iterador;
		iterador = iterador->siguiente;
	}

	if (!iterador) return head;

	if (iterador->clave == clave) {
		*quitado = iterador->elemento;
		if (!anterior) head = iterador->siguiente;
		else anterior->siguiente = iterador->siguiente;
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

	// printf("QUITADO: %s\n", (const char *)quitado);
	return quitado;
}

entrada_t *recorrer_hasta_encontrar(entrada_t *head, const char *clave)
{
	if (!head) return NULL;

	if (head->clave == clave) return head;

	return recorrer_hasta_encontrar(head->siguiente, clave);
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	entrada_t *a = NULL;
	printf("\nBUSCANDO CLAVE: %s\n", clave);

	for (int i = 0; i < hash->capacidad; i++) {
		a = recorrer_hasta_encontrar(hash->tabla[i], clave);
		if (a != NULL) {
			printf("ENCONTRE LA CLAVE :D %s\n", a->clave);
			return a->elemento;
		};
	}
	printf("NO ENCONTRE LA CLAVE D:");
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
	return 0;
}

void hash_destruir(hash_t *hash)
{
	if (!!hash) free(hash);
}

entrada_t *lista_quitar_y_destruir_ultimo(entrada_t *head, void (*destructor)(void *))
{
	if (!head) return NULL;

	if (!head->siguiente) {
		destructor(head->elemento);
		// destructor = destructor;
		// free(head->elemento);
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
	hash_destruir(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
						   bool (*f)(const char *clave, void *valor, void *aux),
						   void *aux)
{
	return 0;
}