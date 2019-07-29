
void sendRfidTag(String tag) {
  if (client) {
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    tag.toCharArray(RfidBuffer, 20);
    Udp.write(RfidBuffer);
    boolean sent = Udp.endPacket();
    //String rip = String(remoteIp);
    if (!sent) {
      IPAddress remotIp = Udp.remoteIP();
      Serial.print("Error sending: ");
      Serial.print(remotIp);
      Serial.print(", port ");
      Serial.println(Udp.remotePort());
      greenLight();
    }
  }
}

void receiveReplyPacket() {
  /*Serial.print("Received packet of size ");
  Serial.println(packetSize);
  Serial.print("From ");
  IPAddress remoteIp = Udp.remoteIP();
  Serial.print(remoteIp);
  Serial.print(", port ");
  Serial.println(Udp.remotePort());*/

  int len = Udp.read(packetBuffer, 255); // read the packet into packetBufffer
  if (len > 0) {
    packetBuffer[len] = 0;
  }
  //Serial.println("Contents:");
  //Serial.println(packetBuffer);

  if (strcmp(packetBuffer, "conectando") == 0)  //Send a reply only if receives "conectando"
  { // send a reply, to the IP address and port that sent us the packet we received
    if (!client) {
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(ReplyBuffer);
      boolean sent = Udp.endPacket();
      if (sent) {
        Serial.println("Client connected.");
        client = true;
        blueLight();
      }
      else {
        Serial.println("Client connection failed. ");
        IPAddress remotIp = Udp.remoteIP();
        Serial.print(remotIp);
        Serial.print(", port ");
        Serial.println(Udp.remotePort());
      }
    }
  }
}

void checkWifiDevice() {
  if (WiFi.status() == WL_NO_MODULE) { // check for the WiFi module
    Serial.println("Communication with WiFi module failed!");
    while (true);// don't continue
  }

  WiFiDrv::pinMode(25, OUTPUT); //rojo
  WiFiDrv::pinMode(26, OUTPUT); //verde
  WiFiDrv::pinMode(27, OUTPUT); //azul

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
