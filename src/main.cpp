#include <Arduino.h>
#include <AsyncTCP.h>
#include <AsyncElegantOTA.h>
//#include <WiFi.h>
//#include <WiFiMulti.h>
#include <string.h>
#include <iostream>
#include <Stream.h>
//#include <EEPROM.h>
#include "reloj.h"
#include "backend.h"
#include "almacen.h"
//#include "varSistema.h"

//using namespace std;
extern wifiConfig wifi_usuario;

extern EEPROMClass eeprom;
//Instanciación del RTC

ESP32Time rtc(0);            //GMT-0.

//Instanciación del servidor web.
AsyncWebServer server(80);

//Instanciación del documento JSON para recibir la configurcación
//del sistema.

DynamicJsonDocument docJson(512);

// Función para controlar la bomba a encender
void Bomba(bool encendido) {

}

void handle_NoEncontrado() {

}

void setup() {

  EEPROM.begin(sizeof(struct wifiConfig));
  EEPROM.get(0, wifi_usuario);
  //Inicia_EEPROM();
  //Adición de redes a las que se puede conectar el dispositivo.
  //wifiMulti.addAP("ABACANTVWIFI8440","85047373038805");
  wifiMulti.addAP("TP-LINK_D6BF4E","480Secur325");
  wifiMulti.addAP("Delfos", "Joseph#29");

  //Configuración de velocidad del puerto serial.
  Serial.begin(115200);
  Serial.println("Entrando a configuración de aplicación");

  //Inicialización del sistema de preferencias NVS.
  
  Inicia_Almacen();

  //Lectura de los estados iniciales del sistema.
  memoriaEstado.getBool("SisEncender",ledEstado);
  memoriaEstado.getBool("bombaActiva",bombaActiva);


  //Configuración de las salidas del sistema.
  pinMode(ledPinSistemaApagado,OUTPUT);
  pinMode(ledPinSistemaEncendido,OUTPUT);
  pinMode(ledModoSistema,OUTPUT);
  pinMode(bombaUno,OUTPUT);
  pinMode(bombaDos,OUTPUT);

  //Inicialización SPIFFS.
  Inicia_SPIFFS();

  //Configuración e inicialización del wifi.
  WiFi.mode(WIFI_STA);
  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando al WiFi");
  }
  Serial.print("Conectado a la red " + WiFi.SSID() + "\n");
  Serial.print("Dirección IP asignada: ");
  Serial.println(WiFi.localIP());

  Define_Backend();

  //Inicio del servidor web.
  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  server.begin();

  Serial.println("Saliendo de configuración de aplicación");
}
 
void loop() {
  
  /*if(sistema.reloj && modoSistema)
  {
    //superT && (nivelA >= 255/(volMax/volMin)) &&
    if(!bombaActiva && Compara_RTC(rtc.getTimeDate(true).c_str(), semana))
    {
      Bomba(true);
      bombaActiva = true;
      digitalWrite(ledPinSistemaApagado, true);
    }
    else
    {
      Bomba(false);
      bombaActiva = false;
      digitalWrite(ledPinSistemaApagado, false);
    }
  }
  else if(!modoSistema)
  {
    /*switch(modoEstado)
    {
      case 0:
      {

      }
      break;
    }
  }
  delay(1000);*/
}