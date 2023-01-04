#include <Arduino.h>
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <preferences.h>

//Declaración de usuario y clave de red WiFi.
WiFiMulti wifiMulti;

//Instanciación del objeto preferencias.

Preferences memoriaEstado;

//Declaración de terminales de salida en el ESP-WROOM-32.

const uint8_t ledPinEstado = 2;   //Led que indica si el sistema está encendido o en parada.
String ledEstado;
const uint8_t bombaUnoEstado = 23;//Salida para controlar la bomba 1.
bool bomba1LED = false;
const uint8_t bombaDosEstado = 22;//Salida para controlar la bomba 2.
bool bomba2LED = false;
bool bombaActiva = false;         //Falso significa bomba 1, verdadero significa bomba 2.
bool ledModoSistema = false;      //modoSistema = false significa sistema en modo manual, modoSistema = true
                                  //significa sistema en modo automático.
bool marchaSistemaLOED = false;   //marchaSistema = false significa sistema parado, marchaSistema = true significa
                                  //sistema activo.

//Declaración de terminales de entrada para el sistema.

const uint8_t supervisorT = 1;    //Esta es la entrada para el supervisor mono/trifásico.
bool superT = false;
const uint8_t nivelAgua = 3;      //Nivel del agua será una medición analógica.
uint8_t nivelA = 0;

String modoEstado;

//Instanciación del servidor web.
AsyncWebServer server(80);

//Función de procesamiento de datos. Debido al
//funcionamiento interno de la función send(), la función
//procesador es llamada cada vez que encuentra un
//placeholder en el código html, por esa razón el código
//interno de esta función está hecho de esa manera.

String Procesador(const String& var){//Función que chequea si el sistema está encendido y envía el estado.
  Serial.println("Entrando a Procesador.");
  if(var == "ESTADO_SISTEMA"){
    if(digitalRead(ledPinEstado)){
      ledEstado = "ENCENDIDO";
    }
    else{
      ledEstado = "APAGADO";
    }
    Serial.print("Sistema " + ledEstado + "." + "\n");
    Serial.println("Saliendo de Procesador");
    return ledEstado;
  }
  else if(var == "MODO_SISTEMA" && ledEstado == "APAGADO"){
    Serial.print("Sistema " + modoEstado + "." + "\n");
    Serial.println("Saliendo de Procesador");
    return modoEstado;
  }
  else if(var == "MODO_SISTEMA" && ledEstado == "ENCENDIDO"){
    if(digitalRead(ledModoSistema)){
      modoEstado = "AUTO";
    }
    else{
      modoEstado = "MANUAL";
    }
    Serial.print("Sistema " + modoEstado + "." + "\n");
    Serial.println("Saliendo de Procesador");
    return modoEstado;
  }
  Serial.println("Saliendo de Procesador.");
  return String();
}

// Función para controlar la bomba a encender
void Bomba() {

}

void handle_NoEncontrado() {

}

void setup() {

  //Adición de redes a las que se puede conectar el dispositivo.
  wifiMulti.addAP("ABACANTVWIFI8440","85047373038805");
  wifiMulti.addAP("TP-LINK_D6BF4E","480Secur325");

  //Configuración de velocidad del puerto serial.
  Serial.begin(115200);
  Serial.println("Entrando a configuración de aplicación");

  //Inicialización del sistema de preferencias NVS.
  memoriaEstado.begin("estado_sistema", false);
  memoriaEstado.clear();

  //Lectura de los estados iniciales del sistema.
  memoriaEstado.getBool("SisEncender",ledEstado);
  memoriaEstado.getBool("bombaActiva",bombaActiva);


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
  Serial.print("Conectado a la red " + WiFi.SSID() + "\n");
  Serial.print("Dirección IP asignada: ");
  Serial.println(WiFi.localIP());

  //Funciones para el manejo de la página web.

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

    //Manejo de botones de activación/desactivación del sistema.
  server.on("/apagar", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPinEstado,LOW);
    request->send(SPIFFS,"/index.html",String(),false,Procesador);
  });

  server.on("/encender", HTTP_GET,[](AsyncWebServerRequest *request){
    digitalWrite(ledPinEstado,HIGH);
    request->send(SPIFFS,"/index.html",String(),false,Procesador);
  });

  //Manejo de las imágenes y logos.

  server.on("/logoE", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(SPIFFS, "/Logo-empresa.png", String());
  });

  server.on("/logo-venapp", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(SPIFFS, "/logo-venapp.png", String());
  });

  server.on("/logo-whatsapp", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(SPIFFS, "/logo-whatsapp.png", String());
  });

  server.on("/logo-facebook", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(SPIFFS, "/logo-facebook.png", String());
  });

  server.on("/logo-youtube", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(SPIFFS, "/logo-youtube.png", String());
  });

  server.on("/logo-instagram", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(SPIFFS, "/logo-instagram.png", String());
  });

  server.on("/logo-linkedin", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(SPIFFS, "/logo-linkedin.png", String());
  });

  server.on("/logo-correo", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(SPIFFS, "/logo-correo.png", String());
  });

  server.on("/Pozo", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/Pozo.png", String());
  });

    //Manejo de botones para selección del modo manual/automático
    //del sistema
  server.on("/auto", HTTP_GET,[](AsyncWebServerRequest *request){
    if(ledEstado == "ENCENDIDO"){
      digitalWrite(ledModoSistema,HIGH);
    }
    request->send(SPIFFS, "/index.html", String(), false,Procesador);
  });

  server.on("/manual", HTTP_GET,[](AsyncWebServerRequest *request){
    if(ledEstado == "ENCENDIDO"){
      digitalWrite(ledModoSistema,LOW);
    }
    request->send(SPIFFS,"/index.html", String(), false,Procesador);
  });

  //Manejo de la página principal del sistema
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, Procesador);
  });

  //Manejo de la página de configuración.
  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Entrando a página configuración.");
    request->send(SPIFFS, "/config.html", String());
  });

  //Manejo de la página Sobre Nosotros
  server.on("/sobre-nosotros", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Entrando a página Sobre Nosotros.");
    request->send(SPIFFS, "/sobre-nosotros.html", String());
  });

  //Manejo de la página Contacto.
  server.on("/contacto", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Entrando a página Contacto.");
    request->send(SPIFFS, "/contacto.html", String());
  });

  //Manejo de la página "No encontrado".

  server.onNotFound([](AsyncWebServerRequest *request){
    Serial.println("Página no encontrada.");
    request->send(SPIFFS, "/no_encontrado.html", String());
  });

  //Inicio del servidor web.
  server.begin();

  Serial.println("Saliendo de configuración de aplicación");
}
 
void loop() {

}