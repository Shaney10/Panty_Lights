//
// Master mode bluetooth connection and pin 
// It creates a bridge between Serial and Classical Bluetooth (SPP)
// This is an extention of the SerialToSerialBT
//

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

String MACadd = "AA:BB:CC:11:22:33";
uint8_t address[6]  = {0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33};
//uint8_t address[6]  = {0x00, 0x1D, 0xA5, 0x02, 0xC3, 0x22};
String name = "OBDII";
const char *pin = "1234";    // <- standard pin would be provided by default
bool connected;

bool Door_Switch = 0;    // Booleen variable for door switch position
int Door_Timer = 0;    // Integer variable for door open time
const int Door_Open_Time = 15;    //Time in minutes to turn off light if door left open
int Relay_Position;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);    // Configure Built in LED as Output
  pinMode(26, INPUT_PULLUP);    // Set pin A0 GPIO 26 to input pull high
  digitalWrite(26, HIGH);    // Turn on pullup resistors
  
  Serial.begin(115200);    // Begin Serial communication
  SerialBT.begin("Door_Switch", true);   //Begin Bluettoth communication (Name,State)
  Serial.println("The device started in master mode, make sure remote BT device is on!");
  
  // connect(address) is fast (upto 10 secs max), connect(name) is slow (upto 30 secs max) as it needs
  // to resolve name to address first, but it allows to connect to different devices with the same name.
  // Set CoreDebugLevel to Info to view devices bluetooth address and device names
  connected = SerialBT.connect("Light_Switch");
  //connected = SerialBT.connect(address);
  
  if(connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while(!SerialBT.connected(10)) {
      SerialBT.disconnect();
      SerialBT.connect();
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }
  // Disconnect() may take upto 10 secs max
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Succesfully!");
  }
  // This would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
  SerialBT.connect();

digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on by making the voltage HIGH
}
void loop() {

  Door_Switch = digitalRead(26);    // Read voltage level on GPIO pin 26
  Serial.println(Door_Switch);    // Print bool value of GPIO pin 26
  
  delay(500);    //wait 1/2 second to look for new door position
  if (Door_Switch == 1 && Door_Timer < (Door_Open_Time * 120)) {    // 1 value when door is open and door open time in minutes
    SerialBT.write(1);
    digitalWrite(LED_BUILTIN, HIGH);    // Turn the LED on by making the voltage HIGH
    Door_Timer = Door_Timer + 1;
    Serial.println(Door_Timer);    // Print integer value for door timer 
  }
  else if (Door_Switch == 0) {
    SerialBT.write(0);    // 0 value when door is closed
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on by making the voltage HIGH
    Door_Timer = 0;  // Set Door_Timer back to zero when door is closed
  }  
  else {
    SerialBT.write(0);    // 0 value when door is closed
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED off by making the voltage LOW
  }


//  if (Serial.available()) {
//    SerialBT.write(Serial.read());
//  }
  if (SerialBT.available()) {
    Relay_Position = SerialBT.read();
    Serial.println(Relay_Position);
  }
//  if (Relay_Position == 10 || Relay_Position == 11) {
//  }
  else {
    SerialBT.disconnect();
    Serial.println("Disconnected Succesfully!");
    SerialBT.connect();
  }
}