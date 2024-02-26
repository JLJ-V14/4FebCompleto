
#include "definiciones_globales.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>

/*Este subprograma se utiliza para crear los arrays que almacenan los puntos inciales y finales de las baterias
  conectadas alos terminales */

int calcular_puntos_iniciales_finales_baterias(informacion_procesada_t* informacion_sistema,OSQPInt terminal_actual,
  int * numero_baterias_terminales,int ** puntos_iniciales, int ** puntos_finales) {
  for (int numero_bateria = 0; numero_bateria < informacion_sistema->informacion_baterias.numero_baterias; numero_bateria++) {
    if (informacion_sistema->informacion_baterias.baterias[numero_bateria].numero_terminal == terminal_actual) {
      (*numero_baterias_terminales)++;
      int* temp  = realloc(puntos_iniciales, *numero_baterias_terminales * sizeof(int));
      int* temp2 = realloc(puntos_finales, *numero_baterias_terminales * sizeof(int));

      if ((temp == NULL) || (temp2 == NULL)) {
        printf("No se ha podido reservar memoria para el array de los puntos iniciales y finales del vector l al configurar los vectores l y u\n");
        registrar_error("No se ha podido reservar memoria para el array de los puntos iniciales y finales del vector l al configurar los vectores l y u\n", REGISTRO_ERRORES);
        return ERROR;
      }
      *puntos_iniciales = temp;
      (*puntos_iniciales)[*numero_baterias_terminales] = informacion_sistema->informacion_baterias.baterias[numero_bateria].punto_inicio;
      *puntos_finales = temp2;
      (*puntos_finales)[*numero_baterias_terminales] = informacion_sistema->informacion_baterias.baterias[numero_bateria].punto_final;
    }
  }
  return EXITO;
}

/*Este subprograma se utiliza para calcular un array de las potencias maximas que pueden aceptar las baterias
  ordenadas de la misma forma que el array de puntos iniciales y finales, cada termino de este array se corresponde
  con el termino correspondiente del array de puntos inciales  y finales */

int calcular_maxima_potencia_baterias(informacion_procesada_t* informacion_sistema,OSQPInt terminal_actual,
   int* numero_baterias_terminales, int* puntos_iniciales, int* puntos_finales,OSQPFloat** potencias_maximas) {
  //Se crea el array para almacenar los valores de potencias maximas que pueden intercambiar los terminales, si
  //hay algún terminal
  *potencias_maximas = (OSQPFloat*)malloc(*numero_baterias_terminales * sizeof(OSQPFloat));

  //Se comprueba que la reserva de memoria ha tenido éxito
  if (*potencias_maximas == NULL) {
    printf("Ha habido un error reservando memoria para el array de potencias maximas para la construcción del vector l\n");
    registrar_error("Ha habido un error reservando memoria para el array de potencias maximas para la construcción del vector l\n",REGISTRO_ERRORES);
    return ERROR;
  }


  //Se itera por todas las baterias que se tienen presenten en el sistema, si coinciden con el terminal, se procede
  //a añadir en el array si no está ya presente
  //Se crea una variable booleana para controlar si la bateria se ha añadido 
  bool bateria_anyadida = false;
  //Se crea una variable para llevar la cuenta sobre a que posicion del array de puntos iniciales se esta
  //accediendo
  int  index_adicional;

  //Se calcula el limite de potencia que la bateria conectada al terminal puede intercambiar, se tiene en cuenta
  //los limites del terminal, y los limites de la propia bateria
  int limite_potencia_terminal = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_estaciones_carga[terminal_actual - 1];
  int limite_potencia_bateria;
  for (int numero_bateria = 0; numero_bateria < informacion_sistema->informacion_baterias.numero_baterias; numero_bateria) {
    bateria_anyadida = false;
    index_adicional = 0;
    limite_potencia_bateria = informacion_sistema->informacion_baterias.baterias->maxima_potencia;

    if (informacion_sistema->informacion_baterias.baterias[numero_bateria].numero_terminal == terminal_actual) {
      while (bateria_anyadida == false) {
        
        if (puntos_iniciales[index_adicional] == informacion_sistema->informacion_baterias.baterias[numero_bateria].punto_inicio) {
          if (limite_potencia_terminal < limite_potencia_bateria) {
            bateria_anyadida = true;
            index_adicional++;
            *potencias_maximas[numero_bateria] = limite_potencia_bateria;
          }
          else {
            bateria_anyadida = true;
            index_adicional++;
            *potencias_maximas[numero_bateria] = limite_potencia_terminal;
          }

        }
      }
    }
  }

  return EXITO;
}