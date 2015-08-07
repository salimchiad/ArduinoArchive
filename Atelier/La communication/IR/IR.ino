#include <IRremote.h>

int led1 = 9;

unsigned long com = 0;
int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  pinMode(led1, OUTPUT);
}

void loop() {

    if (irrecv.decode(&results)) {
    com = results.value;
    
    Serial.println(com, BIN);
    Serial.println(com);
    irrecv.resume(); // Receive the next value
    
  } 
  else {
    
    if (com == 16753245){
      digitalWrite(led1, HIGH);
    }
    
     if (com == 16769565){
      digitalWrite(led1, LOW);
    }   

    com = 0;

  }
}










