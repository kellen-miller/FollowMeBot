/****************************************************************************** 
MLX90614 ------------- Arduino
  VDD ------------------ 3.3V
  VSS ------------------ GND
  SDA ------------------ SDA (A4 on older boards) WHITE
  SCL ------------------ SCL (A5 on older boards) YELLOW

******************************************************************************/

/*Pin 9 sets the enable and speed of shield outputs 1 & 2
Pin 10 sets the enable and speed of shield outputs 3 & 4
Pin 8 from Uno controls the state of shield output 1
Pin 11 from Uno controls the state of shield output 2
Pin 12 from Uno controls the state of shield output 3
Pin 13 from Uno controls the state of shield output 4
*/

#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> // SparkFunMLX90614 Arduino library
#include <Servo.h>

IRTherm therm; // Create an IRTherm object to interact with throughout

int right_motor_pos = 8; // pin 8 --- motor (+) white wire
int right_motor_neg = 11; // pin 11 --- motor (-) black wire
int right_motor_speed = 9; // pin 9 --- motor speed signal

int left_motor_pos = 12; // pin 12 --- motor (+) white wire
int left_motor_neg = 13; // pin 13 --- motor (-) black wire
int left_motor_speed = 10; // pin 10 --- motor speed signal

#define trigPin 5
#define echoPin 6
long duration, cm, inches;

Servo head;
int pos;

int heat_direction;
int temp;
long distance;

void setup() {
  Serial.begin(9600);
  therm.begin(); // Initialize thermal IR sensor
  therm.setUnit(TEMP_F); // Set the library's units to Farenheit
  head.attach(2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(left_motor_pos, OUTPUT);
  pinMode(left_motor_neg, OUTPUT);
  pinMode(left_motor_speed, OUTPUT);
  pinMode(right_motor_pos, OUTPUT);
  pinMode(right_motor_neg, OUTPUT);
  pinMode(right_motor_speed, OUTPUT);
  head.write(53); /* Center Head*/
}

void loop(){

  go_forward();
  do {
    distance = distance_read();
    temp = temp_read();
    delay(25);
  } while (distance >= 45 and temp < 75);

  if (distance < 45 and temp < 75) {
    go_backward(500);
    turn_left(500);
  } else if (temp >= 75){
    analogWrite(left_motor_speed,0);
    analogWrite(right_motor_speed,0);
    heat_direction = heat_search();
    if (heat_direction == 1) {
       turn_left(200);
       delay(300);
       temp = temp_read();
       if (temp >= 75){
        go_forward();
        do {
          distance = distance_read();
          temp = temp_read();
          delay(25);
        } while (distance >= 45 and temp >= 75);
        
        if (distance < 45) {
          analogWrite(left_motor_speed,0);
          analogWrite(right_motor_speed,0);
          do {
            //Serial.println("See human left");
            distance = distance_read();
            delay(25);
          } while (distance < 45);
        }
      }
    } else if (heat_direction == 2) {
       turn_right(300);
       delay(300);
       temp = temp_read();
       if (temp >= 75){
        go_forward();
        do {
          distance = distance_read();
          temp = temp_read();
          delay(25);
        } while (distance >= 45 and temp >= 75);
        if (distance < 45) {
          analogWrite(left_motor_speed,0);
          analogWrite(right_motor_speed,0);
          do {
            //Serial.println("See human right");
            distance = distance_read();
            delay(25);
          } while (distance < 45);
        }
      }
    } else if (heat_direction == 3) {
       temp = temp_read();
       if(temp >= 75){
        go_forward();
        do {
          distance = distance_read();
          temp = temp_read();
          delay(25);
        } while (distance >= 45 and temp >= 75);
        if (distance < 45) {
          analogWrite(left_motor_speed,0);
          analogWrite(right_motor_speed,0);
          do {
            //Serial.println("See human center");
            distance = distance_read();
            delay(25);
          } while (distance < 45);
        }
      }
    }
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

  if (cm == 5 or cm > 3000) {
    distance_read();
  }
  else {
    return(cm);
  }
}

long temp_read() 
{
  if (therm.read()) // On success, read() will return 1, on fail 0.
  {
    // Use the object() and ambient() functions to grab the object and ambient
  // temperatures.
  // They'll be floats, calculated out to the unit you set with setUnit().
    /*Serial.println("Object: " + String(therm.object(), 2)); */
  }
  //Serial.println(therm.object());
  return(therm.object());
}

long heat_search(){
  float right_temp;
  float left_temp;
  float center_temp;
  
  head.write(10);
  delay(2000);
  right_temp = temp_read();
  
  head.write(96);
  delay(2000);
  left_temp = temp_read();

  head.write(53);
  delay(2000);
  center_temp = temp_read();
  
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
