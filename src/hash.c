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

/*
 * Hashea un string dado, donde el puntero no puede ser nulo.
 * 
 * Devuelve un número size_t.
 */
size_t funcion_hash(const char *str)
{
        unsigned long hash = 5381;
        int c;

        while ((c = *str++))
                hash = ((hash << 5) + hash) + (unsigned long)c;

        return hash;
}

/*
 * Recibe un string y devuelve una copia, asignándole memoria. En caso de ser nulo,
 * devuelve NULL.
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

/*
 * Crea una nueva entrada con una clave y un elemento. Si la clave es un puntero nulo,
 * devuelve NULL. Sino, asigna memoria y devuelve la dirección de la nueva entrada.
 */
entrada_t *nueva_entrada(const char *clave, void *elemento)
{
        if (!clave)
                return NULL;

        entrada_t *nueva_entrada = malloc(sizeof(entrada_t));
        if (!nueva_entrada)
                return NULL;

        nueva_entrada->clave = duplicar_string(clave);
        nueva_entrada->elemento = elemento;
        nueva_entrada->siguiente = NULL;

        return nueva_entrada;
}

/*
 * Dado un hash no nulo, devuelve true si se debe rehashear según las condiciones explicitadas,
 * sino false.
 * Si el hash es nulo, devuelve false.
*/
bool debo_rehash(hash_t *hash)
{  
        if (!hash)
                return false;

        return ( hash_cantidad(hash) >= (double)hash->capacidad * FACTOR_CARGA_MAX )
                ? true : false;
}

/*
 * Dada una lista, itera sobre cada elemento, ejecutando la función pasada si no es nula sobre cada
 * elemento. Devuelve true si la función nunca devolvió false, o false cuando la función devuelva
 * false para algún elemento. Actualiza la cantidad de elementos iterados en el contador pasado.
 */
bool iterador(lista_t lista, bool (*f)(const char *clave, void *valor, void *aux), void *aux, size_t *contador)
{
        entrada_t *iterador = lista.head;

        while (iterador != NULL) {
                (*contador)++;

                if ( !f(iterador->clave, iterador->elemento, aux) )
                        return false;

                iterador = iterador->siguiente;
        }

        return true;
}

/*
 * Dada una lista y una clave no nula, devuelve true si el elemento está en la lista, y false
 * si no lo está.
 */
bool lista_contiene_elemento(lista_t lista, const char *clave)
{
        entrada_t *iterador = lista.head;

        while ( iterador != NULL && strcmp(iterador->clave, clave) != 0 )
                iterador = iterador->siguiente;

        if (!iterador)
                return false;

        if ( strcmp(iterador->clave, clave) == 0 )
                return true;

        return false;
}

/*
 * Dada una lista y una clave no nula, devuelve el elemento correspondiente a la clave dada,
 * o NULL si no se encuentra el elemento.
 */
void *lista_encontrar_elemento(lista_t lista, const char *clave)
{
        entrada_t *iterador = lista.head;

        while (iterador != NULL && strcmp(iterador->clave, clave) != 0)
                iterador = iterador->siguiente;

        if (!iterador)
                return NULL;

        if (strcmp(iterador->clave, clave) == 0)
                return iterador->elemento;

        return NULL;
}

/*
 * Dada una clave no nula y la capacidad del hash, devuelve la posición de la tabla a la cual se
 * se hasheó la clave.
 */
size_t posicion(const char *clave, size_t capacidad)
{
    return funcion_hash(clave) % capacidad;
}

/*
 * Dada una lista, una clave no nula, un elemento, un puntero y un contador, inserta la clave
 * y el elemento en la lista si no se encuentra. Aumenta el contador. Si la clave ya se
 * encuentra almacenada actualiza el elemente y devuelve el elemento anterior en el puntero pasado.
 * Devuelve la lista actualizada.
 */
lista_t lista_insertar(lista_t lista, const char *clave, void *elemento, void ***anterior, size_t *ocupados)
{
        if (lista.cantidad == 0 || !lista_contiene_elemento(lista, clave)) {
                (*ocupados)++;

                if (lista.cantidad == 0) {
                        lista.head = nueva_entrada(clave, elemento);
                        lista.tail = lista.head;    
                } else {        
                        lista.tail->siguiente = nueva_entrada(clave, elemento);
                        lista.tail = lista.tail->siguiente;
                }

                lista.cantidad++;

                if (anterior != NULL && (*anterior) != NULL)
                        **anterior = NULL;
                
                return lista;
        }

        entrada_t *iterador = lista.head;

        while ( strcmp(clave, iterador->clave) != 0 )
                iterador = iterador->siguiente;

        if (anterior != NULL && (*anterior) != NULL)
                **anterior = iterador->elemento;

        iterador->elemento = elemento;

        return lista;
}

/*
 * Dada una lista, una clave no nula, un elemento, un puntero y un contador, elimina la clave
 * y el elemento si se encuentran. Decrementa el contador. Si la clave no se encuentra, devuelve
 * la lista.
 * Devuelve la lista actualizada.
 */
lista_t lista_quitar(lista_t lista, const char *clave, void **quitado, size_t *ocupados)
{
        if (lista.cantidad == 0 || !lista_contiene_elemento(lista, clave))
                return lista;

        entrada_t *iterador = lista.head;
        entrada_t *anterior = NULL;

        while (strcmp(clave, iterador->clave) != 0) {
                anterior = iterador;
                iterador = iterador->siguiente;
        }

        *quitado = iterador->elemento;
        (*ocupados)--;
        lista.cantidad--;

        if (!iterador->siguiente) {
                lista.tail = anterior;
                if (anterior != NULL) anterior->siguiente = NULL;
        }

        if (!anterior)
                lista.head = iterador->siguiente;
        else
                anterior->siguiente = iterador->siguiente;

        free((char *)iterador->clave);
        free(iterador);

        return lista;
}

/*
 * Dado un hash no nulo, reacomoda todos sus elementos en una tabla más grande y devuelve
 * el hash ampliado con todos sus elementos correctamente ubicados.
 */
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
        hash->tabla = nueva_tabla;
        hash->ocupados = nuevo_ocupados;

        return hash;
}

/*
 * Dada una tabla no nula, la capacidad de la tabla y opcionalmente una función destructora,
 * destruye todas las listas de la tabla, cada elementos si la función no es nula, y la tabla,
 * liberando toda la memoria.
 */
void destruir_tabla(lista_t *tabla, size_t capacidad, void (*destructor)(void *))
{
        for (int i = 0; i < capacidad; i++) {

                while ( tabla[i].head != NULL) {
                        entrada_t *auxiliar = tabla[i].head;
                        tabla[i].head = tabla[i].head->siguiente;

                        if (destructor != NULL)
                                destructor(auxiliar->elemento);
                                
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
        if (!hash)
                return NULL;

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
        if (!hash || !clave)
                return NULL;

        if (debo_rehash(hash))
                hash = rehash(hash);

        size_t pos = posicion(clave, hash->capacidad);

        hash->tabla[pos] = lista_insertar(hash->tabla[pos], clave, elemento, &anterior, &hash->ocupados);

        return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
        if (!hash || !clave)
                return NULL;

        void *quitado = NULL;

        size_t pos = posicion(clave, hash->capacidad);

        hash->tabla[pos] = lista_quitar(hash->tabla[pos], clave, &quitado, &hash->ocupados);

        return quitado;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
        if (!hash)
                return NULL;

        size_t pos = posicion(clave, hash->capacidad);

        return lista_encontrar_elemento(hash->tabla[pos], clave);
}

bool hash_contiene(hash_t *hash, const char *clave)
{
        if (!hash)
                return NULL;

        return !hash_obtener(hash, clave) ? false : true; 
}

size_t hash_cantidad(hash_t *hash)
{
        if (!hash)
                return 0;

        return hash->ocupados;
}

void hash_destruir(hash_t *hash)
{
        if (!hash)
                return;

        hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
        if (!hash)
                return;

        destruir_tabla(hash->tabla, hash->capacidad, destructor);

        free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
                           bool (*f)(const char *clave, void *valor, void *aux),
                           void *aux)
{
        if (!hash)
                return 0;

        size_t contador = 0;

        int i = 0;

        while (i < hash->capacidad && iterador(hash->tabla[i], f, aux, &contador))
                i++;

        return contador;
}
