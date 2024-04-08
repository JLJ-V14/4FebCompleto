
#include "definiciones_globales.h"
#include "osqp.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>
/*Este es un subprograma auxiliar par comprobar si la informacion de vehiculos y baterias que tiene su
  carga de programacion terminales*/

void mostrar_informacion_carga_terminales(informacion_carga_terminales_t* informacion_terminales) {

  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {

  
    int numero_elementos_terminales = informacion_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;



    for (int elemento_terminal = 0; elemento_terminal < numero_elementos_terminales; elemento_terminal++) {
      informacion_elemento_carga_t informacion_elemento_carga = informacion_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_terminal];
     

    }

  }
}