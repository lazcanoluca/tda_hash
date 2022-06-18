#include "hash.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define FACTOR_REHASH 2
#define FACTOR_CARGA_MAX 0.66
#define CAPACIDAD_MIN_HASH 3

typedef struct entrada {
    char *clave;
    void *elemento;
    struct entrada *siguiente;
} entrada_t;

typedef struct lista {
    entrada_t *head;
    entrada_t *tail;
    size_t cantidad;
} lista_t;

struct hash {
    lista_t *tabla;
    size_t capacidad;
    size_t ocupados;
};


size_t funcion_hash(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + (unsigned long)c;
    return hash;
    // return (size_t)str[0];
}

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

entrada_t *nueva_entrada(const char *clave, void *elemento)
{
    if (!clave) return NULL;
    entrada_t *nueva_entrada = malloc(sizeof(entrada_t));
    nueva_entrada->clave = duplicar_string(clave);
    nueva_entrada->elemento = elemento;
    nueva_entrada->siguiente = NULL;
    return nueva_entrada;
}

bool debo_rehash(hash_t *hash)
{  
    if (!hash) return false;
    return ( hash_cantidad(hash) >= (double)hash->capacidad * FACTOR_CARGA_MAX )
           ? true : false;
}

bool iterador(lista_t lista, bool (*f)(const char *clave, void *valor, void *aux), void *aux, size_t *contador)
{
	entrada_t *iterador = lista.head;
	
	while (iterador != NULL) {
		(*contador)++;
		if ( !f(iterador->clave, iterador->elemento, aux) ) return false;
		iterador = iterador->siguiente;
	}

	return true;
}

bool bool_lista_encontrar_elemento(lista_t lista, const char *clave)
{
    entrada_t *iterador = lista.head;

    while ( iterador != NULL && strcmp(iterador->clave, clave) != 0 ) {
        iterador = iterador->siguiente;
    }

    if (!iterador) return false;

    if ( strcmp(iterador->clave, clave) == 0) {
        return true;
    }

    return false;
}

void *lista_encontrar_elemento(lista_t lista, const char *clave)
{
    entrada_t *iterador = lista.head;

    while ( iterador != NULL && strcmp(iterador->clave, clave) != 0 ) {
        iterador = iterador->siguiente;
    }

    if (!iterador) return NULL;

    if ( strcmp(iterador->clave, clave) == 0) {
        return iterador->elemento;
    }

    return NULL;
}

size_t posicion(const char *clave, size_t capacidad)
{
    return funcion_hash(clave) % capacidad;
}

lista_t lista_insertar(lista_t lista, const char *clave, void *elemento, void ***anterior, size_t *ocupados)
{
    if (lista.cantidad == 0) {
        (*ocupados)++;
        lista.head = nueva_entrada(clave, elemento);
        lista.tail = lista.head;
        lista.cantidad++;
        if (anterior != NULL && (*anterior) != NULL) **anterior = NULL;
        return lista;
    }

    if ( !bool_lista_encontrar_elemento(lista, clave) ) {
        (*ocupados)++;
        lista.tail->siguiente = nueva_entrada(clave, elemento);
        lista.tail = lista.tail->siguiente;
        lista.cantidad++;
        if (anterior != NULL && (*anterior) != NULL) **anterior = NULL;
        return lista;
    }

    entrada_t *iterador = lista.head;

    while ( strcmp(clave, iterador->clave) != 0 ) {
        iterador = iterador->siguiente;
    }

    if (anterior != NULL && (*anterior) != NULL) **anterior = iterador->elemento;
	iterador->elemento = elemento;
    
    return lista;
}

lista_t lista_quitar(lista_t lista, const char *clave, void **quitado, size_t *ocupados)
{
    if (lista.cantidad == 0) return lista;

    if ( !bool_lista_encontrar_elemento(lista, clave) ) return lista;

    entrada_t *iterador = lista.head;
    entrada_t *anterior = NULL;

    while ( strcmp(clave, iterador->clave) != 0) {
        anterior = iterador;
        iterador = iterador->siguiente;
    }

    *quitado = iterador->elemento;
    lista.cantidad--;
    (*ocupados)--;
    if (!iterador->siguiente) lista.tail = anterior;
    if (!anterior) lista.head = iterador->siguiente;
    else anterior->siguiente = iterador->siguiente;
    free((char *)iterador->clave);
    free(iterador);

    return lista;
}

hash_t *rehash(hash_t* hash)
{
    size_t nueva_capacidad = hash->capacidad * FACTOR_REHASH;
    size_t nuevo_ocupados = 0;
    lista_t *nueva_tabla = calloc(nueva_capacidad, sizeof(lista_t));

    size_t pos = 0;

    for (int i = 0; i < hash->capacidad; i++) {

        entrada_t *iterador = hash->tabla[i].head;
        entrada_t *aux = iterador;

        while (iterador != NULL) {
            pos = posicion(iterador->clave, nueva_capacidad);
            nueva_tabla[pos] = lista_insertar(nueva_tabla[pos], iterador->clave, iterador->elemento, NULL, &nuevo_ocupados);
            aux = iterador;
            iterador = iterador->siguiente;
            free((char *)aux->clave);
            free(aux);
        }
    }

    free(hash->tabla);
    hash->capacidad = nueva_capacidad;
    if ( nuevo_ocupados != hash->ocupados ) printf("estem xd");
    hash->tabla = nueva_tabla;

    return hash;
}

void destruir_tabla(lista_t *tabla, size_t capacidad, void (*destructor)(void *))
{
    for (int i = 0; i < capacidad; i++) {

        while ( tabla[i].head != NULL) {

            entrada_t *auxiliar = tabla[i].head;
            tabla[i].head = tabla[i].head->siguiente;

            if (destructor != NULL) destructor(auxiliar->elemento);
            free((char *)auxiliar->clave);
            free(auxiliar);

            tabla[i].cantidad--;
        }

        tabla[i].head = NULL;
        tabla[i].tail = NULL;
    }

    free(tabla);
}


hash_t *hash_crear(size_t capacidad)
{
    hash_t *hash = calloc(1, sizeof(hash_t));
    if (!hash) return NULL;
    
    hash->capacidad = capacidad < CAPACIDAD_MIN_HASH ? CAPACIDAD_MIN_HASH : capacidad;

    hash->tabla = calloc(hash->capacidad, sizeof(lista_t));
    if (!hash->tabla) {
        free(hash);
        return NULL;
    }

    return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento, void** anterior)
{
    if (!hash || !clave) return NULL;

    if ( debo_rehash(hash) ) hash = rehash(hash);

    size_t pos = posicion(clave, hash->capacidad);
    
    hash->tabla[pos] = lista_insertar(hash->tabla[pos], clave, elemento, &anterior, &hash->ocupados);

    // size_t cant;
    // if ((cant = hash_cantidad(hash)) % 4999 == 0) printf("%i\n", (int)cant);
    // if ((cant = hash_cantidad(hash)) < 50000 && cant > 49990) printf("%i\n", (int)cant);
    // if ((cant = hash_cantidad(hash)) > 50000) printf("%i\n", (int)cant);

    // if (hash_cantidad(hash) == 50001) hash->ocupados--;

    return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
    if (!hash || !clave) return NULL;

    void *quitado = NULL;

    size_t pos = posicion(clave, hash->capacidad);

    hash->tabla[pos] = lista_quitar(hash->tabla[pos], clave, &quitado, &hash->ocupados);

    return quitado;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
    if (!hash) return NULL;

    size_t pos = posicion(clave, hash->capacidad);

    return lista_encontrar_elemento(hash->tabla[pos], clave);
}

bool hash_contiene(hash_t *hash, const char *clave)
{
    if (!hash) return NULL;

    return !hash_obtener(hash, clave) ? false : true; 
}

size_t hash_cantidad(hash_t *hash)
{
    if (!hash) return 0;

    return hash->ocupados;
}

void hash_destruir(hash_t *hash)
{
    if (!hash) return;

    hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
    if (!hash) return;

    destruir_tabla(hash->tabla, hash->capacidad, destructor);

    free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
						   bool (*f)(const char *clave, void *valor, void *aux),
						   void *aux)
{
    if (!hash) return 0;

	size_t contador = 0;

	int i = 0;
	while (i < hash->capacidad && iterador(hash->tabla[i], f, aux, &contador)) {
		i++;
	}

	return contador;
}
