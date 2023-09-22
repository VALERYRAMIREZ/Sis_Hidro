#ifndef BACKEND_H
#define BACKEND_H

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <Stream.h>
#include <AsyncJson.h>

//Estructura para almacenar la fecha a programar al sistema.

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

#include "estado.h"
#include "reloj.h"

extern const uint8_t ledPinSistemaEncendido;
extern const uint8_t ledPinSistemaApagado;
extern String ledEstado;
extern String modoEstado;
extern bool modoSistema;
extern bool ledModoSistema;
extern char* usuarioHTTP;
extern char* claveHTTP;
extern String ultimaPaginaCargada;
extern estadoSistema sistema;
extern AsyncWebServer server;

extern FechaProg tiempo;
extern FechaProg tiempoLeido;

extern Semana semana;

uint8_t Extrae_Data(String trama, FechaProg* destino,char sep);

void Func_Backend(void);

String Procesador(const String& var);

#endif