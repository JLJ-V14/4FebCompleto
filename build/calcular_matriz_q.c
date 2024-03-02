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
  OSQPInt numero_terminal,OSQPInt numero_baterias_terminal,OSQPFloat* porcentaje_baterias_deseada,
  OSQPInt* puntos_objetivos) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se define el offset del termino SOC a situar en el vectro
  int offset_SOC = (numero_terminal - 1) * numero_puntos_simulacion;

  for (int numero_bateria = 0; numero_bateria < numero_baterias_terminal; numero_bateria++) {
    q[offset_SOC + puntos_objetivos[numero_bateria]] = -2 * COEFICIENTE_CARGA * porcentaje_baterias_deseada[numero_bateria];
  }
}

/*Este subprograma se utiliza para escribir en el vector q los términos correspondientes a los objetivos
  de carga de batería lineales*/

void escribir_objetivo_carga_vehiculos_terminal(informacion_procesada_t* informacion_sistema,OSQPFloat* q,
  OSQPInt* puntos_iniciales_vehiculos,OSQPInt* puntos_finales_vehiculos,OSQPFloat* baterias_deseadas_vehiculos,
  bool* carga_rapida,OSQPInt numero_terminal,OSQPInt numero_vehiculos_terminal) {

  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Cargo el offset del primer termno SOC a situar en el vector
  int offset_SOC = (numero_terminal - 1) * numero_puntos_simulacion;

  for (OSQPInt numero_vehiculo = 0; numero_vehiculo < numero_vehiculos_terminal; numero_vehiculo ++ ) {
    //Cargo el punto final a partir del cual esta presente el vehiculo
    OSQPInt punto_final_vehiculo = puntos_finales_vehiculos[numero_vehiculo];

    if (carga_rapida[numero_vehiculo] == true) {

      //Cargo el punto inicial a partir del cual esta presente el vehiculo
      OSQPInt punto_inicial_vehiculo = puntos_iniciales_vehiculos[numero_vehiculo];
    

      for (OSQPInt punto_actual = punto_inicial_vehiculo + 1; punto_actual <= puntos_finales_vehiculos; punto_actual++) {
        q[offset_SOC + punto_actual] = -2 * COEFICIENTE_CARGA * baterias_deseadas_vehiculos[numero_vehiculo];
      }

    }
    else {
      q[offset_SOC + punto_final_vehiculo] = -2 * COEFICIENTE_CARGA * baterias_deseadas_vehiculos[numero_vehiculo];
    }
  }

}

/*El siguiente subprograma se utiliza para incluir los objetivos de carga de los vehículos o baterías
  conectados a los terminales*/

int obtener_objetivos_carga(informacion_procesada_t* informacion_sistema,OSQPInt numero_terminal,
    OSQPFloat* q) {

  //Cargo el numero de vehiculos
  OSQPInt numero_vehiculos = informacion_sistema->informacion_vehiculos.numero_vehiculos;
  OSQPInt numero_vehiculos_terminales = 0;
  OSQPInt numero_baterias_terminales_objetivo = 0;

  OSQPInt*  puntos_iniciales_vehiculos = NULL;
  OSQPInt*  puntos_finales_vehiculos = NULL;
  bool* carga_rapida = NULL;
  OSQPFloat* baterias_deseadas_vehiculos = NULL;
  OSQPInt*  puntos_objetivos_baterias = NULL;
  OSQPFloat* baterias_deseadas_baterias = NULL;

  OSQPInt* temp = NULL;
  OSQPInt* temp2 = NULL;
  OSQPInt* temp3 = NULL;
  bool* temp4 = NULL;
  OSQPInt* temp5 = NULL;
  OSQPInt* temp6 = NULL;

  //Se itera por todos los vehiculos
  for (OSQPInt vehiculo_actual = 0; vehiculo_actual < numero_vehiculos; vehiculo_actual++) {
    if (informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].numero_terminal == numero_terminal) {
      numero_vehiculos_terminales++;

       temp = realloc(puntos_iniciales_vehiculos, numero_vehiculos_terminales * sizeof(int));
       temp2 = realloc(puntos_finales_vehiculos, numero_vehiculos_terminales * sizeof(int));
       temp3 = realloc(baterias_deseadas_vehiculos, numero_vehiculos_terminales * sizeof(OSQPFloat));
       temp4 = realloc(carga_rapida, numero_vehiculos_terminales * sizeof(bool));

      if ((temp == NULL) || (temp2 == NULL) || (temp3 == NULL)||(temp4 == NULL)) {
        printf("Ha habido un error reservando memoria en la matriz q de objetivos\n");
        registrar_error("Ha habido un error reservando memoria en la matriz q de objetivos\n", REGISTRO_ERRORES);
        //Se libera memoria del los arrays utilizados
        if (temp)free(temp);
        if (temp2)free(temp2);
        if (temp3)free(temp3);
        if (temp4)free(temp4);
        if (puntos_iniciales_vehiculos) free(puntos_iniciales_vehiculos);
        if (puntos_finales_vehiculos)free(puntos_finales_vehiculos);
        if (carga_rapida)free(carga_rapida);
        if (baterias_deseadas_vehiculos)free(baterias_deseadas_vehiculos);
        return ERROR;
      }
      puntos_iniciales_vehiculos = temp;
      puntos_iniciales_vehiculos[vehiculo_actual] = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].punto_inicio;
      puntos_finales_vehiculos = temp2;
      puntos_finales_vehiculos[vehiculo_actual] = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].punto_final;
      baterias_deseadas_vehiculos = temp3;
      baterias_deseadas_vehiculos[vehiculo_actual] = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].bateria_final;
     
      if (informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].modo_carga == "rapida") {
        carga_rapida = temp4;
        carga_rapida[vehiculo_actual] = true;
      }

      else {
        carga_rapida = temp4;
        carga_rapida[vehiculo_actual] = false;
      }
    }
  }

  //Se itera por todas las baterias

  for (OSQPInt bateria_actual = 0; bateria_actual < informacion_sistema->informacion_baterias.numero_baterias; bateria_actual++) {
    if (informacion_sistema->informacion_baterias.baterias[bateria_actual].numero_terminal == numero_terminal) {

       //Si se desea que la batería esté cargada en determinado punto es necesario incluirlo en el vector q, ya
      //que es un objetivo de carga más.

      if (informacion_sistema->informacion_baterias.baterias[bateria_actual].considerar_objetivo == true) {

        //Se ajusta la memoria reservada para almacenar los objetivos de bateria y los momentos en los que esasa
        //baterias tienen que estar cargada
        numero_baterias_terminales_objetivo++;
        temp5 = realloc(puntos_objetivos_baterias,numero_baterias_terminales_objetivo*sizeof(OSQPInt));
        temp6 = realloc(baterias_deseadas_baterias,numero_baterias_terminales_objetivo* sizeof(OSQPFloat));

        if ((temp5 == NULL) || (temp6 == NULL)) {
          printf("Ha habido un error en la reserva de memoria para el vector q y los objetivos de batería\n");
          registrar_error("Ha habido un error en la reserva de memoria para el vector q y los objetivos de batería\n", REGISTRO_ERRORES);
          if (temp)free(temp);
          if (temp2)free(temp2);
          if (temp3)free(temp3);
          if (temp4)free(temp4);
          if (temp5)free(temp5);
          if (temp6)free(temp6);
          if (puntos_iniciales_vehiculos)free(puntos_iniciales_vehiculos);
          if (puntos_finales_vehiculos)free(puntos_finales_vehiculos);
          if (carga_rapida)free(carga_rapida);
          if (baterias_deseadas_vehiculos)free(baterias_deseadas_vehiculos);
          if (puntos_objetivos_baterias)free(puntos_objetivos_baterias);
          if (baterias_deseadas_baterias)free(baterias_deseadas_baterias);
          return ERROR;
        }
      }
     

    }
  }
  escribir_objetivo_carga_vehiculos_terminal(informacion_sistema, q, puntos_iniciales_vehiculos, puntos_finales_vehiculos,
    baterias_deseadas_vehiculos, carga_rapida, numero_terminal, numero_vehiculos_terminales);

  escribir_objetivos_carga_baterias_terminal(informacion_sistema, q, numero_terminal,
    numero_baterias_terminales_objetivo, baterias_deseadas_baterias,puntos_objetivos_baterias);



  //Se procede a liberar memoria para los arrays utilizados:
  if (temp)free(temp);
  if (temp2)free(temp2);
  if (temp3)free(temp3);
  if (temp4)free(temp4);
  if (temp5)free(temp5);
  if (temp6)free(temp6);
  if (puntos_iniciales_vehiculos)free(puntos_iniciales_vehiculos);
  if (puntos_finales_vehiculos)free(puntos_finales_vehiculos);
  if (carga_rapida)free(carga_rapida);
  if (baterias_deseadas_vehiculos)free(baterias_deseadas_vehiculos);
  if (puntos_objetivos_baterias)free(puntos_objetivos_baterias);
  if (baterias_deseadas_baterias)free(baterias_deseadas_baterias);

  return EXITO;
}





/*Este subprograma se utiliza para incluir en la matriz q los objetivos de carga lineales*/

int obtener_objetivos_carga_lineales(informacion_procesada_t* informacion_sistema,OSQPFloat*q) {

  //Se va a iterar por los 12 terminales y se va a comprobar si hay que incluir los objetivos de carga de algo
  //conectado a un terminal
  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {
    if (obtener_objetivos_carga(informacion_sistema, numero_terminal, q) == ERROR) {
      printf("No se ha podido escribir el objetivo de carga en el vector q correspondemente\n");
      registrar_error("No se ha podido escribir el objetivo de carga en el vector q correspondemente\n", REGISTRO_ERRORES);
      return ERROR;
    }
  }
  return EXITO;
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
  //Se incluye el objetivo lineal de los precios
  calcular_objetivo_precio(informacion_sistema,q);
  //Se incluye el objetivo lineal de las cargas de las baterías
  if (obtener_objetivos_carga_lineales(informacion_sistema, q) == ERROR) {
    printf("No se ha podido incluir los objetivos no lineales de carga en el vector q\n");
    registrar_error("No se ha podido incluir los objejtivos no lineales de carga en el vector q\n", REGISTRO_ERRORES);
    return ERROR;
  }
  return EXITO;
}