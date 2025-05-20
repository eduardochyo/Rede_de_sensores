#ifndef TASKTIMER_H
#define TASKTIMER_H

#include <RH_ASK.h> 
#include <SPI.h> 
#define pinSensorLuz 4 // Definir um pino analógico adequado
#include <stdio.h>
#include <string.h> // Para usar strcat()
#include <Arduino.h>
#include "driver/ledc.h"
#include <ArduinoJson.h>

typedef struct {
  RH_ASK *rf_driver;
  QueueHandle_t xQueue;
} TransmissorParams_t;


void dados(TimerHandle_t xTimer);
//le os dados depois que um timer estourar, coloca dentro de um arquivo json, que será serializado, colocado dentro de uma queue e depois a transmissão é acordada



void transmissor(void *pvParameters);
//pega os dados dentro de uma queue, e fica enviando até o ack confirmar




#endif