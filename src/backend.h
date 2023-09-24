#ifndef BACKEND_H
#define BACKEND_H

    #include <sstream>
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
    #include <SPIFFS.h>
    #include <AsyncJson.h>
    #include "variables_sistema.h"
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
    
    //Estructura para almacenar la fecha a programar al sistema.

extern FechaProg tiempo;
extern FechaProg tiempoLeido;

extern Semana semana;

    uint8_t Extrae_Data(String trama, FechaProg* destino,char sep);

    void Func_Backend(void);

    String Procesador(const String& var);

#endif