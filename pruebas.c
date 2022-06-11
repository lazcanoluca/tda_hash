#include "src/hash.h"
// #include "src/hash.c" // BORRAR DESPUES
#include "pa2m.h"

#include <stdlib.h>

// entrada_t *en_posicion(hash_t *hash, size_t posicion);

void pruebas_insercion_simple()
{
	hash_t *hash = hash_crear(10);

	void *anterior = NULL;

	char *mariano = (char *)malloc(8*sizeof(char));
	char *lucas = (char *)malloc(6*sizeof(char));
	char *manu = (char *)malloc(5*sizeof(char));

	mariano = "Mariano";
	lucas = "Lucas";
	manu = "Manu";
 
	pa2m_afirmar( hash_cantidad(hash) == 0, "El hash está vacío.");
	pa2m_afirmar( !!hash_insertar(hash, "AC123BD", mariano, &anterior), "Se insertó 'AC123BD'." );
	pa2m_afirmar( hash_cantidad(hash) == 1, "El hash tiene un elemento.");
	pa2m_afirmar( !!hash_insertar(hash, "OPQ976", lucas, &anterior), "Se insertó el 'OPQ976'." );
	pa2m_afirmar( hash_cantidad(hash) == 2, "El hash tiene dos elementos.");
	pa2m_afirmar( !!hash_insertar(hash, "A421ACB", manu, &anterior), "Se insertó el 'A421ACB'." );
	pa2m_afirmar( hash_cantidad(hash) == 3, "El hash tiene tres elementos.");

	pa2m_afirmar( hash_contiene(hash, "AC123BD"), "El hash contiene 'AC123BD'.");
	pa2m_afirmar( hash_obtener(hash, "AC123BD") == mariano, "El elemento de la clave 'AC123BD' es Mariano.");
	pa2m_afirmar( hash_contiene(hash, "OPQ976"), "El hash contiene 'OPQ976'.");
	pa2m_afirmar( hash_obtener(hash, "OPQ976") == lucas, "El elemento de la clave 'OPQ976' es Lucas.");
	pa2m_afirmar( hash_contiene(hash, "A421ACB"), "El hash contiene 'A421ACB'.");
	pa2m_afirmar( hash_obtener(hash, "A421ACB") == manu, "El elemento de la clave 'A421ACB' es Manu.");
	
	hash_destruir_todo(hash, NULL);
}

void pruebas_insercion_con_claves_repetidas()
{
	hash_t *hash = hash_crear(10);

	void *anterior = NULL;

	char *mariano = (char *)malloc(8*sizeof(char));
	char *lucas = (char *)malloc(6*sizeof(char));
	char *manu = (char *)malloc(5*sizeof(char));
	char *alen = (char *)malloc(5*sizeof(char));
	char *fabi = (char *)malloc(5*sizeof(char));
	char *fran = (char *)malloc(5*sizeof(char));

	mariano = "Mariano";
	lucas = "Lucas";
	manu = "Manu";
	alen = "Alen";
	fabi = "Fabi";
	fran = "Fran";

	pa2m_afirmar( hash_cantidad(hash) == 0, "El hash está vacío.");
	pa2m_afirmar( !!hash_insertar(hash, "AC123BD", mariano, &anterior), "Se insertó 'AC123BD'." );
	pa2m_afirmar( hash_cantidad(hash) == 1, "El hash tiene un elemento.");
	pa2m_afirmar( !!hash_insertar(hash, "OPQ976", lucas, &anterior), "Se insertó el 'OPQ976'." );
	pa2m_afirmar( hash_cantidad(hash) == 2, "El hash tiene dos elementos.");
	pa2m_afirmar( !!hash_insertar(hash, "A421ACB", manu, &anterior), "Se insertó el 'A421ACB'." );
	pa2m_afirmar( hash_cantidad(hash) == 3, "El hash tiene tres elementos.");

	pa2m_afirmar( hash_contiene(hash, "AC123BD"), "El hash contiene 'AC123BD'.");
	pa2m_afirmar( hash_obtener(hash, "AC123BD") == mariano, "El elemento de la clave 'AC123BD' es Mariano.");
	pa2m_afirmar( hash_contiene(hash, "OPQ976"), "El hash contiene 'OPQ976'.");
	pa2m_afirmar( hash_obtener(hash, "OPQ976") == lucas, "El elemento de la clave 'OPQ976' es Lucas.");
	pa2m_afirmar( hash_contiene(hash, "A421ACB"), "El hash contiene 'A421ACB'.");
	pa2m_afirmar( hash_obtener(hash, "A421ACB") == manu, "El elemento de la clave 'A421ACB' es Manu.");

	pa2m_afirmar( !!hash_insertar(hash, "AC123BD", alen, &anterior), "Se volvió a insertar 'AC123BD'." );
	// free(anterior);
	pa2m_afirmar( hash_cantidad(hash) == 3, "El hash sigue teniendo tres elementos.");
	pa2m_afirmar( hash_contiene(hash, "AC123BD"), "El hash contiene 'AC123BD'.");
	pa2m_afirmar( hash_obtener(hash, "AC123BD") == alen, "El elemento de la clave 'AC123BD' es Alen.");

	pa2m_afirmar( !!hash_insertar(hash, "OPQ976", fabi, &anterior), "Se volvió a insertar 'OPQ976'." );
	// free(anterior);
	pa2m_afirmar( hash_cantidad(hash) == 3, "El hash sigue teniendo tres elementos.");
	pa2m_afirmar( hash_contiene(hash, "OPQ976"), "El hash contiene 'OPQ976'.");
	pa2m_afirmar( hash_obtener(hash, "OPQ976") == fabi, "El elemento de la clave 'OPQ976' es Fabi.");

	pa2m_afirmar( !!hash_insertar(hash, "A421ACB", fran, &anterior), "Se volvió a insertar 'A421ACB'." );
	// free(anterior);
	pa2m_afirmar( hash_cantidad(hash) == 3, "El hash sigue teniendo tres elementos.");
	pa2m_afirmar( hash_contiene(hash, "A421ACB"), "El hash contiene 'A421ACB'.");
	pa2m_afirmar( hash_obtener(hash, "A421ACB") == fran, "El elemento de la clave 'A421ACB' es Fran.");

	hash_destruir_todo(hash, NULL);
	
}

int main()
{
	pa2m_nuevo_grupo("Pruebas insercion simple.");
	pruebas_insercion_simple();
	
	pa2m_nuevo_grupo("Pruebas inserciones repetidas.");
	pruebas_insercion_con_claves_repetidas();

	return pa2m_mostrar_reporte();
}
