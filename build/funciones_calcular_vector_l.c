
#include "definiciones_globales.h"
#include "funciones_auxiliares_l_u.h"
#include "funciones_calcular_a_x.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>

void calcular_limite_inferior_restricciones_bateria(informacion_procesada_t* informacion_sistema,OSQPFloat *l) {
  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Creo una variable para situar los terminos en el punto correcto del vector
  int offset_index_vector_l ;
  //Como hay 12 terminales hay 12 variables de estados de bateria
  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {
    offset_index_vector_l = numero_terminal * numero_puntos_simulacion;
    for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
      l[offset_index_vector_l + punto_simulacion] =(OSQPFloat) 0.0;
    }
  }
}




/*Este subprograma se utiliza para indicar los límites de las potencias de los terminales, si se tiene un vehiculo o
  no se tiene nada el límite inferior es 0, si se tiene una batería puede ser bidireccional*/

int calcular_limite_inferior_restricciones_potencia_terminales(informacion_procesada_t* informacion_sistema,
  OSQPFloat* l) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Se crea una variable para situar los terminos en el punto correcto del vector
  int offset_index_vector_l;

  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {
    //Calculo el offset para las potencias de los terminales están después de las restricciones de borde
    int offset_potencia_terminal = NUMERO_TERMINALES * numero_puntos_simulacion;

    int  numero_baterias_terminal = 0;
    int* puntos_iniciales = NULL;
    int* puntos_finales = NULL;
    int  index_adicional = 0;

    //Se crea un array para almacenar la minima potencia que puede intercambiar el terminal dependiendo de si
    //hay una bateria conectada o no.
    OSQPFloat* potencias_maxima_terminal = NULL;

    //Se rellenan los arrays, en donde quedan guardado cuando se conectan y se desconectan las baterías de los
    //terminales
    if (calcular_puntos_iniciales_finales_baterias(informacion_sistema,numero_terminal+1,&numero_baterias_terminal,
        &puntos_iniciales,&puntos_finales) == ERROR) {
      printf("No se ha podido cargar los puntos iniciales de las baterias conectadas a los diferentes terminales\n");
      registrar_error("No se ha podido cargar los puntos iniciales de las baterias conectadas a los diferentes terminales\n", REGISTRO_ERRORES);
      return ERROR;
    }
    qsort(puntos_iniciales, numero_baterias_terminal, sizeof(int), comparar_ints);
    qsort(puntos_finales, numero_baterias_terminal, sizeof(int), comparar_ints);

    if (calcular_maxima_potencia_baterias(informacion_sistema,numero_terminal + 1,&numero_baterias_terminal,puntos_iniciales,
        puntos_finales,&potencias_maxima_terminal) == ERROR) {
      printf("No se ha podido calcular el limite inferior de las potencias intercambiadas por los terminales\n");
      registrar_error("No se ha podido calcular el limite inferior de las potencias intercambiadas por los terminales\n", REGISTRO_ERRORES);
      return ERROR;
    }

    for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
      if (numero_baterias_terminal > index_adicional) {
        if (comprobar_rango(punto_simulacion, puntos_iniciales[index_adicional], puntos_finales[index_adicional]) == true) {
          if (punto_simulacion == puntos_finales[index_adicional]) {
            l[offset_potencia_terminal + numero_terminal * numero_puntos_simulacion + punto_simulacion] = -(OSQPFloat)potencias_maxima_terminal[index_adicional];
            index_adicional++;
          }
          else {
            l[offset_potencia_terminal + numero_terminal * numero_puntos_simulacion + punto_simulacion] = -(OSQPFloat)potencias_maxima_terminal[index_adicional];
          }
        }
        else {
          l[offset_potencia_terminal + numero_terminal * numero_puntos_simulacion + punto_simulacion] =(OSQPFloat) 0.0;
        }
      }
      else {
        //Si no hay una bateria presente el limite inferior es 0
        l[offset_potencia_terminal + numero_terminal * numero_puntos_simulacion + punto_simulacion] =(OSQPFloat)0.0;
      }
    }
  }
  return EXITO;
}
