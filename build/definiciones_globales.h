

#ifndef DEFINICIONES_GLOBALES_H
#define DEFINICIONES_GLOBALES_H

//En este archivo se definen constantes que se van a usar 
// por todo el codigo->

//Constantes para los codigos de error del main

#define SALIDA_ERROR 1 
//Una constante para el subprograma que compara
//las fechas

#define NO_INCLUIR_FECHA_IGUAL 0 
#define INCLUIR_FECHA_IGUAL    1

//Constantes relacionadas con el subprograma que
//obtiene si los datos del CSV son numeros 

#define DECIMAL_INCLUIDO      1
#define DECIMAL_NO_INCLUIDO   0



//Constantes relacionadas con el uso de la libreria
//time .h

//Una constante para rellenar informacion
//en un campo que no proceda
//Defino ademas 
#define PRIMERA_FILA             1
#define NO_PROCEDE              -1
//Defino si se incluye el minuto en una fecha
// o no

#define NO_INCLUIR_MINUTO        0
#define SI_INCLUIR_MINUTO        1
//Numero de segundos que hay en una hora->

#define SEGUNDOS_HORA           3600
//En esta libreria los años van a partir de 1900
//y los meses a partir de 0. Luego hay que restar
//esas cantidades.

#define DESPLAZAMIENTO_ANYO 1900
#define DESPLAZAMIENTO_MES  1

//Constantes para los valores que retornan las funciones
//tipo int.

typedef enum {
    EXITO,
    ERROR
} Resultado;

//Defino las filas y columnas en donde se encuentran los 
//diferentes datos de entrada en los diferentes CSVS.
// 
//Datos_Algoritmo:
#define NUM_COLUMNAS_CSV_INFO_ALGORITMO 11
#define NUM_FILAS_CSV_INFO_ALGORITMO    2





//Defino donde se encuentra los distintos tipos
//de informacion en el CSV de los precios

#define COLUMNA_ANYO_PRECIO    0
#define COLUMNA_MES_PRECIO     1
#define COLUMNA_DIA_PRECIO     2
#define COLUMNA_HORA_PRECIO    3
#define COLUMNA_PRECIO         4

//Defino donde se encuentra la informacion en el CSV de las restricciones->
#define PRIMERA_COLUMNA_DATOS_RESTRICCIONES 1
#define FILA_INFORMACION_CSV_RESTRICCIONES  1

//Se define informacion relacionada con los
//terminales de la placa de redistribucion.
#define  NUMERO_TERMINALES 12
#define  COLUMNA_NUMERO_TERMINAL 0
#define  COLUMNA_FASE            1







//Se define el nombre de los archivos.csv que
//contienen la informacion de entrada

#define INFORMACION_TERMINALES            "Informacion_Terminales.csv"
#define INFORMACION_VEHICULOS             "Informacion_Vehiculos.csv"
#define INFORMACION_BATERIAS              "Informacion_Baterias.csv"
#define INFORMACION_ALGORITMO             "Informacion_Algoritmo.csv"
#define INFORMACION_PRECIO_COMPRA         "Precio_Compra_Kilovatio.csv"
#define INFORMACION_PRECIO_VENTA          "Precio_Venta_Kilovatio.csv"
#define INFORMACION_RESTRICCIONES_SISTEMA "Restricciones_Sistema.csv"
#define REGISTRO_ERRORES                  "registro_errores.log"




#endif
