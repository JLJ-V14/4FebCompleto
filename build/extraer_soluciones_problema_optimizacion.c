
#include "definiciones_globales.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdlib.h>
#include <stdio.h>

int extraer_soluciones_problema_optimizacion(informacion_procesada_t* informacion_sistema, problema_optimizacion_t* problema_optimizacion) {

  

  //Se obtiene el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

 //Se obtiene el vector de solucion x
  OSQPFloat* x = problema_optimizacion->solver->solution->x;

 

  //Se abre un fichero CSV para escribir
  FILE* archivo_csv = fopen("resultados_optimizacion.csv", "w");

 

  //Se comprueba que se haya podido abrir el archivo correctamente
  if (archivo_csv == NULL) {
    printf("No se ha podido abrir el archivo de resultados de optimizacion correctamente\n");
    registrar_error("No se ha podido abrir el archivo de resultados de optimizacion correctamente\n", REGISTRO_ERRORES);
    return ERROR;
  }

  
  printf("MARCA\n");
  //Se indica que el csv de destino usada UTF-8
  
  fprintf(archivo_csv, "\xEF\xBB\xBF");
  
  //Se escribe los encabezados de cada variable
  
  fprintf(archivo_csv, "Numero Punto Simulacion,");
  fprintf(archivo_csv, "bateria terminal 1,");
  fprintf(archivo_csv, "bateria terminal 2,");
  fprintf(archivo_csv, "bateria terminal 3,");
  fprintf(archivo_csv, "bateria terminal 4,");
  fprintf(archivo_csv, "bateria terminal 5,");
  fprintf(archivo_csv, "bateria terminal 6,");
  fprintf(archivo_csv, "bateria terminal 7,");
  fprintf(archivo_csv, "bateria terminal 8,");
  fprintf(archivo_csv, "bateria terminal 9,");
  fprintf(archivo_csv, "bateria terminal 10,");
  fprintf(archivo_csv, "bateria terminal 11,");
  fprintf(archivo_csv, "bateria terminal 12,");
  fprintf(archivo_csv, "potencia terminal 1,");
  fprintf(archivo_csv, "potencia terminal 2,");
  fprintf(archivo_csv, "potencia terminal 3,");
  fprintf(archivo_csv, "potencia terminal 4,");
  fprintf(archivo_csv, "potencia terminal 5,");
  fprintf(archivo_csv, "potencia terminal 6,");
  fprintf(archivo_csv, "potencia terminal 7,");
  fprintf(archivo_csv, "potencia terminal 8,");
  fprintf(archivo_csv, "potencia terminal 9,");
  fprintf(archivo_csv, "potencia terminal 10,");
  fprintf(archivo_csv, "potencia terminal 11,");
  fprintf(archivo_csv, "potencia terminal 12,");
  fprintf(archivo_csv, "potencia red,");
  fprintf(archivo_csv, "potencia entrada sistema,");
  fprintf(archivo_csv, "potencia salida  sistema,");
  fprintf(archivo_csv, "potencia red fase R,");
  fprintf(archivo_csv, "potencia red fase S,");
  fprintf(archivo_csv, "potencia red fase T,");
  fprintf(archivo_csv, "potencia entrada sistema fase R,");
  fprintf(archivo_csv, "potencia entrada sistema fase S,");
  fprintf(archivo_csv, "potencia entrada sistema fase T,");
  fprintf(archivo_csv, "potencia salida  sistema fase R,");
  fprintf(archivo_csv, "potencia salida  sistema fase S,");
  fprintf(archivo_csv, "potencia salida  sistema fase T,");
  //Se pasa a la siguiente linea del csv
  fprintf(archivo_csv, "\n");

  /*

  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
    //Se imprime en el CSV el número de punto de simulacion
    fprintf(archivo_csv, "%d", punto_simulacion);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el termino de la bateria del terminal 1
    fprintf(archivo_csv, "%f", x[punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el termino de la bateria del terminal 2
    fprintf(archivo_csv, "%.f", x[1*numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el termino de la bateria del terminal 3
    fprintf(archivo_csv, "%.f", x[2*numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la batería del terminal 4
    fprintf(archivo_csv, "%.f", x[3 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la batería del termino 5
    fprintf(archivo_csv, "%.f", x[4 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la batería del termino 6
    fprintf(archivo_csv, "%.f", x[5 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la batería del termino 7
    fprintf(archivo_csv, "%.f", x[6 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el térmnio de la batería del termino 8
    fprintf(archivo_csv, "%.f", x[7 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la batería del termino 9
    fprintf(archivo_csv, "%.f", x[8 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la batería del termino 10
    fprintf(archivo_csv, "%.f", x[9 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la batería del termino 11
    fprintf(archivo_csv, "%.f", x[10 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la bateria del termino 12
    fprintf(archivo_csv, "%.f", x[11 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada por el terminal 1
    fprintf(archivo_csv, "%.f", x[12 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada por el terminal 2
    fprintf(archivo_csv, "%.f", x[13 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada por el terminal 3
    fprintf(archivo_csv, "%.f", x[14 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada por el terminal 4
    fprintf(archivo_csv, "&.f", x[15 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada por el terminal 5
    fprintf(archivo_csv, "%.f", x[16 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada por el terminal 6
    fprintf(archivo_csv, "%.f", x[17 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada por el terminal 7
    fprintf(archivo_csv, "%.f", x[18 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada por el terminal 8
    fprintf(archivo_csv, "%.f", x[19 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada por el terminal 9
    fprintf(archivo_csv, "%.f", x[20 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada por el terminal 10
    fprintf(archivo_csv, "%.f", x[21 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada por el terminal 11
    fprintf(archivo_csv, "%.f", x[22 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada por el terminal 12
    fprintf(archivo_csv, "%.f", x[23 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia intercambiada con la red
    fprintf(archivo_csv, "%.f", x[24 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia que entra al sistema
    fprintf(archivo_csv, "%.f", x[25 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de la potencia que sale del sistema
    fprintf(archivo_csv, "%.f", x[26 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de potencia que intercambia el sistema en fase R
    fprintf(archivo_csv, "%.f", x[27 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de potencia que intercambia el sistema en fase S
    fprintf(archivo_csv, "%.f", x[28 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de potencia que intercambia el sistema en fase T
    fprintf(archivo_csv, "%.f", x[29 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de potencia que entra a el sistema en fase R
    fprintf(archivo_csv, "%.f", x[30 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de potencia que entra a el sistema en fase S
    fprintf(archivo_csv, "%.f", x[31 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de potencia que entra a el sistema en fase T
    fprintf(archivo_csv, "%.f", x[32 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de potencia que sale el sistema en fase R
    fprintf(archivo_csv, "%.f", x[33 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de potencia que sale el sistema en fase S
    fprintf(archivo_csv, "%.f", x[34 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    //Se imprime en el csv el término de potencia que sale el sistema en fase T
    fprintf(archivo_csv, "%.f", x[35 * numero_puntos_simulacion + punto_simulacion]);
    fprintf(archivo_csv, ",");
    fprintf(archivo_csv, "\n");
  }
  //Se escribe los encabezados de las diferentes variables:
  //fprintf(archivo_csv,"NºPunto Simulacion")
  */
  fclose(archivo_csv);
  
  return EXITO;
}