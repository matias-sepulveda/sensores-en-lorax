/* Requiere las siguientes librerías:
 *  
 *  -DHT sensor library para el sensor de humedad
 *  -DallasTemperature y OneWire para el sensor de temperatura
 *  -U8g2 para la pantalla
 */
 
#include <U8x8lib.h> 
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//sensor de temperatura
#define pin_temperatura 22

//sensor de humedad
#define pin_humedad 0
#define tipo_sensor DHT11

#define factor 1000000 //Factor de conversión de segundos a microsegundos

DHT sensor_humedad(pin_humedad, tipo_sensor); //creamos un objeto de la clase DHT, y le asignamos características (pin y tipo de sensor)

U8X8_SSD1306_128X64_NONAME_SW_I2C pantalla(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16); //configurando la pantalla

OneWire bus_datos(pin_temperatura); //especificando cual es nuestro bus de datos (solo del sensor de temperatura)
DallasTemperature sensor_temperatura(&bus_datos);
  
void setup(){
  
  Serial.begin(9600);
  
  pantalla.begin(); //inicializando la pantalla 
  pantalla.setFont(u8x8_font_pxplusibmcgathin_r); //estableciendo la fuente de letras a usar
  
  sensor_humedad.begin(); //inicializamos el sensor de humedad
  sensor_temperatura.begin(); //inicializamos el sensor de temperatura

  delay(1000); //Esperando a que se abra el monitor serial
  
}


void loop(){
  
  sensor_temperatura.requestTemperatures();
  
  float temperatura = sensor_temperatura.getTempCByIndex(0);
  
  float humedad = sensor_humedad.readHumidity();

  //mostrando por el puerto serial                                                                         
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" *C ");

  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %\t");
  
  //mostrando por la pantalla
  pantalla.drawString(0, 0, "Datos:");
  pantalla.drawString(0, 1, "Temp: "); //temperatura
  pantalla.setCursor(6, 1);
  pantalla.print(temperatura);
  pantalla.drawString(12, 1, "C*");

  
  pantalla.drawString(0, 2, "Humedad: ");
  pantalla.setCursor(9, 2);
  pantalla.print(humedad);
  pantalla.drawString(13, 2, " %");
  
  delay(1000); //delay para poder leer los datos de la pantalla
}
