//Head Servo Initlization

#include <Servo.h>

Servo head;

//Heat Sensor Initialization

/********************************************************* 
MLX90614 ------------- Arduino
  VDD ------------------ 3.3V
  VSS ------------------ GND
  SDA ------------------ SDA
  SCL ------------------ SCL

*********************************************************/

#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> // SparkFunMLX90614 Arduino library

IRTherm therm; // Create an IRTherm object to interact with throughout
int temp, heat_direction;


//Drive Servos Initialization

/*********************************************************
Pin 9 sets the enable and speed of shield outputs 1 & 2
Pin 8 from Uno controls the state of shield output 1
Pin 11 from Uno controls the state of shield output 2

Pin 10 sets the enable and speed of shield outputs 3 & 4
Pin 12 from Uno controls the state of shield output 3
Pin 13 from Uno controls the state of shiled output 4
**********************************************************/

int right_motor_speed = 9; // pin 9 --- motor speed signal
int right_motor_pos = 8; // pin 8 --- motor (+) white wire
int right_motor_neg = 11; // pin 11 --- motor (-) black wire

int left_motor_speed = 10; // pin 10 --- motor speed signal
int left_motor_pos = 12; // pin 12 --- motor (+) white wire
int left_motor_neg = 13; // pin 13 --- motor (-) black wire


//Distance Sensor Initialization

#define trigPin 5
#define echoPin 6

long duration, cm, inches;
int distance;


//Main Program
void setup() {
  Serial.begin(9600);

  head.attach(2); //attach head to pin 2
  head.write(53); //Center Head
  
  therm.begin(); //initialize thermal IR sensor
  therm.setUnit(TEMP_F); //set units to fahrenheit

  //set motor pins and set as outputs 
  pinMode(left_motor_pos, OUTPUT);
  pinMode(left_motor_neg, OUTPUT);
  pinMode(left_motor_speed, OUTPUT);
  pinMode(right_motor_pos, OUTPUT);
  pinMode(right_motor_neg, OUTPUT);
  pinMode(right_motor_speed, OUTPUT);

  //set distance sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
}

void loop(){

  go_forward(); //move forward
  do { //do while outside minimum range and no heat sensed, take readings on both every 25 millis
    distance = distance_read();
    temp = temp_read();
    delay(25);
  } while (distance >= 45 and temp < 75);

  //above loop broken, set motor speed to zero
  analogWrite(left_motor_speed,0); 
  analogWrite(right_motor_speed,0);

  if (distance < 45 and temp < 75) { //if below temperature threshold and we are too close
    go_backward(500); //back up for 500 millis
    turn_right(500); //turn right for 500 millis
  } else if (temp >= 75){ //if the temperature is above threshold
    heat_direction = heat_search(); //find direction of temperature
    if (heat_direction == 1) { //if to the left
       turn_left(200); //turn left for 200 millis
       delay(300); //delay for 300 millis
    } else if (heat_direction == 2) { //if to the right
       turn_right(200); //turn right for 200 millis
       delay(300); //delay for 300 millis
    }
    human_in_sight(); //call function
  }
}

void go_forward() {
  analogWrite(left_motor_speed,255);
  analogWrite(right_motor_speed,255);
  digitalWrite(left_motor_neg,LOW);
  digitalWrite(right_motor_pos,LOW);
  digitalWrite(left_motor_pos,HIGH);
  digitalWrite(right_motor_neg,HIGH);
}

void go_backward(int duration){
  analogWrite(left_motor_speed,255);
  analogWrite(right_motor_speed,255);
  digitalWrite(left_motor_pos,LOW);
  digitalWrite(right_motor_neg,LOW); 
  digitalWrite(left_motor_neg,HIGH);
  digitalWrite(right_motor_pos,HIGH);
  delay(duration); 
}

void turn_right(int duration){
  analogWrite(left_motor_speed,255);
  analogWrite(right_motor_speed,255);
  digitalWrite(right_motor_neg,LOW);
  digitalWrite(left_motor_neg,LOW);
  digitalWrite(right_motor_pos,HIGH);
  digitalWrite(left_motor_pos,HIGH);
  delay(duration);
}

void turn_left(int duration) {
  analogWrite(left_motor_speed,255);
  analogWrite(right_motor_speed,255);
  digitalWrite(right_motor_pos,LOW);
  digitalWrite(left_motor_pos,LOW);
  digitalWrite(right_motor_neg,HIGH);
  digitalWrite(left_motor_neg,HIGH);
  delay(duration);
}

long distance_read() {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  cm = (duration/2) /29.1;

  if (cm == 5 or cm > 3000) {//if erroneous output given, recall the function until corrected
    distance_read();
  }
  else {
    return(cm);
  }
}

long temp_read() 
{
  if (therm.read()) //if success
  {
    return(therm.object()); //return object temp
  }
}

long heat_search(){
  float right_temp, left_temp, center_temp;
  
  head.write(10); //look right
  delay(2000);
  right_temp = temp_read(); //take temp
  
  head.write(96); //look left
  delay(2000);
  left_temp = temp_read(); //take temp

  head.write(53); //look center
  delay(2000);
  center_temp = temp_read(); //take temp
  
  if (left_temp < 75 and right_temp < 75 and center_temp < 75){
    return(0); /* nothing */
  } else if (left_temp > right_temp and left_temp > center_temp){
    Serial.println("left");
    return(1); /* left */
  } else if (right_temp > left_temp and right_temp > center_temp){
    Serial.println("right");
    return(2); /* right */
  } else {
    Serial.println("center");
    return(3); /* center */
  }
}

void human_in_sight() {
  temp = temp_read(); //retake temperature to try and rule out errors
  if (temp >= 75){ //if above threshold
    go_forward(); //move forward
    do { //do this until distance is below threshold or temperature is below threshold
      distance = distance_read();
      temp = temp_read();
      delay(25);
    } while (distance >= 45 and temp >= 75);

    //loop is broken from something changing
    if (distance < 45 and temp >= 75) { //if because distance is below threshold and temp is still above
      //stop
      analogWrite(left_motor_speed,0);
      analogWrite(right_motor_speed,0);
      do { //stay stopped until above threshold
        distance = distance_read();
        delay(25);
      } while (distance < 45);
    }
  }
}

