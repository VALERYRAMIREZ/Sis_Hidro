#ifndef RELOJ_H
#define RELOJ_H

#include "ESP32Time.h"
#include "backend.h"

extern ESP32Time rtc;
extern const char* dataM;
extern Semana semana;

bool Compara_RTC(const char* data, Semana tPrograma);

#endif