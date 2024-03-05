
#include "definiciones_globales.h"
#include "operaciones_string.h"
#include "tipos_optimizacion.h"
#include "registrar_errores.h"
#include  <stdio.h>
#include <stdlib.h>


/*Este subprograma es una funcion auxiliar para reordenar los elementos que tienen su carega programada en
  base al punto de inicio*/

int comparar_punto_inicio(const void*a, const void*b) {
  const informacion_elemento_carga_t* elementoA = (const informacion_elemento_carga_t*)a;
  const informacion_elemento_carga_t* elementoB = (const informacion_elemento_carga_t*)b;

  // Ascending order
  if (elementoA->punto_inicio < elementoB->punto_inicio) return -1;
  if (elementoA->punto_inicio > elementoB->punto_inicio) return 1;
  return 0;
}


/*Este subprograma se utiliza para almacenar la informacion de los vehiculos que tienen programada su carga en los
  diferentes terminales */

int obtener_informacion_carga_vehiculos(informacion_procesada_t* informacion_sistema,
  informacion_carga_terminales_t* programacion_carga_terminales) {

  //Se carga el numero de vehiculos que se tienen en el sistema
  int numero_vehiculos = informacion_sistema->informacion_vehiculos.numero_vehiculos;

  for (int vehiculo_actual = 0; vehiculo_actual<numero_vehiculos; vehiculo_actual++) {

    //Se carga el numero de terminal al que está conectado el vehículo
    int numero_terminal = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].numero_terminal;

    //Se procede a cargar la informacion de la carga del vehiculo en la posicion correspondiente dentro de la
    //variable programacion carga terminales

    //Primero se actualiza el numero de vehiculos que se tienen en el terminal
    programacion_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal++,
    programacion_carga_terminales->informacion_carga_terminales[numero_terminal].numero_vehiculos_terminal++;

    //Se reajusta la cantidad de memoria reservada para los elementos que se tienen conectados al terminal
    int nuevo_tamano = programacion_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;
    size_t nuevo_tamano_bytes = nuevo_tamano * sizeof(informacion_elemento_carga_t);

    informacion_elemento_carga_t* temp = realloc(programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal, nuevo_tamano_bytes);

    //Se comprueba si la reserva de memoria ha sido realizada con éxito, si lo ha sido se procede a almacenar la
    //informacion
    if (temp == NULL) {
      printf("No se ha podido reservar memoria a la hora de procesar la programacion de los vehículos y baterias de los diferentes elementos conectados a los terminales\n");
      registrar_error("No se ha podido reservar memoria a la hora de procesar la programacion de los vehículos y baterías de los diferentes elemmentos contectados a los terminales\n", REGISTRO_ERRORES);
      return ERROR;
    }

    else {
      //Cargo el numero de vehiculo a rellenar la informacion
      int index_vehiculo_actual = programacion_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal - 1;
      //Se utiliza la memoria reservada para almacenar la informacion
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal = temp;
      //Se carga el inicio y final de la carga del vehiculo.
      int punto_inicio_vehiculo = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].punto_inicio;
      int punto_final_vehiculo = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].punto_final;
      //Se procede a inspeccionar si la carga del vehiculo es rapida o normal
      bool carga_rapida_vehiculo;

      //Se carga la capacidad de la bateria, el porcentaje de la bateria inicial, el porcentaje de bateria deseada
      //y la potencia limite que puede intercambiar el vehiculo.
      OSQPFloat capacidad_bateria = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].capacidad_bateria;
      OSQPFloat bateria_inicial = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].bateria_inicial;
      OSQPFloat bateria_deseada = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].bateria_final;
      OSQPFloat potencia_limite = informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].potencia_maxima;

      if (strings_iguales(informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].modo_carga,"Rapida")) {
        carga_rapida_vehiculo = true;
      }

      else {
        carga_rapida_vehiculo = false;
      }

      //Se rellena la informacion del vehiculo
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_vehiculo_actual].punto_inicio = punto_inicio_vehiculo;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_vehiculo_actual].punto_final = punto_final_vehiculo;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_vehiculo_actual].carga_rapida = carga_rapida_vehiculo;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_vehiculo_actual].punto_objetivo = punto_final_vehiculo;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_vehiculo_actual].considerar_objetivo = true;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_vehiculo_actual].vehiculo = true;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_vehiculo_actual].bateria = false;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_vehiculo_actual].capacidad_bateria = capacidad_bateria;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_vehiculo_actual].bateria_inicial = bateria_inicial;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_vehiculo_actual].bateria_deseada = bateria_deseada;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_vehiculo_actual].potencia_maxima = potencia_limite;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_vehiculo_actual].potencia_minima = 0;
    }

  }
  //Se libera la memoria para el array auxiliar que se ha liberado
  
  return EXITO;
}

//Este subprograma se utiliza para obtener los datos relevantes de carga de las baterías que tienen su carga
//programada en el terminal.
int obtener_informacion_carga_baterias(informacion_procesada_t* informacion_sistema,
 informacion_carga_terminales_t* programacion_carga_terminales) {

  //Se carga el numero de baterias en el sistema
  int numero_baterias = informacion_sistema->informacion_baterias.numero_baterias;

  for (int numero_bateria = 0; numero_bateria < numero_baterias; numero_bateria++) {

    //Se carga el numero de terminal al que esta conectado la bateria
    int numero_terminal = informacion_sistema->informacion_baterias.baterias[numero_bateria].numero_terminal;

    //Es necesario actualizar el numero de baterias y elementos que se tienen en el terminal
    programacion_carga_terminales->informacion_carga_terminales->numero_elementos_terminal++;
    programacion_carga_terminales->informacion_carga_terminales->numero_baterias_terminal++;


    //Se reajusta la cantidad de memoria reservada para los elementos que se tienen conectados al terminal
    int nuevo_tamano = programacion_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;
    size_t nuevo_tamano_bytes = nuevo_tamano * sizeof(informacion_elemento_carga_t);

    informacion_elemento_carga_t* temp = realloc(programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal, nuevo_tamano_bytes);

    //Se comprueba si la reserva de memoria ha tenido exito
    if (temp == NULL) {
      printf("No se ha podido reservar memoria a la hora de procesar la programacion de los vehículos y baterias de los diferentes elementos conectados a los terminales\n");
      registrar_error("No se ha podido reservar memoria a la hora de procesar la programacion de los vehículos y baterías de los diferentes elemmentos contectados a los terminales\n", REGISTRO_ERRORES);
      return ERROR;
    }

    else {

      int index_bateria_actual = programacion_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal - 1;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal = temp;
      //Se carga el inicio y fin de la carga de la batería
      int punto_inicio_bateria = informacion_sistema->informacion_baterias.baterias[numero_bateria].punto_inicio;
      int punto_final_bateria = informacion_sistema->informacion_baterias.baterias[numero_bateria].punto_final;

      //Se carga la información de la batería
      OSQPFloat bateria_inicial   = informacion_sistema->informacion_baterias.baterias[numero_bateria].punto_inicio;
      OSQPFloat capacidad_bateria = informacion_sistema->informacion_baterias.baterias[numero_bateria].capacidad_bateria;
      OSQPFloat potencia_maxima = informacion_sistema->informacion_baterias.baterias[numero_bateria].maxima_potencia;
      OSQPFloat bateria_deseada;

      //Se procede a inspeccionar si la carga del vehiculo es rapida o normal
      bool consideracion_objetivo_carga;

      //Si se desea que la bateria cumpla un determinado objetivo de carga en alguna fecha, se carga la información
      //correspondiente
      if (informacion_sistema->informacion_baterias.baterias[numero_bateria].considerar_objetivo == true) {

        //Se carga que se considera el objetivo de carga y se procede a cargar el porcentaje de bateria deseada y
        //el punto de simulacion en el cual se desea que la bateria esté cargada
        consideracion_objetivo_carga = true;
        OSQPInt punto_objetivo_carga = informacion_sistema->informacion_baterias.baterias[numero_bateria].punto_objetivo;
        programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].punto_objetivo = punto_objetivo_carga;
        bateria_deseada = informacion_sistema->informacion_baterias.baterias[numero_bateria].bateria_objetivo;
        programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].bateria_deseada = bateria_deseada;
      }

      //Si no se desea que la cumpla ningún determinado objetivo de carga pues no se carga este tipo de información
      else {
        consideracion_objetivo_carga = false;
        programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].punto_objetivo = 0;
        programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].bateria_deseada = 0;
      }

      //Se rellena la informacion de la bateria
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].punto_inicio = punto_inicio_bateria;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].punto_final = punto_final_bateria;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].bateria_inicial = bateria_inicial;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].capacidad_bateria = capacidad_bateria;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].potencia_maxima =   potencia_maxima;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].potencia_minima = - potencia_maxima;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].carga_rapida = false;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].considerar_objetivo = consideracion_objetivo_carga;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].vehiculo = false;
      programacion_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_bateria_actual].bateria = true;
    }

    
  }

  
  return EXITO;
}

//El siguiente subprograma se utiliza para reordenar los elementos que tienen programada su carga en los diferentes
//terminales para que estén ordenados en funcion de cuando se conectan al sistema

void reordenar_elementos_cargas_terminales(informacion_procesada_t* informacion_sistema,
  informacion_carga_terminales_t* programacion_carga_terminales) {

  //Se itera por todos los terminales
  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {
    informacion_elementos_terminal_t* terminal = &(programacion_carga_terminales->informacion_carga_terminales[numero_terminal]);
    qsort(terminal->elementos_terminal, terminal->numero_elementos_terminal, sizeof(informacion_elemento_carga_t),comparar_punto_inicio);
  }
}



//Este subprograma se utiliza para almacenar la informacion de los elementos que tienen su carga
//programada en los diferentes terminales

int obtener_informacion_carga_terminales(informacion_procesada_t* informacion_sistema,
  informacion_carga_terminales_t* programacion_carga_terminales) {

  //Se llama al subprograma que almacena la programacion de la carga de los diferentes vehiculo
  if (obtener_informacion_carga_vehiculos(informacion_sistema,programacion_carga_terminales) == ERROR) {
    printf("No se ha podido programar la carga de los vehiculos de los terminales por problemas de reserva de memoria\n");
    registrar_error("No se ha podido programar la carga de los vehiculos de los terminales por problemas de reserva de memoria\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se llama al subprograma que almacena la programacion de carga de las baterias
  if (obtener_informacion_carga_baterias(informacion_sistema, programacion_carga_terminales) == ERROR) {
    printf("No se ha podido programar la carga de las baterias de los terminales por problemas de reserva de memoria\n");
    registrar_error("No se ha podido programar la carga de los vehiculos de los terminales por problemas de reserva de memoria\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se reordenan los elementos conectados a los diferentes terminales en orden de conexión
  reordenar_elementos_cargas_terminales(informacion_sistema,programacion_carga_terminales);

  return EXITO;
}