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

void pruebas_rehash()
{
	hash_t *hash = hash_crear(3);

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

	pa2m_afirmar( !!hash_insertar(hash, "AC123BD", mariano, &anterior), "Se insertó 'AC123BD'." );
	pa2m_afirmar( hash_cantidad(hash) == 1, "El hash tiene un elemento.");
	pa2m_afirmar( !!hash_insertar(hash, "OPQ976", lucas, &anterior), "Se insertó el 'OPQ976'." );
	pa2m_afirmar( hash_cantidad(hash) == 2, "El hash tiene dos elementos.");
	pa2m_afirmar( !!hash_insertar(hash, "A421ACB", manu, &anterior), "Se insertó el 'A421ACB'." );
	pa2m_afirmar( hash_cantidad(hash) == 3, "El hash tiene tres elementos.");
	pa2m_afirmar( !!hash_insertar(hash, "EFGHI", alen, &anterior), "Se insertó 'EFGHI'." );
	pa2m_afirmar( hash_cantidad(hash) == 4, "El hash tiene cuatro elementos.");
	pa2m_afirmar( !!hash_insertar(hash, "JKLMN", fabi, &anterior), "Se insertó el 'JKLMN'." );
	pa2m_afirmar( hash_cantidad(hash) == 5, "El hash tiene cinco elementos.");
	pa2m_afirmar( !!hash_insertar(hash, "OPQRST", fran, &anterior), "Se insertó el 'OPQRST'." );
	pa2m_afirmar( hash_cantidad(hash) == 6, "El hash tiene seis elementos.");

	hash_destruir_todo(hash, NULL);
}

void pruebas_de_insercion_replica_chanu()
{
	hash_t *hash = hash_crear(3);

	pa2m_afirmar( !!hash_insertar(hash, "CLAVE1", "1", NULL), "Inserto <CLAVE1,1>");
	pa2m_afirmar( !!hash_insertar(hash, "CLAVE2", "2", NULL), "Inserto <CLAVE2,2>");
	pa2m_afirmar( !!hash_insertar(hash, "CLAVE3", "3", NULL), "Inserto <CLAVE3,3>");
	pa2m_afirmar( !!hash_insertar(hash, "CLAVE4", "4", NULL), "Inserto <CLAVE4,4>");
	pa2m_afirmar( hash_contiene(hash, "CLAVE1"), "Busco CLAVE1");
	pa2m_afirmar( hash_contiene(hash, "CLAVE2"), "Busco CLAVE2");
	pa2m_afirmar( hash_contiene(hash, "CLAVE3"), "Busco CLAVE3");
	pa2m_afirmar( hash_contiene(hash, "CLAVE4"), "Busco CLAVE4");
	pa2m_afirmar( !!hash_insertar(hash, "CLAVE1", "5", NULL), "Inserto <CLAVE1,5>");
	pa2m_afirmar( !!hash_insertar(hash, "CLAVE2", "6", NULL), "Inserto <CLAVE2,6>");
	pa2m_afirmar( hash_contiene(hash, "CLAVE1"), "Busco CLAVE1");
	pa2m_afirmar( hash_contiene(hash, "CLAVE2"), "Busco CLAVE2");
	pa2m_afirmar( hash_contiene(hash, "CLAVE3"), "Busco CLAVE3");
	pa2m_afirmar( hash_contiene(hash, "CLAVE4"), "Busco CLAVE4");
	pa2m_afirmar( !!hash_insertar(hash, "CLAVE1", "7", NULL), "Inserto <CLAVE1,7>");
	pa2m_afirmar( !!hash_insertar(hash, "CLAVE2", "8", NULL), "Inserto <CLAVE2,8>");
	pa2m_afirmar( hash_contiene(hash, "CLAVE1"), "Busco CLAVE1");
	pa2m_afirmar( hash_contiene(hash, "CLAVE2"), "Busco CLAVE2");
	pa2m_afirmar( hash_contiene(hash, "CLAVE3"), "Busco CLAVE3");
	pa2m_afirmar( hash_contiene(hash, "CLAVE4"), "Busco CLAVE4");
	pa2m_afirmar( hash_cantidad(hash) == 4, "La cantidad de elementos del hash es 4.");

}

int main()
{
	pa2m_nuevo_grupo("Pruebas insercion simple.");
	pruebas_insercion_simple();
	
	pa2m_nuevo_grupo("Pruebas inserciones repetidas.");
	pruebas_insercion_con_claves_repetidas();

	pa2m_nuevo_grupo("Pruebas donde se rehashea.");
	pruebas_rehash();

	pa2m_nuevo_grupo("Pruebas de inserción");
	pruebas_de_insercion_replica_chanu();

	return pa2m_mostrar_reporte();
}
