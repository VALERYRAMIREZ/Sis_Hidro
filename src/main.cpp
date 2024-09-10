#include <Arduino.h>
#include <AsyncTCP.h>
//#include <AsyncElegantOTA.h>
//#include <WiFi.h>
//#include <WiFiMulti.h>
#include <string.h>
#include <iostream>
#include <Stream.h>
#include <EEPROM.h>
#include "reloj.h"
#include "backend.h"
#include "almacen.h"
#include "mWifi.h"
#include <actualOTA.h>
#include "elexternos.h"
//#include "varSistema.h"

extern AsyncElegantOtaClass OTA;
//using namespace std;
extern wifiConfig wifi_usuario;

extern EEPROMClass eeprom;

extern estadoSistema sistema;

//Instanciación del RTC

ESP32Time rtc(0);            //GMT-0.

//Instanciación del servidor web.
AsyncWebServer server(80);

//Instanciación del documento JSON para recibir la configurcación
//del sistema.

DynamicJsonDocument docJson(512);

//Variable para controlar el temporizador.
extern hw_timer_t *tempo;
extern volatile bool haTemporizado;
extern uint32_t cuentaTempo;

//Función para manejar el temporizador.
void IRAM_ATTR miTemporizador()
{
  cuentaTempo++;
  if((sistema.interMed > 0) && (cuentaTempo >= sistema.interMed))
  {
    cuentaTempo = 0;
    haTemporizado = true;
  }
}

//Variable para controlar el estado de la interrupción.
extern volatile bool haInterrumpido;

//Función para controlar la interrupción.
void IRAM_ATTR miInterrupcion() 
{
  haInterrumpido = true;
}

void setup() {

  StreamString valorTanque;
  //Configuración del temporizador.
  tempo = timerBegin(0, 80, true);
  timerAttachInterrupt(tempo, &miTemporizador, true);
  timerAlarmWrite(tempo, 10000000, true);
  timerAlarmEnable(tempo);

  //Configuración de la interrupción.
  pinMode(pinGatillo, OUTPUT);
  pinMode(pinInt, INPUT);

  //Inicia_EEPROM();
  /*eeprom.begin(sizeof(struct wifiConfig) + sizeof(struct estadoSistema));
  eeprom.get(0, wifi_usuario);*/
  Inicia_EEPROM();
  //eeprom.get(sizeof(struct wifiConfig) + 1, sistema);
  sistema.encBomba = true;
  eeprom.put(sizeof(wifi_usuario) + 1, sistema);
  eeprom.commit();

  //Adición de redes a las que se puede conectar el dispositivo.
  wifiMulti.addAP("ABACANTVWIFI8440","85047373038805");
  wifiMulti.addAP("TP-LINK_D6BF4E","480Secur325");
  wifiMulti.addAP("Delfos", "Joseph#29");

  //Configuración de velocidad del puerto serial.
  Serial.begin(115200);
  Serial.println("Entrando a configuración de aplicación");

  //Inicialización del sistema de preferencias NVS.
  
  Inicia_Almacen();

  //Lectura de los estados iniciales del sistema.
  memoriaEstado.getBool("SisEncender",ledEstado);
  //memoriaEstado.getBool("bombaActiva",bombaActiva);


  //Configuración de las salidas del sistema.
  pinMode(ledPinSistemaApagado,OUTPUT);
  pinMode(ledPinSistemaEncendido,OUTPUT);
  pinMode(ledModoSistema,OUTPUT);

  pinMode(bombaUno,OUTPUT);
  pinMode(bombaDos,OUTPUT);
  pinMode(bombaTres,OUTPUT);

  //Inicialización SPIFFS.
  Inicia_SPIFFS();

  //Configuración e inicialización del wifi.
  if(initWiFi())
  {
    Define_Backend(true);
  }
  else
  {
    Define_Backend(false);
  }
  /*WiFi.mode(WIFI_STA);
  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando al WiFi");
  }
  Serial.print("Conectado a la red " + WiFi.SSID() + "\n");
  Serial.print("Dirección IP asignada: ");
  Serial.println(WiFi.localIP());*/

  //  Define_Backend();

  //Inicio del servidor web.
  OTA.begin(&server);    // Start AsyncElegantOTA
  server.begin();


  //Varaibles relacionadas con el tanque.

  tipoTanque = 2;           //Tanque cilíndrico.
  alturaTanque = 3.50;
  radioSup = 1.00;
  radioInf = 0.50;

  Serial.println("Saliendo de configuración de aplicación");
}
 
void loop()
{
  //Procesamiento del temporizador. Aquí se procesa lo
  //que hará el programa cuando se active el temporizador.
  if(haTemporizado)
  {
    //sistema.nTanque = Json_Sensor_Nivel_AJ(false);
    sistema.nTanque = Json_Sensor_Nivel_AJ(false);
    Serial.print("El nivel del líquido se encuentra a ");
    Serial.print(sistema.nTanque);
    Serial.println(" m de profundidad.");
    Serial.println(sistema.nTanque.toFloat());
    sistema.vTanque = Json_Sensor_Volumen(sistema.nTanque.toFloat(), alturaTanque, false);
    Serial.print("El volumen del tanque es: ");
    Serial.print(sistema.vTanque);
    Serial.println(" litros.");
    eeprom.put(sizeof(wifi_usuario) + 1, sistema);
    eeprom.commit();
    haTemporizado = false;
  }
  
  // Procesamiento de activación de bombas en secuencia si
  // el sistema está trabajando en automático.
  if(sistema.reloj && modoSistema)
  {
    if(!sistema.bombaActiva && Compara_RTC(rtc.getTimeDate(true).c_str(), semana))
    {
      bombaPin = Activa_Bomba(3);
      digitalWrite(Activa_Bomba(3), true);
      sistema.bombaActiva = true;
      eeprom.put(sizeof(struct wifiConfig) + 1, sistema);
      eeprom.commit();
      Actual_Bomba_Auto();
    }
    else
    {
      digitalWrite(bombaPin, false);
      sistema.bombaActiva = false;
      eeprom.put(sizeof(struct wifiConfig) + 1, sistema);
      eeprom.commit();
    }
  }
  else if((ledEstado == "ENCENDIDO") && !modoSistema)
  {
    /*  Debe tomar en cuenta solo si se presiona el tanque para que se encienda
        el sistema. */
        switch(sistema.bombaActiva && sistema.encBomba)
        {
          case false:
          {
            digitalWrite(bombaPin, false);
            eeprom.put(sizeof(struct wifiConfig) + 1, sistema);
            eeprom.commit();   
          }
          break;
          case true:
          {
            Serial.println("*****Entrando al caso en el que la bomba se enciende en modo manual.*****");
            //sistema.bombaActiva = false;
            sistema.encBomba = false;
            Serial.println("Llamando a Activa_Bomba().");
            //bombaPin = Activa_Bomba(3);
            Serial.println("Activando pin de bomba.");
            digitalWrite(Activa_Bomba(3), true);
            eeprom.put(sizeof(struct wifiConfig) + 1, sistema);
            eeprom.commit();
            Serial.println("*****Saliendo de cas en el que la bomca se enciende en modo manual.******");
          }
          break;
        };
  }
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
  Serial.println(rtc.getDateTime(true));
  delay(1000);*/
}