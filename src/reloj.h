#ifndef RELOJ_H
#define RELOJ_H

#include <sstream>
#include <ESP32Time.h>
#include "varSistema.h"

bool Compara_RTC(const char * data, Semana tPrograma);

void Enciende_Temporizador(uint8_t tiempo);

void Apaga_Temporizador(void);

#endif