#include "backend.h"

using namespace std;

//Instanciación del servidor web.
AsyncWebServer server(80);
Semana semana = {};

extern ESP32Time rtc;

FechaProg tiempo = {}, tiempoLeido = {};

//Función de procesamiento de datos. Debido al
//funcionamiento interno de la función send(), la función
//procesador es llamada cada vez que encuentra un
//placeholder en el código html, por esa razón el código
//interno de esta función está hecho de esa manera.

String Procesador(const String& var){//Función que chequea si el sistema está encendido y envía el estado.
  Serial.println("Entrando a Procesador.");
  if(var == "ESTADO_SISTEMA"){
    if(!digitalRead(ledPinSistemaApagado) && digitalRead(ledPinSistemaEncendido)){
      ledEstado = "ENCENDIDO";
    }
    else if(digitalRead(ledPinSistemaApagado) && !digitalRead(ledPinSistemaEncendido)){
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

uint8_t Extrae_Data(String trama, FechaProg* destino,char sep) {
  uint8_t contador = 0;
  string lectura = "";
  stringstream cadena_leida(&trama[0]);
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
          destino->anio = stoi(lectura);
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

void Func_Backend(void)
{
//Funciones para el manejo de la página web.

        server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/style.css", "text/css");
        });

        server.on("/app", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/app.js", "text/javascript");
        });

            //Manejo de botones de activación/desactivación del sistema.
        server.on("/apagar", HTTP_GET, [](AsyncWebServerRequest *request){
            digitalWrite(ledPinSistemaEncendido,LOW);
            digitalWrite(ledPinSistemaApagado,HIGH);
            request->send(SPIFFS,"/index.html",String(),false,Procesador);
            
        });

        server.on("/encender", HTTP_GET,[](AsyncWebServerRequest *request){
            digitalWrite(ledPinSistemaApagado,LOW);
            digitalWrite(ledPinSistemaEncendido,HIGH);
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
            modoSistema = true;
            }
            request->send(SPIFFS, "/index.html", String(), false,Procesador); 
        });

        server.on("/manual", HTTP_GET,[](AsyncWebServerRequest *request){
            if(ledEstado == "ENCENDIDO"){
            digitalWrite(ledModoSistema,LOW);
            modoSistema = false;
            }
            request->send(SPIFFS,"/index.html", String(), false,Procesador);
                
        });

        //Manejo de la página principal del sistema
        server.on("/index", HTTP_GET, [](AsyncWebServerRequest *request){
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
            Serial.print("El anio es: ");
            Serial.println(tiempo.anio);
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

            /*Serial.print("La hora actual es: ");
            Serial.println((const char *) jsonObj["hora-actual"]);*/
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

            /*Serial.print("El horario del lunes en la mañana es: ");
            Serial.println(semana.lunes.inicioManana);
            Serial.print("El horario del domingo en la tarde es: ");
            Serial.println(semana.domingo.finTarde);
            request->send(200);
            Serial.println("Configuración recibida.");*/
            
            rtc.setTime(0, tiempo.minuto, tiempo.hora, tiempo.dia, tiempo.mes, tiempo.anio);
            sistema.reloj = true;
        });
        server.addHandler(manejadorJson);

        //Manejo de la página "No encontrado".

        server.onNotFound([](AsyncWebServerRequest *request){
            Serial.println("Cargando página no encontrada.");
            request->send(SPIFFS, "/no_encontrado.html", String());
                
        });    
}