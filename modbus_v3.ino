#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ModbusIP_ESP8266.h>

//   WIFI configuration
const char* ssid = "TP-Link_CF68_2.4";
const char* password = "54368910";
IPAddress local_IP(192, 168, 0, 123);  //  IP for ESP8266
IPAddress gateway(192, 168, 0, 1);     // gateway
IPAddress subnet(255, 255, 255, 0);   // mask

//   MODBUS
ModbusIP mb;

//   pins
const int ledPin = LED_BUILTIN;

//
bool myCoil[15]; 

void setup() {

  //   Serial output
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  //   siec WiFi
  WiFi.mode(WIFI_STA);
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
 
  //   Modbus configuration
  
  //   Start the Modbus server on port 502
  mb.server();
  
  // add 10 coils, start from 0, initial value: false
  mb.addCoil(0, false, 10);

  //adds 9 registers staring from 0, initial value:1
  mb.addHreg(0,1,10);
  
  //
  Serial.println("Ready to receive Modbus TCP/IP requests");
  
}

// Write data to serial, 6 second interval
void WriteDataToSerial() {
  Serial.println("coil 0 = "+ String(mb.Coil(0)));
  Serial.println("coil 1 = "+ String(mb.Coil(1)));
  Serial.println("coil 2 = "+ String(mb.Coil(2)));
  Serial.println("coil 3 = "+ String(mb.Coil(3)));
  Serial.println("coil 4 = "+ String(mb.Coil(4)));
  Serial.println("coil 5 = "+ String(mb.Coil(5)));
  Serial.println("coil 6 = "+ String(mb.Coil(6)));
  Serial.println("coil 7 = "+ String(mb.Coil(7)));
  Serial.println("coil 8 = "+ String(mb.Coil(8)));
  Serial.println("coil 9 = "+ String(mb.Coil(9)));
    
  Serial.println( "register [0] = " + String(mb.Hreg(0)) );
  Serial.println( "register [1] = " + String(mb.Hreg(1)) );
  Serial.println( "register [2] = " + String(mb.Hreg(2)) );
  Serial.println( "register [3] = " + String(mb.Hreg(3)) );
  Serial.println( "register [4] = " + String(mb.Hreg(4)) );
  Serial.println( "register [5] = " + String(mb.Hreg(5)) );
  Serial.println( "register [6] = " + String(mb.Hreg(6)) );
  Serial.println( "register [7] = " + String(mb.Hreg(7)) );
  Serial.println( "register [8] = " + String(mb.Hreg(8)) );
  Serial.println( "register [9] = " + String(mb.Hreg(9)) );

  Serial.println( "LED:  = " + String(digitalRead(ledPin)) );

  //   writing data to register from esp
  int x =  mb.Hreg(3);    
  x = x + 1;      // example data
  mb.Hreg(3,x);   // (a.Hreg(x,y); - x = nr register, y = value
}

void loop() {
 
  // reading modbus data
  mb.task(); 

  

  //   call WriteDataToSerial(), 6000 ms interval
  static unsigned long lastMillis = 0;  
  if (millis() - lastMillis >= 6000) {  
    WriteDataToSerial();  
    lastMillis = millis();  
  }

  //   example
  //   1 socod interval task: writing time in second from start program to modbus register 4 from esp
  static unsigned long timer2 = 0;  
  if (millis() - timer2 >= 1000) {  
    
    unsigned long ulTimer2temp = millis() / 1000 ;
    mb.Hreg(4,ulTimer2temp);
      
    timer2 = millis();  
  }
 
  // example writing modbus value to naother variable
  myCoil[2] = mb.Coil(2);

  // 
  digitalWrite(ledPin,  myCoil[2]);

}
