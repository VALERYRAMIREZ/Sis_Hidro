#include "mWifi.h"
#include <EEPROM.h>
#include "debuguear.h"
#include "almacen.h"
#include "varSistema.h"

extern wifiConfig wifi_usuario;
extern wifiConfig wifi_usuario_leida;
extern EEPROMClass eeprom;

IPAddress iPDir;
IPAddress gateWay;
IPAddress subNet;

IPAddress localIP(192, 168, 1, 163);

IPAddress localGateway(192, 168, 1, 1);

IPAddress subnet(255,255,255,0);

bool pruebaDireccionIP(String ip, String gate)
{
  IPAddress ipVal;
  IPAddress gateVal;
  ipVal.fromString(ip);
  gateVal.fromString(gateway);
  if(!IPAddress().fromString(ip.c_str()) && !IPAddress().fromString(gate.c_str()))
  {
    return false;
  }
  return true;
}

bool initWiFi(void) {
  wifiConfig dataWifi = wifi_usuario;
  uint8_t cuentaCiclo = 0;
  ssid = dataWifi.ssid;
  pass = dataWifi.clave;
  ip = dataWifi.ipDir;
  gateway = dataWifi.gatewaydir;
  IPAddress iPDir;
  IPAddress gateWay;
  IPAddress subNet;
  imprimeDebug(habDebug, "ssid: ", ssid);
  imprimeDebug(habDebug, "clave: ", pass);
  imprimeDebug(habDebug, "Dirección IP: ", dataWifi.ipDir);
  imprimeDebug(habDebug, "Gateway: ", dataWifi.gatewaydir);
  imprimeDebug(habDebug,"Tiempo de espera","vacio");
  if(habDebug)
  {
    Serial.print(dataWifi.tEnEspera);
    Serial.println("");
  }
  imprimeDebug(habDebug,"Nombre de la red", dataWifi.nombre_red);
  imprimeDebug(habDebug, "Clave de la red AP", dataWifi.claveAP);
  if(ssid == "" || ip == "") {
    imprimeDebug(habDebug, "Dirección IP o dirección MAC indefinida", "vacio");
    return false;
  }
  
  imprimeDebug(habDebug, "Parámetros para configuración como STA: ", "vacio");
  if(habDebug)
  {
    Serial.println(ip);
    Serial.println(iPDir.fromString(ip));
    Serial.println(iPDir);
    Serial.println(gateway);
    Serial.println(gateWay.fromString(gateway));
    Serial.println(gateWay);
    Serial.println(subnet);
  }
  if(!pruebaDireccionIP(ip, gateway)) // || !WiFi.config(iPDir, gateWay, subnet)) 
  {
    imprimeDebug(habDebug, "Fallo de configuración como STA", "vacio");
    return false;
  }
  imprimeDebug(habDebug, "Red: ", ssid);
  imprimeDebug(habDebug, "Clave: ", pass);
  WiFi.setSleep(WIFI_PS_NONE); 
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);                    //la conexión.
  //localIP.fromString(ip.c_str());
  WiFi.mode(WIFI_AP_STA);
  eeprom.get(0, wifi_usuario_leida);
  //wifi_usuario_leida = Lee_EEPROM();
  imprimeDebug(habDebug, "Red AP: ", dataWifi.nombre_red);
  imprimeDebug(habDebug, "Clave AP: ", dataWifi.claveAP);
  if(!dataWifi.accesoAP)
  {
    WiFi.softAP(AP_ssid, AP_clave) ? imprimeDebug(habDebug, "Creada la red AP con los parámetros pasados por configuración","vacio") :
                                     imprimeDebug(habDebug,"No se pudo crear la red AP con los parámetros pasados por configuración", "vacio");
  }
  else if(dataWifi.accesoAP)
  {
    WiFi.softAP(&wifi_usuario_leida.nombre_red[0], &wifi_usuario_leida.claveAP[0]) ? 
                                     imprimeDebug(habDebug, "Creada la red AP con los parámetros pasados por configuración","vacio") :
                                     imprimeDebug(habDebug,"No se pudo crear la red AP con los parámetros pasados por configuración", "vacio");
  }
//  WiFi.softAP(AP_ssid, AP_clave);
  WiFi.begin(ssid.c_str(), pass.c_str());
  imprimeDebug(habDebug, "Conectando al WiFi...", "vacio");

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;
  imprimeDebug(habDebug, "Contando los milisegundos antes de evaluar si se conectó", "vacio");
  if(habDebug)
  {
    Serial.print("La red en modo AP configurada es: ");
    Serial.println(WiFi.softAPSSID());
  }
  while (WiFi.status() != WL_CONNECTED)
  {
    cuentaCiclo++;
    delay(5000);
    currentMillis = millis();
    imprimeDebug(habDebug, "Estado de la conexión: ", (String) WiFi.status());
    imprimeDebug(habDebug, "Tiempo transcurrido en milisegundos: ", "vacio");
    if(habDebug)
    {
      Serial.println(currentMillis - previousMillis);
    }
    imprimeDebug(habDebug, "Tiempo máximo de espera: ", "vacio");
    if(habDebug)
    {
      Serial.println(60000*dataWifi.tEnEspera);
    }
    eeprom.get(0, wifi_usuario);
    //wifi_usuario = Lee_EEPROM();
    if((currentMillis - previousMillis >= 60000*dataWifi.tEnEspera))
    {
      imprimeDebug(habDebug, "Falla de conexión.", "vacio");
      return false;
    }
  }
  if(habDebug)
  {
    Serial.print("La dirección IP del ESP-01S es: ");
    Serial.println(WiFi.localIP());
  }
  imprimeDebug(habDebug, "La dirección MAC del ESP es: ", WiFi.softAPmacAddress());
  if (habDebug)
  {
    Serial.print("Máximo de cuentaCiclo");
    Serial.println(60000*dataWifi.tEnEspera/5000);
    Serial.print("Red captada?: ");
    Serial.println(dataWifi.estaWiFi);
  }
  
    if(dataWifi.estaWiFi && ((cuentaCiclo > 1) && (cuentaCiclo < (60000*dataWifi.tEnEspera/5000))))
    //Esto lo que hace es verificar si la red se activó al menos dos ciclos de verificación después de que
    //se configuró e inició el módulo wifi, en ese caso es seguro que el ESP no se va a conectar al servidor,
    //por lo cual se reinicia para que se conecte al servidor de sinric inmediatamente al agarrar conexión.
    //Para esto se está asumiendo que si el ESP se conecta a la red, lo hará en el primer ciclo de chequeo
    //después de reiniciado.
  {
    imprimeDebug(habDebug, "ESP se va a reiniciar", "vacio");
    delay(1000);
    ESP.restart();
  }
  else if(!dataWifi.estaWiFi)
  {
    dataWifi.estaWiFi = true;
    //Escribe_EEPROM(0);
    eeprom.put(0, wifi_usuario);
    eeprom.commit();
  }
    return true;
}