//Coleta dados sensor e transmite
// ESP8266 433MHz transmitter test 1
#include "tasktimer.h"

RH_ASK rf_driver(500, 9, 10); // Velocidade, RX, TX

#define configTICK_RATE_HZ 1000

QueueHandle_t xQueue;
TimerHandle_t coleta_dados;

void setup() {
  Serial.begin(115200);
  pinMode(pinSensorLuz, INPUT);
  delay(5000);
  Serial.println("Fim do setup()");

  Serial.println("ESP8266 433MHz transmitter");
  if (!rf_driver.init()) {
    Serial.println("init failed");
    while (1) delay(10000);
  }
  Serial.println("Transmitter: rf_driver initialised");
  rf_driver.setModeIdle();
  rf_driver.setHeaderId(1); //quem esta enviando a mensagem e quem sou eu
  rf_driver.setHeaderTo(2); //para quem é a mensagem
  rf_driver.setHeaderFrom(1); //de onde veio a mensagem

  xQueue = xQueueCreate(50, 64); //numero de itens e tamanho de cada item para a fila
  coleta_dados = xTimerCreate(
    "dados_timer",               // nome
    pdMS_TO_TICKS(30000),         // tempo (em ticks, use pdMS_TO_TICKS para ms)
    pdTRUE,                      // auto-reload
    (void *) xQueue,             // ID (usado para passar o QueueHandle_t)
    dados                        // função callback correta
  );

  xTimerStart(coleta_dados, 0);
 

  static TransmissorParams_t params;
  params.rf_driver = &rf_driver;
  params.xQueue = xQueue;
  
  xTaskCreate(
    transmissor,                     // ponteiro para função da task
    "task_transmissor_rodando",      // nome da task (para debug)
    1000,                            // tamanho da stack (em palavras)
    (void *)&params,                 // parâmetro passado para a task (pvParameters)
    1,                               // prioridade da task (maior número = maior prioridade)
    NULL                             // handle da task (pode guardar para controle futuro)
  );
}


void loop() {
  vTaskSuspend(NULL);  // Suspende a própria task do loop() para sempre
}