/*En este archivo se definen las funciones que se encargan de almacenar las
  posiciones de la informacion en los diferentes csvs.*/

#include "tipos_optimizacion.h"
#define FILA_ENCABEZADOS                      0
  //Filas y columnas del csv de la informacion del algoritmo
#define FILA_INFORMACION                      1
#define COLUMNA_ANYO_INICIAL_ALGORITMO        0
#define COLUMNA_MES_INICIAL_ALGORITMO         1
#define COLUMNA_DIA_INICIAL_ALGORITMO         2
#define COLUMNA_HORA_INICIAL_ALGORITMO        3
#define COLUMNA_MINUTO_INICIAL_ALGORITMO      4
#define COLUMNA_ANYO_FINAL_ALGORITMO          5
#define COLUMNA_MES_FINAL_ALGORITMO           6
#define COLUMNA_DIA_FINAL_ALGORITMO           7
#define COLUMNA_HORA_FINAL_ALGORITMO          8
#define COLUMNA_MINUTO_FINAL_ALGORITMO        9
#define COLUMNA_RESOLUCION_TIEMPO_SIMULACION  10 

//Filas y columnas del csv de la informacion del algoritmo
#define COLUMNA_ANYO_PRECIO   0
#define COLUMNA_MES_PRECIO    1
#define COLUMNA_DIA_PRECIO    2
#define COLUMNA_HORA_PRECIO   3

//Columnas del Csv que contiene la informacion de los terminales
#define COLUMNA_CSV_NUMERO_TERMINAL 0
#define COLUMNA_CSV_FASE            1

//Columnas del Csv que contiene la informacion de los vehiculos
#define COLUMNA_CSV_VEHICULOS_NUM_TERMINAL      0
#define COLUMNA_CSV_VEHICULOS_MODO_CARGA        1
#define COLUMNA_CSV_VEHICULOS_CAPACIDAD_BATERIA 2
#define COLUMNA_CSV_VEHICULOS_BATERIA_INICIAL   3
#define COLUMNA_CSV_VEHICULOS_BATERIA_DESEADA   4
#define COLUMNA_CSV_VEHICULOS_MAXIMA_POTENCIA   5
#define COLUMNA_CSV_VEHICULOS_ANYO_INICIAL      6
#define COLUMNA_CSV_VEHICULOS_MES_INICIAL       7
#define COLUMNA_CSV_VEHICULOS_DIA_INICIAL       8
#define COLUMNA_CSV_VEHICULOS_HORA_INICIAL      9
#define COLUMNA_CSV_VEHICULOS_MINUTO_INICIAL    10
#define COLUMNA_CSV_VEHICULOS_ANYO_FINAL        11
#define COLUMNA_CSV_VEHICULOS_MES_FINAL         12
#define COLUMNA_CSV_VEHICULOS_DIA_FINAL         13 
#define COLUMNA_CSV_VEHICULOS_HORA_FINAL        14
#define COLUMNA_CSV_VEHICULOS_MINUTO_FINAL      15


/*Columnas del CSV que contiene la informacion de las baterias*/
#define FILA_ENCABEZADOS_CSV_BATERIAS            0
#define COLUMNA_CSV_BATERIAS_NUMERO_TERMINAL     0
#define COLUMNA_CSV_BATERIAS_CAPACIDAD_BATERIAS  1
#define COLUMNA_CSV_BATERIAS_BATERIA_INICIAL     2
#define COLUMNA_CSV_BATERIAS_BATERIA_DESEADA     3
#define COLUMNA_CSV_BATERIAS_MAXIMA_POTENCIA     4
#define COLUMNA_CSV_BATERIAS_ANYO_INICIAL        5
#define COLUMNA_CSV_BATERIAS_MES_INICIAL         6
#define COLUMNA_CSV_BATERIAS_DIA_INICIAL         7
#define COLUMNA_CSV_BATERIAS_HORA_INICIAL        8
#define COLUMNA_CSV_BATERIAS_MINUTO_INICIAL      9
#define COLUMNA_CSV_BATERIAS_ANYO_FINAL          10
#define COLUMNA_CSV_BATERIAS_MES_FINAL           11
#define COLUMNA_CSV_BATERIAS_DIA_FINAL           12 
#define COLUMNA_CSV_BATERIAS_HORA_FINAL          13
#define COLUMNA_CSV_BATERIAS_MINUTO_FINAL        14
#define COLUMNA_CSV_BATERIAS_CONSIDERAR_OBJETIVO 15
#define COLUMNA_CSV_BATERIAS_ANYO_OBJETIVO       16
#define COLUMNA_CSV_BATERIAS_MES_OBJETIVO        17
#define COLUMNA_CSV_BATERIAS_DIA_OBJETIVO        18
#define COLUMNA_CSV_BATERIAS_HORA_OBJETIVO       19 
#define COLUMNA_CSV_BATERIAS_MINUTO_OBJETIVO     20

/*Filas y columnas donde se encuentra la informacion csv de las restricciones*/
#define FILA_CSV_RESTRICCIONES_ENCABEZADOS                 0
#define FILA_CSV_RESTRICCIONES_VALORES                     1
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_SISTEMA     1
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_1  2
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_2  3
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_3  4
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_4  5
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_5  6
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_6  7
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_7  8
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_8  9
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_9  10
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_10 11
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_11 12
#define COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_12 13


static void inicializar_ubicacion_informacion_algoritmo(ubicacion_csv_algoritmo_t* posicion_info_algoritmo) {
    //Filas del csv
    posicion_info_algoritmo->fila_encabezados = FILA_ENCABEZADOS;
    posicion_info_algoritmo->fila_informacion = FILA_INFORMACION;
    //Columna de resolucion del algoritmo
    posicion_info_algoritmo->resolucion_minutos = COLUMNA_RESOLUCION_TIEMPO_SIMULACION;
    //Ubicacion de la Fecha de inicio del algoritmo
    posicion_info_algoritmo->ubicacion_fecha_inicial_algoritmo.columna_anyo = COLUMNA_ANYO_INICIAL_ALGORITMO;
    posicion_info_algoritmo->ubicacion_fecha_inicial_algoritmo.columna_mes = COLUMNA_MES_INICIAL_ALGORITMO;
    posicion_info_algoritmo->ubicacion_fecha_inicial_algoritmo.columna_dia = COLUMNA_DIA_INICIAL_ALGORITMO;
    posicion_info_algoritmo->ubicacion_fecha_inicial_algoritmo.columna_hora = COLUMNA_HORA_INICIAL_ALGORITMO;
    posicion_info_algoritmo->ubicacion_fecha_inicial_algoritmo.columna_minuto = COLUMNA_MINUTO_INICIAL_ALGORITMO;
    //Ubicacion de la Fecha de finalizacion del algoritmo
    posicion_info_algoritmo->ubicacion_fecha_final_algoritmo.columna_anyo = COLUMNA_ANYO_FINAL_ALGORITMO;
    posicion_info_algoritmo->ubicacion_fecha_final_algoritmo.columna_mes = COLUMNA_MES_FINAL_ALGORITMO;
    posicion_info_algoritmo->ubicacion_fecha_final_algoritmo.columna_dia = COLUMNA_DIA_FINAL_ALGORITMO;
    posicion_info_algoritmo->ubicacion_fecha_final_algoritmo.columna_hora = COLUMNA_HORA_FINAL_ALGORITMO;
    posicion_info_algoritmo->ubicacion_fecha_final_algoritmo.columna_minuto = COLUMNA_MINUTO_FINAL_ALGORITMO;

}
/*Se utiliza este subprograma para guardar las filas y columnas donde se encuentra la informacion en el
  csv de las restricciones*/
static void inicializar_ubicacion_informacion_restricciones(ubicacion_csv_restricciones_t* posicion_info_restricciones) {
    posicion_info_restricciones->fila_encabezados = FILA_CSV_RESTRICCIONES_ENCABEZADOS;
    posicion_info_restricciones->fila_valores = FILA_CSV_RESTRICCIONES_VALORES;
    posicion_info_restricciones->columna_max_potencia_sistema = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_SISTEMA;
    posicion_info_restricciones->columna_max_potencia_terminal_1 = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_1;
    posicion_info_restricciones->columna_max_potencia_terminal_2 = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_2;
    posicion_info_restricciones->columna_max_potencia_terminal_3 = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_3;
    posicion_info_restricciones->columna_max_potencia_terminal_4 = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_4;
    posicion_info_restricciones->columna_max_potencia_terminal_5 = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_5;
    posicion_info_restricciones->columna_max_potencia_terminal_6 = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_6;
    posicion_info_restricciones->columna_max_potencia_terminal_7 = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_7;
    posicion_info_restricciones->columna_max_potencia_terminal_8 = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_8;
    posicion_info_restricciones->columna_max_potencia_terminal_9 = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_9;
    posicion_info_restricciones->columna_max_potencia_terminal_10 = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_10;
    posicion_info_restricciones->columna_max_potencia_terminal_11 = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_11;
    posicion_info_restricciones->columna_max_potencia_terminal_12 = COLUMNA_CSV_RESTRICCIONES_MAX_POTENCIA_TERMINAL_12;

}
/*Este subprograma se utiliza para indicar en la variable correspondiente
  las filas y columnas donde se encuentra la informacion relevante del precio*/
static void inicializar_ubicacion_informacion_precios(ubicacion_csv_precios_t* posicion_info_precio) {


    posicion_info_precio->fila_encabezados = FILA_ENCABEZADOS;
    posicion_info_precio->ubicacion_fecha_precios.columna_anyo = COLUMNA_ANYO_PRECIO;
    posicion_info_precio->ubicacion_fecha_precios.columna_mes = COLUMNA_MES_PRECIO;
    posicion_info_precio->ubicacion_fecha_precios.columna_dia = COLUMNA_DIA_PRECIO;
    posicion_info_precio->ubicacion_fecha_precios.columna_hora = COLUMNA_HORA_PRECIO;

}

/*Este subprograma se utiliza para indicar en la variable correspondiente
  las filas y columnas donde se encuentra la informacion relevante del csv
  de los terminales */

static void inicializar_ubicacion_informacion_terminales(ubicacion_csv_terminales_t* ubicacion_info_terminales) {
    ubicacion_info_terminales->fila_encabezados = FILA_ENCABEZADOS;
    ubicacion_info_terminales->columna_numero_terminal = COLUMNA_CSV_NUMERO_TERMINAL;
    ubicacion_info_terminales->columna_fase = COLUMNA_CSV_FASE;

}
/*Este subprograma se utiliza para indicar en la variable correspondiente
  en que filas y columnas esta la informacion relacionada con los vehiculos*/

static void inicializar_ubicacion_informacion_vehiculos(ubicacion_csv_vehiculos_t* ubicacion_info_vehiculos) {

    //Se carga la posicion de la informacion que no son fechas
    ubicacion_info_vehiculos->columna_terminales = COLUMNA_CSV_VEHICULOS_NUM_TERMINAL;
    ubicacion_info_vehiculos->columna_modo_carga = COLUMNA_CSV_VEHICULOS_MODO_CARGA;
    ubicacion_info_vehiculos->columna_capacidad_bateria = COLUMNA_CSV_VEHICULOS_CAPACIDAD_BATERIA;
    ubicacion_info_vehiculos->columna_porcentaje_bateria_deseada = COLUMNA_CSV_VEHICULOS_BATERIA_INICIAL;
    ubicacion_info_vehiculos->columna_porcentaje_bateria_inicial = COLUMNA_CSV_VEHICULOS_BATERIA_DESEADA;
    ubicacion_info_vehiculos->columna_maxima_potencia = COLUMNA_CSV_VEHICULOS_MAXIMA_POTENCIA;

    //Se carga la informacion de en que columnas se encuentra
    //la fecha 
    ubicacion_fecha_t* ubicacion_fecha_inicial = &(ubicacion_info_vehiculos->ubicacion_fecha_inicial_vehiculo);
    ubicacion_fecha_t* ubicacion_fecha_final = &(ubicacion_info_vehiculos->ubicacion_fecha_final_vehiculo);
    ubicacion_fecha_inicial->columna_anyo = COLUMNA_CSV_VEHICULOS_ANYO_INICIAL;
    ubicacion_fecha_inicial->columna_mes = COLUMNA_CSV_VEHICULOS_MES_INICIAL;
    ubicacion_fecha_inicial->columna_dia = COLUMNA_CSV_VEHICULOS_DIA_INICIAL;
    ubicacion_fecha_inicial->columna_hora = COLUMNA_CSV_VEHICULOS_HORA_INICIAL;
    ubicacion_fecha_inicial->columna_minuto = COLUMNA_CSV_VEHICULOS_MINUTO_INICIAL;


    ubicacion_fecha_final->columna_anyo = COLUMNA_CSV_VEHICULOS_ANYO_FINAL;
    ubicacion_fecha_final->columna_mes = COLUMNA_CSV_VEHICULOS_MES_FINAL;
    ubicacion_fecha_final->columna_dia = COLUMNA_CSV_VEHICULOS_DIA_FINAL;
    ubicacion_fecha_final->columna_hora = COLUMNA_CSV_VEHICULOS_HORA_FINAL;
    ubicacion_fecha_final->columna_minuto = COLUMNA_CSV_VEHICULOS_MINUTO_FINAL;
}

/*Este subprograma se utiliza para indicar la localizacion de las columnas en el
  CSV */
static void inicializar_ubicacion_informacion_baterias(ubicacion_csv_baterias_t* ubicacion_baterias) {
    ubicacion_baterias->columna_terminal = COLUMNA_CSV_BATERIAS_NUMERO_TERMINAL;
    ubicacion_baterias->columna_capacidad_bateria = COLUMNA_CSV_BATERIAS_CAPACIDAD_BATERIAS;
    ubicacion_baterias->columna_porcentaje_bateria_inicial = COLUMNA_CSV_BATERIAS_BATERIA_INICIAL;
    ubicacion_baterias->columna_porcentaje_bateria_deseada = COLUMNA_CSV_BATERIAS_BATERIA_DESEADA;
    ubicacion_baterias->columna_potencia_maxima_bateria = COLUMNA_CSV_BATERIAS_MAXIMA_POTENCIA;

    //Se registran las columnas relacionadas con la fecha inicial de las  baterias
    ubicacion_baterias->ubicacion_fecha_inicial_baterias.columna_anyo = COLUMNA_CSV_BATERIAS_ANYO_INICIAL;
    ubicacion_baterias->ubicacion_fecha_inicial_baterias.columna_mes = COLUMNA_CSV_BATERIAS_MES_INICIAL;
    ubicacion_baterias->ubicacion_fecha_inicial_baterias.columna_dia = COLUMNA_CSV_BATERIAS_DIA_INICIAL;
    ubicacion_baterias->ubicacion_fecha_inicial_baterias.columna_hora = COLUMNA_CSV_BATERIAS_HORA_INICIAL;
    ubicacion_baterias->ubicacion_fecha_inicial_baterias.columna_minuto = COLUMNA_CSV_BATERIAS_MINUTO_INICIAL;

    //Se registran las columnas relacionadas con la fecha final de las baterias
    ubicacion_baterias->ubicacion_fecha_final_baterias.columna_anyo = COLUMNA_CSV_BATERIAS_ANYO_FINAL;
    ubicacion_baterias->ubicacion_fecha_final_baterias.columna_mes = COLUMNA_CSV_BATERIAS_MES_FINAL;
    ubicacion_baterias->ubicacion_fecha_final_baterias.columna_dia = COLUMNA_CSV_BATERIAS_DIA_FINAL;
    ubicacion_baterias->ubicacion_fecha_final_baterias.columna_hora = COLUMNA_CSV_BATERIAS_HORA_FINAL;
    ubicacion_baterias->ubicacion_fecha_final_baterias.columna_minuto = COLUMNA_CSV_BATERIAS_MINUTO_FINAL;

    //Se registran las columnas relacionadas con la fecha objetivo de carga de la bateria
    ubicacion_baterias->columna_consideracion_objetivo = COLUMNA_CSV_BATERIAS_CONSIDERAR_OBJETIVO;
    ubicacion_baterias->ubicacion_fecha_objetivo_baterias.columna_anyo = COLUMNA_CSV_BATERIAS_ANYO_OBJETIVO;
    ubicacion_baterias->ubicacion_fecha_objetivo_baterias.columna_mes = COLUMNA_CSV_BATERIAS_MES_OBJETIVO;
    ubicacion_baterias->ubicacion_fecha_objetivo_baterias.columna_dia = COLUMNA_CSV_BATERIAS_DIA_OBJETIVO;
    ubicacion_baterias->ubicacion_fecha_objetivo_baterias.columna_hora = COLUMNA_CSV_BATERIAS_HORA_OBJETIVO;
    ubicacion_baterias->ubicacion_fecha_objetivo_baterias.columna_minuto = COLUMNA_CSV_BATERIAS_MINUTO_OBJETIVO;
}


/*Se llama a este subprograma para guardar las columnas y filas
  en donde se encuentra la informacion en los diferentes csvs
  de entradas.*/
void indicar_posiciones_informacion(informacion_entrada_t* informacion_sistema) {
    ubicacion_csv_algoritmo_t* puntero_info_ubicacion_algoritmo;
    ubicacion_csv_precios_t* puntero_info_ubicacion_precio_compra;
    ubicacion_csv_precios_t* puntero_info_ubicacion_precio_venta;
    ubicacion_csv_terminales_t* puntero_info_ubicacion_terminales;
    ubicacion_csv_restricciones_t* puntero_info_ubicacion_restricciones;
    ubicacion_csv_baterias_t* puntero_info_ubicacion_baterias;
    ubicacion_csv_vehiculos_t* puntero_info_ubicacion_vehiculos;

    puntero_info_ubicacion_algoritmo = &(informacion_sistema->datos_algoritmo.posiciones_informacion_algoritmo);
    puntero_info_ubicacion_precio_compra = &(informacion_sistema->datos_precio_compra.posiciones_informacion_precio);
    puntero_info_ubicacion_precio_venta = &(informacion_sistema->datos_precio_venta.posiciones_informacion_precio);
    puntero_info_ubicacion_terminales = &(informacion_sistema->datos_terminales.posiciones_informacion_terminales);
    puntero_info_ubicacion_restricciones = &(informacion_sistema->datos_restricciones.posiciones_informacion_restricciones);
    puntero_info_ubicacion_baterias = &(informacion_sistema->datos_baterias.posiciones_informacion_baterias);
    puntero_info_ubicacion_vehiculos = &(informacion_sistema->datos_vehiculos.posiciones_informacion_vehiculos);

    /*Se llaman a los subprogramas que se encargan de guardar las columnas en las que se
      encuentran la informacion de los csvs de entrada*/
    inicializar_ubicacion_informacion_algoritmo(puntero_info_ubicacion_algoritmo);
    inicializar_ubicacion_informacion_precios(puntero_info_ubicacion_precio_compra);
    inicializar_ubicacion_informacion_precios(puntero_info_ubicacion_precio_venta);
    inicializar_ubicacion_informacion_terminales(puntero_info_ubicacion_terminales);
    inicializar_ubicacion_informacion_restricciones(puntero_info_ubicacion_restricciones);
    inicializar_ubicacion_informacion_baterias(puntero_info_ubicacion_baterias);
    inicializar_ubicacion_informacion_vehiculos(puntero_info_ubicacion_vehiculos);

}