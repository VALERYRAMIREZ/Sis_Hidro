#ifndef VARIABLES_SISTEMA_H
#define VARIABLES_SISTEMA_H
    // Campo de bits para almacenar el estado del sistema

typedef struct {
  bool bateria  : 1;
  bool reloj    : 1;
} estadoSistema;

typedef struct {
  int dia;
  int mes;
  int anio;
  int hora;
  int minuto;
  int segundo;
  char diaSemana[20];
  char mesAnio[20];
} FechaProg;

// Estructura para almacenar el día y programa respectivo.
typedef struct {
  const char* inicioManana;
  const char* finManana;
  const char* inicioTarde;
  const char* finTarde;
} diaSemana;

// Estructura para almacenar los programas de todos los 
// dias de la semana.
typedef struct {
  diaSemana lunes;
  diaSemana martes;
  diaSemana miercoles;
  diaSemana jueves;
  diaSemana viernes;
  diaSemana sabado;
  diaSemana domingo;
}  Semana;

//Definición del usuario, clave y estado de acceso.

String usu = "admin", clav = "admin";
char* usuarioHTTP = &usu[0];
char* claveHTTP = &clav[0];
bool eHTTP = false;

#endif