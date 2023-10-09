#ifndef ALMACEN_H
#define ALMACEN_H

#include <EEPROM.h>

extern EEPROMClass eeprom;

void Inicia_Almacen(void);

void Inicia_EEPROM(void);

//wifiConfig Lee_EEPROM(void);

//void Escribe_EEPROM(uint32_t dir);

#endif