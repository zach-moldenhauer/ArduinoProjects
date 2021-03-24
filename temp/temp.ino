const int sensorPin = A0;
const float baselineTemp = 20;


void setup() {
  Serial.begin(9600); // open a serial port


  for (int pinNumber = 2; pinNumber < 5; pinNumber++){
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
}

void loop() {
  int sensorVal = analogRead(sensorPin); 

  Serial.print("sensor Val: ");
  Serial.print(sensorVal);

  //convert the ADC reading to voltage

  float voltage = (sensorVal/1024.0) * 5.0;

  Serial.print(", Volts: ");
  Serial.print(voltage);

  Serial.print(", degrees C: ");
  //convert the voltage to temparature in degrees
  float temp = (voltage - .5) * 100;
  Serial.println(temp);

}
