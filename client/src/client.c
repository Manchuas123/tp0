#include "client.h"
#include <readline/readline.h>
 #include <string.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	log_info(logger,"soy un log");
	
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	if (config == NULL){
		config_destroy(config);
		abort();
	}else{
		valor = config_get_string_value(config,"CLAVE");
		ip = config_get_string_value(config,"IP");
		puerto = config_get_string_value(config,"PUERTO");

		log_info(logger,valor);
		log_info(logger,ip);
		log_info(logger,puerto);
	}

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	// Loggeamos el valor de config

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);
	
	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	
	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	return log_create("/home/utnso/tp0/client/tp0.log", "1",true, LOG_LEVEL_INFO);
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("/home/utnso/tp0/client/cliente.config");
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	int salida =1;
	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger,leido);
	free(leido);

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(salida != 0){
		leido = readline("> ");
		log_info(logger,leido);
		salida = strcmp(leido,"");
		free(leido);
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	/*
	agregar_a_paquete(3): Dado un stream y su tamaño, lo agrega al paquete.
	enviar_paquete(2): Dada una conexión y un paquete, lo envía a través de ella.
	eliminar_paquete(1): Elimina la memoria usada por el paquete.*/
	leido = readline("> ");
	int tamanio = strlen(leido);
	if (tamanio != 0){
		agregar_a_paquete(paquete,leido,tamanio+1);
		enviar_paquete(paquete,conexion);
	}
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(conexion);
	config_destroy(config);
	log_destroy(logger);
}
