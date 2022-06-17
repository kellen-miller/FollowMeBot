/*Pin 9 sets the enable and speed of shield outputs 1 & 2

Pin 10 sets the enable and speed of shield outputs 3 & 4

Pin 8 from Uno controls the state of shield output 1

Pin 11 from Uno controls the state of shield output 2

Pin 12 from Uno controls the state of shield output 3

Pin 13 from Uno controls the state of shiled output 4
*/

int left_motor_pos = 8; // pin 8 --- motor (+) white wire
int left_motor_neg = 11; // pin 11 --- motor (-) black wire
int left_motor_speed = 9; // pin 9 --- motor speed signal

int right_motor_pos = 12; // pin 12 --- motor (+) white wire
int right_motor_neg = 13; // pin 13 --- motor (-) black wire
int right_motor_speed = 10; // pin 10 --- motor speed signal


void setup() {
  pinMode(left_motor_pos, OUTPUT);
  pinMode(left_motor_neg, OUTPUT);
  pinMode(left_motor_speed, OUTPUT);
  pinMode(right_motor_pos, OUTPUT);
  pinMode(right_motor_neg, OUTPUT);
  pinMode(right_motor_speed, OUTPUT);
}

void loop(){
  analogWrite(left_motor_speed,255);
  analogWrite(right_motor_speed,255);

  go_foward();

  delay(2000);

  go_backward();

  delay(2000);
   
}

void go_foward() {
  digitalWrite(left_motor_neg,LOW);
  digitalWrite(right_motor_neg,LOW);
  digitalWrite(left_motor_pos,HIGH);
  digitalWrite(right_motor_pos,HIGH);
}

void go_backward (){
  digitalWrite(left_motor_pos,LOW);
  digitalWrite(right_motor_pos,LOW);
  digitalWrite(left_motor_neg,HIGH);
  digitalWrite(right_motor_neg,HIGH);  
}