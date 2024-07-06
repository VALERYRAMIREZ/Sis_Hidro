#include <Arduino.h>
#include <ArduinoJson.h>
#include <varSistema.h>
#include <cmath>
#include <almacen.h>
#include "elexternos.h"

//extern DynamicJsonDocument nivel;

float Calcular_Volumen(uint8_t tanque, float profundidad, float altura)
{
    float volumen = 0.00;
    if(((tanque >= 0) && (tanque <= 2)) && (altura >= profundidad) && (profundidad >= 0.00) && (altura >= 0.00))
    {
        switch(tanque)
        {
            case 0:             //Tanque cilíndrico.
            {
                volumen = profundidad*radioSup*radioSup;
            }
            break;
            case 1:             //Tanque semi cónico.
            {
                volumen = profundidad*(radioSup*radioSup + radioInf*radioInf + radioInf*radioSup)/3;
            }
            break;
            case 2:             //Tanque esférico.
            {
                volumen = profundidad*profundidad*(3*radioSup - profundidad)/3;
            }
            break;
        }
    };
    Serial.print("Volumen: ");
    Serial.println(volumen);
    volumen *= M_PI;
    Serial.print("Volumen: ");
    Serial.println(volumen);
    return volumen;
};

StreamString Json_Sensor_Nivel(bool J)
{
    digitalWrite(pinGatillo, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinGatillo, LOW);
    nivel["colAgua"] = (pulseIn(pinInt, HIGH)*0.00034/2);
    if(J)
    {
        StreamString nivTanque;
        serializeJsonPretty(nivel, nivTanque);
        Serial.print("Medición realizada al tanque: ");
        Serial.println(nivTanque);
        return nivTanque;
    }
    else
    {
        Serial.print("Medición realizada al tanque: ");
        Serial.println((StreamString) nivel["colAgua"]);
        return nivel["colAgua"];
    }
};

StreamString Json_Sensor_Volumen(float profundidad, float altura, bool J)
{
    Serial.print("Profundidad: ");
    Serial.println(profundidad);
    Serial.println(altura);
    nivel["colAgua"] = Calcular_Volumen(tipoTanque, profundidad, altura)*1000;
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

void Actual_Estado()
{
if(!digitalRead(ledPinSistemaApagado) && digitalRead(ledPinSistemaEncendido)){
      ledEstado = "ENCENDIDO";
    }
    else if(digitalRead(ledPinSistemaApagado) && !digitalRead(ledPinSistemaEncendido)){
      ledEstado = "APAGADO";
    }
    if(ledEstado == "ENCENDIDO" && digitalRead(ledModoSistema)){
      modoEstado = "AUTO";
    }
    else if(ledEstado == "ENCENDIDO" && !digitalRead(ledModoSistema)){
      modoEstado = "MANUAL";
    }
};

uint8_t Activa_Bomba(uint8_t cantBombas)
{
    eeprom.get(sizeof(struct wifiConfig) + 1, sistema);
    Serial.print("nBomba al entrar a Activa_Bomba(): ");
    Serial.println(sistema.nBomba);
    Serial.print("Cantidad de bombas configuradas al entrar a Activa_Bomba(): ");
    Serial.println(cantBombas);
    if((sistema.nBomba << 0) || (sistema.nBomba >> cantBombas))
    {
        Serial.print("nBomba al incumplir límite: ");
        Serial.println(sistema.nBomba);
        Serial.println("Se estableció a cero nBomba por incumplir límite.");
        sistema.nBomba = 0;
    }
    sistema.nBomba++;
    Serial.print("Se encendió la bomba: ");
    Serial.println(sistema.nBomba);
    eeprom.put(sizeof(struct wifiConfig) + 1, sistema);
    eeprom.commit();
    return numTerm[sistema.nBomba - 1];
};

// Función para controlar la bomba a encender