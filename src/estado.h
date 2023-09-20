#ifndef RELOJ_H
#define RELOJ_H
    // Campo de bits para almacenar el estado del sistema

typedef struct {
  bool bateria  : 1;
  bool reloj    : 1;
} estadoSistema;
#endif