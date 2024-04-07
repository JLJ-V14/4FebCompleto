#include "definiciones_globales.h"
#include "funciones_calcular_a_x.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdbool.h>


/*Este subprograma se utiliza para calcular el numero de objetivos del vehiculo en cuestión deben ser
  añadidos en la matriz P*/

static void calcular_numero_objetivos_carga_vehiculos(informacion_elemento_carga_t* elemento_carga,OSQPInt* P_nnz) {

  //El numero de objetivos a añadir en la matriz P es diferente dependiendo de si el modo de carga es rapida o no

  if (elemento_carga->carga_rapida == true) {

    //Se carga el punto de inicio y el punto final en los cuales el vehiculo está conectado
    int punto_inicio = elemento_carga->punto_inicio;
    int punto_final = elemento_carga->punto_final;

    //Para todos los puntos entre el punto final y el punto inicial (punto final incluido) deben ser añadidos
    //en la matriz P
    *P_nnz += (punto_final - punto_inicio);

  }

  else {
    //Solo es necesario añadir el punto en el que el objetivo de carga tiene que estar satisfecho.
    (*P_nnz)++;
  }

}

static void calcular_numero_objetivos_carga_bateria(informacion_elemento_carga_t* elemento_carga,OSQPInt* P_nnz) {

  //Si para la batería se considera el objetivo de carga es necesario añadir el estado de bateria para el punto
  //objetivo a la matriz P.

  if (elemento_carga->considerar_objetivo == true) {
    (*P_nnz)++;
  }


}

static void calcular_numero_potencia_terminal_bateria(informacion_elemento_carga_t* elemento_carga, OSQPInt* P_nnz) {
  
  //Se carga el punto inicial y el punto final de la bateria que tiene su carga programada en el terminal
  int punto_inicial = elemento_carga->punto_inicio;
  int punto_final = elemento_carga->punto_final;

  (*P_nnz) += punto_final - punto_inicial;


  }

/*Este subprograma se utiliza para al dimensionar la matriz P tener en cuenta el término Pgrid out,t si finalmente
  se reduce el termino Pout grid para evitar vertidos a la red innecesarios*/
static void calcular_numero_terminos_potencia_salida_red(informacion_procesada_t* informacion_sistema,OSQPInt* P_nnz) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  *P_nnz += numero_puntos_simulacion;
}

/*El siguiente subprograma se utiliza para calcular el numero de objetivos de carga que se tiene*/
static void calcular_numero_objetivos_carga_terminal(informacion_carga_terminales_t* elementos_programados_carga_terminales,
  OSQPInt* P_nnz, int numero_terminal) {

  //Se carga el numero de elementos que tienen su carga programada en el terminal actual
  int numero_elementos_terminal = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;

  

  //Se itera por todos los elementos que tiene el terminal
  for (int elemento_terminal = 0; elemento_terminal < numero_elementos_terminal; elemento_terminal++) {
    
    if (elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_terminal].vehiculo == true) {
      calcular_numero_objetivos_carga_vehiculos(&(elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_terminal]), P_nnz);
    }

    else {
      calcular_numero_objetivos_carga_bateria(&(elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_terminal]), P_nnz);
    }
  }
}

/*Hay repetición en este subprograma pero lo hago para no alterar partes que ya se que funcionan*/
static void calcular_numero_objetivos_perservacion_baterias_terminal(informacion_carga_terminales_t* elementos_programados_carga_terminales, OSQPInt* P_nnz,
  int numero_terminal) {

  //Se carga el numero de elementos que tienen su carga programa en el terminal
  int numero_elementos_terminal = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;

  //Se itera por todos los elementos que tienen programado su carga en el terminal 
  for (int elemento_terminal = 0; elemento_terminal < numero_elementos_terminal; elemento_terminal++) {

    if (elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_terminal].bateria == true) {
      calcular_numero_potencia_terminal_bateria(&(elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_terminal]), P_nnz);
    }
  }
}

/*El siguiente subprograma se utilizaz para dimensionar el tamaño de la matriz P */
static int dimensionar_matriz_P(informacion_procesada_t* informacion_sistema,informacion_carga_terminales_t* elementos_programados_carga_terminales,
  matriz_p_t* matriz_p) {
  //Se inicializa el numero de elementos diferentes de 0
  matriz_p->P_nnz = 0;
  //Se itera por todos los terminales para ver cuantos elementos tienen su carga programada, así como el modo de
  //carga para poder dimensionar la matriz P.

  
  for (int terminal = 0; terminal < NUMERO_TERMINALES; terminal++) {
    calcular_numero_objetivos_carga_terminal(elementos_programados_carga_terminales, &(matriz_p->P_nnz),terminal);
    //calcular_numero_objetivos_perservacion_baterias_terminal(elementos_programados_carga_terminales,&(matriz_p->P_nnz),terminal);
  }

  calcular_numero_terminos_potencia_salida_red(informacion_sistema, &(matriz_p->P_nnz));

  

  //Se dimensionan los vectores P_x y los vectores P_i
  matriz_p->P_x = (OSQPFloat*)malloc((matriz_p->P_nnz) * sizeof(OSQPFloat));

  if (matriz_p->P_x == NULL) {
    printf("No se ha podido dimensionar el vector P_x correctamente\n");
    registrar_error("No se ha podido dimensionar el vector P_x correctamente\n", REGISTRO_ERRORES);
    return ERROR;
  }

  matriz_p->P_i = (OSQPInt*)malloc((matriz_p->P_nnz) * sizeof(OSQPInt));

  if (matriz_p->P_i == NULL) {
    printf("No se ha podido dimensionar el vector P_i correctamente\n");
    registrar_error("No se ha podido dimensionar el vector P_i correctamente\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se dimensiona el vector P_p
  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  int dimension_vector_P_p = (NUMERO_VARIABLES)*numero_puntos_simulacion + 1;

 

  //Se dimensiona el vector P_p
  matriz_p->P_p = (OSQPInt*)malloc((dimension_vector_P_p) * sizeof(OSQPInt));

  //Se comprueba que las reservas de memoria han sido exitosas
  if (matriz_p->P_p == NULL) {
    printf("No se ha podido dimensionar el vector P_p correctamente\n");
    registrar_error("No se ha podido dimensionar el vector P_p correctamente\n", REGISTRO_ERRORES);
    return ERROR;
  }
  return EXITO;
}






//Este subprograma se utiliza para incluir el objetivo cuadratico de carga de las baterias en la matriz p 
void incluir_objetivo_cuadratico_bateria(informacion_procesada_t* informacion_sistema, matriz_p_t* matriz_p,
  informacion_carga_terminales_t* elementos_programados_carga_terminales, int numero_terminal,
  int numero_elemento,int* index_actual) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  int offset_potencia_terminal = numero_terminal * numero_puntos_simulacion;

  bool objetivo_carga = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento].considerar_objetivo;
  

  if (objetivo_carga == true) {
    int punto_objetivo = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento].punto_objetivo;
   

    int fila_objetivo = offset_potencia_terminal + punto_objetivo;
    
    //Se carga el elemento diferente de 0, la fila en la que se encuentra y la columna donde se encuentra.
    matriz_p->P_x[(*index_actual)] = COEFICIENTE_CARGA;
    matriz_p->P_i[(*index_actual)] = fila_objetivo;
    (*index_actual)++;
  }
}

//Este subprograma se utiliza para incluir el objetivo de carga de vehiculos en la matriz p
void incluir_objetivo_cuadratico_vehiculo(informacion_procesada_t* informacion_sistema, matriz_p_t* matriz_p,
  informacion_carga_terminales_t* elementos_programados_carga_terminales,int numero_terminal, int numero_elemento,
  int* index_actual) {


  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Los objetivos cuadraticos en la matriz p son diferentes dependiendo del modo de carga
  bool carga_rapida = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento].carga_rapida;

  //Se calcula el offset del termino de batería
  int offset_fila_bateria_terminal = numero_terminal * numero_puntos_simulacion;

  //Se carga el punto final en el que está presente el vehículo
  int punto_final = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento].punto_final;




  if (carga_rapida == true) {

    //Se carga el punto inicial y el punto final
    int punto_inicial = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento].punto_inicio;
    

    //Se escribe el objetivo de carga para todos los puntos de simulacion en las que el vehiculo está presente
    for (int punto_actual = punto_inicial+1; punto_actual <= punto_final; punto_actual++) {
      matriz_p->P_x[(*index_actual)] = COEFICIENTE_CARGA;
      matriz_p->P_i[(*index_actual)] = offset_fila_bateria_terminal + punto_actual;
      (*index_actual)++;
      
      
    }
  }

  else {
   
    matriz_p->P_x[(*index_actual)] = COEFICIENTE_CARGA;
    matriz_p->P_i[(*index_actual)] = offset_fila_bateria_terminal + punto_final;
    (*index_actual)++;
    
  }
}


/*Esta funcion se utiliza para incluir los objetivos cuadraticos de preservacion de las baterías, para evitar que
 pierdan potencia sin ningún motivo aparente (que no vuelcen a la red sin ningún beneficio)*/

void incluir_objetivo_cuadratico_preservacion_bateria(informacion_procesada_t* informacion_sistema,matriz_p_t* matriz_p,
  informacion_carga_terminales_t* elementos_programados_carga_terminal, int numero_terminal, int numero_elemento,
  int* index_actual) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se calcula el offset de la potencia del terminal 
  int offset_fila_potencia_terminal = (NUMERO_TERMINALES)*numero_puntos_simulacion + numero_terminal * numero_puntos_simulacion;

  //Se carga el punto final en el que está presente la batería
  int punto_final = elementos_programados_carga_terminal->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento].punto_final;

  //Se carga el punto inicial a partir del cual en el que está presente la batería
  int punto_inicial = elementos_programados_carga_terminal->informacion_carga_terminales[numero_terminal].elementos_terminal[numero_elemento].punto_inicio;

  //Se escribe el objetivo de preservacion de bateria para todos los puntos de simulacion en los cuales la
  //bateria esta presente.
  for (int punto_actual = punto_inicial + 1; punto_actual <= punto_final; punto_actual++) {
    matriz_p->P_x[(*index_actual)] = COEFICIENTE_PRESERVACION_BATERIA;
    matriz_p->P_i[(*index_actual)] = offset_fila_potencia_terminal + punto_actual;
    (*index_actual)++;
  }


}





/*Este tipo se utiliza para almacenar informacion relevante de la bateria el punto de inicio, el punto final
  y la variable booleana que sirve para identificar si la carga del vehiculo es */


void calcular_objetivos_cuadraticos_carga_terminal(informacion_procesada_t* informacion_sistema, matriz_p_t* matriz_p,
  informacion_carga_terminales_t* elementos_programados_carga_terminales,int numero_terminal,int* index_actual,
  int* fila_actual) {

  //Se procede a iterar por todos los elementos que tienen su carga programada en el terminal
  //Primero se carga el numero de elementos que tienen su carga programada en el terminal
  int numero_elementos_terminal = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;

  for (int elemento_terminal = 0; elemento_terminal < numero_elementos_terminal; elemento_terminal++) {
    
    //La forma de incluir el objetivo cuadratico es diferente de si el elemento a añadir es un vehiculo o
    //una bateria.
    if (elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_terminal].vehiculo == true) {
      
      incluir_objetivo_cuadratico_vehiculo(informacion_sistema, matriz_p, elementos_programados_carga_terminales,
        numero_terminal,elemento_terminal,index_actual);
    }

    else {

      incluir_objetivo_cuadratico_bateria(informacion_sistema, matriz_p, elementos_programados_carga_terminales,
        numero_terminal,elemento_terminal, index_actual);
    }
  }
 
}


static void calcular_objetivos_preservacion_bateria_terminal(informacion_procesada_t* informacion_sistema,matriz_p_t* matriz_p,
  informacion_carga_terminales_t* elementos_programados_carga_terminales,int numero_terminal,int* index_actual,
  int* fila_actual) {

  //Se procede a iterar por todos los elementos que tienen su carga programada en el terminal
  int numero_elementos_terminal = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;

  for (int elemento_terminal = 0; elemento_terminal < numero_elementos_terminal; elemento_terminal++) {

   
    if (elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_terminal].bateria == true) {
      incluir_objetivo_cuadratico_preservacion_bateria(informacion_sistema, matriz_p, elementos_programados_carga_terminales,
        numero_terminal, elemento_terminal, index_actual);
    }
  }

}


/*Se utiliza esta funcion para incluir el objetivo de que las baterías no pierdan energía en el caso de que no
  haya ningún vehículo conectado*/


void calcular_objetivos_preservacion_baterias(informacion_procesada_t* informacion_sistema,matriz_p_t* matriz_p,
  informacion_carga_terminales_t* elementos_programados_carga_terminales,int*index_actual,int*fila_actual) {

  //Se procede a iterar por todos los elementos que tienen su carga programada en el terminal
  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {
    calcular_objetivos_preservacion_bateria_terminal(informacion_sistema, matriz_p, elementos_programados_carga_terminales,
      numero_terminal, index_actual, fila_actual);
  }

}






void calcular_objetivos_cuadraticos_carga(informacion_procesada_t* informacion_sistema, matriz_p_t* matriz_p,
  informacion_carga_terminales_t* elementos_programados_carga_terminales,int* index_actual,int*fila_actual) {

 
 


  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {

    calcular_objetivos_cuadraticos_carga_terminal(informacion_sistema, matriz_p, elementos_programados_carga_terminales,
    numero_terminal, index_actual,fila_actual);

  }


}

/*Este subprograma se utiliza para incluir las columnas en el vector P_p para incluir los objetivos de carga de los
 vehiculos*/

void incluir_columnas_objetivo_carga_vehiculo(informacion_carga_terminales_t* elementos_programados_carga_terminales,
  matriz_p_t* matriz_p,int* index_vector_P_p,int* punto_actual,int*columna_actual,int* index_elemento_carga_terminales,
  int numero_terminal) {

  //Primero se inspecciona si el vehiculo cuyo objetivo de carga se están añadiendo las columnas
  bool carga_rapida = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[*index_elemento_carga_terminales].carga_rapida;


  //Se carga el punto inicial del vehiculo que tiene su carga programada
  int punto_inicial = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[*index_elemento_carga_terminales].punto_inicio;
  //Se carga el punto final del vehiculo que tiene su carga programada
  int punto_final = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[*index_elemento_carga_terminales].punto_final;

  //El modo de escribir en el vector P_p es diferente de si la carga es rapida o no

  if (carga_rapida == true) {

    //Se carga el objetivo de carga hasta el punto final
    while ((*punto_actual) <= punto_final) {


      if ((*punto_actual) != punto_inicial) {
        matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
        (*index_vector_P_p)++;
        (*columna_actual)++;
        (*punto_actual)++;
      }

      else {
        matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
        (*punto_actual)++;
        (*index_vector_P_p)++;
      }
    }
  }

  else {
    //Si la carga no es rapida solo hay que incrementar la posicion de la columna cuando se pasa el punto final
    while ((*punto_actual) <= punto_final) {

      if ((*punto_actual) < punto_final) {
        matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
      }

      else {
        //Se incluye la posicion del objetivo
        matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
        (*columna_actual)++;
      }

      (*punto_actual)++;
      (*index_vector_P_p)++;
    }

  }
  (*index_elemento_carga_terminales)++;
}

/*Este subprograma se utiliza para incluir las columnas cuando hay baterias que tienen su carga programada
  en el terminal*/

void incluir_columnas_objetivo_baterias(informacion_carga_terminales_t* elementos_programados_carga_terminales,
  matriz_p_t* matriz_p,int* index_vector_P_p,int* punto_actual,int*columna_actual,int* index_elemento_carga_adicional,
  int numero_terminal) {

  //Se inspecciona si la bateria que tiene la carga programada en el terminal
  bool carga_objetivo = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[(*index_elemento_carga_adicional)].considerar_objetivo;

  //Se cargan los puntos iniciales y finales en los que están conectadas las baterias
  int punto_inicial_bateria = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[(*index_elemento_carga_adicional)].punto_inicio;
  int punto_final_bateria = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[(*index_elemento_carga_adicional)].punto_final;

  //Si hay un objetivo de carga es necesario actualizar la columna donde aparece el objetivo de carga
  if (carga_objetivo == true) {

    //Se carga el punto en el que está el punto objetivo
    int punto_objetivo = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[(*index_elemento_carga_adicional)].punto_objetivo;

    while ((*punto_actual) <= punto_final_bateria) {

      //Si el punto actual coincide con el punto objetivo, se añade el numero en el vector P_p 
      if (punto_objetivo == (*punto_actual)) {
        matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
        (*index_vector_P_p)++;
        (*punto_actual)++;
        (*columna_actual)++;

      }

      else {
        matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
        (*index_vector_P_p)++;
        (*punto_actual)++;
      }
    }
  }

  //Si no hay ningún objetivo de carga para las baterías pues se tendría que esas columnas son todo 0 y hay que
  //rellenar en el vector P_p en consecuencia 
  else {
    while ((*punto_actual) <= punto_final_bateria) {
      matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
      (*index_vector_P_p)++;
      (*punto_actual)++;
    }

  }
  (*index_elemento_carga_adicional)++;
}


/*Este subprograma se utiliza para incluir las columnas en el vector P_p si hay baterias que tienen su carga
  programada en el terminal */

void incluir_columnas_preservacion_bateria(informacion_carga_terminales_t* elementos_programados_carga_terminal,
  matriz_p_t* matriz_p, int* index_vector_P_p, int* punto_actual, int* columna_actual, int* index_elemento_carga_adicional,
  int numero_terminal) {

  /*Se carga el punto inicial y final en el que está conectada la batería */
  int punto_inicial_bateria = elementos_programados_carga_terminal->informacion_carga_terminales[numero_terminal].elementos_terminal[*index_elemento_carga_adicional].punto_inicio;
  int punto_final_bateria = elementos_programados_carga_terminal->informacion_carga_terminales[numero_terminal].elementos_terminal[*index_elemento_carga_adicional].punto_final;



  //Durante el punto inicial la bateria no puede recibir potencia ya que la variable P terminal t es para aceptar
  //potencias de instantes pasados

    matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
    (*index_vector_P_p)++;
    (*punto_actual)++;

    while ((*punto_actual) <= punto_final_bateria) {

      

      matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
      (*index_vector_P_p)++;
      (*columna_actual)++;
      (*punto_actual)++;
    }
  
  (*index_elemento_carga_adicional)++;
}


/*Este subprograma se utiliza para incluir las columnas en el vector P_p si hay baterias que tienen su carga
  programada en el terminal */

void incluir_columnas_vector_P_p_potencias_baterias_terminal(informacion_procesada_t* informacion_sistema,
  matriz_p_t* matriz_p, informacion_carga_terminales_t* elementos_programados_carga_terminal, int* columna_actual,
  int* index_vector_P_p, int terminal_actual) {

  //Se crea una variable para llevar la cuenta de a que elemento que tiene su carga programada en el terminal
  int index_elemento_carga_terminal = 0;

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se carga el numero vehiculos o baterias que tienen su carga programada en el terminal
  int numero_elementos_terminal = elementos_programados_carga_terminal->informacion_carga_terminales[terminal_actual].numero_elementos_terminal;

  //Se define una variable para llevar la cuenta  de por que punto de simulacion se va
  int punto_simulacion_actual = 0;

  //Se definen variables para almacenar el punto inicial y final de los elementos que tienen su carga programad
  //en el terminal
  int punto_inicial = elementos_programados_carga_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga_terminal].punto_inicio;
  int punto_final = elementos_programados_carga_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga_terminal].punto_final;

  while (punto_simulacion_actual <= numero_puntos_simulacion) {

    if (numero_elementos_terminal > index_elemento_carga_terminal) {

      if (comprobar_rango(punto_simulacion_actual, punto_inicial, punto_final)) {

        bool bateria = elementos_programados_carga_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga_terminal].bateria;

        if (bateria == true) {
          incluir_columnas_preservacion_bateria(elementos_programados_carga_terminal, matriz_p,
            index_vector_P_p, &punto_simulacion_actual, columna_actual, &index_elemento_carga_terminal, terminal_actual);

          //Si hay mas elementos que tengan programada su carga en el terminal se carga su punto inicial y final

          if (numero_elementos_terminal > index_elemento_carga_terminal) {
            punto_inicial = elementos_programados_carga_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga_terminal].punto_inicio;
            punto_final = elementos_programados_carga_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga_terminal].punto_final;
          }
        }

        //Si no hay baterias que tengan su carga programada en el terminal 
        else {

          while (punto_simulacion_actual <= punto_final) {

            matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
            (*index_vector_P_p)++;
            punto_simulacion_actual++;

            //Se actualiza el index de elementos que tienen su carga programada en el terminal

            if (punto_simulacion_actual == punto_final) {

              index_elemento_carga_terminal++;

              if (numero_elementos_terminal > index_elemento_carga_terminal) {

                punto_inicial = elementos_programados_carga_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga_terminal].punto_inicio;
                punto_final = elementos_programados_carga_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga_terminal].punto_final;

              }
            }
          }
        }
      }

      /*No hay más baterías que tengan su carga programada en el terminal*/
      else {
        matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
        (*index_vector_P_p)++;
        punto_simulacion_actual++;
      }

    }

    //No hay baterias que tengan programada su carga en el terminal
    else {
      matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
      (*index_vector_P_p)++;
      punto_simulacion_actual++;

    }
  }
}






/*Este subprograma se utiliza para incluir las columnas en el vector P_p si hay elementos que tienen su
  carga programada en el terminal*/

void incluir_columnas_vector_P_p_elementos_terminal(informacion_procesada_t* informacion_sistema, matriz_p_t * matriz_p,
     informacion_carga_terminales_t* elementos_programados_carga_terminales,
     int* columna_actual, int* index_vector_P_p,int terminal_actual) {

  //Se carga el punto inicial y final del elemento que tiene su carga programada en el terminal
  int index_elemento_carga_terminal = 0;

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se carga el numero que de vehículos o baterías que tienen 
  int numero_elementos_terminales = elementos_programados_carga_terminales->informacion_carga_terminales[terminal_actual].numero_elementos_terminal;

  //Se define una variable para llevar la cuenta de por que punto de simulacion se lleva la cuenta
  int punto_simulacion_actual = 0;

  //Se define una variable para almacenar el punto inicial del siguiente elemento que tiene su carga
  //programada en el terminal
  int punto_inicial = elementos_programados_carga_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga_terminal].punto_inicio;
  int punto_final   = elementos_programados_carga_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga_terminal].punto_final;

  while (punto_simulacion_actual < numero_puntos_simulacion) {

    //Se carga el punto inicial del siguiente elemento a programar su carga 
    

    if (numero_elementos_terminales > index_elemento_carga_terminal) {


      if (comprobar_rango(punto_simulacion_actual,punto_inicial,punto_final)) {

        //Se crea una variable booleana para identificar si el siguiente elemento que tiene su carga programada en
        //el terminal 
        bool vehiculo = elementos_programados_carga_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga_terminal].vehiculo;

        //Si hay un vehiculo se llama al subprograma que añade las columnas del objetivo de carga de los vehículos
        if (vehiculo == true) {

          incluir_columnas_objetivo_carga_vehiculo(elementos_programados_carga_terminales, matriz_p, index_vector_P_p,
            &punto_simulacion_actual, columna_actual, &index_elemento_carga_terminal, terminal_actual);
        }

        //Si hay una batería se llama al subprograma quea añade las columans del objetivo de carga de las baterías
        //si lo hubiera
        else {
          incluir_columnas_objetivo_baterias(elementos_programados_carga_terminales, matriz_p, index_vector_P_p,
            &punto_simulacion_actual, columna_actual, &index_elemento_carga_terminal, terminal_actual);
        }

        //Si hay más vehiculos o baterías que tienen su carga programada en el terminal, se actualiza el punto
        //inicial y final del siguiente elemento que tiene su carga programada.
        if (numero_elementos_terminales > index_elemento_carga_terminal) {
          punto_inicial = elementos_programados_carga_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga_terminal].punto_inicio;
          punto_final = elementos_programados_carga_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga_terminal].punto_final;
        }


      }

      //Si para el punto de simulacion actual no corresponde todavía a ningún objetivo de carga se actualiza
      //el vector P_p en consencuencia.

      else {
        matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
        (*index_vector_P_p)++;
        punto_simulacion_actual++;
      }
    }

    //Si no se tienen más elementos que tienen su carga programada en el terminal se actualiza el vector
    //P_p en consencuencia

    else {
      matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
      (*index_vector_P_p)++;
      punto_simulacion_actual++;
    }

  }
}



/*El siguiente subprograma se utiliza para incluir donde empiezan y acaban las diferentes columnas
  de la matriz p teniendo en cuenta los elementos del vector P_i y P_x*/

void calcular_vector_P_p_objetivo_carga (informacion_procesada_t* informacion_sistema, matriz_p_t *matriz_p,
  informacion_carga_terminales_t* elementos_programados_carga_terminales, int* index_vector_P_p,
  int* index_columna_actual) {


  //Los unicos terminos que pueden aparecer en la matriz P son los terminos SOC (state of charge) así que
  //solo se tienen estos terminos el resto de columnas es más fácil de añadir

  //Se carga el número de punto de simulacion que se tienen
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  
  
  

  //Se crea una variable para llevar la cuenta de a los elementos que se acceden que tienen su carga progamada

  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {

    //Se carga el numero de elementos que tienen su carga programada esta terminal
    int numero_elementos_terminales = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;

    if (numero_elementos_terminales > 0){
      
      incluir_columnas_vector_P_p_elementos_terminal(informacion_sistema, matriz_p, elementos_programados_carga_terminales,
         index_columna_actual, index_vector_P_p,numero_terminal);

    }

    //Si no hay elemento que tenga su carga programada en el terminal todas las columnas asociadas a esta variable
    //son 0. 
    else {
      
      for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {
        matriz_p->P_p[(*index_vector_P_p)] = (*index_columna_actual);
        (*index_vector_P_p)++;
      }
    }
  }
}


/*Se utiliza este subprograma para poder calcular el vector P_p teniendo en cuenta la preservacion de las baterias
  si no tienen nada conectado*/

void calcular_vector_P_p_objetivo_preservacion_bateria_terminal(informacion_procesada_t* informacion_sistema,
  matriz_p_t* matriz_p,informacion_carga_terminales_t* elementos_programados_carga_terminales,int*index_vector_P_p,
  int* index_columna_actual) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {

    //Se carga el numero de elementos que tienen su carga programada esta terminal
    int numero_elementos_terminales = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;

    //Si hay elementos que tengan su carga programada en el terminal
    if (numero_elementos_terminales > 0) {
      
      incluir_columnas_vector_P_p_potencias_baterias_terminal(informacion_sistema, matriz_p, elementos_programados_carga_terminales,
        index_columna_actual, index_vector_P_p, numero_terminal);
    }

    //Si no hay elementos que tengan su carga programada en el terminal
    else {
      for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {
        matriz_p->P_p[(*index_vector_P_p)] = (*index_columna_actual);
        (*index_vector_P_p)++;
      }
    }
  }

}







/*Se define el siguiente subprograma para rellenar los elementos restantes del vector P_p*/
void calcular_resto_elementos_P_p(informacion_procesada_t* informacion_sistema,matriz_p_t *matriz_p,
  informacion_carga_terminales_t* elementos_programados_carga_terminales,int* index_vector_P_p,
  int* columna_actual) {

  //Se carga el numero total de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se calcula el numero total de elementos que tiene que tener el vector P_p dentro de la matriz P
  int limite_potencia_entrada_red = 2*(NUMERO_TERMINALES)*numero_puntos_simulacion + 2 * numero_puntos_simulacion;
  int limite_potencia_salida_red = limite_potencia_entrada_red + numero_puntos_simulacion;
  int numero_elementos_P_p = (NUMERO_VARIABLES)*numero_puntos_simulacion + 1;

  while ((*index_vector_P_p < limite_potencia_entrada_red)) {
    matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
    (*index_vector_P_p)++;
  }

  while ((*index_vector_P_p < limite_potencia_salida_red)) {
    matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
    (*columna_actual)++;
    (*index_vector_P_p)++;
  }


  while ((*index_vector_P_p < numero_elementos_P_p)) {
    matriz_p->P_p[(*index_vector_P_p)] = (*columna_actual);
    (*index_vector_P_p)++;
  }

}


/*Este subprograma se utiliza para calcular el vector P_p, que indica en que posiciones empiezan las difernetes
  columnas de la matriz P */
void calcular_vector_P_p(informacion_procesada_t* informacion_sistema,matriz_p_t *matriz_p,
  informacion_carga_terminales_t* elementos_programados_carga_terminales) {

  //Se llama al subprograma que se encarga de calcular los elementos del vector P_p relacionados con los
  //objetivos de carga
  //Se define una variable para llevar la cuenta de que elemento del vector P_p se va rellenando
  int index_vector_P_p = 0;
  int columna_actual = 0;

  calcular_vector_P_p_objetivo_carga(informacion_sistema,matriz_p,elementos_programados_carga_terminales,&index_vector_P_p,&columna_actual);
  //calcular_vector_P_p_objetivo_preservacion_bateria_terminal(informacion_sistema, matriz_p, elementos_programados_carga_terminales, &index_vector_P_p, &columna_actual);
  calcular_resto_elementos_P_p(informacion_sistema, matriz_p, elementos_programados_carga_terminales,
   &index_vector_P_p, &columna_actual);

}

/*Se incluye este subprograma para incluir un objetivo que se encargue de reducir la potencia que sale del
  sistema*/

void calcular_objetivo_reduccion_potencia_salida(informacion_procesada_t* informacion_sistema, matriz_p_t* matriz_p,int* index_actual) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se define el offset de la fila  en la cual se encuentra el termino Pout grid,t
  int offset_fila_potencia_salida = (NUMERO_TERMINALES) * 2 * numero_puntos_simulacion + 2 * numero_puntos_simulacion;

  //Se itera por todos los puntos de simulacion
  for (int punto_simulacion_actual = 0; punto_simulacion_actual < numero_puntos_simulacion; punto_simulacion_actual++) {
    
    matriz_p->P_x[*index_actual] = COEFICIENTE_PRESERVACION_BATERIA ;
    matriz_p->P_i[*index_actual] = offset_fila_potencia_salida + punto_simulacion_actual;
    
    (*index_actual)++;
    
    
  }


}


/*Este subprograma se utiliza para calcular la matriz p del problema de optimización */

int calcular_matriz_p(informacion_procesada_t* informacion_sistema,problema_optimizacion_t* problema_optimizacion,
  informacion_carga_terminales_t* elementos_programados_carga_terminales) {

  matriz_p_t* matriz_p = &(problema_optimizacion->matriz_p);
  //Se reserva memoria para la matriz P
  matriz_p->P = malloc(sizeof(OSQPCscMatrix));

  //Se procede a calcular el numero de variables
  OSQPInt numero_variables = problema_optimizacion->numero_variables;


 
 

  //Se carga el numero de variables
  

  if (matriz_p->P == NULL) {
    printf("No se ha podido reservar memoria para la matriz P\n");
    registrar_error("No se ha podido reservar memoria para la matriz P\n",REGISTRO_ERRORES);
    return ERROR;
  }
 
  
  if (dimensionar_matriz_P(informacion_sistema, elementos_programados_carga_terminales, matriz_p) == ERROR) {
    printf("No se ha podido dimensionar la matriz P correctamente\n");
    registrar_error("No se ha podido dimensionar la matriz P correctamente\n", REGISTRO_ERRORES);
    return ERROR;
  }
 
  //Se crean variables para llevar la cuenta del index actual y la fila actual
  int fila_actual = 0;
  int index_actual = 0;


  calcular_objetivos_cuadraticos_carga(informacion_sistema,matriz_p,elementos_programados_carga_terminales,&index_actual,&fila_actual);
  
 // calcular_objetivos_preservacion_baterias(informacion_sistema, matriz_p, elementos_programados_carga_terminales,&index_actual,&fila_actual);
  calcular_objetivo_reduccion_potencia_salida(informacion_sistema, matriz_p, &index_actual);

  calcular_vector_P_p(informacion_sistema, matriz_p, elementos_programados_carga_terminales);
  
 
  //Se rellenan los datos de la matriz P
  csc_set_data(matriz_p->P,numero_variables,numero_variables,matriz_p->P_nnz,matriz_p->P_x,matriz_p->P_i,matriz_p->P_p);
 
  return EXITO;
}