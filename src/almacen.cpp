#include "almacen.h"


void Inicia_Almacen(void)
{
    memoriaEstado.begin("estado_sistema", false);
    memoriaEstado.clear();
    return;
}

