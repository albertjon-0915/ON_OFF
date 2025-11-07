#ifndef ONOFF_H
#define ONOFF_H

#include <Arduino.h>

// declare the struct on setup
struct Params_onoff {
  uint8_t pin;
  bool startState = false;
  bool activeLow = false;
  bool debug = false;
};

enum statusType {
  ON,
  OFF
};

enum MessageType {
  MSG_ON,
  MSG_OFF,
  MSG_TOGGLE,
  MSG_PIN
};

class ONOFF {
public:
  ONOFF();
  ONOFF(uint8_t pin, bool startState = false, bool activeLow = false, bool debug = false);
  ONOFF(const Params_onoff& params);

  void init(uint8_t pin, bool startState = false, bool activeLow = false, bool debug = false);  // initialize later
  void init(const Params_onoff& params);

  void on(bool canTurnOn = true);
  void off(bool canTurnOff = true);
  void toggle(bool canToggle = true);
  void begin(int baud = 9600);
  statusType status();

private:
  uint8_t relayPin;
  bool relayState;
  bool activeLow;
  bool initialized;
  bool debug;

  void debugMessage(MessageType message);
};

// Default constructor implementation
inline ONOFF::ONOFF()
  : relayPin(255), relayState(false), activeLow(false), initialized(false), debug(false) {}

// Constructor with parameters
inline ONOFF::ONOFF(uint8_t pin, bool relayState, bool activeLow, bool debug)
  // initializers ---> no need for manual set up of this->debug = debug;, etc...
  : relayPin(pin), relayState(relayState), activeLow(activeLow), initialized(true), debug(debug) {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, (relayState ^ activeLow) ? HIGH : LOW);

  debugMessage(MSG_PIN);
}

// Constructor from struct params (delegates)
inline ONOFF::ONOFF(const Params_onoff& params)
  : ONOFF(params.pin, params.startState, params.activeLow, params.debug) {
  initialized = true;
}

// Init function with parameters (for later initialization)
inline void ONOFF::init(uint8_t pin, bool startState, bool activeLow, bool debug) {
  relayPin = pin;
  this->activeLow = activeLow;
  this->debug = debug;
  relayState = startState;
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, (relayState ^ activeLow) ? HIGH : LOW);
  initialized = true;
}

// Init function from struct params
inline void ONOFF::init(const Params_onoff& params) {
  init(params.pin, params.startState, params.activeLow, params.debug);
  initialized = true;
}

inline void ONOFF::begin(int baud) {
  Serial.begin(baud);
  
  if (initialized) {
    relayState = digitalRead(relayPin);
    Serial.println("ONOFF library mounted");
  } else {
    Serial.println("ONOFF not initialized!");
  }
}

inline void ONOFF::on(bool canTurnOn) {
  debugMessage(activeLow ? MSG_OFF : MSG_ON);

  if (canTurnOn && !relayState) {
    digitalWrite(relayPin, activeLow ? LOW : HIGH);
    relayState = activeLow ? false : true;
  }
}

inline void ONOFF::off(bool canTurnOff) {
  debugMessage(activeLow ? MSG_ON : MSG_OFF);

  if (canTurnOff && relayState) {
    digitalWrite(relayPin, activeLow ? HIGH : LOW);
    relayState = activeLow ? true : false;
  }
}

inline void ONOFF::toggle(bool canToggle) {
  if (!canToggle) return;

  debugMessage(MSG_TOGGLE);

  relayState = !relayState;
  // XOR conditions for inverting toggle when activeLow
  // digitalWrite(relayPin, (relayState && !activeLow) ? HIGH : LOW) ---> almost same as this
  digitalWrite(relayPin, (relayState ^ activeLow) ? HIGH : LOW);
}

inline statusType ONOFF::status() {
  return relayState ? ON : OFF;
}

inline void ONOFF::debugMessage(MessageType message) {
  if (!debug) return;
  char buf[32];
  sprintf(buf, "PIN NO: %d --->  ", relayPin);
  Serial.print(buf);


  switch (message) {
    case MSG_ON:
      Serial.println("Turning ON");
      break;
    case MSG_OFF:
      Serial.println("Turning OFF");
      break;
    case MSG_TOGGLE:
      Serial.print("Toggle state: ");
      Serial.println((relayState ^ activeLow) ? "OFF" : "ON");
      break;
    case MSG_PIN:
      Serial.println(digitalRead(relayPin));
      break;
  }
}

#endif
