#include <Homie.h>
//#include <Automaton.h>
//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>

//#define SEALEVELPRESSURE_HPA (1013.25)

//Adafruit_BME280 bme;
//Atm_timer timer;
const int TEMPERATURE_INTERVAL = 5000;
unsigned long lastTemperatureSent = 0;

HomieNode airNode("BME280", "airSensor");

float temperature, humidity, pressure, altitude;

void setup()
{
  Serial.begin(115200);
  Serial << endl
         << endl;

  Homie_setFirmware("airSensor", "1.0.0");
  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);

  /*
  Wire.begin(D6,D5); // Define which ESP8266 pins to use for SDA, SCL of the Sensor
  Wire.setClock(100000);    // Set I2C bus speed 
  
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
  */

  //delay(100);

  /*timer.begin(2000)
      .repeat(ATM_COUNTER_OFF)
      .onTimer(timer_callback)
      .start();
*/
  Homie.setup();
}

void setupHandler()
{
  airNode.advertise("air");
}

void loopHandler()
{
  if (millis() - lastTemperatureSent >= TEMPERATURE_INTERVAL * 1000UL || lastTemperatureSent == 0) {
    read();
    lastTemperatureSent = millis();
  }
}

void loop()
{
  Homie.loop();
  //automaton.run();
}

//void timer_callback(int idx, int v, int up)
//{
  //read();
//}

void read()
{
  //temperature = bme.readTemperature();
  //humidity = bme.readHumidity();
  //pressure = bme.readPressure() / 100.0F;
  //altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  temperature = 1.0;

  /*Serial << "\ntemperature: " << temperature
         // << ", humidity: " << humidity
         // << ", pressure: " << pressure
         // << ", altitude: " << altitude
         << endl;
         */

  Homie.getLogger() << "Temperature: " << temperature << " °C" << endl;
  airNode.setProperty("temperature").send(String(temperature));
  //airNode.setProperty("humidity").send(String(humidity));
  //airNode.setProperty("pressure").send(String(pressure));
}