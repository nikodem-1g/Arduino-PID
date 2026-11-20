// ng912
// 18/11/2026

#include <ESP32Encoder.h>

const int PotPin = A0;
const int EncPin1 = D2;
const int EncPin2 = D3;
const int MotPin1 = D4;
const int MotPin2 = D5;

double dt = 0, integral = 0, previous = 0, output = 0;
double kp = 75.0, ki = 2.5, kd = 0.05;
double setpoint = 0, actual = 0;
unsigned long starttime = 0, oldtime = 0;

const float CountsPerRev = 12.0f;
const float GearRatio = 298.0f;
const float CountsPerOutRev = CountsPerRev * GearRatio;

const unsigned long interval = 10;   
const int minPWM = 40;               
const double deadzone = 1.0; //in degrees     

int64_t oldCount = 0;
double rpm = 0;          // raw output-shaft RPM
double rpmFiltered = 0;  // smoothed output-shaft RPM
const double rpmAlpha = 0.2;  // 0-1, lower = smoother but slower to respond

ESP32Encoder encoder;

void setup()
{
  Serial.begin(115200);
  pinMode(MotPin1, OUTPUT);
  pinMode(MotPin2, OUTPUT);

  encoder.attachFullQuad(digitalPinToGPIONumber(EncPin1), digitalPinToGPIONumber(EncPin2));
  encoder.clearCount();
  
  starttime = millis();
}

void loop()
{
  unsigned long nowtime = millis();
  if (nowtime - oldtime < interval) return;

  // Clock
  dt = (nowtime - oldtime) / 1000.0f;
  oldtime = nowtime;

  // Mapping trimmer to angle
  //setpoint = map(analogRead(PotPin), 0, 4095, 0, 360);

  // Testing routine
  setpoint = testing(nowtime);

  // Reading encoder
  int64_t nowCount = encoder.getCount();

  int64_t deltaCount = nowCount - oldCount;
  oldCount = nowCount;

  rpm = ((double)deltaCount / CountsPerOutRev) / dt * 60.0;   // revs / sec * 60
  rpmFiltered = rpmAlpha * rpm + (1.0 - rpmAlpha) * rpmFiltered;

  float angle = fmodf(((float)nowCount / CountsPerOutRev) * 360.0f, 360.0f);
  if (angle < 0) angle += 360.0f;
  actual = angle;

  // PID 
  double error = setpoint - actual;
  if (error > 180)  error -= 360;
  if (error < -180) error += 360;

  if (fabs(error) < deadzone) {
    integral = 0;
    output = 0;
  } else {
    output = pid(error);
    output = constrain(output, -255, 255);
    //minimum drive to overcome friction
    if (output > 0 && output < minPWM)   output = minPWM;
    if (output < 0 && output > -minPWM)  output = -minPWM;
  }

  // Motor control
  if (output >= 0) {
    analogWrite(MotPin1, (int)output);
    analogWrite(MotPin2, 0);
  } else {
    analogWrite(MotPin1, 0);
    analogWrite(MotPin2, (int)(-output));
  }

  // Debugging / testing
  //Serial.print(nowtime);
  //Serial.print(",");
  //Serial.print(setpoint);
  //Serial.print(",");
  //Serial.print(error);
  //Serial.print(",");
  //Serial.print(nowCount);
  //Serial.print(oldCount);
  //Serial.print(actual);
  //Serial.print(",");
  //Serial.println(rpmFiltered);
}

double pid(double error)
{
  integral += error * dt;
  integral = constrain(integral, -200, 200); //prevents windup

  double derivative = (error - previous) / dt;
  previous = error;

  return (kp * error) + (ki * integral) + (kd * derivative);
}

// Testing routine
double testing(unsigned long nowtime)
{
  if(nowtime - starttime <= 2000) {
    return 60;
  } else{
    return 240;
  }
}