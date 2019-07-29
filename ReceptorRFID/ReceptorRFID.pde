/*
 ReceptorRFID Android/Desktop App
 By Lot Amorós.
 
 This App receives RFID tags from an Arduino running WifiRFIDSender. 
 Don't forget to activate Permissions: INTERNET, WRITE_EXTERNAL_STORAGE
 
 The file with the RFIOD tags in Android is saved in 
 /storage/emulated/0/RFIDXXXX.txt
 */

import controlP5.*;
import hypermedia.net.*;
ControlP5 cp5;
UDP udp; 

int localPort = 6000;
int remotePort = 12000;
String remoteIp       = "255.255.255.255";  // the remote IP address

int dd = 3; //Display Density
//int dd = displayDensity; //Display Density

int w = 540;//1080
int h = 960;//1920

PImage logo; 
PFont font; 
boolean welcome = true;
boolean connected = false;
boolean running = false;
boolean saved = false;
int startTime;
int stopTime;

Btn bNext;
Btn bStart;
Btn bSave;

Textarea terminal;
Println console;
Scan scan;

void setup() {
  fullScreen();
  // size(540,960);
  noStroke();
  fill(0);

  w = width;
  h = height;
  font = createFont("SansSerif", 20 * dd);
  cp5 = new ControlP5(this);
  udp = new UDP( this, localPort );
  scan = new Scan();
  udp.listen( true );
  logo = loadImage("logo.png");

  imageMode(CENTER);
  textFont(font);
  textAlign(CENTER, CENTER);

  bNext = new Btn(int((w/3)-120), int( 4.5*(h/10)), "Comenzar");
  bStart = new Btn(int((w/3)-120), int(h/10), "Iniciar");
  bSave = new Btn(int((w/3)-120), int(8*(h/10)), "Guardar");

  terminal = cp5.addTextarea("txt")
    //.setFont(font,20)
    .setPosition((w/3)-250, 3*(h/10))
    .setSize((w/2)+300, 3*(h/10))
    .setFont(createFont("", 60))
    .setLineHeight(70)
    .setColor(color(0))
    .setColorBackground(color(220, 200))
    .setColorForeground(color(255, 100))
    .hide();
  console = cp5.addConsole(terminal);
}

void draw() {
  background(255);
  if (welcome)
    welcomeScreen();
  else
    receiverScreen();
}

public void receiverScreen() {
  textSize(18 * dd);
  bStart.draw();
  bSave.draw();
  if (connected) {
    tint(0, 255, 0, 255);
    image(logo, w/2, h/18, 150, 150);    
    fill(0, 250, 0);
    text("Conectado", (w/3), (h/4));
    fill(0);
    if (running)
      text(getElapsedTimeString(), 2*(w/3), (h/4));
  } else {
    tint(255, 0, 0, 128);
    image(logo, w/2, h/18, 150, 150);
    fill(250, 0, 0);
    text("NO Conectado", w/3, (h/4));   
    fill(0);
  }
  tint(255);
  textSize(20 * dd);
  text("Tags detectados:" + scan.items(), w/2, 7*(h/10));

  if (saved) {
    textSize(12 * dd);
    text("Guardado en: "+scan.fileName(), w/2, 9.5*(h/10));  
    textSize(20 * dd);
  }

  if (!connected) {
    sendConnect();
  }
}

public void welcomeScreen() {
  image(logo, w/2, h/4);
  text("Gracias por utilizar este software.", w/2, 2*(h/3));
  bNext.draw();
}

void mousePressed() {
  if (bNext.isPressed()) 
    comenzar();
  if (bStart.isPressed()) 
    iniciar();
  if (bSave.isPressed()) 
    guardar();
}

//Boton comenzar
public void comenzar() {
  sendConnect();
  welcome = false;
  terminal.show();
  console.clear();
}

public void sendConnect() {
  String message  = "conectando";             // the message to send
  udp.send( message, remoteIp, remotePort );  // send the message
}

public void iniciar() {
  if (connected) {
    if (running)
      stopScan();
    else  
    startScan();
  }
}

public void guardar() {
  stopScan();
  scan.save();
  console.clear();
  saved = true;
}

public void startScan() {
  scan = new Scan();
  console.clear();
  running = true;
  saved = false;
  startTime = millis();
  bStart.setLabel("Parar");
}

public void stopScan() {
  running = false;
  bStart.setLabel("Iniciar");
}

/**
 * This method will be automatically called by the UDP object each time 
 * he receive a nonnull message.
 */
void receive( byte[] data, String ip, int port ) {  // <-- extended handler
  String msg = new String(data);
  //println( "Received: \""+msg+"\" from "+ip+" on port "+port );
  if (msg.equals("Connected") == true) {
    connected = true;
  } else
  {
    //println( "Received: \""+message+"\" from "+ip+" on port "+port );
    if (running)
    {
      println(msg);
      scan.add(msg);
    }
  }
}

int getElapsedTime() {
  int elapsed;
  if (running) {
    elapsed = (millis() - startTime);
  } else {
    elapsed = (stopTime - startTime);
  }
  return elapsed;
}

String getElapsedTimeString() {
  return (minutes() +":"+seconds());
}

int seconds() {
  return (getElapsedTime() / 1000) % 60;
}
int minutes() {
  return (getElapsedTime() / (1000*60)) % 60;
}
int hours() {
  return (getElapsedTime() / (1000*60*60)) % 24;
}
