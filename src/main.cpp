#include <Arduino.h>
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <preferences.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "ESP32Time.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <Stream.h>
using namespace std;

//Declaración de usuario y clave de red WiFi.
WiFiMulti wifiMulti;

//Definición del usuario, clave y estado de acceso.

char* usuarioHTTP = "admin";
char* claveHTTP = "admin";
bool eHTTP = false;

//Variable que almacenará la última página cargada.

String ultimaPaginaCargada = "";

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

//Instanciación del RTC
ESP32Time rtc(0);            //GMT-0.

//Instanciación del servidor web.
AsyncWebServer server(80);

//Instanciación del documento JSON para recibir la configurcación
//del sistema.

DynamicJsonDocument docJson(512);

/*********Estructuras para el almacenamiento de***********/
/**********la programación horaria del sistema************/

//Estructura para almacenar la fecha a programar al sistema.

struct FechaProg {
  int dia;
  int mes;
  int ano;
  int hora;
  int minuto;
  int segundo;
};

// Estructura para almacenar el día y programa respectivo.
struct diaSemana{
  const char* inicioManana;
  const char* finManana;
  const char* inicioTarde;
  const char* finTarde;
};

// Estructura para almacenar los programas de todos los 
// dias de la semana.
struct Semana {
  diaSemana lunes;
  diaSemana martes;
  diaSemana miercoles;
  diaSemana jueves;
  diaSemana viernes;
  diaSemana sabado;
  diaSemana domingo;
};

// Campo de bits para almacenar el estado del sistema

struct estadoSistema {
  bool bateria  : 1;
  bool reloj    : 1;
};
// Instanciación de las estructuras de tiempo y programación de
// trabajo.
FechaProg tiempo = {};
Semana semana = {"0"};
estadoSistema sistema;

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

// Función para analizar la data enviada por la página web.

void manejaJson(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  Serial.println("Recibiendo configuración de la dirección IP ." + request->client()->remoteIP().toString() + " " + request->url());
  if(!filename) {
    Serial.println("No hay archivo.");
  }
  if(!index) {
    request->_tempFile = SPIFFS.open("/" + filename, "w");
    Serial.println("Carga Iniciada" + String(filename));
  }
  if(len) {
    request->_tempFile.close();
    Serial.println("Escribiendo archivo: " + String(filename) + ", tamaño: " + String(len));
  }
  if(final) {
    request->_tempFile.close();
    Serial.println("Carga completa: " + String(filename) + ", tamaño: " + String(index + filename));
    request->redirect("/");
  }
  else {
    Serial.println("No se cumplieron las condiciones para manejar la data recibida");
  }
}

uint8_t Extrae_Data(string trama, FechaProg* destino,char sep) {
  uint8_t contador = 0;
  string lectura = "";
  stringstream cadena_leida(trama);
  Serial.print("La trama es: ");
  Serial.println(trama.c_str());
  Serial.print("El stringstream es: ");
  Serial.println(cadena_leida.str().c_str());
  while(getline(cadena_leida, lectura, sep))
  {
    if(sep == '-')
    {
      switch(contador)
      {
        case 0:
        {
          destino->ano = stoi(lectura);
        }
        break;
        case 1:
        {
          destino->mes = stoi(lectura);
        }
        break;
        case 2:
        {
          destino->dia = stoi(lectura);
        }
        break;
      }
    }
    else if(sep == ':')
    {
      switch(contador)
      {
        case 0:
        {
          destino->hora = stoi(lectura);
        }
        break;
        case 1:
        {
          destino->minuto = stoi(lectura);
        }
        break;
      }
    }
    contador++;
  }
  Serial.print("El contador es: ");
  Serial.println(contador);
  return contador;
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
  wifiMulti.addAP("Delfos", "Joseph#29");

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

  server.on("/app", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/app.js", "text/javascript");
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
    Serial.println("Cargando logo-empresa");
    request->send(SPIFFS, "/Logo-empresa.png", String());
        
  });

  server.on("/logo-venapp", HTTP_GET, [](AsyncWebServerRequest * request){
    Serial.println("Cargando logo-venapp");
    request->send(SPIFFS, "/logo-venapp.png", String());
        
  });

  server.on("/logo-whatsapp", HTTP_GET, [](AsyncWebServerRequest * request){
    Serial.println("Cargando logo-whatsapp");
    request->send(SPIFFS, "/logo-whatsapp.png", String());
        
  });

  server.on("/logo-facebook", HTTP_GET, [](AsyncWebServerRequest * request){
    Serial.println("Cargando logo-facebook");
    request->send(SPIFFS, "/logo-facebook.png", String());
        
  });

  server.on("/logo-youtube", HTTP_GET, [](AsyncWebServerRequest * request){
    Serial.println("Cargando logo-youtube");
    request->send(SPIFFS, "/logo-youtube.png", String());
        
  });

  server.on("/logo-instagram", HTTP_GET, [](AsyncWebServerRequest * request){
    Serial.println("Cargando logo-instagram");
    request->send(SPIFFS, "/logo-instagram.png", String());
        
  });

  server.on("/logo-linkedin", HTTP_GET, [](AsyncWebServerRequest * request){
    Serial.println("Cargando logo-linkedin");
    request->send(SPIFFS, "/logo-linkedin.png", String());
        
  });

  server.on("/logo-correo", HTTP_GET, [](AsyncWebServerRequest * request){
    Serial.println("Cargando logo-correo");
    request->send(SPIFFS, "/logo-correo.png", String());
        
  });

  server.on("/Pozo", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Cargando imagen página no encontrada");
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
    if(!request->authenticate(usuarioHTTP, claveHTTP)) {
      return request->requestAuthentication("Ingreso al Sistema");
    };
    ultimaPaginaCargada = "/index.html";
    Serial.println("Entrando a página principal");
    request->send(SPIFFS, ultimaPaginaCargada, String(), false, Procesador);
        
  });

  //Manejo de la página de configuración.
  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(usuarioHTTP, claveHTTP)) {
      return request->requestAuthentication();
    };
    ultimaPaginaCargada = "/config.html";
    Serial.println("Entrando a página configuración.");
    request->send(SPIFFS, ultimaPaginaCargada, String());
        
  });

  //Manejo de la página Sobre Nosotros
  server.on("/sobre-nosotros", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(usuarioHTTP, claveHTTP)) {
      return request->requestAuthentication();
    };
    ultimaPaginaCargada = "/sobre-nosotros.html";
    Serial.println("Entrando a página Sobre Nosotros.");
    request->send(SPIFFS, ultimaPaginaCargada, String());
        
  });

  //Manejo de la página Contacto.
  server.on("/contacto", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(usuarioHTTP, claveHTTP)) {
      return request->requestAuthentication();
    };
    ultimaPaginaCargada = "/contacto.html";
    Serial.println("Entrando a página Contacto.");
    request->send(SPIFFS, ultimaPaginaCargada, String());
        
  });

//Manejo de la página de sistema bloqueado.

  server.on("/no-permitido", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Enviando no permitido.");
    request->send(401);
  });

  server.on("/autenticar", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(usuarioHTTP, claveHTTP)) {
      return request->requestAuthentication();
    };
    Serial.println("Bloqueando el sistema");
    ultimaPaginaCargada = "/autenticar.html";
    request->send(SPIFFS, ultimaPaginaCargada, String());
  });

  server.on("/bloqueado", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Bloqueando el sistema.");
    AsyncWebServerResponse *respuesta = request->beginResponse(SPIFFS, "/bloqueado.html", String());
    respuesta->addHeader("Clear-Site-Data", "cache");
    respuesta->addHeader("Clear-Site-Data", "cookies");
    request->send(respuesta);
    Serial.println("Página 'bloqueado' cargada.");
    Serial.println("Solicitando autenticación.");
    if(!request->authenticate("salir", "sistema")) {
      Serial.println("Autenticación incorrecta, intente nuevamente");
      request->requestAuthentication();
      Serial.println("Autenticación por datos incorrectos solicitada");
    }
    else {
      Serial.println("Solicitanto última página cargada al retornar de bloqueo del sistema.");
      request->send(SPIFFS, ultimaPaginaCargada, String(), false, Procesador);
      Serial.println("Última página cargada solicitada");
    };
  });

  //Manejo de la data de programación recibida.

  AsyncCallbackJsonWebHandler* manejadorJson = new AsyncCallbackJsonWebHandler("/forma-dato", [](AsyncWebServerRequest *request, JsonVariant &docJson) {
    auto&& jsonObj = docJson.as<JsonObject>();
    Serial.print("La fecha actual es :");
    Serial.println((const char *) jsonObj["fecha"]);
    if(Extrae_Data(jsonObj["fecha"], &tiempo, '-') != 3)
    {
      Serial.println("Fecha con formato incorrecto, no se puede activar el reloj, el contador");      
    }
    else
    {
      Serial.println("Fecha con formato correcto, el reloj se puede configurar.");
      Serial.print("El día es: ");
      Serial.println(tiempo.dia);
      Serial.print("El mes es: ");
      Serial.println(tiempo.mes);
      Serial.print("El ano es: ");
      Serial.println(tiempo.ano);
    }
    if(Extrae_Data(jsonObj["hora-actual"], &tiempo, ':') != 2)
    {
      Serial.println("Hora con formato incorrecto, no se puede activar el reloj, el contador");
    }
    else
    {
      Serial.println("Fecha con formato correcto, el reloj se puede configurar.");
      Serial.print("La hora es: ");
      Serial.println(tiempo.hora);
      Serial.print("Los minutos son: ");
      Serial.println(tiempo.minuto);
      Serial.print("Los segundos son: ");
      Serial.println(tiempo.segundo);
    }

    Serial.print("La hora actual es: ");
    Serial.println((const char *) jsonObj["hora-actual"]);
    semana.lunes.inicioManana = jsonObj["lunes-manana-inicio"];
    semana.lunes.finManana = jsonObj["lunes-manana-fin"];
    semana.lunes.inicioTarde = jsonObj["lunes-tarde-inicio"];
    semana.lunes.finTarde = jsonObj["lunes-tarde-fin"];
    semana.martes.inicioManana = jsonObj["martes-manana-inicio"];
    semana.martes.finManana = jsonObj["martes-manana-fin"];
    semana.martes.inicioTarde = jsonObj["martes-tarde-inicio"];
    semana.martes.finTarde = jsonObj["martes-tarde-fin"];
    semana.miercoles.inicioManana = jsonObj["miercoles-manana-inicio"];
    semana.miercoles.finManana = jsonObj["miercoles-manana-fin"];
    semana.miercoles.inicioTarde = jsonObj["miercoles-tarde-inicio"];
    semana.miercoles.finTarde = jsonObj["miercoles-tarde-fin"];
    semana.jueves.inicioManana = jsonObj["jueves-manana-inicio"];
    semana.jueves.finManana = jsonObj["jueves-manana-fin"];
    semana.jueves.inicioTarde = jsonObj["jueves-tarde-inicio"];
    semana.jueves.finTarde = jsonObj["jueves-tarde-fin"];
    semana.viernes.inicioManana = jsonObj["viernes-manana-inicio"];
    semana.viernes.finManana = jsonObj["viernes-manana-fin"];
    semana.viernes.inicioTarde = jsonObj["viernes-tarde-inicio"];
    semana.viernes.finTarde = jsonObj["viernes-tarde-fin"];
    semana.sabado.inicioManana = jsonObj["sabado-manana-inicio"];
    semana.sabado.finManana = jsonObj["sabado-manana-fin"];
    semana.sabado.inicioTarde = jsonObj["sabado-tarde-inicio"];
    semana.sabado.finTarde = jsonObj["sabado-tarde-fin"];
    semana.domingo.inicioManana = jsonObj["domingo-manana-inicio"];
    semana.domingo.finManana = jsonObj["domingo-manana-fin"];
    semana.domingo.inicioTarde = jsonObj["domingo-tarde-inicio"];
    semana.domingo.finTarde = jsonObj["domingo-tarde-fin"];

    Serial.print("El horario del lunes en la mañana es: ");
    Serial.println(semana.lunes.inicioManana);
    Serial.print("El horario del domingo en la tarde es: ");
    Serial.println(semana.domingo.finTarde);
    request->send(200);
    Serial.println("Configuración recibida.");
    
    rtc.setTime(0, tiempo.minuto, tiempo.hora, tiempo.dia, tiempo.mes, tiempo.ano);
    sistema.reloj = true;
  });
  server.addHandler(manejadorJson);

  //Manejo de la página "No encontrado".

  server.onNotFound([](AsyncWebServerRequest *request){
    Serial.println("Cargando página no encontrada.");
    request->send(SPIFFS, "/no_encontrado.html", String());
        
  });

  //Inicio del servidor web.
  server.begin();

  Serial.println("Saliendo de configuración de aplicación");
}
 
void loop() {
  if(sistema.reloj)
  {
    Serial.println(rtc.getTimeDate(true));
    delay(1000);
  }
}