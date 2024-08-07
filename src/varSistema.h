#ifndef VARSISTEMA_H
#define VARSISTEMA_H
    #include <WiFiMulti.h>
    #include <preferences.h>
    #include <StreamString.h>

    #define AP_ssid           "CONTROL-WIFI-RED"
    #define AP_clave          "administrador"
    
        /*****Claves de acceso a programación****/

    extern const char* usuarioAdmin;
    extern const char* claveAdmin;

    /*****Parámetros a buscar una vez********/
    /***realizada la carga de los datos de***/
    /**********la red a utilizar*************/

    extern const char* PARA_ENTRADA_1;
    extern const char* PARA_ENTRADA_2;
    extern const char* PARA_ENTRADA_3;
    extern const char* PARA_ENTRADA_4;
    extern const char* PARA_ENTRADA_5;
    extern const char* PARA_ENTRADA_6;
    extern const char* PARA_ENTRADA_7;
    
    //Declaración de usuario y clave de red WiFi.
    extern WiFiMulti wifiMulti;

    extern unsigned long previousMillis;
    const long interval = 30000;  // interval to wait for Wi-Fi connection (milliseconds)

    extern String ssid;
    extern String pass;
    extern String ip;
    extern String gateway;

    /*****Habilitador de impresión en********/
    /*************modo debug*****************/

    extern bool habDebug;

    //Definición del usuario, clave y estado de acceso.

    extern String usu;
    extern String clav;
    extern char* usuarioHTTP;
    extern char* claveHTTP;
    extern bool eHTTP;


    /******Contador de acceso a  página /index********/
    /***********antes de introducir clave.************/

    extern bool cuentaAcceso;

    //Variable que almacenará la última página cargada.

    extern String ultimaPaginaCargada;

    //Instanciación del objeto preferencias.

    extern Preferences memoriaEstado;

    // Variables relativas al estado del sistema.

    extern const uint8_t ledPinSistemaApagado;   //Led que indica si el sistema está apagado.
    extern const uint8_t ledPinSistemaEncendido; //Led que indica si el sistema está encendido.
    extern String ledEstado;
    extern uint8_t numTerm[3];
    extern const uint8_t bombaUno;      //Salida para controlar la bomba 1.
    extern bool bomba1LED;
    extern const uint8_t bombaDos;      //Salida para controlar la bomba 2.
    extern bool bomba2LED;
    extern const uint8_t bombaTres;     //Salida para controlar la bomba 3.
    extern uint8_t bombaPin;            // Identifica el pin de la bomba actual a encender.
    extern bool modoSistema;            //modoSistema = false significa sistema en modo manual, modoSistema = true
    extern uint8_t ledModoSistema;      //Significa sistema en modo automático cuando ledModoSistema es true. Cuando
    extern bool marchaSistemaLED;       //marchaSistema = false significa sistema parado, marchaSistema = true significa

    //Declaración de terminales de entrada para el sistema.

    extern const uint8_t supervisorT;    //Esta es la entrada para el supervisor mono/trifásico.
    extern bool superT;
    extern const uint8_t nivelAgua;      //Nivel del agua será una medición analógica.
    extern uint8_t nivelA;

    //Terminal de gatillo para sensor de nivel.
    extern const int pinGatillo;

    // Terminal de interrupcion.
    extern const int pinInt;

    extern String modoEstado;

    //Declaración de las variables para el tipo de tanque y volumen mínimo.

    extern uint8_t tipoTanque;
    extern uint8_t volMax;
    extern uint8_t volMin;
    extern float radioSup;
    extern float radioInf;
    extern float alturaTanque;
    extern float profTanque;

    extern const char* dataM;

 //Estructura para almacenar la fecha a programar al sistema.

    typedef struct FechaProg {
        int dia;
        int mes;
        int anio;
        int hora;
        int minuto;
        int segundo;
        char diaSemana[20];
        char mesAnio[20];
    } FechaProg;

    // Estructura para almacenar el día y programa respectivo.
    typedef struct diaSemana {
        const char* inicioManana;
        const char* finManana;
        const char* inicioTarde;
        const char* finTarde;
        bool diaHabil;
    } diaSemana;

    // Estructura para almacenar los programas de todos los 
    // dias de la semana.
    typedef struct Semana {
        diaSemana lunes;
        diaSemana martes;
        diaSemana miercoles;
        diaSemana jueves;
        diaSemana viernes;
        diaSemana sabado;
        diaSemana domingo;
    } Semana;

       // Campo de bits para almacenar el estado del sistema

    typedef struct estadoSistema {
        bool bateria        : 1;
        bool reloj          : 1;
        bool bombaActiva    : 1;
        bool encBomba       : 1;
        uint8_t nBomba; 
        uint32_t interMed;
        StreamString nTanque;
        StreamString vTanque;
    } estadoSistema;

    typedef struct wifiConfig {
    char ssid[30];
    char clave[30];
    char ipDir[30];
    char gatewaydir[30];
    char subred[30];
    bool accesoAP;
    bool estaWiFi;
    uint8_t tEnEspera;
    char nombre_red[30];
    char claveAP[30];
    } wifiConfig;

    extern FechaProg tiempo;
    extern FechaProg tiempoLeido;
    extern Semana semana;
    extern estadoSistema sistema;
#endif