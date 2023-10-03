#include <Servo.h> 
int servoPin = 10; 
Servo Servo1; 
double err[3];
double *resulterr;
int trigpin_fr = 5;
int echopin_fr = 4;
int trigpin_fl = 9;
int echopin_fl = 8;
int trigpin_br = 2;
int echopin_br = 3;
int trigpin_bl = 6;
int echopin_bl = 7;
const int in1 = 12;
const int in2 = 13;
int pwma=11;
double eout[3];
long duration;
float average;
float diff;
double ki = 0;
double kd = 0;
double kp = 1.5;
unsigned long currentTime, previousTime;
double elapsedTime;
double error[5],cumerror[5],raterror[5];
double lastError = 0;
double input, setPoint;
int output;
double Error,cumError, rateError;
int avgl;
int avgr;
void setup() {
    Servo1.attach(servoPin); 
    pinMode(trigpin_fr,OUTPUT);
    pinMode(echopin_fr,INPUT);
    pinMode(trigpin_fl,OUTPUT);
    pinMode(echopin_fl,INPUT);
    pinMode(trigpin_br,OUTPUT);
    pinMode(echopin_br,INPUT);
    pinMode(trigpin_bl,OUTPUT);
    pinMode(echopin_bl,INPUT);
    pinMode(pwma,OUTPUT) ;    
    pinMode(in1,OUTPUT);
    pinMode(in2,OUTPUT) ;
    Servo1.write(150);    
    Serial.begin(9600); 
}
void loop() {
digitalWrite(in1,LOW);
digitalWrite(in2,HIGH);
analogWrite(pwma,90);
int j=0;
while(j<3){
int dfr= sensor(trigpin_fr,echopin_fr);
int dfl= sensor(trigpin_fl,echopin_fl);
int dbr= sensor(trigpin_br,echopin_br);
int dbl= sensor(trigpin_bl,echopin_bl);
//Serial.println(dbr);
avgr=(dfr+dbr)/2;
avgl = (dfl +dbl)/2;
input = abs(avgr-avgl);
resulterr = computError(input);
error[j] =  *(resulterr+0);
cumerror[j] = *(resulterr+1);
raterror[j] = *(resulterr+2);
++j;}
int len = sizeof(error)/sizeof(error[0]);
int len1 = sizeof(cumerror)/sizeof(cumerror[0]);
int len2 = sizeof(raterror)/sizeof(raterror[0]);
qsort(error,len,sizeof(error[0]),sort_desc);
qsort(cumerror,len1,sizeof(cumerror[0]),sort_desc);
qsort(raterror,len2,sizeof(raterror[0]),sort_desc);
//Serial.println(error[2]);
output = computPID(error[1],cumerror[1],raterror[1]);
Serial.println(output);
if((avgr-avgl)>0)  // turn right
 {
   if(output>20)
     {
        output=20;
     }
     if(output>15)
     {
          analogWrite(pwma,105);
     }
  Servo1.write(150-output);
 }
if(avgl-avgr>0) // turns left
 {
  if(output>20)
   {
      output= 20;
   }
   if(output>15)
     {
          analogWrite(pwma,105);
     }
   Servo1.write(150+output); 
 }

}
int sensor(int trig,int echo){
 int distance = 0;
 digitalWrite(trig,LOW);
 delay(10);
 digitalWrite(trig,HIGH);
 delay(10);
 digitalWrite(trig,LOW);
 duration = pulseIn(echo,HIGH);
 distance = duration*(0.034/2);
 return distance;
}
double* computError(double inp){ 
//        int Setpoint =0 ; 
        currentTime = millis();               
        elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
        
        Error = inp;                                // determine error
        cumError += Error * elapsedTime;
        if(cumError>100)
            cumError = int(cumError)%100;
        rateError = (Error - lastError)/elapsedTime;                
        previousTime = currentTime; 
        lastError = Error;  
          err[0]=Error;
          err[1]= cumError;
          err[3]=rateError;
          return err;                          
}
int computPID(double error,double cumError,double rateError)
{
       double out = kp*error + ki*cumError + kd*rateError; 
       if(out>=255){
          out = 255;
         }
       return int(out); 
}
int sort_desc(const void *cmp1, const void *cmp2)
{
  // Need to cast the void * to int *
  int a = *((int *)cmp1);
  int b = *((int *)cmp2);
  // The comparison
  return a > b ? -1 : (a < b ? 1 : 0);
  // A simpler, probably faster way:
  //return b - a;
}
