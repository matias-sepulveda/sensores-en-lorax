/*
 NodeMCU Uso del modo DEEP-SLEEP por DaniBits www.ESPloradores.com
 Enciende un LED durante 1 segundo y lo mantiene apagado 5 segundos dejando el microprocesador 
 en modo DEEP-SLEEP. Al no requerirse uso de red WiFi no se habilita cuando se enciende el 
 microprocesador (WAKE_RF_DISABLED).
 
 IMPORTANTE: Es necesario desconectar el cable GPIO16(D0)-RST para subir el sketch a la placa y 
 reconectarlo para que funcione su ejecución.
 
 Este código de ejemplo es de público dominio.
 */
 
#define LED 25                      // Pin LED GPIO02 (D4)
uint32_t TIEMPO_DeepSleep = 5e6;  // Tiempo en modo deep-sleep en microsegundos
uint8_t TIEMPO_Referencia;        // Tiempo de referencia para encender 1 segundo el LED 
 
void setup() {                          
  pinMode(LED, OUTPUT);           // Inicializa el LED_aparpadear como una salida
  digitalWrite(LED, HIGH);        // Enciende el LED                           
  TIEMPO_Referencia = millis();
  }
 
void loop() {
  if (millis()-TIEMPO_Referencia > 1000){
    //ESP.deepSleep(TIEMPO_DeepSleep, WAKE_RF_DEFAULT); // Calibración de señal de radio si es necesario 
    //ESP.deepSleep(TIEMPO_DeepSleep, WAKE_RFCAL);      // Calibración de señal de radio siempre
    //ESP.deepSleep(TIEMPO_DeepSleep, WAKE_NO_RFCAL);   // Sin calibración de la señal de radio
    ESP.deepSleep(TIEMPO_DeepSleep, WAKE_RF_DISABLED);  // Desabilita la señal de radio después del reencendido
    }
  }
