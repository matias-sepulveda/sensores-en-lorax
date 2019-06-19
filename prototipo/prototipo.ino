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

#define pin 25
#define factor 1000000 //Factor de conversión de segundos a microsegundos


byte voltajeBateria = 13; //pin del que se lee el voltaje de la batería
byte pin_actLectura = 21; //pin que abre o cierra el transistor del circuito de la batería, en HIGH se cierra y el LOW se abre

RTC_DATA_ATTR int contador = 0; //contador de veces que la placa se va a hibernar, se guarda en la memoria RTC

DHT sensor_humedad(pin_humedad, tipo_sensor); //creamos un objeto de la clase DHT, y le asignamos características (pin y tipo de sensor)

U8X8_SSD1306_128X64_NONAME_SW_I2C pantalla(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16); //configurando la pantalla

OneWire bus_datos(pin_temperatura); //especificando cual es nuestro bus de datos (solo del sensor de temperatura)
DallasTemperature sensor_temperatura(&bus_datos);

//Función para saber por qué la placa salió de hibernación

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Placa despertada por señal externa usando los pines RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Placa despertada por señal externa usando RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Placa despertada por reloj"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Placa despertada por touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Placa despertada por un programa del procesador ULP"); break;
    default : Serial.printf("La placa no desperto desde deep sleep: %d\n",wakeup_reason); break;
  }
 }

//Función que suspende la placa
void suspender(int tiempo) {
  
  pantalla.setPowerSave(true); //Poniendo la pantalla en modo ahorro de energía, este modo se desactiva automáticamente al salir
                               //del modo suspensión cuando se ejecuta pantalla.begin()
  
  contador++;
  Serial.println("Arranque número: " + String(contador));
  print_wakeup_reason(); //muestra por qué salió de la suspensión

  esp_sleep_enable_timer_wakeup(tiempo * factor); //Estableciendo un mecanismo para despertar la placa (timer_wakeup)
  
  Serial.println("ESP32 configurado para suspenderse por: " + String(tiempo) + " segundos");

  Serial.println("Suspendiendo la placa...\n");
  
  Serial.flush(); //esperando a que se termine de transmitir cualquier dato pendiente a través del puerto serial

  esp_deep_sleep_start(); //buenas noches
  }

float medirBateria() {
  
  //Midiendo voltaje de la batería
  digitalWrite(pin_actLectura, LOW); //Abriendo el transistor del circuito de la batería y poder tomar una muestra
  delay(500); //esperando a que se estabilice
  float voltaje = analogRead(voltajeBateria)/495.79611650485; //Tomando una muestra del voltaje que circula por el circuito de la batería y escalandola
  digitalWrite(pin_actLectura, HIGH); //Cerrando el transistor para ahorrar energía
  
  return voltaje;
  }
  
float medirHumedad() {
  
  // Leemos la humedad relativa
  float humedad = sensor_humedad.readHumidity();
  
  return humedad;
}

float medirTemperatura() {
  
  sensor_temperatura.requestTemperatures();

  // Leemos la temperatura en grados centígrados (por defecto)
  float temperatura = sensor_temperatura.getTempCByIndex(0); //el número es para identificar los sensores de temperatura cuando hay varios conectados
  

  return temperatura;
}

void pSerial(float temperatura, float humedad, float voltaje) {

    //mostrando por el puerto serial                                                                         
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" *C ");

  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %\t");

  Serial.print("Voltaje: ");
  Serial.print(voltaje);
  Serial.println(" v");

  }

void mostrarPantalla(float temperatura, float humedad, float voltaje) {
  
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
  
  pantalla.drawString(0, 3, "Bateria: ");
  pantalla.setCursor(10, 3);
  pantalla.print(voltaje);
  pantalla.drawString(14, 3, "v");

  }
  
void setup(){

  pinMode(pin_actLectura, OUTPUT);
  pinMode(voltajeBateria, INPUT);
  Serial.begin(9600);
  pantalla.begin(); //inicializando la pantalla 
  pantalla.setFont(u8x8_font_pxplusibmcgathin_r); //estableciendo la fuente de letras a usar
  sensor_humedad.begin(); //inicializamos el sensor de humedad
  sensor_temperatura.begin(); //inicializamos el sensor de temperatura

  //haciendo que se encienda el led cuando inicia
  pinMode(pin,OUTPUT);
  digitalWrite(pin, HIGH);
  
  delay(1000); //Esperando a que se abra el monitor serial
}


void loop(){
  //El crear variables usando lo que retornan las funciones medirTemperatura, medirBateria y medirHumedad lo hice así pensando
  //en la integración con los chicos que trabajan en LORA, de ese modo pueden llegar y enviar los datos con su código

  float voltaje = medirBateria();
  float temperatura = medirTemperatura();
  float humedad = medirHumedad();

  pSerial(temperatura, humedad, voltaje); //mostrando los datos por el puerto serial
  mostrarPantalla(temperatura, humedad, voltaje); //mostrando los datos por pantalla
  
  delay(5000); //delay para poder leer los datos de la pantalla
  suspender(60); //recibe como argumento el tiempo en segundos que se quiere mantener suspendida la placa
}
