#include "interrupcion.h"
#include "varSistema.h"

float profundidad(uint32_t instante)
{
    if((tiempoUno == 0) && (tiempoDos == 0))
    {
        tiempoUno = instante;
        return 0.0;
    }
    if((tiempoUno != 0) && (tiempoDos == 0))
    {
        tiempoDos = instante;
        float resultado = (float) ((tiempoDos - tiempoUno)*340/2);
        tiempoUno = 0;
        tiempoDos = 0;
        return resultado;
    }
    return 0.0;
}