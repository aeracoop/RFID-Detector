/*
   Wifi UDP Sender packets with RFID tag data.
   by Lot Amoros

   This software is based in WifiUdpSendReceiveString of Nina library, and ContinuousRead of RFID Library
   This software runs in a Arduino UNO Wifi Rev2 and Sparkfun Simultaneous RFID Reader M6E-NANO.

   When a RFID tag is readed, a UDP packet is sent to the client on port remotePort using the WiFi module.

   Color RGB led:
   * Red: Not Connected / Connecting
   * Green: Connected to Wifi. & No clients connected
   * Blue: Client Connected.
*/
#include <SoftwareSerial.h> //Used for transmitting to the device
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include "arduino_secrets.h"
#include <utility/wifi_drv.h>
#include "SparkFun_UHF_RFID_Reader.h" //Library for controlling the M6E Nano module
SoftwareSerial softSerial(2, 3); //RX, TX
RFID nano; //Create instance
WiFiUDP Udp;

#define BUZZER1 9 // 0 For testing quietly
#define BUZZER2 10

//Cambiar esta variable readPower a 2700 para potencia Maxima.
//######################
int readPower = 500;
//######################

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your WPA network password

unsigned int localPort = 12000;   // local port to listen on
int status = WL_IDLE_STATUS;
boolean client;

char packetBuffer[255];               //buffer to hold incoming packet
int packetSize = 0;
char  ReplyBuffer[] = "Connected";    // a string to send back
char  RfidBuffer[] = "000000000000";  // a string to send rfid tag

void setup() {
  Serial.begin(115200); //Initialize serial and wait for port to open
  //while (!Serial); //wait for serial port to connect. Needed for native USB port only

  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);

  checkWifiDevice();
  redLight();
  Serial.println("Welcome to WifiRFIDSender.");
  while (status != WL_CONNECTED) {
    Serial.println("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);    // wait 5 seconds for connection:
  }
  greenLight();
  printWifiStatus();

  Serial.println("\nWaiting for a connection...");
  Udp.begin(localPort); // if you get a connection, report back via serial:

  if (setupNano(38400) == false) //Configure nano to run at 38400bps
  {
    Serial.println(F("Module failed to respond. Please check wiring."));
    while (1); //Freeze!
  }
  nanoConfig();
}

void loop() {
  packetSize = Udp.parsePacket();// if there's data available, read a packet
  if (packetSize) {   
    receiveReplyPacket();
  }
  nanoCheck();
  delay(500);
}
