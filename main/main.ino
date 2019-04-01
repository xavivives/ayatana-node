#include <Homie.h>
#include <Automaton.h>

//ADC_MODE(ADC_VCC); Triggers wifi disconnect event?

const int ANALOG_IN_PIN = 0;
Atm_analog currentMeter;

const float vcc = 3.15;
const float z1 = 178000;
const float z2 = 270000;
const float readOffset = -7;
const float sensorOffset = 0.45;

HomieNode currentNode("relay", "switch");

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

float tensionDivider(float v, float z1, float z2)
{
  return z2 / (z1 + z2) * v;
}

float correctReadOffset(float v)
{
  return v + readOffset;
}

float readToVolts(float v)
{
  return v * 3.3 / 1023;
}

float correctSensorOffset(float v)
{
  return v + sensorOffset;
}

float tensionMultiplier(float v, float z1, float z2)
{
  return v / (z2 / (z1 + z2));
}

float tensionToCurrent (float v)
{
  return (v-2.5)/(66/1000);
}

void onCurrentChange(int idx, int v, int up)
{

  //float v1 = correctReadOffset(v);
  //float v2 = readToVolts(v1);
  //float v3 = tensionMultiplier(v2, z1, z2);
  //float v4 = tensionToCurrent(v3);

  //float vcc = ((float)ESP.getVcc())/1024;
  float zeroCurrentVoltage = 5/2; //Get real tension

  float cleanRead = v + readOffset;
  float sensedVoltage = cleanRead * vcc / 1024; //digital read to volts
  float tensionMultiplier = sensedVoltage / (z2 / (z1 + z2));
  float difference = tensionMultiplier - zeroCurrentVoltage;
  float tensionToCurrent = difference/(66/1000);
  

  Serial << "\nRead: " << v
    << ", NoOffset: " << cleanRead
    << ", sensedVoltage: "<< sensedVoltage
    << ", TensionMultiplier: " << tensionMultiplier
    <<", Current: "<< tensionToCurrent
    <<", Vcc: "<< vcc
    << endl;

  // Do something when the analog value changes
}