#include <DHT.h>

#define pin_humedad 0

#define tipo_sensor DHT11

DHT sensor(pin_humedad, tipo_sensor); //creamos un objeto de la clase DHT, y le asignamos características (pin y tipo de sensor)


void setup() {
 Serial.begin(9600); //inicializamos el puerto serial
 sensor.begin(); //inicializamos el sensor
}

void loop() {
  
  // Leemos la humedad relativa
  float humedad = sensor.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float temperatura = sensor.readTemperature();
  
  // Calcular el índice de calor en grados centígrados
  float indice_calor = sensor.computeHeatIndex(temperatura, humedad, false); //el último parámetro indica si la temperatura introducida en la
                                                                             //función está en grados celcius(false) o fahrenheit(true)
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" *C ");
  Serial.print("Índice de calor: ");
  Serial.print(indice_calor);
  Serial.println(" *C ");
}
