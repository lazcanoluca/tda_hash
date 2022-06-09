#include "src/hash.h"
// #include "src/hash.c" // BORRAR DESPUES
#include "pa2m.h"

// entrada_t *en_posicion(hash_t *hash, size_t posicion);

void pruebas_insercion()
{
	hash_t *hash = hash_crear(10);

	void *anterior = NULL;
	// pa2m_afirmar(hash_insertar(hash, ''))

	pa2m_afirmar( !!hash_insertar(hash, "AC123BD", "Auto de Mariano", &anterior), "Se insertó el elemento." );
	pa2m_afirmar( !!hash_insertar(hash, "OPQ976", "Auto de Lucas", &anterior), "Se insertó el elemento." );
	pa2m_afirmar( !!hash_insertar(hash, "A421ACB", "Moto de Manu", &anterior), "Se insertó el elemento." );
	// printf("%i\n", 'A');
	// printf("%i\n", 65 % 10);
	// printf("%i\n", 'O');
	// printf("%i\n", 79 % 10);
	// printf("Clave: %s, elemento: %s, posicion en el hash: %i", en_posicion(hash, 65 % 10)->clave, (char *)en_posicion(hash, 65 % 10)->elemento, 65%10);
	// printf("Clave: %s, elemento: %s, posicion en el hash: %i\n", hash->tabla[65%10]->clave, (char *)hash->tabla[65%10]->elemento, 65%10);
	// printf("Clave: %s, elemento: %s, posicion en el hash: %i\n", hash->tabla[65%10]->siguiente->clave, (char *)hash->tabla[65%10]->siguiente->elemento, 65%10);
	// printf("Clave: %s, elemento: %s, posicion en el hash: %i\n", hash->tabla[79%10]->clave, (char *)hash->tabla[79%10]->elemento, 79%10);
	// pa2m_afirmar( hash_insertar(hash, "AA442CD", "Auto de Guido", &anterior), "Se insertó el elemento." );
	// pa2m_afirmar( hash_insertar(hash, "AC152AD", "Auto de Agustina", &anterior), "Se insertó el elemento." );
	// pa2m_afirmar( hash_insertar(hash, "DZE443", "Auto de Jonathan", &anterior), "Se insertó el elemento." );
	// pa2m_afirmar( hash_insertar(hash, "AA436BA", "Auto de Gonzalo", &anterior), "Se insertó el elemento." );
	// pa2m_afirmar( hash_insertar(hash, "QDM443", "Auto de Daniela", &anterior), "Se insertó el elemento." );
	// pa2m_afirmar( hash_insertar(hash, "BD123AC", "Auto de Pablo", &anterior ), "Se insertó el elemento.");
	// pa2m_afirmar( hash_insertar(hash, "CD442AA", "Auto de Micaela", &anterior), "Se insertó el elemento." );
	// pa2m_afirmar( hash_insertar(hash, "PQO697", "Auto de Juan", &anterior), "Se insertó el elemento." );
	// pa2m_afirmar( hash_insertar(hash, "DZE443", "Auto de Jonathan otra vez", &anterior), "Se insertó el elemento." );
	// pa2m_afirmar( hash_insertar(hash, "AC152AD", "Auto de Agustina otra vez", &anterior), "Se insertó el elemento." );

}

int main()
{
	pa2m_nuevo_grupo("Pruebas insercion");
	pruebas_insercion();

	return pa2m_mostrar_reporte();
}
