#include "definiciones_globales.h"
#include "funciones_calcular_vector_l.h"
#include "funciones_calcular_restricciones_ecuaciones.h"
#include "funciones_calcular_vector_u.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdlib.h>
#include <stdio.h>




/*El siguiente subprograma se utiliza para calcular los vectores l y u */

int calcular_vector_l_u(informacion_procesada_t* informacion_sistema, OSQPFloat** l,OSQPFloat**u) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se reserva memoria para los vectores l y u el tamaño de estos vectores es igual al numero de restricciones
  //de borde y el numero de ecuaciones que se tienen en el sistema
  int numero_elementos_inicial = (NUMERO_VARIABLES+NUMERO_ECUACIONES_BASE) * numero_puntos;
  size_t size = sizeof(OSQPFloat) * numero_elementos_inicial;

  OSQPFloat* l = (OSQPFloat*)malloc(numero_elementos_inicial);
  OSQPFloat* u = (OSQPFloat*)malloc(numero_elementos_inicial);


  //Se procede a calcular los terminos de los vectores l y u
  //Primero se añaden los terminos de las restricciones de borde de las baterias
  calcular_limite_inferior_restricciones_bateria(informacion_sistema, *l, *u);
  calcular_limite_superior_restricciones_bateria(informacion_sistema, *l, *u);

  /*A continuación se añaden los terminos de las restricciones inferiores de las potencias que pueden intercambiar
    los terminales -> */

  if (calcular_limite_inferior_restricciones_potencia_terminales(informacion_sistema, *l) == ERROR) {
    printf("No se han podido calcular los límites inferiores de las potencias que pueden intercambiar los terminales\n");
    registrar_error("No se han podido calcular los límites inferiores de las potencias que pueden intercambiar los terminales\n", REGISTRO_ERRORES);
    return ERROR;
  }

  /*Es necesario tener en cuenta que si hay una bateria conectado al terminal los valores de la potencia que pueden
    intercambiar el terminal es negativo*/
  if (calcular_limites_superior_potencia_terminales(informacion_sistema,*u) == ERROR){
    printf("No se han podido calcular la restricciones de borde superiores de las potencias de los terminales\n");
    registrar_error("No se han podido calcular la restricciones de borde superiores de las potencias de los terminales\n", REGISTRO_ERRORES);
    return ERROR;
  }
  calcular_restricciones_borde_inferiores_potencia_red(informacion_sistema,*l);
  calcular_restricciones_superiores_borde_potencias_red(informacion_sistema, *u);
  calcular_limite_inferior_borde_potencia_red_fases(informacion_sistema, *l);
  calcular_restricciones_superiores_potencia_red_fase(informacion_sistema, *u);
  calcular_limite_inferior_borde_potencia_red_entrada_fases(informacion_sistema,*l);
  calcular_restricciones_superiores_potencia_entrada_fase(informacion_sistema,*u);
  calcular_limite_inferior_borde_potencia_red_salida_fase(informacion_sistema, *l);
  calcular_restricciones_borde_superiores_potencia_salida_fase(informacion_sistema,*u);

  //Se pasa a llamar a los subprogramas que sirven para calcular el resultado de la ecuación del balance de fase

  calcular_resultado_balance_fase(informacion_sistema, *l, *u);
  if (calcular_ecuaciones_balance_bateria(informacion_sistema, *l, *u) == ERROR) {
    printf("No se han podido calcular las ecuaciones de balance de batería en la realización del vector l y u\n");
    registrar_error("No se han podido calcular las ecuaciones de balance de batería en la realización del vector l y u\n", REGISTRO_ERRORES);
    return ERROR;
  }
  calcular_resto_ecuaciones(informacion_sistema, *l, *u);
  return EXITO;
}