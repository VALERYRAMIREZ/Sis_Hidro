#include "reloj.h"

extern void IRAM_ATTR miTemporizador();
extern hw_timer_t *tempo;

bool Compara_RTC(const char * data, Semana tPrograma) {
  bool comparado = false;
  uint8_t manHoraIniProg = 0, manMinIniProg = 0, manHoraFinProg = 0, manMinFinProg = 0, tarHoraIniProg = 0, tarMinIniProg = 0, tarHoraFinProg = 0, tarMinFinProg = 0;
  Serial.print("Fecha completa: ");
  Serial.println(data);
  if(sscanf(data, "%2d:%2d:%2d %9[^,], %s %2d %4d", &tiempoLeido.hora, &tiempoLeido.minuto, &tiempoLeido.segundo, tiempoLeido.diaSemana, tiempoLeido.mesAnio, &tiempoLeido.dia, &tiempoLeido.anio) != 7)
  {
    Serial.println("La trama de reloj recibida es incorrecta, no se puede operar en automático.");
    comparado = false;
    return comparado;
  }
  else
  {
    if((std::string) tiempoLeido.diaSemana == "Monday")
    {
      sscanf(semana.lunes.inicioManana, "%2d:%2d", &manHoraIniProg, &manMinIniProg);
      sscanf(semana.lunes.finManana, "%2d:%2d", &manHoraFinProg, &manMinFinProg);
      sscanf(semana.lunes.inicioTarde, "%2d:%2d", &tarHoraIniProg, &tarMinIniProg);
      sscanf(semana.lunes.finTarde, "%2d:%2d", &tarHoraFinProg, &tarMinFinProg);
      Serial.println(manHoraIniProg);
      Serial.println(manMinIniProg);
      Serial.println(manHoraFinProg);
      Serial.println(manMinFinProg);
      Serial.println(tarHoraIniProg);
      Serial.println(tarMinIniProg);
      Serial.println(tarHoraFinProg);
      Serial.println(tarMinFinProg);
      if((((tiempoLeido.hora >= manHoraIniProg) && (tiempoLeido.hora <= manHoraFinProg)) && ((tiempoLeido.minuto >= manMinIniProg) && (tiempoLeido.minuto < manMinFinProg)))
        || (((tiempoLeido.hora >= tarHoraIniProg) && (tiempoLeido.hora <= tarHoraFinProg)) && ((tiempoLeido.minuto >= tarMinIniProg) && (tiempoLeido.minuto < tarMinFinProg))))
      {
        comparado = true;
      }
    }
    else if((std::string) tiempoLeido.diaSemana == "Tuesday")
    {
      sscanf(semana.martes.inicioManana, "%2d:%2d", &manHoraIniProg, &manMinIniProg);
      sscanf(semana.martes.finManana, "%2d:%2d", &manHoraFinProg, &manMinFinProg);
      sscanf(semana.martes.inicioTarde, "%2d:%2d", &tarHoraIniProg, &tarMinIniProg);
      sscanf(semana.martes.finTarde, "%2d:%2d", &tarHoraFinProg, &tarMinFinProg);
      Serial.println(manHoraIniProg);
      Serial.println(manMinIniProg);
      Serial.println(manHoraFinProg);
      Serial.println(manMinFinProg);
      Serial.println(tarHoraIniProg);
      Serial.println(tarMinIniProg);
      Serial.println(tarHoraFinProg);
      Serial.println(tarMinFinProg);
      if((((tiempoLeido.hora >= manHoraIniProg) && (tiempoLeido.hora <= manHoraFinProg)) && ((tiempoLeido.minuto >= manMinIniProg) && (tiempoLeido.minuto < manMinFinProg)))
        || (((tiempoLeido.hora >= tarHoraIniProg) && (tiempoLeido.hora <= tarHoraFinProg)) && ((tiempoLeido.minuto >= tarMinIniProg) && (tiempoLeido.minuto < tarMinFinProg))))
      {
        comparado = true;
      }
    }
    else if((std::string) tiempoLeido.diaSemana == "Wednesday")
    {
      sscanf(semana.miercoles.inicioManana, "%2d:%2d", &manHoraIniProg, &manMinIniProg);
      sscanf(semana.miercoles.finManana, "%2d:%2d", &manHoraFinProg, &manMinFinProg);
      sscanf(semana.miercoles.inicioTarde, "%2d:%2d", &tarHoraIniProg, &tarMinIniProg);
      sscanf(semana.miercoles.finTarde, "%2d:%2d", &tarHoraFinProg, &tarMinFinProg);
      Serial.println(manHoraIniProg);
      Serial.println(manMinIniProg);
      Serial.println(manHoraFinProg);
      Serial.println(manMinFinProg);
      Serial.println(tarHoraIniProg);
      Serial.println(tarMinIniProg);
      Serial.println(tarHoraFinProg);
      Serial.println(tarMinFinProg);
      
      if((((tiempoLeido.hora >= manHoraIniProg) && (tiempoLeido.hora <= manHoraFinProg)) && ((tiempoLeido.minuto >= manMinIniProg) && (tiempoLeido.minuto < manMinFinProg)))
        || (((tiempoLeido.hora >= tarHoraIniProg) && (tiempoLeido.hora <= tarHoraFinProg)) && ((tiempoLeido.minuto >= tarMinIniProg) && (tiempoLeido.minuto < tarMinFinProg))))
      {
        comparado = true;
      }
    }
    else if((std::string) tiempoLeido.diaSemana == "Thursday")
    {
      sscanf(semana.jueves.inicioManana, "%2d:%2d", &manHoraIniProg, &manMinIniProg);
      sscanf(semana.jueves.finManana, "%2d:%2d", &manHoraFinProg, &manMinFinProg);
      sscanf(semana.jueves.inicioTarde, "%2d:%2d", &tarHoraIniProg, &tarMinIniProg);
      sscanf(semana.lunes.finTarde, "%2d:%2d", &tarHoraFinProg, &tarMinFinProg);
      Serial.println(manHoraIniProg);
      Serial.println(manMinIniProg);
      Serial.println(manHoraFinProg);
      Serial.println(manMinFinProg);
      Serial.println(tarHoraIniProg);
      Serial.println(tarMinIniProg);
      Serial.println(tarHoraFinProg);
      Serial.println(tarMinFinProg);
      if((((tiempoLeido.hora >= manHoraIniProg) && (tiempoLeido.hora <= manHoraFinProg)) && ((tiempoLeido.minuto >= manMinIniProg) && (tiempoLeido.minuto < manMinFinProg)))
        || (((tiempoLeido.hora >= tarHoraIniProg) && (tiempoLeido.hora <= tarHoraFinProg)) && ((tiempoLeido.minuto >= tarMinIniProg) && (tiempoLeido.minuto < tarMinFinProg))))
      {
        comparado = true;
      }
    }
    else if((std::string) tiempoLeido.diaSemana == "Friday")
    {
      sscanf(semana.viernes.inicioManana, "%2d:%2d", &manHoraIniProg, &manMinIniProg);
      sscanf(semana.viernes.finManana, "%2d:%2d", &manHoraFinProg, &manMinFinProg);
      sscanf(semana.viernes.inicioTarde, "%2d:%2d", &tarHoraIniProg, &tarMinIniProg);
      sscanf(semana.lunes.finTarde, "%2d:%2d", &tarHoraFinProg, &tarMinFinProg);
      Serial.println(manHoraIniProg);
      Serial.println(manMinIniProg);
      Serial.println(manHoraFinProg);
      Serial.println(manMinFinProg);
      Serial.println(tarHoraIniProg);
      Serial.println(tarMinIniProg);
      Serial.println(tarHoraFinProg);
      Serial.println(tarMinFinProg);
      if((((tiempoLeido.hora >= manHoraIniProg) && (tiempoLeido.hora <= manHoraFinProg)) && ((tiempoLeido.minuto >= manMinIniProg) && (tiempoLeido.minuto < manMinFinProg)))
        || (((tiempoLeido.hora >= tarHoraIniProg) && (tiempoLeido.hora <= tarHoraFinProg)) && ((tiempoLeido.minuto >= tarMinIniProg) && (tiempoLeido.minuto < tarMinFinProg))))
      {
        comparado = true;
      }
    }
    else if((std::string) tiempoLeido.diaSemana == "Saturday")
    {
      sscanf(semana.sabado.inicioManana, "%2d:%2d", &manHoraIniProg, &manMinIniProg);
      sscanf(semana.sabado.finManana, "%2d:%2d", &manHoraFinProg, &manMinFinProg);
      sscanf(semana.sabado.inicioTarde, "%2d:%2d", &tarHoraIniProg, &tarMinIniProg);
      sscanf(semana.sabado.finTarde, "%2d:%2d", &tarHoraFinProg, &tarMinFinProg);
      Serial.println(manHoraIniProg);
      Serial.println(manMinIniProg);
      Serial.println(manHoraFinProg);
      Serial.println(manMinFinProg);
      Serial.println(tarHoraIniProg);
      Serial.println(tarMinIniProg);
      Serial.println(tarHoraFinProg);
      Serial.println(tarMinFinProg);
      if((((tiempoLeido.hora >= manHoraIniProg) && (tiempoLeido.hora <= manHoraFinProg)) && ((tiempoLeido.minuto >= manMinIniProg) && (tiempoLeido.minuto < manMinFinProg)))
        || (((tiempoLeido.hora >= tarHoraIniProg) && (tiempoLeido.hora <= tarHoraFinProg)) && ((tiempoLeido.minuto >= tarMinIniProg) && (tiempoLeido.minuto < tarMinFinProg))))
      {
        comparado = true;
      }
    }
    else if((std::string) tiempoLeido.diaSemana == "Sunday")
    {
      sscanf(semana.domingo.inicioManana, "%2d:%2d", &manHoraIniProg, &manMinIniProg);
      sscanf(semana.domingo.finManana, "%2d:%2d", &manHoraFinProg, &manMinFinProg);
      sscanf(semana.domingo.inicioTarde, "%2d:%2d", &tarHoraIniProg, &tarMinIniProg);
      sscanf(semana.domingo.finTarde, "%2d:%2d", &tarHoraFinProg, &tarMinFinProg);
      Serial.println(semana.domingo.finTarde);
      Serial.println(manHoraIniProg);
      Serial.println(manMinIniProg);
      Serial.println(manHoraFinProg);
      Serial.println(manMinFinProg);
      Serial.println(tarHoraIniProg);
      Serial.println(tarMinIniProg);
      Serial.println(tarHoraFinProg);
      Serial.println(tarMinFinProg);
      if((((tiempoLeido.hora >= manHoraIniProg) && (tiempoLeido.hora <= manHoraFinProg)) && ((tiempoLeido.minuto >= manMinIniProg) && (tiempoLeido.minuto < manMinFinProg)))
        || (((tiempoLeido.hora >= tarHoraIniProg) && (tiempoLeido.hora <= tarHoraFinProg)) && ((tiempoLeido.minuto >= tarMinIniProg) && (tiempoLeido.minuto < tarMinFinProg))))
      {
        comparado = true;
      }
    }
    else
    {
      Serial.println("No se encontró el día de programación");
      comparado = false;
    }
  }
  Serial.print("Comparado: ");
  Serial.println(comparado);
  return comparado;
};

void Enciende_Temporizador(uint8_t tiempo)
{
  tempo = timerBegin(0, 80, true);
  timerAttachInterrupt(tempo, &miTemporizador, true);
  timerAlarmWrite(tempo, 1000000*tiempo, true);
  timerAlarmEnable(tempo);
};

void Apaga_Temporizador(void)
{
  timerAlarmDisable(tempo);
}

