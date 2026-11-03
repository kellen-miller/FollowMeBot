#include <Servo.h>

Servo head;

int right_motor_pos = 8; // pin 8 --- motor (+) white wire
int right_motor_neg = 11; // pin 11 --- motor (-) black wire
int right_motor_speed = 9; // pin 9 --- motor speed signal

int left_motor_pos = 12; // pin 12 --- motor (+) white wire
int left_motor_neg = 13; // pin 13 --- motor (-) black wire
int left_motor_speed = 10; // pin 10 --- motor speed signal


int pos, turn_direction;


void setup() {
  Serial.begin(9600);
  head.attach(2);
}

void loop() {
  turn_direction = random(1,3);
  Serial.println(turn_direction);
  if (turn_direction == 1) {
    turn_left(200);
    delay(300);
  } else if (turn_direction == 2) {
    turn_right(200);
    delay(300);
  }
  analogWrite(left_motor_speed,0);
  analogWrite(right_motor_speed,0);
  delay(1000);
  pos = random(10,96);
  head.write(pos);
  delay(1000);
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