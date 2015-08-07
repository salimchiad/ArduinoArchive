#include <IRremote.h>


int led1 = 9;
int led2 = 10;
int led3 = 11;
int led4 = 4;
int led5 = 5;
int led6 = 6;

int l1 = 0;
int l2 = 0;
int l3 = 0;
int l4 = 0;
int l5 = 0;
int l6 = 0;



unsigned long com = 0;
int RECV_PIN = 13;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);

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
    
    if (com == 16720605){
      digitalWrite(led4, HIGH);
    }

    if (com == 16769055){
      digitalWrite(led5, HIGH);
    }

    if (com == 16738455){
      digitalWrite(led6, HIGH);
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
    
    if (com == 16761405){
      digitalWrite(led4, LOW);
    }

    if (com == 16748655){
      digitalWrite(led5, LOW);
    }

    if (com == 16756815){
      digitalWrite(led6, LOW);
    }
    
    
    if (com == 16718055){
      digitalWrite(led1, HIGH);
      digitalWrite(led4, HIGH);
    }
    
    if (com == 16726215){
      digitalWrite(led2, HIGH);
      digitalWrite(led5, HIGH);
    }
    
      if (com == 16730805){
      digitalWrite(led3, HIGH);
      digitalWrite(led6, HIGH);
    }
    
    
    
    if (com == 16712445){
      digitalWrite(led1, LOW);
      digitalWrite(led4, LOW);
    }
    
    if (com == 16754775){
      digitalWrite(led2, LOW);
      digitalWrite(led5, LOW);
    }
    
    if (com == 16750695){
      digitalWrite(led3, LOW);
      digitalWrite(led6, LOW);
    }
    
    

    if (com == 16753245){
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, HIGH);
      digitalWrite(led5, HIGH);
      digitalWrite(led6, HIGH);
      
    }

    if (com == 16769565){
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
      digitalWrite(led6, LOW);
    }   


    com = 0;







  }



}











