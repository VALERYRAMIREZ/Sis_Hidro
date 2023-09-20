#pragma once
#include "ESP32Time.h"
#include "backend.h"

#ifndef RELOJ_H
#define RELOJ_H


extern ESP32Time rtc;
extern const char* dataM;

bool Compara_RTC(const char* data, Semana tPrograma);

#endif