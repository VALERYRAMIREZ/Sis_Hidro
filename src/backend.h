#ifndef BACKEND_H
#define BACKEND_H
#include <Arduino.h>
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include "varSistema.h"

String Procesador(const String& var);

bool Inicia_SPIFFS(void);

void manejaJson(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

uint8_t Extrae_Data(std::string trama, FechaProg* destino,char sep);

void notFound(AsyncWebServerRequest *request);

void Define_Backend(bool tipoWeb);

#endif