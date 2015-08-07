
int led2 = 12;
int led1 = 13; // choose the pin for the LED
int inPin = 2; // choose the input pin (for a pushbutton)
int val = 0; // variable for reading the pin status
void setup() {
pinMode(led1, OUTPUT); // declare LED as output
pinMode(led2, OUTPUT);
pinMode(inPin, INPUT); // declare pushbutton as input
}
void loop(){
val = digitalRead(inPin); // read input value
if (val == HIGH) { // check if the input is HIGH (button released)
digitalWrite(led1, LOW); // turn LED OFF
digitalWrite(led2, HIGH);
} else {
digitalWrite(led1, HIGH); // turn LED ON
digitalWrite(led2, LOW);
}
delay(10);
}
