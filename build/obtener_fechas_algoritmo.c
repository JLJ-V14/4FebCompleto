#include <stdio.h>
#include <time.h>

/*Se utiliza el siguiente subprograma para obtener la fecha actual */

void obtener_fecha_actual(struct tm *fecha_actual) {

  time_t tiempo_actual;

 
  //Se obtiene el tiempo actual
  time(&tiempo_actual);
  *fecha_actual = *localtime(&tiempo_actual);

 

}