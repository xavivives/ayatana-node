#include <Homie.h>
#include <Automaton.h>

//ADC_MODE(ADC_VCC); Triggers wifi disconnect event?

const int ANALOG_IN_PIN = 0;
Atm_analog currentMeter;

const float vcc = 3.15;
//const float z1 = 178800;
//const float z2 = 265000;
const float z1 = 18.2;
const float z2 = 26.8;
const float readOffset = -8;
const float sensorOffset = -0.72;

HomieNode currentNode("CurrentSensor", "current");

void setup()
{
  Serial.begin(115200);
  Serial << endl
         << endl;

  Homie_setFirmware("currentMeter", "1.0.0");
  currentNode.advertise("current");
  currentMeter.begin(ANALOG_IN_PIN, 1000).onChange(onCurrentChange);

  Homie.setup();
}

void loop()
{
  Homie.loop();
  currentMeter.cycle();
}

void onCurrentChange(int idx, int v, int up)
{
  //float vcc = ((float)ESP.getVcc())/1024;
  float zeroCurrentVoltage = 4.68/2; //Get real tension

  float cleanRead = v + readOffset;
  float readVoltage = cleanRead * vcc / 1024; //digital read to volts
  float sensorOutputVoltage = readVoltage / (z2 / (z1 + z2));
  //float cleanSensed = sensorOutputVoltage + sensorOffset;
  float difference = sensorOutputVoltage - zeroCurrentVoltage;
  float tensionToCurrent = difference/(66.0/1000);
  
  Serial << "\nRead: " << v
    << ", NoOffset: " << cleanRead
    << ", readVoltage: "<< readVoltage
    << ", sensorOutputVoltage: " << sensorOutputVoltage
    //<< ", CleanSensed: "<< cleanSensed
    << ", Difference: " << difference
    <<", Current: "<< tensionToCurrent
    << endl;
}