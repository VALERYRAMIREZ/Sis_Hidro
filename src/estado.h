#ifndef _ESTADO_H_
#define _ESTADO_H_
    // Campo de bits para almacenar el estado del sistema

typedef struct {
  bool bateria  : 1;
  bool reloj    : 1;
} estadoSistema;
#endif