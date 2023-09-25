#ifndef VARSISTEMA_H
#define VARSISTEMA_H
    #include <WiFiMulti.h>
    #include <preferences.h>

    //Declaración de usuario y clave de red WiFi.
    WiFiMulti wifiMulti;

    //Definición del usuario, clave y estado de acceso.

    String usu = "admin", clav = "admin";
    char* usuarioHTTP = &usu[0];
    char* claveHTTP = &clav[0];
    bool eHTTP = false;

    //Variable que almacenará la última página cargada.

    String ultimaPaginaCargada = "";

    //Instanciación del objeto preferencias.

    Preferences memoriaEstado;

    const uint8_t ledPinSistemaApagado = 2;   //Led que indica si el sistema está apagado.
    const uint8_t ledPinSistemaEncendido = 4; //Led que indica si el sistema está encendido.
    String ledEstado;
    const uint8_t bombaUno = 23;//Salida para controlar la bomba 1.
    bool bomba1LED = false;
    const uint8_t bombaDos = 22;//Salida para controlar la bomba 2.
    bool bomba2LED = false;
    bool bombaActiva = false;         //Falso significa bomba 1, verdadero significa bomba 2.
    bool modoSistema = false;         //modoSistema = false significa sistema en modo manual, modoSistema = true
    bool ledModoSistema = false;      //Significa sistema en modo automático cuando ledModoSistema es true. Cuando
    bool marchaSistemaLED = false;    //marchaSistema = false significa sistema parado, marchaSistema = true significa

    //Declaración de terminales de entrada para el sistema.

    const uint8_t supervisorT = 1;    //Esta es la entrada para el supervisor mono/trifásico.
    bool superT = false;
    const uint8_t nivelAgua = 3;      //Nivel del agua será una medición analógica.
    uint8_t nivelA = 0;

    String modoEstado;

    //Declaración de las variables para el tipo de tanque y volumen mínimo.

    uint8_t tipoTanque = 0;
    uint8_t volMax = 0;
    uint8_t volMin = 0;

    //Estructura para almacenar la fecha a programar al sistema.

    typedef struct {
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
    typedef struct {
        const char* inicioManana;
        const char* finManana;
        const char* inicioTarde;
        const char* finTarde;
    } diaSemana;

    // Estructura para almacenar los programas de todos los 
    // dias de la semana.
    typedef struct {
        diaSemana lunes;
        diaSemana martes;
        diaSemana miercoles;
        diaSemana jueves;
        diaSemana viernes;
        diaSemana sabado;
        diaSemana domingo;
    } Semana;

    //Variable para almacenar la data leída del rtc.

    //const char * dataM;

    // Campo de bits para almacenar el estado del sistema

    struct estadoSistema {
        bool bateria  : 1;
        bool reloj    : 1;
};

#endif