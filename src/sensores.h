
#ifndef __SENSORES_H
#define __SENSORES_H
#include <StreamString.h>

extern const int pinInt;

extern JsonDocument nivel;

float Calcular_Volumen(uint8_t tanque, float profundidad, float altura);

StreamString Json_Sensor_Nivel(bool J);

StreamString Json_Sensor_Volumen(float profundidad, float altura, bool J);

#endif