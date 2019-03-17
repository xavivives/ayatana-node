#include <Homie.h>
#include <Automaton.h>

const int ANALOG_IN_PIN = 0;
Atm_analog currentMeter;

HomieNode currentNode("relay", "switch");


void setup()
{
  Serial.begin(115200);
  Serial << endl
         << endl;

  Homie_setFirmware("currentMeter", "1.0.0");
  currentNode.advertise("current");
  currentMeter.begin(ANALOG_IN_PIN, 1000).onChange( onCurrentChange);


  Homie.setup();
}

void loop()
{
  Homie.loop();
  currentMeter.cycle();
}

void onCurrentChange( int idx, int v, int up ) {
   Serial << "\nReading " << v << endl;
  // Do something when the analog value changes

}