#include <Arduino.h>
#include <ArduinoJson.h>
#include <varSistema.h>
#include "sensores.h"

//extern DynamicJsonDocument nivel;

StreamString Json_Sensor_Nivel(bool J)
{
    digitalWrite(pinGatillo, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinGatillo, LOW);
    nivel["colAgua"] = (pulseIn(pinInt, HIGH)*0.00034/2);
    if(J)
    {
        StreamString volTanque;
        serializeJsonPretty(nivel,volTanque);
        return volTanque;
    }
    else
    {
        return nivel["colAgua"];
    }
};

/*StreamString Json_Data_Nivel(bool J, JsonVariantConst dataNivel)
{
    StreamString volTanque;
    serializeJsonPretty(dataNivel, (void*) volTanque, sizeof(volTanque));
    return volTanque;    
};*/