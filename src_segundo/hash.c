#include "hash.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h> //ELIMINAR DESPUES

#define FACTOR_REHASH 2
#define FACTOR_CARGA_MAX 0.64
#define CAPACIDAD_MIN_HASH 3

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

typedef entrada_t* tabla_t;

// AUXILIARES

size_t funcion_hash(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + (unsigned long)c;
    return hash;
}

// Defino el criterio para rehashear.
bool rehasheo(hash_t *hash)
{   
    if ( hash_cantidad(hash) >= (double)hash->capacidad * FACTOR_CARGA_MAX) {
        return true;
    }
    return false;
    // return ( (float)hash->ocupados + 1 ) / (float)hash->capacidad >= FACTOR_CARGA_MAX
    //        ? true : false;
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

size_t posicion(const char *clave, size_t capacidad)
{
    return funcion_hash(clave) % capacidad;
}

entrada_t *nueva_entrada(const char *clave, void *elemento)
{
    entrada_t *nueva_entrada = malloc(sizeof(entrada_t));
    nueva_entrada->clave = duplicar_string(clave);
    nueva_entrada->elemento = elemento;
    nueva_entrada->siguiente = NULL;
    return nueva_entrada;
}

entrada_t *lista_insertar(entrada_t *head, const char *clave, void *elemento, void ***anterior, size_t *ocupados)
{
    if (!head) {
        if (anterior != NULL && (*anterior) != NULL) **anterior = NULL;
		(*ocupados)++;
        return nueva_entrada(clave, elemento);
    }

    entrada_t *iterador = head;

    // ITERO HASTA ENCONTRAR LA CLAVE O HASTA EL ÚLTIMO
    while (iterador->siguiente != NULL && strcmp(iterador->clave, clave) != 0) {
        iterador = iterador->siguiente;
    }

    if ( strcmp(iterador->clave, clave) == 0 ) {
        if (anterior != NULL && (*anterior) != NULL) **anterior = head->elemento;
		iterador->elemento = elemento;
        return head;
    }

    if (!iterador->siguiente) {
        iterador->siguiente = nueva_entrada(clave, elemento);
        if (anterior != NULL && (*anterior) != NULL) **anterior = NULL;
		(*ocupados)++;
        return head;
    }

    return head;
}

entrada_t *lista_quitar(entrada_t *head, const char *clave, void **quitado, size_t *ocupados)
{
    if (!head) return head;

    entrada_t *iterador = head;
    entrada_t *anterior = NULL;

    // ITERO HASTA ENCONTRAR LA CLAVE O HASTA NULL
    while (iterador != NULL && strcmp(iterador->clave, clave) != 0) {
        anterior = iterador;
        iterador = iterador->siguiente;
    }

    if (!iterador) return head;

    if ( strcmp(iterador->clave, clave) == 0) {
        *quitado = iterador->elemento;
        (*ocupados)--;
        if (!anterior) head = iterador->siguiente;
        else anterior->siguiente = iterador->siguiente;
        free((char *)iterador->clave);
        free(iterador);
    }

    return head;
}

entrada_t *lista_encontrar_elemento(entrada_t *head, const char *clave)
{
    if (!head) return NULL;

	if ( strcmp(head->clave, clave) == 0 ) return head;

	return lista_encontrar_elemento(head->siguiente, clave);
}

bool iterador(entrada_t *head, bool (*f)(const char *clave, void *valor, void *aux), void *aux, size_t *contador)
{
	entrada_t *iterador = head;
	
	while (iterador != NULL) {
		(*contador)++;
		if ( !f(iterador->clave, iterador->elemento, aux) ) return false;
		iterador = iterador->siguiente;
	}

	return true;
}

void destruir_tabla(entrada_t **tabla, size_t capacidad, void (*destructor)(void *))
{
    for (int i = 0; i < capacidad; i++) {

        while (tabla[i] != NULL) {

            entrada_t *iterador = tabla[i];
            entrada_t *anterior = NULL;

            while (iterador->siguiente != NULL) {
                anterior = iterador;
                iterador = iterador->siguiente;
            }

            if (destructor != NULL) destructor(iterador->elemento);
            if (!anterior) tabla[i] = NULL;
            else anterior->siguiente = NULL;
            free((char *)iterador->clave);
            free(iterador);
        }
    }

    free(tabla);
}



hash_t *hash_crear(size_t capacidad)
{
    hash_t *hash = calloc(1, sizeof(hash_t));
    if (!hash) return NULL;
    
    hash->capacidad = capacidad < CAPACIDAD_MIN_HASH ? CAPACIDAD_MIN_HASH : capacidad;

    entrada_t **tabla = calloc(hash->capacidad, sizeof(entrada_t *));
    hash->tabla = tabla;

    return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento, void **anterior)
{
    if (!hash) return NULL;

    // REHASH
    if ( rehasheo(hash) ) {

        printf("REHASH con %i\n", (int)hash->ocupados);

        size_t nueva_capacidad = hash->capacidad * FACTOR_REHASH;
        size_t nuevo_ocupados= 0;
        entrada_t **nueva_tabla = calloc(nueva_capacidad, sizeof(entrada_t *));

        size_t pos = 0;

        for (int i = 0; i < hash->capacidad; i++) {
            
            entrada_t *iterador = hash->tabla[i];
            entrada_t *xd = iterador;

            while (iterador != NULL) {
                pos = posicion(iterador->clave, nueva_capacidad);
                nueva_tabla[pos] = lista_insertar(nueva_tabla[pos], iterador->clave, iterador->elemento, NULL, &nuevo_ocupados);
                xd = iterador;
                iterador = iterador->siguiente;
                free((char *)xd->clave);
                free(xd);
            }
        }

        free(hash->tabla);

        hash->capacidad = nueva_capacidad;
        hash->ocupados = nuevo_ocupados;
        hash->tabla = nueva_tabla;
    }
    
    size_t pos = posicion(clave, hash->capacidad);
    hash->tabla[pos] = lista_insertar(hash->tabla[pos], clave, elemento, &anterior, &hash->ocupados);

    return hash;

}

void *hash_quitar(hash_t *hash, const char *clave)
{
    if (!hash) return NULL;

    size_t pos = posicion(clave, hash->capacidad);
    void *quitado = NULL;

    hash->tabla[pos] = lista_quitar(hash->tabla[pos], clave, &quitado, &hash->ocupados);

    return quitado;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
    entrada_t *a = NULL;

    int i = 0;
    while (i < hash->capacidad && !(a = lista_encontrar_elemento(hash->tabla[i], clave) ) ) i++;

    return !a ? NULL : a->elemento;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
    return !hash_obtener(hash, clave) ? false : true; 
}

size_t hash_cantidad(hash_t *hash)
{
    return hash->ocupados;
}

void hash_destruir(hash_t *hash)
{
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
	size_t contador = 0;

	int i = 0;
	while (i < hash->capacidad && iterador(hash->tabla[i], f, aux, &contador)) {
		i++;
	}

	return contador;
}