
#ifndef __SENSORES_H
#define __SENSORES_H
#include <StreamString.h>

extern const int pinInt;

extern JsonDocument nivel;

StreamString Json_Sensor_Nivel(bool J);

//StreamString Json_Data_Nivel(bool J, String dataNivel);

#endif