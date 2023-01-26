
//This creates a bridge between Serial and Classical Bluetooth (SPP)
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
bool BTRecieve = 0;    // Booleen variable for door switch recieved position
const int Door_Open = 11;
const int Door_Closed = 10;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);    // Configure Built in LED as Output
  pinMode(15, OUTPUT);    // Set pin GPIO 15 to Output

  Serial.begin(115200);
  SerialBT.begin("Light_Switch"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on by making the voltage HIGH
  delay(500);    // wait for 1/2 second
  
}

// the loop function runs over and over again forever
void loop() {
if (SerialBT.available()) {
  BTRecieve = (SerialBT.read());
}
Serial.println(BTRecieve);
  if (BTRecieve == 1) {
    digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on by making the voltage HIGH
    digitalWrite(15, HIGH);   // Turn GPIO pin 26 voltage HIGH
    SerialBT.write(Door_Open);    // Value when door is open
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(15, LOW);   // Turn GPIO pin 26 voltage LOW
    SerialBT.write(Door_Closed);    // Value when door is closed

  }
  delay(500);    // wait for 1/4 second

//  if (Serial.available()) {
//    SerialBT.println(Serial.readString());
//  }
//  if (SerialBT.available()) {
//    Serial.println(SerialBT.readString());
//  }
}

