# RFID-Detector
A wireless RFID multiple tag reader based in M6E-nano and Arduino Uno Wifi

When a RFID tag is readed, a UDP packet is sent to an App via WiFi module.

Color RGB led in Arduino:
   * Red: Not Connected / Connecting
   * Green: Connected to Wifi. & No clients connected
   * Blue: Client Connected.

Arduino requred Libraries:
* Sparkfun RFID
* WifiINA

Processing Libraries:
* UDP
* ControlP5
