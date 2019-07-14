/*
   Wifi UDP Sender packets with RFID tag data.
   by Lot Amoros

   This software is based in WifiUdpSendReceiveString example of Nina library.
   This software runs in a Arduino UNO Wifi Rev2 and Sparkfun Simultaneous RFID Reader M6E-NANO

   When a RFID tag is readed, a UDP packet is sent to the client on port remotePort using the WiFi module.

   Color RGB led:

   Red: Not Connected / Connecting
   Green: Connected to Wifi. & No clients connected
   Blue: Client Connected.
*/

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include "arduino_secrets.h"
#include <utility/wifi_drv.h>

WiFiUDP Udp;

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
  Serial.begin(9600); //Initialize serial and wait for port to open
  while (!Serial) {
    ;   //wait for serial port to connect. Needed for native USB port only
  }

  checkDevice();
  redLight();
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(3000);    // wait 5 seconds for connection:
  }
  greenLight();
  printWifiStatus();

  //Serial.println("\nStarting connection to server...");
  Udp.begin(localPort); // if you get a connection, report back via serial:
}

void loop() {
  packetSize = Udp.parsePacket();// if there's data available, read a packet
  if (packetSize) {
    client = true;
    blueLight();
    receiveReplyPacket();
  }

  if (client) {
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    int rt = random(0, 99999999999); //000000000000
    String randomTag = (String) rt;
    randomTag.toCharArray(RfidBuffer, 12);
    Udp.write(RfidBuffer);
    boolean sent = Udp.endPacket();
    //String rip = String(remoteIp);
    if (sent) {
      Serial.print("Sent: ");
    }
    else {
      Serial.print("Error sending: ");
      greenLight();
    }
    IPAddress remotIp = Udp.remoteIP();
    Serial.print(remotIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());
    delay (1800);
  }
}

void receiveReplyPacket() {
  Serial.print("Received packet of size ");
  Serial.println(packetSize);
  Serial.print("From ");
  IPAddress remoteIp = Udp.remoteIP();
  Serial.print(remoteIp);
  Serial.print(", port ");
  Serial.println(Udp.remotePort());

  int len = Udp.read(packetBuffer, 255); // read the packet into packetBufffer
  if (len > 0) {
    packetBuffer[len] = 0;
  }
  Serial.println("Contents:");
  Serial.println(packetBuffer);

  if (strcmp(packetBuffer, "conectando") == 0)  //Send a reply only if receives "conectando"
  { // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    boolean sent = Udp.endPacket();
    if (sent)
      Serial.print("Sent: ");
    else
      Serial.println("Failed. ");
    IPAddress remotIp = Udp.remoteIP();
    Serial.print(remotIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());
  }
}


void checkDevice() {
  if (WiFi.status() == WL_NO_MODULE) { // check for the WiFi module
    Serial.println("Communication with WiFi module failed!");
    while (true);// don't continue
  }

  WiFiDrv::pinMode(25, OUTPUT); //rojo
  WiFiDrv::pinMode(26, OUTPUT); //verde
  WiFiDrv::pinMode(27, OUTPUT); //BLUE

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
}

// print the SSID of the network you're attached to
void printWifiStatus() {
  Serial.println("Connected to wifi");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void redLight() {
  WiFiDrv::digitalWrite(25, HIGH);
  WiFiDrv::digitalWrite(26, LOW);
  WiFiDrv::digitalWrite(27, LOW);
}

void greenLight() {
  WiFiDrv::digitalWrite(25, LOW);
  WiFiDrv::digitalWrite(26, HIGH);
  WiFiDrv::digitalWrite(27, LOW);
}

void blueLight() {
  WiFiDrv::digitalWrite(25, LOW);
  WiFiDrv::digitalWrite(26, LOW);
  WiFiDrv::digitalWrite(27, HIGH);
}
