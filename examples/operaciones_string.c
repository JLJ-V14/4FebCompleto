#include <ctype.h>
#include <locale.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//En este archivo, se recogen las
//funciones para operar con 
//strings.

//Defino la fila donde se encuentran los
//encabezados en los CSVs


bool strings_iguales(const char* String_1, const char* String_2) {
	//Este subprograma se utiliza para comprobar
	//que dos string son iguales, si lo son
	//se devuelve true si no se devuelve false.

	//Se configura que el lenguaje utilizado
	//es español y UTF-8 para los caracteres.

	//Ademas se configura que funcione tanto en 
	//Linux como en windows.
#ifdef _WIN32
#define strcasecmp _stricmp
#define wcsdup _wcsdup
#endif



	char* u1 = strdup(String_1);
	char* u2 = strdup(String_2);

	if (u1 == NULL || u2 == NULL) {
		free(u1);
		free(u2);
		return false;
	}

	for (size_t i = 0; u1[i] != '\0'; i++) {
		u1[i] = toupper(u1[i]);
	}

	for (size_t i = 0; u2[i] != '\0'; i++) {
		u2[i] = toupper(u2[i]);
	}

	int Resultado = strcoll(u1, u2);

	free(u1);
	free(u2);

	return Resultado == 0;
}
