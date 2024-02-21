#include "definiciones_globales.h"
#include "osqp.h"
#include "tipos_optimizacion.h"

/*En este subprograma se utiliza el numero de restricciones que existen en el problema de optimizacion*/

int calcular_numero_restricciones(informacion_procesada_t* informacion_sistema,OSQPInt* numero_restricciones) {
  //Primero se hace un recopilatorio de cuantas restricciones en total se necesitan
  //Cuenta->
  //12-> Baterías de los vehículos umbral minimos y maximos
  //12-> Potencia que pueden intercambiar las estaciones de cargas máximos y mínimos
  //1->  Potencia mínima y máxima que pueden intercambiar la red con el sistema
  //1->  Potencia mínima y máxima de potencia que puede entrar al sistema
  //1->  Potencia mínima y máxima que puede salir del sistema
  //3->  Potencia mínima y máxima que puede intercambiar la red con el sistema por cada fase
  //3->  Potencia mínima y máxima que puede entrar al sistema por cada fase
  //3->  Potencia mínima y máxima que puede salir del sitema por cada fase
  //3->  Balance de potencia del sistema en cada fase
  //12-> Ecuación de la variación del estado de batería 12 por cada batería
  //1->  Balance que hace que la potencia que intercambia el sistema con la red sea igual a lo que entra menos
  //lo que sale
  //3->  Balance de lo que hace que la potencia que intercambia el sistema con la red sea igual a lo que entra
  //menos lo que sale por cada fase
  //1->  Potencia de lo que entra es igual al sumatorio de lo que entra por cada fase
  //1->  Potencia de lo que sale del sistema es igual al sumatorio de lo  que sale por cada fase
  //1->  Potencia que intercambia la red es igual al sumatorio de lo que se intercambia por cada fase
  //El numero total de restricciones = 12 + 12 + 1 + 1 + 1 + 3 + 3 + 3 + 3 + 12 + 1 + 3 + 1 + 1 + 1 = 58
  //Hay que multiplica el número total de restricciones * el número de puntos simulación
  *numero_restricciones = 58 * (informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion);
}