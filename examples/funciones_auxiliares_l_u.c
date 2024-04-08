
#include "definiciones_globales.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>





/*Este subprograma se utiliza para calcular un array de las potencias maximas que pueden aceptar las baterias
  ordenadas de la misma forma que el array de puntos iniciales y finales, cada termino de este array se corresponde
  con el termino correspondiente del array de puntos inciales  y finales */

void calcular_minima_potencia_baterias(informacion_procesada_t* informacion_sistema, int terminal_actual,
  informacion_carga_terminales_t* elementos_carga_terminales) {


  //Se itera por todas las baterias que se tienen presenten en el sistema, si coinciden con el terminal, se procede
  //a añadir en el array si no está ya presente


  //Se crea una variable para llevar la cuenta sobre a que posicion del array de puntos iniciales se esta
  //accediendo
  int  index_adicional;

  //Se calcula el limite de potencia que la bateria conectada al terminal puede intercambiar, se tiene en cuenta
  //los limites del terminal, y los limites de la propia bateria
  int limite_potencia_terminal = -informacion_sistema->informacion_restricciones_sistema.potencia_maxima_estaciones_carga[terminal_actual];
  int limite_potencia_bateria;

  //Se calcula el número de elementos que tiene el terminal:
  int elementos_terminal = elementos_carga_terminales->informacion_carga_terminales[terminal_actual].numero_elementos_terminal;

  //Se itera por todos los elementos que tienen su carga programada en el terminal y si es una bateria se tiene en
  //cuenta que la potencia mínima puede ser negativa

  for (int elemento_terminal = 0; elemento_terminal < elementos_terminal; elemento_terminal++) {

    //Si se tiene una bateria puede ser necesario cambiar el valor de potencia minima si la restricción del terminal
    //es más restrictiva

    if (elementos_carga_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[elemento_terminal].bateria == true) {

      //Si el terminal puede intercambiar menos potencia (un valor negativo más pequeño en valor absoluto)
      if (limite_potencia_terminal >= elementos_carga_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[elemento_terminal].potencia_minima) {
        elementos_carga_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[elemento_terminal].potencia_minima = limite_potencia_terminal;
      }

      //Si la restriccion de la propia bateria es más restrictiva no se hace nada.
      else{
      }

    }
  }
}
  
