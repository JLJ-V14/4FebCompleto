#include "definiciones_globales.h"
#include "osqp.h"
#include "operaciones_string.h"
#include "tipos_optimizacion.h"

/*Este subprograma se utiliza para calcular cuantas ecuaciones que modelan el comportamiento de la
  batería hay, esto depende de cuanto tiempo hay baterías o vehículos conectados.*/



void calcular_numero_ecuaciones_estado_bateria(informacion_procesada_t* informacion_sistema,
                                               int* numero_ecuaciones_bateria) {
  *numero_ecuaciones_bateria = 0;
  //Se carga durante cuantos puntos de simulación  están presenten los diferentes vehículos
  //Tantas ecuaciones como puntos de simulación en la que está presente el vehículo - 1 en el punto
  //inicial que batería es igual al estado inicial = batería final.

  for (int i = 0; i < informacion_sistema->informacion_vehiculos.numero_vehiculos; i++) {
    *numero_ecuaciones_bateria += informacion_sistema->informacion_vehiculos.vehiculos[i].punto_final - informacion_sistema->informacion_vehiculos.vehiculos[i].punto_inicio;
  }

  for (int i = 0; i < informacion_sistema->informacion_baterias.numero_baterias; i++){
    *numero_ecuaciones_bateria += informacion_sistema->informacion_baterias.baterias[i].punto_final - informacion_sistema->informacion_baterias.baterias[i].punto_inicio;
  }
}

/*Este subprograma se utiliza para añadir los terminos de potencia en las ecuaciones de balance de fase
  y se tienen en cuenta que en la matriz A si los terminales están conectados a alguna fase apareceran
  en la matriz A y por tanto el numero de elementos diferentes de 0 aumentará */

void calcular_numero_terminos_ecuacion_balance_fase(informacion_procesada_t* informacion_sistema,OSQPInt* numero_terminos_adicionales) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;


  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {

    char terminal_actual = informacion_sistema->informacion_terminales.fases_electricas[numero_terminal];

    //Si se está conectado a alguna de las tres fases es necesario incluir el término 
    if ((terminal_actual == 'R') || (terminal_actual == 'r') || (terminal_actual == 'S') ||(terminal_actual == 's')||
       (terminal_actual == 'T')  || (terminal_actual == 't')){
      
      *numero_terminos_adicionales = *numero_terminos_adicionales + numero_puntos_simulacion;
    }

  }

}

/*El siguiente subprograma se utiliza para incluir los términos de las ecuaciones de balance de batería es necesario
  añadir para los puntos de simulacion en la que están presentes*/

void calcular_numero_terminos_ecuaciones_baterias(informacion_procesada_t* informacion_sistema,OSQPInt* numero_terminos_adicionales) {

  //Se carga el numero de vehiculos y el numero de baterias

  int numero_vehiculos = informacion_sistema->informacion_vehiculos.numero_vehiculos;
  int numero_baterias = informacion_sistema->informacion_baterias.numero_baterias;
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  for (int vehiculo = 0; vehiculo < numero_vehiculos; vehiculo++) {

    int punto_final = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo].punto_final;
    int punto_inicial = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo].punto_inicio;

    //Se añade el termino de potencia intercambiado por el terminal y el termino de la bateria del punto anterior
    //conectado al terminal
    (*numero_terminos_adicionales) += 2LL * (punto_final - punto_inicial);
  }

  for (int bateria = 0; bateria < numero_baterias; bateria++) {

    int punto_final = informacion_sistema->informacion_baterias.baterias[bateria].punto_final;
    int punto_inicial = informacion_sistema->informacion_baterias.baterias[bateria].punto_inicio;
    (*numero_terminos_adicionales) += 2LL * (punto_final - punto_inicial);

  }

 

  /*Se itera por el número de ecuaciones de batería*/

}



/*En este subprograma se utiliza el numero de restricciones que existen en el problema de optimizacion*/
void calcular_numero_restricciones(informacion_procesada_t* informacion_sistema,OSQPInt* numero_restricciones) {
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
  //1->  Balance que hace que la potencia que intercambia el sistema con la red sea igual a lo que entra menos
  //lo que sale
  //3->  Balance de lo que hace que la potencia que intercambia el sistema con la red sea igual a lo que entra
  //menos lo que sale por cada fase
  //1->  Potencia de lo que entra es igual al sumatorio de lo que entra por cada fase
  //1->  Potencia de lo que sale del sistema es igual al sumatorio de lo  que sale por cada fase
  //1->  Potencia que intercambia la red es igual al sumatorio de lo que se intercambia por cada fase
  //
  //
  // //12-> Ecuación de la variación del estado de batería 12 por cada batería
  //
  // 
  //El numero total de restricciones (fijas) = 12 + 12 + 1 + 1 + 1 + 3 + 3 + 3 + 3 + 1 + 3 + 1 + 1 + 1 = 46
  //Hay que multiplica el número total de restricciones * el número de puntos simulación
  // OSQPInt numero_restricciones_fijas = 46 * (informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion);
   //Se pasa a calcular el numero de ecuaciones de modelado del estado de la batería que existen
  // OSQPInt numero_restricciones_variables = 0;


  // calcular_numero_ecuaciones_estado_bateria(informacion_sistema, &numero_restricciones_variables);
   //*numero_restricciones = numero_restricciones_fijas + numero_restricciones_variables;


  //El numero de restricciones que se tiene es basicamente el numero de variables que se tienen + el numero
  //de inecuaciones que se tienen esto es ( 36 + 22 ) * numero_puntos_simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  *numero_restricciones = 58 * numero_puntos_simulacion;
  

}