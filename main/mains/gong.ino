#include <Homie.h>
#include <Automaton.h>

const int RELAY_PIN = 2;
Atm_led relay;

HomieNode relayNode("relay", "switch");

bool onTrigger(const HomieRange &range, const String &value)
{
  Serial << "\nGot something: " << value << endl;

  if (value != "true" && value != "false")
    return false;

  bool on = (value == "true");

  relay.trigger( relay.EVT_BLINK );
  //relayNode.setProperty("on").send(value);
  Homie.getLogger() << "Relay is " << (on ? "on" : "off") << endl;

  return true;
}

void setup()
{
  Serial.begin(115200);
  Serial << endl
         << endl;

  Homie_setFirmware("awesome-relay", "1.0.0");
  relayNode.advertise("trigger").settable(onTrigger);

  relay.begin(RELAY_PIN, false);
  relay.blink( 100, 100, 1 );

  Homie.setup();
}

void loop()
{
  Homie.loop();
  relay.cycle();
}

