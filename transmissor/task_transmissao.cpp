#include "tasktimer.h"



void transmissor(void *pvParameters) {
  TransmissorParams_t *params = (TransmissorParams_t *)pvParameters;
  RH_ASK &rf_driver=*params->rf_driver;
  QueueHandle_t xQueue = params->xQueue;
  Serial.println("Task transmissao rodando");
  char valor_transmitindo[256];
  int count = 0;
  
  while (true) {
    if (xQueueReceive(xQueue, &valor_transmitindo, portMAX_DELAY) == pdPASS) { //fica em blocked até ter algum dado na fila
      bool ackReceived = false;
      while(!ackReceived) {
        rf_driver.send((uint8_t *)valor_transmitindo, strlen(valor_transmitindo) + 1);
        rf_driver.waitPacketSent();
        Serial.println("Mensagem enviada");

        // Espera pelo ACK por até 500ms
        uint8_t buf[10];
        uint8_t buflen = sizeof(buf);
        unsigned long start = millis();

        while (millis() - start < 500) {
          if (rf_driver.recv(buf, &buflen)) {
            buf[buflen] = '\0'; // garantir fim de string
            if (strcmp((char *)buf, "ACK") == 0) {
              Serial.println("ACK recebido!");
              ackReceived = true;
              break;
            }
          }
        }

        if (!ackReceived) {
          Serial.println("ACK não recebido. Reenviar...");
          // Você pode repetir a mensagem aqui se quiser
          count++;
        }
        if (count > 5) {
          Serial.println("Não conseguiu enviar o dado");
          break;
        }
      }
      count = 0;
    }
  }
}