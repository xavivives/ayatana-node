#include <Homie.h>
#include <Automaton.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
Atm_timer timer;
HomieNode airNode("AirSensor", "air");

float temperature, humidity, pressure, altitude;

void setup()
{
  Serial.begin(115200);
  Serial << endl
         << endl;

  Homie_setFirmware("airSensor", "1.0.0");
  airNode.advertise("air");

 // Wire.begin(D2, D1);
  Wire.setClock(2000000);    // Set I2C bus speed 
  Wire.begin(D2,D1); // Define which ESP8266 pins to use for SDA, SCL of the Sensor
  //bme.begin(0x76);
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }

  delay(100);

  Homie.setup();

  timer.begin(2000)
      .repeat(ATM_COUNTER_OFF)
      .onTimer(timer_callback)
      .start();
}

void loop()
{
  Homie.loop();
  automaton.run();
}

void timer_callback(int idx, int v, int up)
{
  read();
}

void read()
{
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  Serial << "\ntemperature: " << temperature
         << ", humidity: " << humidity
         << ", pressure: " << pressure
         << ", altitude: " << altitude
         << endl;
}