int potar = 0;
int led0 = 3;
int led1 = 5;
int led2 = 6;
int led3 = 9;
int b1 = 11;
int b2 = 12;
int b3 = 13;
int l0 = 0;
int l1 = 10;
int l2 = 10;
int l3 = 10;
int potars = 0;
int but1 = 0;
int but2 = 0;
int but3 = 0;



void setup(){
pinMode(led0, OUTPUT);
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);
pinMode(b1, INPUT);
pinMode(b2, INPUT);
pinMode(b3, INPUT);
digitalWrite(b1, HIGH);
digitalWrite(b2, HIGH);
digitalWrite(b3, HIGH);

}


void loop(){

  potars = analogRead(potar);
  
  but1 = digitalRead(b1);
  but2 = digitalRead(b2);
  but3 = digitalRead(b3);
  
  l0 = map(potars, 0, 1023, 0, 255);

  if (but1 == LOW){
  l1 = l0;
  } 
  
  if (but2 == LOW){
    l2 = l0;
  } 
  
  if (but3 == LOW){
   l3 = l0;
  } 
 
  analogWrite(led0, l0);
  analogWrite(led1, l1);
  analogWrite(led2, l2);
  analogWrite(led3, l3);
  
  delay(10);

}

