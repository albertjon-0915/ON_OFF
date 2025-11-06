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

```
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
```
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
```
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
```
Params_onoff relayParams = {2, false, false, true};
```

## Usage Examples
### Example 1 – Simple Direct Initialization

```
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

```
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
```
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

## Debug Messages
### If debug is set to true, the library outputs messages like:

```
ONOFF library mounted
Turning ON
Turning OFF
Toggle state: ON
```

### You can change the baud rate with(optional):

```
relay.begin(115200);
```


## Enum Reference

```
enum MessageType {
  ON,
  OFF,
  TOGGLE,
  PIN
};
```
Used internally by the library for debug message control.

