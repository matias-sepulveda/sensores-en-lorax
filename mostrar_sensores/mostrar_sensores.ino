#include <DHT.h>
#include <U8x8lib.h>

//sensor de humedad
#define pin_humedad 0
#define tipo_sensor DHT11

//pantalla (modelo), creamos un objeto y le asignamos características (sus pines)
U8X8_SSD1306_128X64_NONAME_SW_I2C pantalla(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);


DHT sensor(pin_humedad, tipo_sensor); //creamos un objeto de la clase DHT, y le asignamos características (pin y tipo de sensor)

void setup() {
 Serial.begin(9600); //inicializamos el puerto serial
 sensor.begin(); //inicializamos el sensor
 
 pantalla.begin(); //inicializando la pantalla 
 pantalla.setFont(u8x8_font_pxplusibmcgathin_r); //estableciendo la fuente de letras a usar
}

void loop() {
  
  // Leemos la humedad relativa
  float humedad = sensor.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float temperatura = sensor.readTemperature();
  
  // Calcular el índice de calor en grados centígrados
  float indice_calor = sensor.computeHeatIndex(temperatura, humedad, false); //el último parámetro indica si la temperatura introducida en la
                                                                             //función está en grados celcius(false) o fahrenheit(true)
  //mostrando por el puerto serial                                                                         
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" *C ");
  Serial.print("Índice de calor: ");
  Serial.print(indice_calor);
  Serial.println(" *C ");

  //mostrando por la pantalla
  pantalla.drawString(0, 0, "Datos:");
  pantalla.drawString(0, 2, "Temp: "); //temperatura
  pantalla.setCursor(6, 2);
  pantalla.print(temperatura);
  pantalla.drawString(12, 2, "C*");
  
  pantalla.drawString(0, 3, "Hum: "); //humedad
  pantalla.setCursor(5, 3);
  pantalla.print(humedad);
  pantalla.drawString(9, 3, " %");

  pantalla.drawString(0, 4, "Temp.ap: "); //humedad
  pantalla.setCursor(8, 4);
  pantalla.print(indice_calor);
  pantalla.drawString(13, 4, " C*");
}
