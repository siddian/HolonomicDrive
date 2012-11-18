#include <AFMotor.h>

#define channumber 6 
#define SignalPin 9

int value[channumber];
double ValueNormalized[channumber];

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);

//a temp value used for the calculations below
//int val;

//the speed of the motors range: [-200,200]
int motor1speed = 0;
int motor2speed = 0;
int motor3speed = 0;

//desired velocity in body frame
double desVelocity[2] = {0};
double desOmega = 0;

//direction vectors of motors
double F0[2] = {0};
double F1[2] = {0};
double F2[2] = {0};

//wheel baseline in [m]
double b = 0.115;

//radius of wheel in [m]
double r = 0.02;

//scaling of SI units to motor driver units
//this is also used for correcting the rotation direction!
double scaling[3] = {0};

void setup() {
  Serial.begin(115200);           // set up Serial library at 115200 bps
  Serial.println("Driving!!");

  // turn on motor
  motor1.setSpeed(80);
  motor2.setSpeed(80);
  motor3.setSpeed(80);
 
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  
  pinMode(SignalPin, INPUT); //Pin 3 as input
  
  //set the direction of the motors
  F0[0] = -1;
  F0[1] = 0;
  
  F1[0] = 0.5;
  F1[1] = -0.866025404;//-sqrt(3)/2
  
  F2[0] = 0.5;
  F2[1] = 0.866025404;
    
  //setting the correct scaling:
  scaling[0] = 8;
  scaling[1] = 8;
  scaling[2] = 8;
  
}

void loop() {
  if(pulseIn(SignalPin, HIGH) > 2000) {
    for(int x=0; x < channumber; x++) {
      //Loop to store all the channel position
      value[x]=pulseIn(SignalPin, HIGH);
      ValueNormalized[x] = value[x] - 500;
      ValueNormalized[x] /= 1000;
    }
 
//    Serial.print(ValueNormalized[2]); //gas
//    Serial.print(" ");
//    Serial.print(ValueNormalized[3]); //yaw
//    Serial.print(" ");
//    Serial.print(ValueNormalized[0]); //pitch
//    Serial.print(" ");
//    Serial.print(ValueNormalized[1]); //roll
//    Serial.print(" ");
//    Serial.print(ValueNormalized[4]); //switch
//    Serial.print(" ");
//    Serial.print(ValueNormalized[5]); //not used
//    Serial.println(""); //Start a new line
  }
  
  if (ValueNormalized[4] < 0.5) {
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    return;
  }
  
  desVelocity[0] = ValueNormalized[2] - 0.5;
  desVelocity[1] = ValueNormalized[3] - 0.5;
  desOmega = (ValueNormalized[1] - 0.5) * 10;
  
  Serial.print("motorspeeds: ");
  double temp = desVelocity[0]*F0[0] + desVelocity[1]*F0[1];
  temp += b * desOmega;
  temp /= r;
  motor1speed = temp * scaling[0];
  Serial.print(motor1speed);
  Serial.print(" ");
  
  temp = desVelocity[0]*F1[0] + desVelocity[1]*F1[1];
  temp += b * desOmega;
  temp /= r;
  motor2speed = temp * scaling[1];
  Serial.print(motor2speed);
  Serial.print(" ");
  
  temp = desVelocity[0]*F2[0] + desVelocity[1]*F2[1];
  temp += b * desOmega;
  temp /= r;
  motor3speed = temp * scaling[2];
  Serial.print(motor3speed);
  
  Serial.println("");
//  motor1speed = -150;
//  motor2speed = -75;
//  motor3speed = 75;

  if (motor1speed < 0) {
    motor1.setSpeed(-motor1speed);
    motor1.run(BACKWARD);
  } else {
    motor1.setSpeed(motor1speed);
    motor1.run(FORWARD);
  }

  if (motor2speed < 0) {
    motor2.setSpeed(-motor2speed);
    motor2.run(BACKWARD);
  } else {
    motor2.setSpeed(motor2speed);
    motor2.run(FORWARD);
  }

  if (motor3speed < 0) {
    motor3.setSpeed(-motor3speed);
    motor3.run(BACKWARD);
  } else {
    motor3.setSpeed(motor3speed);
    motor3.run(FORWARD);
  }
  
}
