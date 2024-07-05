#include "almacen.h"
#include "varSistema.h"

EEPROMClass eeprom;
extern wifiConfig wifi_usuario;
extern wifiConfig wifi_usuario_leida;

void Inicia_Almacen(void)
{
    memoriaEstado.begin("estado_sistema", false);
    memoriaEstado.clear();
    return;
}

void Inicia_EEPROM(void)
{
    //eeprom.begin(sizeof(struct wifiConfig));
    eeprom.begin(sizeof(struct wifiConfig) + sizeof(struct estadoSistema));
    eeprom.get(0, wifi_usuario);
    eeprom.get(sizeof(struct wifiConfig) + 1, sistema);
    return;
}

/*wifiConfig Lee_EEPROM(uint32_t dir)
{
    return eeprom.get(dir, wifi_usuario_leida);
}

void Escribe_EEPROM(uint32_t dir)
{
    wifiConfig dato = wifi_usuario;
    eeprom.put(dir, dato);
    eeprom.commit();
}*/



