#include "definiciones_globales.h"
#include "operaciones_fecha.h"
#include "operaciones_string.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


void printTm( struct tm* date) {
  if (date == NULL) {
    printf("The date is NULL\n");
    return;
  }

  // Note: tm_year is years since 1900, tm_mon is 0-11, and tm_mday is 1-31
  printf("Year: %d, Month: %02d, Day: %02d\n",
    date->tm_year + 1900, date->tm_mon + 1, date->tm_mday);
  printf("Hour: %02d, Minute: %02d, Second: %02d\n",
    date->tm_hour, date->tm_min, date->tm_sec);

  // Optionally, you can print the day of the week and day of the year
  printf("Weekday (Sunday=0): %d, Day of the year: %d\n",
    date->tm_wday, date->tm_yday);
}


int comparar_valores_tm(struct tm fecha_1, struct tm fecha_2) {
  time_t t1 = mktime(&fecha_1);
  time_t t2 = mktime(&fecha_2);

  if (t1 < t2) return -1;
  if (t1 > t2) return 1;
  return 0;
}




/*El subprograma siguiente se utiliza para eliminar las fechas repetidas del array
  de fechas adicionales */
int ajustar_array_fechas_adicionales(struct tm** fechas_adicionales, int* tamanyo_array) {
  if (*tamanyo_array <= 1) return ERROR;
  
  //Se ordena el array en orden ascendentes de fechas
  qsort(*fechas_adicionales, *tamanyo_array, sizeof(struct tm), comparar_valores_tm);

  
  //Se pasa a eliminar las fechas duplicadas
  int indice_escritura = 0;

  for (int indice_lectura = 1; indice_lectura < *tamanyo_array; ++indice_lectura) {
    if (comparar_valores_tm(((*fechas_adicionales)[indice_lectura]), ((*fechas_adicionales)[indice_escritura])) != 0) {
     
      indice_escritura++;
      (*fechas_adicionales)[indice_escritura] = (*fechas_adicionales)[indice_lectura];
    }
  }
  *tamanyo_array = indice_escritura +1;
  
  struct tm* tempPtr = (struct tm*)realloc(*fechas_adicionales, sizeof(struct tm) * (*tamanyo_array));
  //struct tm* tempPtr = realloc(*fechas_adicionales, sizeof(struct tm) * (*tamanyo_array));
  
  if (tempPtr != NULL) {
    *fechas_adicionales = tempPtr;
    return EXITO;
  }
  else {
    // Realloc failed, handle error or continue with original size
    fprintf(stderr, "Error: No se ha podido redimensionar el array de fechas adicionales.\n");
    registrar_error("Error: No se ha podido redimensionar el array de fechas adicionales.\n", REGISTRO_ERRORES);
    return ERROR;
  }
  /* /* */
  return EXITO;
}
//struct tm* tempPtr = (struct tm*)realloc(*fechas_adicionales, sizeof(struct tm) * tamanyo_actual);

/*Este subprograma se utiliza para añadir las fechas adicionales correspondientes a la conexión, desconexión y
  fecha objetivo de las baterías*/

int cargar_fechas_adicionales_baterias(datos_csv_baterias_t* datos_baterias, struct tm** fechas_adicionales,
  int* numero_fechas_adicionales) {
  
  //Se carga un puntero que apunta a donde se encuentra la informacion leida de los csv de las baterias
  datos_csv_t* informacion_baterias = &(datos_baterias->informacion_baterias);
  
  //Se carga el tamaño actual del array de fechas
  int tamanyo_actual = *numero_fechas_adicionales;
  //Se carga el numero de filas que contiene el csv de las baterias
  int numero_filas_baterias = datos_baterias->informacion_baterias.filas - 1;
  //Se carga la ubicacion donde se encuentra la informacion en el csv de las baterias
  //Se carga las ubicaciones donde se encuentra la fecha inicial de la bateria
  int columna_anyo_inicial_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_anyo;
  int columna_mes_inicial_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_mes;
  int columna_dia_inicial_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_dia;
  int columna_hora_inicial_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_hora;
  int columna_minuto_inicial_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_minuto;
  //Se carga las ubicaciones donde se encuentra la fecha final de la bateria
  int columna_anyo_final_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_anyo;
  int columna_mes_final_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_mes;
  int columna_dia_final_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_dia;
  int columna_hora_final_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_hora;
  int columna_minuto_final_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_minuto;
  //Se carga las ubicaciones donde se encuentra fecha objetivo de la bateria
  int columna_anyo_objetivo_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_anyo;
  int columna_mes_objetivo_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_mes;
  int columna_dia_objetivo_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_dia;
  int columna_hora_objetivo_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_hora;
  int columna_minuto_objetivo_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_minuto;

  //Se carga la ubicacion donde se encuentra si se considera el posible objetivo de carga de la bateria o no.
  int columna_objetivo_carga_bateria = datos_baterias->posiciones_informacion_baterias.columna_consideracion_objetivo;

  
  for (int bateria = 0; bateria < numero_filas_baterias; bateria++) {
    

  
    
    if (strings_iguales(informacion_baterias->datos[bateria + 1][columna_objetivo_carga_bateria],"si") == true) {
      
      tamanyo_actual += 3;
      
      *numero_fechas_adicionales += 3;
      struct tm* tempPtr = (struct tm*)realloc(*fechas_adicionales, sizeof(struct tm) * tamanyo_actual);
     
      if (tempPtr == NULL) {
        printf("Fallo en la adición de las fechas adicionales en los puntos de simulación, fallo al reservar memoria\n");
        registrar_error("Fallo en la adición de las fechas adicionales en los puntos de simulación, fallo al reservar memoria\n", REGISTRO_ERRORES);
        return ERROR;
      }
      //Se pasa a situar en el array de fechas adicionales las fechas iniciales finales y objetivo de las baterias
     
      *fechas_adicionales = tempPtr;
     
      
      
      
      if (cargar_fecha(informacion_baterias, &((*fechas_adicionales)[tamanyo_actual - 3]), columna_anyo_inicial_bateria,
        columna_mes_inicial_bateria, columna_dia_inicial_bateria, columna_hora_inicial_bateria, columna_minuto_inicial_bateria,
        bateria + 1, SI_INCLUIR_MINUTO) == ERROR) {
        printf("Ha habido un error cargando la fecha inicial de la bateria numero %d", bateria);
        registrar_error("Ha habido un error cargando la fecha inicial de la bateria numero %d", REGISTRO_ERRORES);
        return ERROR;
      }
      
      
     

    
      if (cargar_fecha(informacion_baterias, &((*fechas_adicionales)[tamanyo_actual - 2]), columna_anyo_objetivo_bateria,
        columna_mes_objetivo_bateria, columna_dia_objetivo_bateria, columna_hora_objetivo_bateria, columna_minuto_objetivo_bateria,
        bateria + 1, SI_INCLUIR_MINUTO) == ERROR) {
        printf("No se ha podido cargar la fechas objetivos de las baterias en la informacion procesada\n");
        registrar_error("No se ha podido cargar las fechas objetivos de las baterias en la informacion procesada", REGISTRO_ERRORES);
        return ERROR;
      }
      
     
      
      if (cargar_fecha(informacion_baterias, &((*fechas_adicionales)[tamanyo_actual - 1]), columna_anyo_final_bateria,
        columna_mes_final_bateria, columna_dia_final_bateria, columna_hora_final_bateria, columna_minuto_final_bateria,
        bateria + 1, SI_INCLUIR_MINUTO) == ERROR) {
        printf("No se ha podido cargar las fechas finales de las baterias en la informacion procesada\n");
        registrar_error("No se ha podido cargar las fechas finales de las baterias en la informacion procesada", REGISTRO_ERRORES);
        return ERROR;
      }
      
    }
    else {
      
      tamanyo_actual += 2;
     
      *numero_fechas_adicionales += 2;
     
      struct tm* tempPtr = (struct tm*)realloc(*fechas_adicionales, sizeof(struct tm) * tamanyo_actual);
      if (tempPtr == NULL) {
        printf("Fallo en la adición de las fechas adicionales en los puntos de simulación, fallo al reservar memoria\n");
        registrar_error("Fallo en la adición de las fechas adicionales en los puntos de simulación, fallo al reservar memoria\n", REGISTRO_ERRORES);
        return ERROR;
      }
      *fechas_adicionales = tempPtr;
      //Se cargan la fecha inicial y final de la bateria en el array de fechas adicionales a considerar en la simulacion 

      
      if (cargar_fecha(informacion_baterias, &((*fechas_adicionales)[tamanyo_actual - 2]), columna_anyo_inicial_bateria,
        columna_mes_inicial_bateria, columna_dia_inicial_bateria, columna_hora_inicial_bateria, columna_minuto_inicial_bateria,
        bateria + 1, SI_INCLUIR_MINUTO) == ERROR) {
        printf("Problema al cargar las fechas iniciales de las baterias en la informacion procesada\n");
        registrar_error("Problema al cargar la fecha inicial de la baterias en la informacion procesada\n", REGISTRO_ERRORES);
        return ERROR;
      }
     
    

      
      if (cargar_fecha(informacion_baterias, &((*fechas_adicionales)[tamanyo_actual - 1]), columna_anyo_final_bateria,
        columna_mes_final_bateria, columna_dia_final_bateria, columna_hora_final_bateria, columna_minuto_final_bateria,
        bateria + 1, SI_INCLUIR_MINUTO) == ERROR) {
        printf("Problema al cargar las fechas finales de las baterias en la informacion procesada\n");
        registrar_error("Problema al cargar la fecha finales de las baterias en la informacion procesada\n", REGISTRO_ERRORES);
        return ERROR;
      }
      
      
     


       
    }
    
   
  }
  
  
  return EXITO;
}




 /*Este subprograma se utiliza para cargar las fechas adicionales que corresponden a las horas en punto
   estas deben ser añadidas, esto es porque el precio cambia de hora a hora así que los momentos en los
   que se tiene una hora en punto debe ser considerado en la simulacion*/

int cargar_fechas_adicionales_en_punto(struct tm** fechas_adicionales, struct tm* fecha_inicial_algoritmo,
                                       struct tm* fecha_final_algoritmo,  int* numero_fechas_adicionales) {

  

  time_t inicio = mktime(fecha_inicial_algoritmo);
  time_t fin    = mktime(fecha_final_algoritmo);
  


  //Se ajusta la fecha de inicio a una hora en punto si no lo está para contar correctamente el numero de
  //horas en punto
  struct tm* inicio_tm = localtime(&inicio);
  //printTm(inicio_tm);
  if (inicio_tm->tm_min != 0 || inicio_tm->tm_sec != 0) {
    inicio_tm->tm_min = 0;
    inicio_tm->tm_sec = 0;
    inicio_tm->tm_hour++; //Se avanza a la siguiente hora
    inicio = mktime(inicio_tm); //Se recalcula la fecha de inicio
  }
  


  //Se calcula el tamaño para añadir todos los tiempos en puntos entre comienzo y fin del algoritmo 
  struct tm temp;
  int numero_fechas_en_punto = 0;
  for (time_t t = inicio; t <= fin; t += 3600) { // Cada iteracion es una hora
    temp = *localtime(&t);
    numero_fechas_en_punto++;
  }
  
  //Se reajusta el tamaño del array de las fechas adicionales
  int total_fechas = *numero_fechas_adicionales + numero_fechas_en_punto;
  struct tm* nuevas_fechas = realloc(*fechas_adicionales, total_fechas * sizeof(struct tm));


  
 // Se maneja la posibilidad que el reajuste de memoria no tenga éxito
  if (nuevas_fechas == NULL) {
    printf("Error al añadir las fechas en punto al array de fechas adicionales\n");
    registrar_error("Error al añadir las fechas en punto al array de fechas adicionales\n", REGISTRO_ERRORES);
    return ERROR;
  }

  *fechas_adicionales = nuevas_fechas;
  
  //Se rellena el array con las nuevas fechas en punto 
  int i = *numero_fechas_adicionales;
  for (time_t t = inicio; t <= fin; t += 3600) { // Iteracion en cada hora
    temp = *localtime(&t);
    //struct tm nueva_fecha = malloc(sizeof(struct tm));
    /*
    if (nueva_fecha == NULL) { //Se maneja el caso que haya un fallo en la realoación de memoria
      printf("Error al añadir las fechas adicionales en punto en la simulacion\n");
      registrar_error("Error al añadir las fechas en punto al array de fechas adicionales\n", REGISTRO_ERRORES);
      return ERROR;
    }
    */
    struct tm nueva_fecha = temp;
    (*fechas_adicionales)[i++] = nueva_fecha;
  }
  
  //Se comprueba que el array termina en null
  /*
  if (i < total_fechas) {
    fechas_adicionales[i] = NULL;
  }
  */
  *numero_fechas_adicionales = total_fechas;
  /* * */
  return EXITO;
  
}

 /*Este subprograma se utiliza para */

int cargar_fechas_adicionales_vehiculos(datos_csv_vehiculos_t* datos_vehiculos,struct tm ** fechas_adicionales,
                                        int* numero_fechas_adicionales) {
  //Cargo las ubicaciones de donde se encuentra la informacion relevante en el csv de los vehiculos.
 //Ubicacion de la fecha inicial de carga del vehiculo
  int anyo_inicial_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_anyo;
  int mes_inicial_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_mes;
  int dia_inicial_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_dia;
  int hora_inicial_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_hora;
  int minuto_inicial_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_minuto;
  //Ubicacion de la fecha final del vehiculo
  int anyo_final_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_anyo;
  int mes_final_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_mes;
  int dia_final_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_dia;
  int hora_final_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_hora;
  int minuto_final_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_minuto;
  //Tantas vehiculos como filas-1 tenga el csv de los vehículos
  int numero_vehiculos = datos_vehiculos->informacion_vehiculos.filas - 1;
  


  //fechas_adicionales = calloc(numero_vehiculos *2 , sizeof(struct tm*));
  *fechas_adicionales = calloc(numero_vehiculos * 2, sizeof(struct tm));
  



  //Se pasa a rellenar el array de fechas de tipo tm* con todas las fechas adicionales de los vehiculos que
  //hay que añadir
 
  if (*fechas_adicionales == NULL) {
    printf("Error Aquí\n");
    fprintf(stderr, "No se ha podido reservar memoria para la informacion procesada.\n");
    registrar_error("No se ha podido reservar memoria para la informacion procesada", REGISTRO_ERRORES);
    return ERROR;
  }

  
 
  for (int i = 0; i <numero_vehiculos; i++) {
    
   
    //Se carga la fecha inicial del vehiculo
    (*fechas_adicionales)[2 * i].tm_year = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][anyo_inicial_vehiculo]) - 1900;
    (*fechas_adicionales)[2 * i].tm_mon = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][mes_inicial_vehiculo]) - 1;
    (*fechas_adicionales)[2 * i].tm_mday = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][dia_inicial_vehiculo]);
    (*fechas_adicionales)[2 * i].tm_hour = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][hora_inicial_vehiculo]);
    (*fechas_adicionales)[2 * i].tm_min = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][minuto_inicial_vehiculo]);
    //Se carga la fecha final del vehiculo
    (*fechas_adicionales)[2 * i + 1].tm_year = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][anyo_final_vehiculo]) - 1900;
    (*fechas_adicionales)[2 * i + 1].tm_mon  = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][mes_final_vehiculo]) - 1;
    (*fechas_adicionales)[2 * i + 1].tm_mday = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][dia_final_vehiculo]);
    (*fechas_adicionales)[2 * i + 1].tm_hour = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][hora_final_vehiculo]);
    (*fechas_adicionales)[2 * i + 1].tm_min  = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][minuto_final_vehiculo]);
  }
  *numero_fechas_adicionales = 2 * numero_vehiculos;
  

  return EXITO;
}

/*Este subprograma se utiliza para cargar un array las fechas adicionales que deeben ser añadidas en el array
  de fechas adicionales*/

int leer_fechas_adicionales(datos_csv_vehiculos_t* datos_vehiculos, struct tm** fechas_adicionales,
  datos_csv_baterias_t* datos_baterias, int* numero_fechas_adicionales,struct tm* fecha_inicial_algoritmo,
  struct tm*fecha_final_algoritmo) {



  
  /*Se carga las fechas adicionales de los vehiculos que es necesario añadir*/
  if (cargar_fechas_adicionales_vehiculos(datos_vehiculos, fechas_adicionales, numero_fechas_adicionales) == ERROR) {
    printf("Error no se han podido cargar en la simulacion las fechas de los vehiculos\n");
    registrar_error("Error no se han podido cargar en la simulacion las fechas de los vehiculos\n", REGISTRO_ERRORES);
    return ERROR;
  }

  
  
  // Se carga las fechas adicionales de las baterias
  if (cargar_fechas_adicionales_baterias(datos_baterias, fechas_adicionales, numero_fechas_adicionales) == ERROR) {
    printf("Error no se han podido cargar en la simulacion las fechas de las baterias\n");
    registrar_error("Error no se han podido cargar en la simulacion las fechas de las baterias\n", REGISTRO_ERRORES);
    return ERROR;
  }
  
  /*Se carga las fechas adicionales de las fechas en punto */
  
  
  if (cargar_fechas_adicionales_en_punto(fechas_adicionales, fecha_inicial_algoritmo, fecha_final_algoritmo, numero_fechas_adicionales) == ERROR) {
    printf("Error no se han podido cargar las fechas adicionales (horas en punto) a la simulacion\n");
    registrar_error("Error no se han podido cargar en la simulacion las fechas adicionales (horas en punto) a la simulacion\n",REGISTRO_ERRORES);
    return ERROR;
  }
  
 
  /*Se eliminan las fechas repetidas y se ordenan en orden cronológico*/
  if (ajustar_array_fechas_adicionales(fechas_adicionales, numero_fechas_adicionales) == ERROR) {
    printf("Error no se han podido eliminar las fechas adicionales repetidas del array correspondiente\n");
    registrar_error("Error no se han podido eliminar las fechas adicionales repetidas correctamente\n", REGISTRO_ERRORES);
    return ERROR;
  }


  return EXITO;
  
}
/*

/*Se calcula la cantidad máxima de puntos de simulacion que se va a tener */

int calcular_numero_puntos_provisional(time_t* tiempo_actual, time_t *tiempo_final, int delta_simulacion_segundos,
                                       struct tm** fechas_adicionales) {
  // Se calcula la diferencia en segundos entre los dos tiempo
  double diferencia_segundos = difftime(*tiempo_final, *tiempo_actual);

  //Se calcula el numero de puntos de simulacion.
  int numero_puntos = (int)(diferencia_segundos / delta_simulacion_segundos);

  //Si hay un resto se añade un punto adicional 
  if ((int)diferencia_segundos % delta_simulacion_segundos > 0) {
    numero_puntos++;
  }

  // Se calcula el numero de fechas adicionales 
  int numero_fechas_adicionales = 0;
  while (fechas_adicionales[numero_fechas_adicionales] != NULL) {
  numero_fechas_adicionales++;
  }

  //Se incluye el numero de fechas adicionales al numero de puntos de simulacion
  numero_puntos += numero_fechas_adicionales;

  //Se incluye el numero de puntos correspondiente a las horas en punto
  struct tm temp_fecha_actual = *localtime(tiempo_actual);
  int numero_puntos_en_punto = 0;
  for (time_t t = *tiempo_actual; t <= *tiempo_final; t += delta_simulacion_segundos) {
    temp_fecha_actual = *localtime(&t);
    if (temp_fecha_actual.tm_min == 0 && temp_fecha_actual.tm_sec == 0) {
      numero_puntos_en_punto++;
    }
  }

  return numero_puntos += numero_puntos_en_punto;

}

/*Este subprograma se encarga de calcular los puntos de simulacion y la fecha asociada a cada uno de ellos*/
int cacular_puntos_simulacion(informacion_entrada_t* informacion_entrada, struct tm** fechas_adicionales,
  informacion_procesada_t* informacion_procesada, struct tm* fecha_inicial_algoritmo,
  struct tm* fecha_final_algoritmo, int delta_simulacion, const int* numero_fechas_adicionales) {

  /*Se crea un array para almacenar los puntos adicionales*/
  

  informacion_procesada->informacion_puntos_adicionales.puntos = (puntos_adicionales_t*)calloc(*numero_fechas_adicionales, sizeof(puntos_adicionales_t));
  informacion_procesada->informacion_puntos_adicionales.numero_puntos = *numero_fechas_adicionales;



  if (informacion_procesada->informacion_puntos_adicionales.puntos == NULL) {
    printf("Error al reservar memoria para el array de los nuevos puntos adicionales\n");
    registrar_error("Error al reservar memoria para el array de los nuevos puntosa adicionales\n", REGISTRO_ERRORES);
    return ERROR;
  }




  struct tm* fecha_actual = fecha_inicial_algoritmo;

  time_t tiempo_inicial = mktime(fecha_inicial_algoritmo);
  time_t tiempo_actual = mktime(fecha_inicial_algoritmo);
  time_t tiempo_final = mktime(fecha_final_algoritmo);

  //Se convierte el delta de simulacion de minutos a segundos
  int delta_simulacion_segundos = delta_simulacion * 60;
  informacion_procesada->informacion_puntos_simulacion.delta_minutos = delta_simulacion;
  
  //Se obtiene el tamaño provisional del numero de puntos de simulacion
  int numero_puntos_provisionales = calcular_numero_puntos_provisional(&tiempo_actual, &tiempo_final, delta_simulacion_segundos,
    fechas_adicionales);

  informacion_procesada->informacion_puntos_simulacion.puntos_simulacion = calloc(numero_puntos_provisionales, sizeof(punto_simulacion_t));


  //Se maneja el caso de que no se reserve memoria correctamente
  //Si el delta de simulacion en segundos se retorna al programa principal ya que no se puede tener un delta
  //  de 0 minutos //

  if (delta_simulacion_segundos == 0) {
    printf("No se puede tener un delta de 0 minutos\n");
    registrar_error("Fallo no se puede tener un delta de 0 minutos", REGISTRO_ERRORES);
    return ERROR;
  }


  //Cargo una variable que sirva como index del array de fechas adicionales a añadir //
  int index_fecha_adicional = 0;
  //Se crea una variable de tipo int para almacenar el resultado de la comparacion de dos fechas //
  int resultado_comparacion = 0;
  //Se crea una variable para llevar la cuenta de por cual punto de simulacion se va //
  int punto_actual = 0;

  /*Se cargan la fecha inicial del algoritmo como el primer punto*/
  informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].fecha_punto = *fecha_inicial_algoritmo;
  informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].punto_simulacion = punto_actual;
  informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].delta = 0;

  //Se comprueba si la primera fecha adicional a añadir coincide con la fecha inicial del algoritmo si es así
  //se ajusta la posicion del array de fechas adicionales a la siguiente posición
  if (comparar_valores_tm((*fechas_adicionales)[index_fecha_adicional],*fecha_inicial_algoritmo)!=-1) {
  
    informacion_procesada->informacion_puntos_adicionales.puntos[index_fecha_adicional].numero_punto = punto_actual;
    informacion_procesada->informacion_puntos_adicionales.puntos[index_fecha_adicional].fecha_adicional = *fecha_inicial_algoritmo;
    index_fecha_adicional++;
  }

  punto_actual++;
  tiempo_actual += delta_simulacion_segundos;
  fecha_actual = localtime(&tiempo_actual);

  while (tiempo_actual < tiempo_final) {

   // fecha_actual = localtime(&tiempo_actual);
    
    if (index_fecha_adicional < *numero_fechas_adicionales) {
      //Se compara la fecha que se va a añadir en la simulacion con la fecha siguiente a añadir de fechas
      //adicionales en concreto si es igual a una fecha adicional que se va a añadir o si esta por encima
       //teniendo en cuenta que la forma de proceder es distinta

    

      if (comparar_valores_tm(*fecha_actual, ((*fechas_adicionales)[index_fecha_adicional])) == 1) {
        informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].fecha_punto = ((*fechas_adicionales)[index_fecha_adicional]);
        informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].punto_simulacion = punto_actual;
        informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].delta = obtener_diferencia_minutos(&(informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual - 1].fecha_punto), &((*fechas_adicionales)[index_fecha_adicional]));
      
        informacion_procesada->informacion_puntos_adicionales.puntos[index_fecha_adicional].numero_punto = punto_actual;
        informacion_procesada->informacion_puntos_adicionales.puntos[index_fecha_adicional].fecha_adicional = ((*fechas_adicionales)[index_fecha_adicional]);

        //puntos_adicionales[index_fecha_adicional]->numero_punto = punto_actual;
        //puntos_adicionales[index_fecha_adicional]->fecha_adicional = ((*fechas_adicionales)[index_fecha_adicional]);

        //Se hace igual la fecha actual a la fecha adicional

        tiempo_actual = mktime(&((*fechas_adicionales)[index_fecha_adicional]));
        punto_actual++;
        index_fecha_adicional++;

      }

      /*Si el siguiente punto en simulacion a añadir coincide con la siguiente fecha adicional a incluir en
        simulacion*/
      else if (comparar_valores_tm(*fecha_actual, ((*fechas_adicionales)[index_fecha_adicional])) == 0) {
        
        informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].fecha_punto = *fecha_actual;
        informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].punto_simulacion = punto_actual;
        informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].delta = delta_simulacion;

        informacion_procesada->informacion_puntos_adicionales.puntos[index_fecha_adicional].numero_punto = punto_actual;
        informacion_procesada->informacion_puntos_adicionales.puntos[index_fecha_adicional].fecha_adicional = ((*fechas_adicionales)[index_fecha_adicional]);
        tiempo_actual = mktime(&((*fechas_adicionales)[index_fecha_adicional]));
       
        punto_actual++;
        index_fecha_adicional++;
      }
      else {
        
        informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].fecha_punto = *fecha_actual;
        informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].punto_simulacion = punto_actual;
        informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].delta = delta_simulacion;
        punto_actual++;
        
      }

    }

    /*Si el siguiente punto a añadir en simulacion no es un punto adicional simplemente un punto que se obtiene
      al sumarle la resolucion de tiempo al punto anterior*/
    else {
      informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].fecha_punto = *fecha_actual;
      informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].punto_simulacion = punto_actual;
      informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].delta = delta_simulacion;

      punto_actual++;
    }

    /*Se calcula cual es el siguiente tiempo a añadir en simulacion el modo de proceder es diferente si el siguiente
     punto es una hora en punto o no*/

    tiempo_actual += delta_simulacion_segundos;
    fecha_actual  = localtime(&tiempo_actual);

    if (comparar_valores_tm(*fecha_actual, (*fechas_adicionales)[index_fecha_adicional])!=-1) {
      tiempo_actual = mktime(&((*fechas_adicionales)[index_fecha_adicional]));
      tiempo_actual += delta_simulacion_segundos;
    }
    
   // tiempo_actual = mktime(&((*fechas_adicionales)[index_fecha_adicional]));
    //tiempo_actual += delta_simulacion_segundos;

  }

  //Se procede a añadir las fechas adicionales restantes que hay entre el punto actual hasta el final de
  //la simulacion.
  for (int i = index_fecha_adicional; i < *numero_fechas_adicionales; i++) {
    informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].fecha_punto = ((*fechas_adicionales)[index_fecha_adicional]);
    informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].punto_simulacion = punto_actual;
    informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].delta = obtener_diferencia_minutos(&(informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual - 1].fecha_punto), &((*fechas_adicionales)[index_fecha_adicional]));
    
    informacion_procesada->informacion_puntos_adicionales.puntos[index_fecha_adicional].numero_punto = punto_actual;
    informacion_procesada->informacion_puntos_adicionales.puntos[index_fecha_adicional].fecha_adicional = ((*fechas_adicionales)[index_fecha_adicional]);
    punto_actual++;
    index_fecha_adicional++;
  }

  //Se procede a añadir la fecha final del algoritmo en los puntos de la simulacion
  if (comparar_valores_tm((informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual-1].fecha_punto), *fecha_final_algoritmo) !=0){

  informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].fecha_punto = *fecha_final_algoritmo;
  informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].punto_simulacion = punto_actual;
  informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].delta = obtener_diferencia_minutos(&(informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual - 1].fecha_punto), &((*fechas_adicionales)[index_fecha_adicional]));

  }
  /*Se escribe en la variable correspondiente el número de puntos que se tienen en la simulacion*/
  informacion_procesada->informacion_puntos_simulacion.numero_puntos_simulacion = punto_actual;

  //Caso especial en el que la fecha inicial es igual que la fecha final nunca va a suceder.
  if (fecha_inicial_algoritmo == fecha_final_algoritmo) {
    informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].delta = 0;
  }

  
  return EXITO;
}

/*Este subprograma se utiliza para obtener a que punto de simulacion corresponde la llegada o partida de
  un vehiculo, bateria o fecha de carga de una bateria.*/

void obtener_punto_simulacion(OSQPInt* punto_simulacion, struct tm* fecha, puntos_adicionales_t* puntos_adicionales) {
  bool fecha_obtenida = false;
  int punto_adicional_actual = 0;
  
  while (fecha_obtenida == false) {

   
 
    if (comparar_valores_tm((puntos_adicionales[punto_adicional_actual].fecha_adicional), *fecha) == 0) {
     
      *punto_simulacion = (OSQPInt)puntos_adicionales[punto_adicional_actual].numero_punto;
      fecha_obtenida = true;
    }
    punto_adicional_actual++;
    
  }
  
}








