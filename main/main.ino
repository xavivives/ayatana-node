#include <Homie.h>

const int RELAY_PIN = 2;

HomieNode relayNode("relay", "switch");

bool onRelay(const HomieRange &range, const String &value)
{
  Serial << "\nGot something: " << value << endl;

  if (value != "true" && value != "false")
    return false;

  bool on = (value == "true");
  digitalWrite(RELAY_PIN, on ? HIGH : LOW);
  relayNode.setProperty("on").send(value);
  Homie.getLogger() << "Relay is " << (on ? "on" : "off") << endl;

  return true;
}

void setup()
{
  Serial.begin(115200);
  Serial << endl
         << endl;
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  Homie_setFirmware("awesome-relay", "1.0.0");

  relayNode.advertise("on").settable(onRelay);

  Homie.setup();
}

void loop()
{
  Homie.loop();
}