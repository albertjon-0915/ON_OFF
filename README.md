# 🟢 ONOFF Library for Arduino / ESP32

A lightweight, flexible helper class for controlling **relays**, **LEDs**, or any **digital on/off devices** with optional **active-low logic** and **debug serial logging**.

---

## Features
- Simple **ON**, **OFF**, and **TOGGLE** control functions  
- Supports both **active-high** and **active-low** outputs  
- Configurable **debug messages** via Serial  
- Two ways to initialize:
  - Using parameters
  - Using a configuration struct (`Params_onoff`)
- Safe reinitialization using `init()` methods
- Works with **Arduino**, **ESP32**, **ESP8266**, and similar boards

---

## Installation

Download the zip file from the repository by selecting Code dropdown at the top of repository, select Download ZIP.

Click the green Code button → Download ZIP.

In the Arduino IDE, go to Sketch → Include Library → Add .ZIP Library.

Select the downloaded ZIP file.

Once installed, you can include it in your sketch:

```cpp
#include <ONOFF.h>
```


## Configuration Struct
The library uses a simple struct for configuration:

```cpp
struct Params_onoff {
  uint8_t pin;          // GPIO pin number
  bool startState;      // initial relay state
  bool activeLow;       // true if relay is active LOW
  bool debug;           // enable serial debug messages
};
```
You can initialize it using designated initializers (C++17+) or standard positional syntax (C++11) depending on your board.

## ⚙️ Board Compatibility Notes
### For ESP32 / ESP8266 / Modern Boards

These use a newer C++ standard (C++17 or later), so you can safely use designated initializers:
```cpp
Params_onoff relayParams = {
  .pin = 2,
  .startState = false,
  .activeLow = false,
  .debug = true
};

ONOFF relay(relayParams);
```

### For Arduino Nano / UNO / Mega (AVR Boards)

Older AVR compilers only support C++11, which does not allow .pin = ... designated initializers.

Instead, use manual assignment or positional initialization:
#### 🟡 Option 1 – Manual assignment:
```cpp
Params_onoff relayParams;
ONOFF relay;

void setup() {
  relayParams.pin = 2;
  relayParams.startState = false;
  relayParams.activeLow = false;
  relayParams.debug = true;

  relay.init(relayParams);
  relay.begin();
}
```

#### 🟡 Option 2 – Positional initialization:
```cpp
Params_onoff relayParams = {2, false, false, true};
```

## Usage Examples
### Example 1 – Simple Direct Initialization

```cpp
#include <ONOFF.h>

ONOFF relay(25, false, true, true);  // pin, startState, activeLow, debug

void setup() {
  relay.begin(115200);   // optional Serial.begin() + status message
}

void loop() {
  relay.on();     // turn ON
  delay(1000);
  relay.off();    // turn OFF
  delay(1000);
  relay.toggle(); // toggle state
}
```

### Example 2 – Initialization with Struct

```cpp
#include <ONOFF.h>

Params_onoff relayParams = {
  .pin = 27,
  .startState = false,
  .activeLow = false,
  .debug = true
};

ONOFF relay(relayParams);

void setup() {
  relay.begin();
}

void loop() {
  relay.toggle();
  delay(500);
}
```

### Example 3 – Deferred Initialization
 - You can create the object first and initialize it later:
```cpp
#include <ONOFF.h>

ONOFF relay;

void setup() {
  relay.init(26, false, true, true);
  relay.begin(9600);
}

void loop() {
  relay.toggle();
  delay(1000);
}
```

### Example 4 – Checking Relay Status
 - The library includes a status() method that returns the current logical state of the relay:
```cpp
#include <ONOFF.h>

ONOFF relay(2, false, false, true);

void setup() {
  relay.begin();
  relay.on();

  // statusType s = relay.status();
  // if (s == ON) code here..
  // if (s == OFF) code here..

  if (relay.status() == ON) Serial.println("Relay is ON");
  if (relay.status() == OFF) Serial.println("Relay is OFF");
}

```
The method automatically considers active-low logic, so the reported status always matches the logical state of your device (not just the pin voltage).


## Debug Messages
### If debug is set to true, the library outputs messages like:

```cpp
ONOFF library mounted
Turning ON
Turning OFF
Toggle state: ON
```

### You can change the baud rate with(optional):

```cpp
relay.begin(115200);
```


## Enum Reference
### statusType

Represents the logical state of the relay:
```cpp
enum statusType {
  ON,
  OFF
};
```
Used by:
```cpp
statusType ONOFF::status();
```
Returns the current effective relay state, considering active-low behavior.

### MessageType

Used internally for handling debug message types:
```cpp
enum MessageType {
  MSG_ON,
  MSG_OFF,
  MSG_TOGGLE,
  MSG_PIN
};
```
Used internally by the library for debug message control.

