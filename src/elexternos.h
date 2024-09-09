
#ifndef __ELEXTERNOS_H
#define __ELEXTERNOS_H
#include <StreamString.h>

extern const int pinInt;

extern JsonDocument nivel;

float Calcular_Volumen(uint8_t tanque, float profundidad, float altura);

StreamString Json_Sensor_Nivel_AJ(bool J);

StreamString Json_Sensor_Nivel_HC_3(bool J);

StreamString Json_Sensor_Volumen(float profundidad, float altura, bool J);

void Actual_Estado();

uint8_t Activa_Bomba(uint8_t cantBombas);

#endif