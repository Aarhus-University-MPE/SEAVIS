
/*
    Relay Control for INC
    https://github.com/Aarhus-University-MPE/Ice-Nucleation-Controller

    By
    Mads Rosenhoej Jeppesen - Aarhus 2019
    mrj@mpe.au.dk
*/

//--------------------------------------//
//              Initialize              //
//--------------------------------------//

// Request ID's
#define CMD_COOL       101  // Switch to Relays to Cooling (Relay1 & 2)
#define CMD_HEAT       102  // Switch to Relays to Heating (Relay3 & 4)
#define CMD_OFF        103  // Switch to Relays to No Output (All relays off)
#define CMD_REPEAT     104  // Reset timeout period

#define RELAY_PIN_HEAT 7
#define RELAY_PIN_COOL 4

bool active               = false;
unsigned long activeStart = 0;

const long TIMEOUT            = 60000;  // Timeout period in ms
const long TIMEOUT_DELAY      = 15000;  // Timeout period in ms
unsigned long lastTimeoutSent = 0;

int lastCommand = 0;

//--------------------------------------//
//                SETUP                 //
//--------------------------------------//

void setup() {
  digitalWrite(RELAY_PIN_COOL, LOW);
  digitalWrite(RELAY_PIN_HEAT, LOW);
  pinMode(RELAY_PIN_COOL, OUTPUT);
  pinMode(RELAY_PIN_HEAT, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect.
  }
}

//--------------------------------------//
//                MAIN                  //
//--------------------------------------//

void loop() {
  //------------------Commands------------------//
  //--------------------------------------------//

  if (Serial.available()) {
    int CMD = Serial.parseInt();

    switch (CMD) {
      // Cooling Relays
      case CMD_COOL:
        lastCommand = CMD;
        digitalWrite(RELAY_PIN_HEAT, LOW);
        delay(10);
        digitalWrite(RELAY_PIN_COOL, HIGH);
        activeStart = millis();
        active      = true;
        Serial.println("OK");
        break;

      // Heating Relays
      case CMD_HEAT:
        lastCommand = CMD;
        digitalWrite(RELAY_PIN_COOL, LOW);
        delay(10);
        digitalWrite(RELAY_PIN_HEAT, HIGH);
        active      = true;
        activeStart = millis();
        Serial.println("OK");
        break;

      // Disable Relays
      case CMD_OFF:
        digitalWrite(RELAY_PIN_COOL, LOW);
        digitalWrite(RELAY_PIN_HEAT, LOW);
        active = false;
        Serial.println("OK");
        break;

      // Reset timeout period
      case CMD_REPEAT:
        activeStart = millis();
        Serial.println("OK");
        break;

      default:
        break;
    }
  }

  // Send timeout message
  if (active && millis() - activeStart > TIMEOUT) {
    if (millis() - lastTimeoutSent > 2000) {
      lastTimeoutSent = millis();
      Serial.println("TIMEOUT");
    }
  }

  // Timeout
  if (active && millis() - activeStart > TIMEOUT + TIMEOUT_DELAY) {
    digitalWrite(RELAY_PIN_HEAT, LOW);
    digitalWrite(RELAY_PIN_COOL, LOW);
    active = false;
  }
}
