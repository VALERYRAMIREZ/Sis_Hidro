#include <ArduinoJson.h>
#include "varSistema.h"


    
    /*****Claves de acceso a programación****/

    const char* usuarioAdmin = "admin";
    const char* claveAdmin = "admin";


    /*****Parámetros a buscar una vez********/
    /***realizada la carga de los datos de***/
    /**********la red a utilizar*************/

    const char* PARA_ENTRADA_1 = "ssid";
    const char* PARA_ENTRADA_2 = "pass";
    const char* PARA_ENTRADA_3 = "ip";
    const char* PARA_ENTRADA_4 = "gateway";
    const char* PARA_ENTRADA_5 = "tEspera";
    const char* PARA_ENTRADA_6 = "nombre-red-ap";
    const char* PARA_ENTRADA_7 = "clave-red-ap";

    //Declaración de usuario y clave de red WiFi.

    WiFiMulti wifiMulti;

    unsigned long previousMillis = 0;

    String ssid = "";
    String pass = "";
    String ip = "";
    String gateway = "";

    /*****Habilitador de impresión en********/
    /*************modo debug*****************/

    bool habDebug = true;

    String usu = "admin", clav = "admin";
    char* usuarioHTTP = &usu[0];
    char* claveHTTP = &clav[0];
    bool eHTTP = false;

    /******Contador de acceso a  página /index********/
    /***********antes de introducir clave.************/

    bool cuentaAcceso = false;

    /*****Variables a utilizar en el tempodizador*****/
    /***************sensor de nivel.******************/

    DynamicJsonDocument nivel(50);
    hw_timer_t *tempo = NULL;
    volatile bool haTemporizado = false;
    
    /*****Variables a utilizar en la interrupción*****/
    /***************sensor de nivel.******************/

    bool volatile haInterrumpido = false;

    //Variable que almacenará la última página cargada.

    String ultimaPaginaCargada = "";

    //Instanciación del objeto preferencias.

    Preferences memoriaEstado;

    // Variables relativas al estado del sistema.

    DynamicJsonDocument sistemaEstado(50);
    const uint8_t ledPinSistemaApagado = 2;   //Led que indica si el sistema está apagado.
    const uint8_t ledPinSistemaEncendido = 4; //Led que indica si el sistema está encendido.
    String ledEstado = "APAGADO";
    const uint8_t bombaUno = 22;//Salida para controlar la bomba 1.
    bool bomba1LED = false;
    const uint8_t bombaDos = 21;//Salida para controlar la bomba 2.
    bool bomba2LED = false;
    bool bombaActiva = false;         //Falso significa bomba 1, verdadero significa bomba 2.
    bool modoSistema = false;         //modoSistema = false significa sistema en modo manual, modoSistema = true
    uint8_t ledModoSistema = 23;      //Significa sistema en modo automático cuando ledModoSistema es true. Cuando
    bool marchaSistemaLED = false;    //marchaSistema = false significa sistema parado, marchaSistema = true significa

    //Declaración de terminales de entrada para el sistema.

    const uint8_t supervisorT = 1;    //Esta es la entrada para el supervisor mono/trifásico.
    bool superT = false;
    const uint8_t nivelAgua = 3;      //Nivel del agua será una medición analógica.
    uint8_t nivelA = 0;

    //Terminal de gatillo para sensor de nivel.
    const int pinGatillo = 13;

    // Terminal de interrupcion.
    const int pinInt = 12;

    String modoEstado = "MANUAL";

    //Declaración de las variables para el tipo de tanque y volumen mínimo.

    uint8_t tipoTanque = 0;
    uint8_t volMax = 0;
    uint8_t volMin = 0;
    float radioSup = 0.00;
    float radioInf = 0.00;
    float alturaTanque = 0.00;
    float profTanque = 0.00;

    //Variable para almacenar la data leída del rtc.

    const char * dataM;

    FechaProg tiempo = {}, tiempoLeido = {};
    Semana semana = {};
    estadoSistema sistema = {};

    wifiConfig wifi_usuario = {}, wifi_usuario_leida = {};