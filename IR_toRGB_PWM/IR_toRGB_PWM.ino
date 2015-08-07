#include <IRremote.h>


int led1 = 9;
int led2 = 10;
int led3 = 11;

int l1 = 10;
int l2 = 10;
int l3 = 10;

unsigned long com = 0;
int RECV_PIN = 13;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  
  delay(10);
  
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

}

void loop() {



  if (irrecv.decode(&results)) {
    Serial.println(results.value, BIN);
    com = results.value;
    Serial.println(com);
    irrecv.resume(); // Receive the next value
  } 
  else {

    if (com == 16724175){
      digitalWrite(led1, HIGH);
    }

    if (com == 16716015){
      digitalWrite(led2, HIGH);
    }

    if (com == 16728765){
      digitalWrite(led3, HIGH);
    }

    if (com == 16743045){
      digitalWrite(led1, LOW);
    }

    if (com == 16734885){
      digitalWrite(led2, LOW);
    }

    if (com == 16732845){
      digitalWrite(led3, LOW);
    }

    if (com == 16753245){
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
    }

    if (com == 16769565){
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
    }   


    com = 0;







  }



}











