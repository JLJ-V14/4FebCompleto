
#include "definiciones_globales.h"
#include "osqp.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>
/*Este es un subprograma auxiliar par comprobar si la informacion de vehiculos y baterias que tiene su
  carga de programacion terminales*/

void mostrar_informacion_carga_terminales(informacion_carga_terminales_t* informacion_terminales) {

  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {

    printf("El numero de terminal es %d", numero_terminal + 1);
    printf("El numero de elementos en el terminal es %d\n", informacion_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal);
    printf("El numero de vehículos en el terminal es %d\n", informacion_terminales->informacion_carga_terminales[numero_terminal].numero_vehiculos_terminal);
    printf("El numero de baterias en el terminal es %d\n", informacion_terminales->informacion_carga_terminales[numero_terminal].numero_baterias_terminal);
    int numero_elementos_terminales = informacion_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;



    for (int elemento_terminal = 0; elemento_terminal < numero_elementos_terminales; elemento_terminal++) {
      informacion_elemento_carga_t informacion_elemento_carga = informacion_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_terminal];
      printf("Hay una bateria:%s\n", informacion_elemento_carga.bateria ? "Si" : "No");
      printf("El porcentajde de bateria deseada es%f\n", informacion_elemento_carga.bateria_deseada);
      printf("El porcentaje de bateria inicial es%f\n", informacion_elemento_carga.bateria_inicial);
      printf("La capacidad de batería es%f\n", informacion_elemento_carga.capacidad_bateria);
      printf("La carga es rapida:%s\n", informacion_elemento_carga.carga_rapida?"Si":"No");
      printf("¿Se considera el objetivo de carga de la bateria?:%s\n", informacion_elemento_carga.considerar_objetivo?"Si":"No");
      printf("La potencia máxima es%f\n", informacion_elemento_carga.potencia_maxima);
      printf("La potencia minima es%f\n", informacion_elemento_carga.potencia_minima);
      printf("El punto final es %d\n ", informacion_elemento_carga.punto_final);
      printf("El punto inicial es %d\n ", informacion_elemento_carga.punto_inicio);
      printf("El punto objetivo es %d\n", informacion_elemento_carga.punto_objetivo);
      printf("Hay un vehiculo?: es%s\n", informacion_elemento_carga.vehiculo? "Si":"No");

    }

  }
}