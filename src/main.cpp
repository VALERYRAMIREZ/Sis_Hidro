#include <Arduino.h>
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
//#include <WebServer.h>
#include <WiFi.h>
#include <WiFiMulti.h>

//Declaración de usuario y clave de red WiFi.
WiFiMulti wifiMulti;

/*const char* ssid = "ABACANTVWIFI8440";
const char* password = "85047373038805";

const char* ssid = "TP-LINK_D6BF4E";
const char* password = "480Secur325";*/

//Declaración de terminales de salida en el ESP-WROOM-32.

const uint8_t ledPinEstado = 2;   //Led que indica si el sistema está encendido o en parada.
String ledEstado;
const uint8_t bombaUnoEstado = 23;//Salida para controlar la bomba 1.
bool bomba1LED = false;
const uint8_t bombaDosEstado = 22;//Salida para controlar la bomba 2.
bool bomba2LED = false;
bool ledModoSistema = false;      //modoSistema = false significa sistema en modo manual, modoSistema = true
                                  //significa sistema en modo automático.
bool marchaSistemaLOED = false;   //marchaSistema = false significa sistema parado, marchaSistema = true significa
                                  //sistema activo.
String modoEstado;

//Instanciación del servidor web.
AsyncWebServer server(80);

//Funciones de procesamiento de datos.

String processor(const String& var){//Función que chequea si el sistema está encendido y envía el estado.
  Serial.println("Entrando a processor.");
  if(var == "ESTADO_SISTEMA"){
    Serial.println(var);
    if(digitalRead(ledPinEstado)){
      ledEstado = "ENCENDIDO";
    }
    else{
      ledEstado = "APAGADO";
    }
    Serial.println(ledEstado);
    Serial.println("Saliendo de processor, estado modificado.");
    return ledEstado;
  }
  if(var == "MODO_SISTEMA"){
    return modoEstado;
  }
  Serial.println("Saliendo de processor, estado no modificado.");
  return String();
}

String mSistema(const String& varModo){//Función que chequea si el sistema está modo manual y envía el estado.
Serial.println("Entrando a mSistema.");
  if(varModo == "MODO_SISTEMA" && ledEstado == "ENCENDIDO"){
    Serial.println(varModo);
    if(digitalRead(ledModoSistema)){
      modoEstado = "AUTO";
    }
    else{
      modoEstado = "MANUAL";
    }
    Serial.println(modoEstado);
    Serial.println("Saliendo de mSistema, estado modificado.");
    return modoEstado;
  }
  if(varModo == "ESTADO_SISTEMA"){
    return ledEstado;
  }
  Serial.println("Saliendo de mSistema, estado no modificado.");
  return String();
}

void setup() {

  //Adición de redes a las que se puede conectar el dispositivo.
  wifiMulti.addAP("ABACANTVWIFI8440","85047373038805");
  wifiMulti.addAP("TP-LINK_D6BF4E","480Secur325");

  //Configuración de velocidad del puerto serial.
  Serial.begin(115200);
  Serial.println("Entrando a configuración de aplicación");

  //Configuración de las salidas del sistema.
  pinMode(ledPinEstado,OUTPUT);
  pinMode(ledModoSistema,OUTPUT);
  pinMode(bombaUnoEstado,OUTPUT);
  pinMode(bombaDosEstado,OUTPUT);

  //Inicialización SPIFFS.
  if(!SPIFFS.begin(true)){
    Serial.println("Ha ocurrido un error montando el SPIFFS");
    return;
  }
  else{
    Serial.println("Inicializado el Sistema de archivos.");
  }

  //Configuración e inicialización del wifi.
  WiFi.mode(WIFI_STA);
  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando al WiFi");
  }
  Serial.print("Conectado a la red " + WiFi.SSID() + " \n");
  Serial.print("Dirección IP asignada: " + (String) WiFi.localIP() + "\n");

  //Funciones para el manejo de la página web.

    //Manejo inicial de la página web.
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

    //Manejo de botones de activación/desactivación del sistema.
  server.on("/apagar",HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPinEstado,LOW);
    request->send(SPIFFS,"/index.html",String(),false,processor);
  });

  server.on("/encender",HTTP_GET,[](AsyncWebServerRequest *request){
    digitalWrite(ledPinEstado,HIGH);
    request->send(SPIFFS,"/index.html",String(),false,processor);
  });

    //Manejo de botones para selección del modo manual/automático
    //del sistema
  server.on("/auto",HTTP_GET,[](AsyncWebServerRequest *request){
    digitalWrite(ledModoSistema,HIGH);
    request->send(SPIFFS,"/index.html",String(),false,mSistema);
  });

  server.on("/manual",HTTP_GET,[](AsyncWebServerRequest *request){
    digitalWrite(ledModoSistema,LOW);
    request->send(SPIFFS,"/index.html",String(),false,mSistema);
  });

  //Inicio del servidor web.
  server.begin();

  Serial.println("Saliendo de configuración de aplicación");
}
 
void loop() {

}