#include <Arduino.h>
#include <ArduinoJson.h>
#include <varSistema.h>
#include "sensores.h"

//extern DynamicJsonDocument nivel;

StreamString Json_Sensor_Nivel()
{
    StreamString volTanque;    
    digitalWrite(pinGatillo, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinGatillo, LOW);
    nivel["colAgua"] = (pulseIn(pinInt, HIGH)*0.00034/2);
    serializeJsonPretty(nivel,volTanque);
    return volTanque;
};