#include <sstream>
#include <AsyncElegantOTA.h>
#include <EEPROM.h>
#include "SPIFFS.h"
#include "backend.h"
#include "debuguear.h"
#include "varSistema.h"

    
extern AsyncWebServer server;
extern Semana semana;
extern FechaProg tiempo;
extern AsyncElegantOtaClass OTA;

extern wifiConfig wifi_usuario;
//extern ESP32Time rtc;
extern estadoSistema sistema;

extern EEPROMClass eeprom;
extern wifiConfig wifi_usuario_leida;

String Procesador(const String& var){//Función que chequea si el sistema está encendido y envía el estado.
  Serial.print("La variable recibida es: ");
  Serial.println(var);
  Serial.println("Entrando a Procesador.");
  if(var == "DIRECCION")
  {
    Serial.println("Enviando dirección IP.");
    Serial.println(WiFi.softAPIP());
    return WiFi.softAPIP().toString();
  }
  else if(var == "ESTADO_SISTEMA"){
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
  return "String()";
}

bool Inicia_SPIFFS(void)
{
  if(!SPIFFS.begin(true)){
    Serial.println("Ha ocurrido un error montando el SPIFFS");
    return false;
  }
  else{
    Serial.println("Inicializado el Sistema de archivos.");
  }
  return true;
}

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

// Función para extraer la data enviada desde la página web.

uint8_t Extrae_Data(std::string trama, FechaProg* destino,char sep) {
  uint8_t contador = 0;
  std::string lectura = "";
  std::stringstream cadena_leida(trama);
  Serial.print("La trama es: ");
  Serial.println(trama.c_str());
  Serial.print("El stringstream es: ");
  Serial.println(cadena_leida.str().c_str());
  while(std::getline(cadena_leida, lectura, sep))
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

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void Define_Backend(bool tipoWeb)
{
    //Funciones para el manejo de la página web.
  if(tipoWeb)
  {
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/style_2.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/style_2.css", "text/css");
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
    request->send(SPIFFS, "/logo.jpg", String());
        
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

  server.on("/manager", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(usuarioHTTP, claveHTTP)) {
      return request->requestAuthentication();
    };
    ultimaPaginaCargada = "/wifimanager.html";
    imprimeDebug(habDebug, "Entrando a página configuración.", "vacío");
    request->send(SPIFFS, ultimaPaginaCargada, String(), false, Procesador);
        
  });

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
    
    //rtc.setTime(0, tiempo.minuto, tiempo.hora, tiempo.dia, tiempo.mes, tiempo.anio);
    sistema.reloj = true;
  });
  server.addHandler(manejadorJson);

  //Manejo de la página "No encontrado".

  server.onNotFound([](AsyncWebServerRequest *request){
    Serial.println("Cargando página no encontrada.");
    request->send(SPIFFS, "/no_encontrado.html", String());
  });

    imprimeDebug(habDebug, "Inicializando el servidor web para OTA en STA.", "vacio");
    OTA.begin(&server, usuarioAdmin, claveAdmin);
    imprimeDebug(habDebug, "Inicializando el servidor web en STA.", "vacio");
    imprimeDebug(habDebug, "Servidor MDNS iniciado", "vacio");
    server.begin();
    /*if (!MDNS.begin(wifi_usuario.nombre_red)) {
      imprimeDebug(habDebug, "Ocurrió un error al configurar el servidor MDNS", "vacio");
      while (1) {
        delay(1000);
      }
    }*/
  }
  else
  {
    wifi_usuario.accesoAP = false;
    eeprom.put(0, wifi_usuario);
    eeprom.commit();
    imprimeDebug(habDebug, "Configurando PA (Punto de Acceso)", "vacio");
    eeprom.get(0, wifi_usuario_leida);
    imprimeDebug(habDebug, "AP_ssid: ", AP_ssid);
    imprimeDebug(habDebug, "AP_clave: ", AP_clave);
    wifi_usuario_leida.accesoAP   ?   WiFi.softAP(wifi_usuario_leida.nombre_red, wifi_usuario_leida.claveAP) :
                                      WiFi.softAP(AP_ssid, AP_clave);
                                      
    IPAddress IP = WiFi.softAPIP();
    if(habDebug)
    {
      Serial.print("AP IP address: ");
      Serial.println(IP);
    }

    server.onNotFound(notFound);

    server.on("/manager", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/wifimanager.html", String(), false, Procesador);
      imprimeDebug(habDebug, "Desplegando la página de configuración del wifi en AP.", "vacio");
      imprimeDebug(habDebug, "Enviada la página de configuración del WiFi en AP.", "vacio");
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/style.css", "text/css");
    });

    server.on("/style_2.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/style_2.css", "text/css");
    });

    server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request){
      wifi_usuario.estaWiFi = true;
      eeprom.put(0, wifi_usuario);
      eeprom.commit();
      imprimeDebug(habDebug, "Comenzando almacenamiento de datos de para STA.", "vacio");
      int params = request->params();
      for(int i = 0; i < params; i++)
      {
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost())
        {
          if(p->name() == PARA_ENTRADA_1)  //ssid.
          {
            strncpy(wifi_usuario.ssid, p->value().c_str(), sizeof(wifi_usuario.ssid));
            imprimeDebug(habDebug, "SSID establecido a: ", wifi_usuario.ssid);
          }
          if(p->name() == PARA_ENTRADA_2)  //pass.
          {
            strncpy(wifi_usuario.clave, p->value().c_str(), sizeof(wifi_usuario.clave));
            imprimeDebug(habDebug, "Clave establecida a: ", wifi_usuario.clave);
          }
          if(p->name() == PARA_ENTRADA_3)  //IP.
          {
            strncpy(wifi_usuario.ipDir, p->value().c_str(), sizeof(wifi_usuario.ipDir));
            imprimeDebug(habDebug, "Dirección IP establecida a: ", wifi_usuario.ipDir);
          }
           if(p->name() == PARA_ENTRADA_4)  //gateway.
          {
            strncpy(wifi_usuario.gatewaydir, p->value().c_str(), sizeof(wifi_usuario.gatewaydir));
            imprimeDebug(habDebug, "Gateway establecida a: ", wifi_usuario.gatewaydir);
          }
          if(p->name() == PARA_ENTRADA_5)  //tEspera.
          {
            imprimeDebug(habDebug, "Tiempo de espera por red establecido a: ", p->value().c_str());
            uint8_t num = atoi(p->value().c_str());
            char buffer[10];
            sprintf(buffer, "%d", num);
            wifi_usuario.tEnEspera = atoi(p->value().c_str());
            eeprom.put(0, wifi_usuario);
            eeprom.commit();
            //imprimeDebug(habDebug, "Gateway establecida a: ", std::to_string(wifi_usuario.tEnEspera));
          }
          if(p->name() == PARA_ENTRADA_6)  //nombre-red.
          {
            imprimeDebug(habDebug, "Nombre de equip en STA y AP establecido a: ", p->value().c_str());
            strncpy(wifi_usuario.nombre_red, p->value().c_str(),sizeof(wifi_usuario.nombre_red));
          }
          if(p->name() == PARA_ENTRADA_7)  //clave-ap.
          {
            imprimeDebug(habDebug, "Clave de AP establecida a: ", p->value().c_str());
            strncpy(wifi_usuario.claveAP, p->value().c_str(),sizeof(wifi_usuario.claveAP));
          }
        }
      }
      imprimeDebug(habDebug, "Almacenando parámetros de red en la estructura.", "vacio");
      wifi_usuario.accesoAP = true;
      imprimeDebug(habDebug, "Almacenados los datos en la estructura, almacenando estructura en la EEPROM", "vacio");
      eeprom.put(0, wifi_usuario);
      if(eeprom.commit())
      {
        imprimeDebug(habDebug, "Datos almacenados en la EEPROM después del commit()", "vacio");
        eeprom.get(0, wifi_usuario_leida);
        imprimeDebug(habDebug, "", wifi_usuario_leida.ssid);
        imprimeDebug(habDebug, "", wifi_usuario_leida.clave);
        imprimeDebug(habDebug, "", wifi_usuario_leida.ipDir);
        imprimeDebug(habDebug, "", wifi_usuario_leida.gatewaydir);
        if(habDebug)
        {
          Serial.print(wifi_usuario_leida.tEnEspera);
          Serial.println("");
        }
        imprimeDebug(habDebug, "", wifi_usuario_leida.nombre_red);
        imprimeDebug(habDebug, "", wifi_usuario_leida.claveAP);
        if(habDebug)
        {
          Serial.println(wifi_usuario_leida.accesoAP);
        }
      }
      delay(10000);
      imprimeDebug(habDebug, "En 10 segundos se estará reiniciando el ESP. Una vez reiniciado vaya a la dirección IP: ",  (const char*) wifi_usuario_leida.ipDir);
      delay(10000);
      ESP.restart();
    });
    imprimeDebug(habDebug, "Inicializando el servidor web para OTA en AP.", "vacio");
    AsyncElegantOTA.begin(&server, usuarioAdmin, claveAdmin);
    imprimeDebug(habDebug, "Inicializando el servidor web en AP", "vacio");
    server.begin();
    imprimeDebug(habDebug, "Iniciado el servidor Web en modo AP.", "vacio");
    imprimeDebug(habDebug, "La dirección MAC del módulo es: ", "vacio");
    imprimeDebug(habDebug, WiFi.softAPmacAddress(), "vacio");
  }
  imprimeDebug(habDebug, "Saliendo de configuración", "vacio");
}