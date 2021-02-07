
#include <QTRSensors.h>


#define NUM_SENSORS   6     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2
#define l1 A0
#define l2 A1
#define r1 A2
#define r2 A3
#define pwm1 11
#define pwm2 10
int lms =80;
int rms =80;
int maxrrs= 128;
 int maxlrs= 128;

// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {5, 6, 7, 8, 9, 12},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];
float Kp=0.056,Kd=0.5;
int error=0,derivative=0;
float some;
int lrs=0,rrs=0;
int black=100,white=100;//black was 400

void setup()
{
  delay(500);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(A5,OUTPUT);
  digitalWrite(13, HIGH);
  analogWrite(A5,255);// turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
 // delay(1000);
}

int lasterror=0;
void Forward()
{
  analogWrite(l1,255);
  analogWrite(l2,0);
  analogWrite(r1,255);
  analogWrite(r2,0);
  analogWrite(pwm1,128);
  analogWrite(pwm2,128);
}
void Stop()
{
  analogWrite(l1,0);
  analogWrite(l2,0);
  analogWrite(r1,0);
  analogWrite(r2,0);
  analogWrite(pwm1,0);
  analogWrite(pwm2,0);
}
void Left()
{analogWrite(l1,0);
analogWrite(l2,255);
  analogWrite(r1,255);
  analogWrite(r2,0);
  analogWrite(pwm1,90);//was 80
  analogWrite(pwm2,90);//was 80
  
}
void Right()
{
  analogWrite(l1,255);
  analogWrite(l2,0);
  analogWrite(r1,0);
  analogWrite(r2,255);
analogWrite(pwm1,90);//was 80
analogWrite(pwm2,90);//was 80
}
void loop()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  //  qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
  unsigned int position = qtrrc.readLine(sensorValues);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  //Serial.println(); // uncomment this line if you are using raw values
  Serial.print(position); // comment this line out if you are using raw values
  Serial.print('\t');
  //delay(250);
  //pid(position);
  error=position-2500;
 derivative=(error-lasterror);
 some=Kp*(error)+Kd*(derivative);
 lasterror=error;
  lrs=lms+some;
  rrs=rms-some;
  Serial.print(error);
  Serial.print('\t');
  if(lrs>maxlrs)
  lrs=maxlrs;
  if(rrs>maxrrs)
  rrs=maxrrs;
  if(rrs<0)
  rrs=0;
  if(lrs<0)
  lrs=0;
  Serial.print(lrs); 
  Serial.print('\t');
  Serial.println(rrs);
 //forward();
  analogWrite(11,lrs);
  analogWrite(10,rrs);
  analogWrite(l1,130);//was 255//was 150
  analogWrite(l2,0);
  analogWrite(r1,130);//was 150
  analogWrite(r2,0);

  
//if((sensorValues[2]>600 && sensorValues[3]>600)&&(sensorValues[1]<200 && sensorValues[4]<200)&&(sensorValues[5]<200 && sensorValues[0]<200))
//{
// position = qtrrc.readLine(sensorValues);
//Forward();
// }
//  if((sensorValues[2]>500||sensorValues[3]>500)&&(sensorValues[0]<200||sensorValues[1]<200)&&(sensorValues[4]>500||sensorValues[5]>500))
//{
//  
//  Forward();
//delay(20);
//position = qtrrc.readLine(sensorValues);
//}
//if((sensorValues[2]>600 && sensorValues[3]>600)&&(sensorValues[1]<200 && sensorValues[4]<200)&&(sensorValues[5]<200 && sensorValues[0]<200))
//{
//Forward();
// }
//}
// if((sensorValues[2]>500||sensorValues[3]>500)&&(sensorValues[0]<200||sensorValues[1]<200)&&(sensorValues[4]>500||sensorValues[5]>500))
//{
// Forward();
//}
//delay(20);
//position = qtrrc.readLine(sensorValues);
//if((sensorValues[2]<200 && sensorValues[3]<200)&&(sensorValues[0]<200 && sensorValues[5]<200)&&(sensorValues[1]<200 && sensorValues[4]<200))
//{
//Right();
//delay(100);
//position = qtrrc.readLine(sensorValues);
//Stop();
//delay(1000);
//while(!(sensorValues[2]>600 || sensorValues[3]>600))
//{
//Right();
//position = qtrrc.readLine(sensorValues);
//}}
//if((sensorValues[2]>600 && sensorValues[3]>600)&&(sensorValues[0]>600 && sensorValues[1]>600))     //condition for left
//{
//  Stop();
//  delay(1000);
//Left();
//delay(200);
//position = qtrrc.readLine(sensorValues);
//while(!(sensorValues[2]>600 || sensorValues[3]>600))
//{
//Left();
//position = qtrrc.readLine(sensorValues);
//}
//}
//********************************************************************************************************
if((sensorValues[2]>black &&sensorValues[3]>black)&&(sensorValues[0]>black && sensorValues[1]>black)&&(sensorValues[4]>black && sensorValues[5]>black))
{
  Forward();
  delay(10);//was 50//was 20
  if((sensorValues[2]>black &&sensorValues[3]>black)&&(sensorValues[0]>black && sensorValues[1]>black)&&(sensorValues[4]>black && sensorValues[5]>black))
Stop();
else
 {
  Left();
delay(10);//was 30
position = qtrrc.readLine(sensorValues);
  while(!((sensorValues[2]>black||sensorValues[3]>black)&&(sensorValues[0]<white && sensorValues[5]<white)))//extra condition added
 {
  Left();
  position = qtrrc.readLine(sensorValues);
 }
}

}

else if((sensorValues[2]>black||sensorValues[3]>black)&&(sensorValues[0]>black||sensorValues[1]>black)) 
{
//Forward();
//delay(10);  //was 20 before
Left();
delay(30);  //was 110 before//70 before(not working)//
//Stop();
//delay(1000);
//Left();
//delay(200);
position = qtrrc.readLine(sensorValues);
while(!((sensorValues[2]>black||sensorValues[3]>black)&&(sensorValues[0]<white && sensorValues[5]<white)))//extra condition added
{
Left();
position = qtrrc.readLine(sensorValues);
}
}


else if((sensorValues[2]>black||sensorValues[3]>black)&&(sensorValues[0]<white||sensorValues[1]<white)&&(sensorValues[4]>black||sensorValues[5]>black))
{
Forward();
delay(20); 
position = qtrrc.readLine(sensorValues);
if((sensorValues[2]<white && sensorValues[3]<white)&&(sensorValues[0]<white && sensorValues[5]<white)&&(sensorValues[1]<white && sensorValues[4]<white))
{
  while(!((sensorValues[2]>black||sensorValues[3]>black)&&(sensorValues[0]<white && sensorValues[5]<white)))//extra condition added
 {
  Right();
  position = qtrrc.readLine(sensorValues);
 }

}
}


}

  
