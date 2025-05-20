
#include "tasktimer.h"

void dados(TimerHandle_t xTimer) {
  
  QueueHandle_t xQueue = (QueueHandle_t) pvTimerGetTimerID(xTimer);
  Serial.println("Task dados rodando");

  char jsonBuffer[256];
  StaticJsonDocument<256> json_mensagem;

  json_mensagem["sensor"] = "luz";
  json_mensagem["valor"] = analogRead(4);
  json_mensagem["data"] = 20; //depois arrumo adicionando um rtc

  serializeJson(json_mensagem, jsonBuffer);

  xQueueSend(xQueue, jsonBuffer, 0);
}
