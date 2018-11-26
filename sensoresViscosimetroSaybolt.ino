#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ---------------------------------------------------------------------------------
// |
// |    SENSORES PARA AUTOMATIZACION DE VISCOSIMETRO SAYBOLT
// |    
// |    Este proyecto fue creado para automatizar la toma de temperatura,
// |    medir el tiempo de cuanto tarda en caer el aceite del dispositivo
// |    al recipiente y avisar en caso de que la temperatura sobrepase
// |    el limite indicado, todo esto con la finalidad de obtener la calidad
// |    de viscosidad del aceite generado.
// |    
// |    Proyecto realizado con la finalidad de apoyar a compañeros de la
// |    carrera de quimica en CUCEI.
// |    
// |    MODULOS/SENSORES UTILIZADOS:
// |     - Sensor de temperatura sumergible DS18B20.
// |     - Modulo laser KY-008.
// |     - LCD 16x2 con i2c.
// |     - Fotoresistencia.
// |    
// |    LIBRERIAS NECESARIAS POR INSTALAR:
// |     - LiquidCrystal_I2C-master.zip (Incluido en el repositorio).
// |     - OneWire (Disponible en el gestor de librerias del IDE).
// |     - DallasTemperature (Disponible en el gestor de librerias del IDE).
// |
// ---------------------------------------------------------------------------------

#define pinDS 2 //INPUT de sensor ds18b20
#define pinLASER 4 //OUTPUT laser
#define pinFR A0 //INPUT fotoresistencia

float duracion, distancia;
bool detecto = false;
int tiempoInicio = 0;
int tiempoActual = 0;
int tiempoTranscurrido = 0;

OneWire ourWire(pinDS);
DallasTemperature sensors(&ourWire);

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x3F, 16, 2); //

void setup() {
  Serial.begin(9600);
  
  // Inicializar el LCD
  lcd.init();

  //Encender la luz de fondo.
  lcd.backlight();

  //Iniciamos el sensor DS
  sensors.begin();

  //Iniciamos el LASER
  pinMode(pinLASER, OUTPUT);
  digitalWrite(pinLASER, HIGH);

}

void loop() {
  lcd.clear();

  int resFR = analogRead(pinFR);
  Serial.println(analogRead(resFR));

  if(resFR<700){
    if(detecto==false){tiempoInicio = millis() / 1000; detecto = true;}
    tiempoActual = millis() / 1000;
  } else if(detecto){
    detecto = false;
  }

  tiempoTranscurrido = tiempoActual - tiempoInicio;

  sensors.requestTemperatures(); //Obtenemos la temperatura

  // Cursor en la primera posición de la primera fila
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(sensors.getTempCByIndex(0));
  lcd.print(" C");

  // Cursor en la primera posición de la 2° fila
  lcd.setCursor(0, 1);
  lcd.print("Tiempo: ");
  lcd.print(tiempoTranscurrido);
  lcd.print("s ");
  lcd.print(int(resFR));

  delay(200);

}
