#include "definiciones_globales.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/*El siguiente subprograma se utiliza para incluir los objetivos de carga de los vehículos o baterías
  conectados a los terminales*/

int obtener_objetivos_carga(informacion_procesada_t* informacion_sistema,OSQPInt numero_terminal,
    OSQPFloat* q) {

  //Cargo el numero de vehiculos
  OSQPInt numero_vehiculos = informacion_sistema->informacion_vehiculos.numero_vehiculos;
  OSQPInt numero_elementos_terminales = 0;

  OSQPInt*  puntos_iniciales_vehiculos = NULL;
  OSQPInt*  puntos_finales_vehiculos = NULL;
  bool* carga_rapida = NULL;
  OSQPFloat* baterias_deseadas_vehiculos = NULL;
  OSQPInt*  puntos_iniciales_baterias = NULL;
  OSQPInt*  puntos_finales_baterias = NULL;
  OSQPFloat* baterias_deseadas_baterias = NULL;

  for (OSQPInt vehiculo_actual = 0; vehiculo_actual < numero_vehiculos; vehiculo_actual++) {
    if (informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].numero_terminal == numero_terminal) {
      numero_elementos_terminales++;

      OSQPInt* temp = realloc(puntos_iniciales_vehiculos, numero_elementos_terminales * sizeof(int));
      OSQPInt* temp2 = realloc(puntos_finales_vehiculos, numero_elementos_terminales * sizeof(int));
      OSQPFloat* temp3 = realloc(, numero_elementos_terminales * sizeof(OSQPFloat));
      bool* temp4 = realloc(carga_rapida, numero_elementos_terminales * sizeof(bool));

      if ((temp == NULL) || (temp2 == NULL) || (temp3 == NULL)) {
        printf("Ha habido un error reservando memoria en la matriz q de objetivos\n");
        registrar_error("Ha habido un error reservando memoria en la matriz q de objetivos\n", REGISTRO_ERRORES);
        return ERROR;
      }
      puntos_iniciales_vehiculos = temp;
      puntos_iniciales_vehiculos[vehiculo_actual] = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].punto_inicio;
      puntos_finales_vehiculos = temp2;
      puntos_finales_vehiculos[vehiculo_actual] = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].punto_final;
      baterias_deseadas_vehiculos = temp3;
      baterias_deseadas_vehiculos[vehiculo_actual] = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].bateria_final;
      carga_rapida[vehiculo_actual]
      if (informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].modo_carga == "rapida") {
        carga_rapida[vehiculo_actual] = true;
      }

      else {

      }
    }
  }
  return EXITO;
}





/*Este subprograma se utiliza para incluir en la matriz q los objetivos de carga lineales*/

void obtener_objetivos_carga_lineales(informacion_procesada_t* informacion_sistema,OSQPFloat*q) {

  //Se va a iterar por los 12 terminales y se va a comprobar si hay que incluir los objetivos de carga de algo
  //conectado a un terminal
  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {

  }
}


/*El siguiente subprograma se utiliza para obtener los precios de compra y de venta de la electricidad*/
void obtener_precio_compra_venta(informacion_procesada_t* informacion_sistema, OSQPFloat* precio_compra,
  OSQPFloat* precio_venta, OSQPInt punto_simulacion, int* index_actual) {

  //Cargo el punto final del precio actual
  int punto_final = informacion_sistema->informacion_precio_compra.precios[*index_actual].punto_final;

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

  for (int punto_actual = 1; punto_actual < numero_puntos_simulacion; punto_actual++) {
    obtener_precio_compra_venta(informacion_sistema, &precio_compra, &precio_venta, punto_actual, &index_actual_precio);
    //Cargo el delta de simulacion
    OSQPFloat delta_simulacion = informacion_sistema->informacion_puntos_simulacion.puntos_simulacion[punto_actual].delta;
    q[offset_potencia_entrada_red + punto_actual] =-coeficiente_precio *((OSQPFloat)(delta_simulacion/60)  *precio_compra);
    q[offset_potencia_salida_red + punto_actual] = -coeficiente_precio *((OSQPFloat)(delta_simulacion/60) *precio_venta);
  }

}


//Este subprograma se utiliza para calcular los terminos lineales del objetivo 
int calcular_vector_q(informacion_procesada_t* informacion_sistema,OSQPFloat **q) {

  //Primero hay que dimensionar  el tamaño de la matriz/vector q
  OSQPFloat* q = (OSQPFloat*)calloc(NUMERO_VARIABLES,sizeof(OSQPFloat));

  if (q == NULL) {
    printf("No se ha podido reservar memoria para la matriz/vector q\n");
    registrar_error("No se ha podido reservar memoria para la matriz/vector q\n", REGISTRO_ERRORES);
    return ERROR;
  }
  calcular_objetivo_precio(informacion_sistema,q);
  return EXITO;
}