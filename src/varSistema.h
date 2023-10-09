#ifndef VARSISTEMA_H
#define VARSISTEMA_H
    #include <WiFiMulti.h>
    #include <preferences.h>

    #define AP_ssid           "CONTROL-WIFI-RED"
    #define AP_clave          "administrador"
    
    //Declaración de usuario y clave de red WiFi.
    extern WiFiMulti wifiMulti;

    unsigned long previousMillis = 0;
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

    //Variable que almacenará la última página cargada.

    extern String ultimaPaginaCargada;

    //Instanciación del objeto preferencias.

    extern Preferences memoriaEstado;

    extern const uint8_t ledPinSistemaApagado;   //Led que indica si el sistema está apagado.
    extern const uint8_t ledPinSistemaEncendido; //Led que indica si el sistema está encendido.
    extern String ledEstado;
    extern const uint8_t bombaUno;//Salida para controlar la bomba 1.
    extern bool bomba1LED;
    extern const uint8_t bombaDos;//Salida para controlar la bomba 2.
    extern bool bomba2LED;
    extern bool bombaActiva;         //Falso significa bomba 1, verdadero significa bomba 2.
    extern bool modoSistema;         //modoSistema = false significa sistema en modo manual, modoSistema = true
    extern bool ledModoSistema;      //Significa sistema en modo automático cuando ledModoSistema es true. Cuando
    extern bool marchaSistemaLED;    //marchaSistema = false significa sistema parado, marchaSistema = true significa

    //Declaración de terminales de entrada para el sistema.

    extern const uint8_t supervisorT;    //Esta es la entrada para el supervisor mono/trifásico.
    extern bool superT;
    extern const uint8_t nivelAgua;      //Nivel del agua será una medición analógica.
    extern uint8_t nivelA;

    extern String modoEstado;

    //Declaración de las variables para el tipo de tanque y volumen mínimo.

    extern uint8_t tipoTanque;
    extern uint8_t volMax;
    extern uint8_t volMin;

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
        bool bateria  : 1;
        bool reloj    : 1;
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