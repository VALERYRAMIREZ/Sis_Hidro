#include "debuguear.h"

void imprimeDebug( bool habilitado, String mensaje, String valor)
{
  if(habilitado == false)
  {
    return;
  }
  if(valor == "vacio")
  {
    Serial.println(mensaje);
  }
  if(valor != "vacio")
  {
    Serial.print(mensaje);
    Serial.println(valor);
  }
  return;
}