#include "definiciones_globales.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>






/*Este subprograma se utiliza para escribir en el vector q los términos correspondientes a los objetivos
  de carga de vehiculo lineales*/

void escribir_objetivos_carga_baterias_terminal(informacion_procesada_t* informacion_sistema, OSQPFloat*q,
  int numero_terminal,int numero_elemento_terminal,informacion_carga_terminales_t* elementos_programados_carga_terminales) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se inspecciona si la batería que se está inspeccionando tiene un objetivo de carga.
  bool objetivo_carga = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento_terminal].considerar_objetivo;

  if (objetivo_carga == true) {

    //Se carga la posicion a incluir el objetivo de carga
    int punto_objetivo = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento_terminal].punto_objetivo;
    int posicion_objetivo_carga = numero_terminal * numero_puntos_simulacion + punto_objetivo;
    //Se carga la bateria deseada
    OSQPFloat porcentaje_bateria = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento_terminal].bateria_deseada;
    porcentaje_bateria = porcentaje_bateria / 2;
    q[posicion_objetivo_carga] = -2 * COEFICIENTE_CARGA * porcentaje_bateria;
  }

}

/*Este subprograma se utiliza para escribir en el vector q los términos correspondientes a los objetivos
  de carga de batería lineales*/

void escribir_objetivo_carga_vehiculos_terminal(informacion_procesada_t* informacion_sistema, OSQPFloat* q,
  int numero_terminal, int numero_elemento_terminal, informacion_carga_terminales_t* elementos_programados_carga_terminales) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Cargo el offset del primer termno SOC a situar en el vector
  int offset_SOC = (numero_terminal)*numero_puntos_simulacion;

  //Se carga el punto inicial en el que el vehiculo está cargado
  int punto_inicial = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento_terminal].punto_inicio;
  int punto_final = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento_terminal].punto_final;

  //Se comprueba si la carga del vehiculo es rapida o no
  bool carga_rapida = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento_terminal].carga_rapida;

  //Defino una variable para almacenar el porcentaje de bateria deseada
  OSQPFloat bateria_deseada;

  if (carga_rapida == true) {

    //Si la carga es de tipo rapida se incluye el objetivo de bateria en todos los puntos de simulacion hasta llegar
    //a el punto final

    bateria_deseada = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento_terminal].bateria_deseada;
    bateria_deseada = bateria_deseada / 2;

    for (int punto_actual = punto_inicial; punto_actual <= punto_final; punto_actual++) {

      q[offset_SOC + punto_actual] = -2 * COEFICIENTE_CARGA * bateria_deseada;

    }
  }

  else {
    OSQPFloat bateria_deseada = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento_terminal].bateria_deseada;
    bateria_deseada = bateria_deseada / 2;
    q[offset_SOC + punto_final] = -2 * COEFICIENTE_CARGA * bateria_deseada;
  }
}

/*El siguiente subprograma se utiliza para incluir los objetivos de carga de los vehículos o baterías
  conectados a los terminales*/

void obtener_objetivos_carga(informacion_procesada_t* informacion_sistema,int numero_terminal,
    OSQPFloat* q,informacion_carga_terminales_t* elementos_programados_carga_terminales) {

  //Se carga el numero de elementos que se tiene en el terminal
  int numero_elementos_terminal = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;




  //Se itera por todos los elementos que tiene el terminal 
  for (int elemento_actual = 0; elemento_actual < numero_elementos_terminal; elemento_actual++) {

    //Se llama al subprograma que se utiliza para escribir los objetivos lineales de carga de los vehiculos en el vector q
    //si el elemento siguiente que tiene su carga programada en el terminal es un vehículo.

    if (elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_actual].vehiculo == true) {

      escribir_objetivo_carga_vehiculos_terminal(informacion_sistema, q, numero_terminal,elemento_actual,
        elementos_programados_carga_terminales);
    }

    //Si no se llama al subprograma que escribe el objetivo de carga lineal de las baterias en el vector q
    else if (elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_actual].bateria == true) {

      escribir_objetivos_carga_baterias_terminal(informacion_sistema, q, numero_terminal,elemento_actual,
         elementos_programados_carga_terminales);

    }
  }

  //Se procede a liberar memoria para los arrays utilizados:
  


}





/*Este subprograma se utiliza para incluir en la matriz q los objetivos de carga lineales*/

void obtener_objetivos_carga_lineales(informacion_procesada_t* informacion_sistema,OSQPFloat*q,
    informacion_carga_terminales_t* elementos_programados_carga_terminal) {

  //Se va a iterar por los 12 terminales y se va a comprobar si hay que incluir los objetivos de carga de algo
  //conectado a un terminal

  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {
    obtener_objetivos_carga(informacion_sistema, numero_terminal, q, elementos_programados_carga_terminal);
  }

}


/*El siguiente subprograma se utiliza para obtener los precios de compra y de venta de la electricidad*/
void obtener_precio_compra_venta(informacion_procesada_t* informacion_sistema, OSQPFloat* precio_compra,
  OSQPFloat* precio_venta, OSQPInt punto_simulacion, int* index_actual) {

  //Cargo el punto final del precio actual
  OSQPInt punto_final = informacion_sistema->informacion_precio_compra.precios[*index_actual].punto_final;
  

  //Si el punto de simulacion actual está por delante del punto final del precio actual es necesario
  //actualizar

  if (punto_final < punto_simulacion) {
  

    
    (*index_actual)++;
   
    (*precio_compra) = informacion_sistema->informacion_precio_compra.precios[*index_actual].precio;
    (*precio_venta) = informacion_sistema->informacion_precio_venta.precios[*index_actual].precio;
  }

}

/*El siguiente subprograma se utiliza para incluir e*/


/*El siguiente subprograma se utiliza para incluir en la matriz objetivo de reducir el precio de
  la electricidad*/

void calcular_objetivo_precio(informacion_procesada_t* informacion_sistema, OSQPFloat*q) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se escribe el coeficiente que le da importancia relativa a la funcion de precio
  int coeficiente_precio = 1;

  //Se escriben los offset de las potencias Pin grid,t y Pout grid,t
  int offset_potencia_entrada_red = 25 * numero_puntos_simulacion;
  int offset_potencia_salida_red = 26 * numero_puntos_simulacion;

  //Cargo el index inicial de los precios
  int index_actual_precio = 0;
  //Cargo los precios iniciales de compra y venta
  OSQPFloat precio_compra = informacion_sistema->informacion_precio_compra.precios[(index_actual_precio)].precio;
  OSQPFloat precio_venta = informacion_sistema->informacion_precio_venta.precios[(index_actual_precio)].precio;


  //Se procede a escribir los terminos de los precios para cada punto de simulacion, en general esta función
  //se plantea de la siguiente forma Cu,t * Pin grid,t -B,t * Pout grid,t

  for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {
    obtener_precio_compra_venta(informacion_sistema, &precio_compra, &precio_venta, punto_actual, &index_actual_precio);
    //Cargo el delta de simulacion
    OSQPFloat delta_simulacion = informacion_sistema->informacion_puntos_simulacion.puntos_simulacion[punto_actual].delta;
    q[offset_potencia_entrada_red + punto_actual] = +COEFICIENTE_REDUCIR_PRECIO *((OSQPFloat)(delta_simulacion/60) *precio_compra);
    q[offset_potencia_salida_red + punto_actual]  = -COEFICIENTE_REDUCIR_PRECIO *((OSQPFloat)(delta_simulacion/60) *precio_venta);
  }

}


//Este subprograma se utiliza para calcular los terminos lineales del objetivo 
int calcular_vector_q(informacion_procesada_t* informacion_sistema,OSQPFloat **q,
  informacion_carga_terminales_t* elementos_carga_programados_terminales) {


  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Primero hay que dimensionar  el tamaño de la matriz/vector q
  *q = (OSQPFloat*)calloc(NUMERO_VARIABLES * numero_puntos_simulacion,sizeof(OSQPFloat));

  if (*q == NULL) {
    printf("No se ha podido reservar memoria para la matriz/vector q\n");
    registrar_error("No se ha podido reservar memoria para la matriz/vector q\n", REGISTRO_ERRORES);
    return ERROR;
  }

  
  //Se incluye el objetivo lineal de los precios
  calcular_objetivo_precio(informacion_sistema,*q);
  
  //Se incluye el objetivo lineal de las cargas de las baterías
  obtener_objetivos_carga_lineales(informacion_sistema,*q, elementos_carga_programados_terminales);
   
  
  return EXITO;
}