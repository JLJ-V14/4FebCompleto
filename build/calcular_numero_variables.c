#include "definiciones_globales.h"
#include "tipos_optimizacion.h"

//Subprograma para calcular el numero de variables que existen en el problema de optimizacion.

void calcular_numero_variables(informacion_procesada_t* informacion_sistema,OSQPInt* numero_variables ) {

  //Para calcular el numero de variables es necesario saber cuantos puntos de simulacion se tienen en total
  OSQPInt numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //El numero de variables totales es el siguiente
  //12-> Potencia que intercambia cada estación de carga
  //12-> Estado de la bateria que tiene cada terminal (ya sea vehículo o no)
  //1->  Potencia que intercambia la red con el sistema
  //3->  Potencia que intercambia la red con el sistema por cada fase
  //1->  Potencia que entra al sistema
  //1->  Potencia que sale  del sistema
  //3->  Potencia que entra al sistema por cada fase
  //3->  Potencia que sale del sistema por cada fase
  //Numero total de variables = (12 + 12 + 1 + 3 + 1 + 1 + 3 + 3) * Numero_puntos_simulacion
   * numero_variables = 36 * (informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion);

}