
#include <Servo.h>

Servo myServo;
const int sensorPin = A0;


int input = 0;


int powerOfTen(int exponent) {
  int number = 1;
  
  for(int i = 0; i < exponent; i++) {
    number *= 10;
  }

  return number;
}



int charToInt(char arr[], int siz) {
  int integer = 0;
  int exponent = siz - 1;
  int charInt = 0;
  int multiple = 0;

  for(int i = 0; i < siz; i++) {

    charInt = arr[i] - 48;

    if(charInt < 0 || charInt > 9) {
      integer /= 10;
      break;
    }
    
    multiple = powerOfTen(exponent);
    
    integer += charInt * multiple;

//    Serial.print("ex: ");
//    Serial.println(exponent);
//
//    Serial.print("pow: ");
//    Serial.println(multiple);
//
//
//    Serial.print("char: ");
//    Serial.println(charInt);
//
//
//    

    exponent -= 1;

    //Serial.print("int: ");
    //Serial.println(in);
    //ex: arr = "111" , siz = 3, integer = ('1' - 48 = 1) * pow(10, (3 - 1) - 0) = 100
    //                                     ('1' - 48 = 1) * pow(10, (3 - 1) - 1) = 10
    //                                     ('1' - 48 = 1) * pow(10, (3 - 1) - 2) = 1
    //                           integer = 111
  }

  return integer;
  
}

void setup() {

  Serial.begin(9600);
  myServo.attach(9);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  
}

void loop() {

  //if message comes in from Serial port
  char buff[4];
   
  if(Serial.available()) {
    Serial.readBytes(buff, 4);
    input = charToInt(buff, 4);

    analogWrite(6, 120); //120 was found to be slow enough to produce proportion of ft
    digitalWrite(7, HIGH);
    delay(input * 3); //turn on motor for input amount of time
                      //multiplying by 3 is the right proportion for units of ft
                      //this was found by trial and error
    digitalWrite(6, LOW);


    myServo.write(0);
    delay(10000);
    myServo.write(90);


    
  }
  


  //if input is over 200 turn on motor
//  if(input > 50) {
//    Serial.println("HEREH");
//    digitalWrite(7,HIGH);
//    analogWrite(6, input/4);
//  } else {
//    analogWrite(6, LOW);
//    digitalWrite(7,LOW);
//        Serial.println("DOWN");
//  }



  //if button is pressed, send input to console
  int switchOn = digitalRead(5);
  if(switchOn == HIGH){
    Serial.println(input);
  }

  //Control lights based on potentiometer
  input = analogRead(sensorPin);
  if(input >= 950) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);

    delay(250);
  }


  if(input > 800 && input < 950 ) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);

    delay(250);
  }

  if(input > 200 && input < 800) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    delay(250);
  }

  if(input < 200){
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    delay(250);
  }
}
