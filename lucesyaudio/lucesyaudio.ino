#include <sd_l0.h>
#include <sd_l1.h>
#include <sd_l2.h>
#include <SimpleSDAudio.h>
#include <SimpleSDAudioDefs.h>

int boton = 2;

int redPin = 3;
int greenPin = 5;
int bluePin = 6;

long tiempo_anterior;
long diferencia;

bool flagLuces = false;

void setup() {
  Serial.begin(9600);

  pinMode(boton, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(boton), botonLuces, FALLING);
  // Using F("...") to avoid wasting RAM
  Serial.print(F("\nInitializing SD card..."));

  // If your SD card CS-Pin is not at Pin 4, enable and adapt the following line:
  // SdPlay.setSDCSPin(10);

  if (!SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER)) {
    Serial.println(F("initialization failed. Things to check:"));
    Serial.println(F("* is a card is inserted?"));
    Serial.println(F("* Is your wiring correct?"));
    Serial.println(F("* maybe you need to change the chipSelect pin to match your shield or module?"));
    Serial.print(F("Error code: "));
    Serial.println(SdPlay.getLastError());
    while(1);
  } else {
   Serial.println(F("Wiring is correct and a card is present."));
  }
  Serial.print(F("Looking for on.wav... "));
  if(!SdPlay.setFile("ALARMA.WAV")) {
    Serial.println(F(" not found on card! Error code: "));
    Serial.println(SdPlay.getLastError());
    while(1);
  } else {
   Serial.println(F("found."));
  }
}

void loop() {
  if(flagLuces){
    flagLuces = false;
    cambiaEstadoLED();
  }
}

void botonLuces(){
  diferencia = millis() - tiempo_anterior;
  tiempo_anterior = millis();
  //Serial.println(diferencia);

  if(diferencia > 500){
    flagLuces = true;
  }
}

void cambiaEstadoLED(){
  SdPlay.play();
  if(digitalRead(greenPin) == false){
    encender();
    Serial.println("encendido");
  }
  else{
    apagar();
    Serial.println("apagado");
  }
}

void encender(){
  for (int brillo = 0; brillo <= 122; brillo++) {
    setColor(0, brillo, brillo); // es esta combinación porque queremos el color aqua
    delay(30);
  }
  for(int brillo = 123; brillo <= 255; brillo++) {
    setColor(0, brillo, brillo);
    delay(5);
  }
}

void apagar(){
  for (int brillo = 255; brillo >= 0; brillo--) {
    setColor(0, brillo, brillo); // es esta combinación porque queremos el color aqua
    delay(10);
  }
}

void setColor(int red, int green,int blue){
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

/*void loop() {
  Serial.print(F("Playing... "));
  SdPlay.play();
  while(!SdPlay.isStopped()) {
    ;   // no worker needed anymore :-)
  }
  Serial.println(F("done."));
  delay(3000);
}*/
