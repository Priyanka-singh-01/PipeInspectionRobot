#include <Servo.h> 
int servoPin = 10; 
Servo Servo1; 
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
//const int in3 = 9;
//const int in4 = 8;
//int pwmb=5;
long duration;
double ki = 0;
double kd = 0;
int kp = 1.8;
unsigned long currentTime, previousTime;
double elapsedTime;
double lastError = 0;
double input, setPoint;
int output;
int Error;
double cumError, rateError;
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
//    pinMode(pwmb,OUTPUT) ;    
//    pinMode(in3,OUTPUT);
//    pinMode(in4,OUTPUT) ;
    Serial.begin(9600); 
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    analogWrite(pwma,90);
    Servo1.write(150); 

}

void loop(){ 
int dfr= sensor(trigpin_fr,echopin_fr);
int dfl= sensor(trigpin_fl,echopin_fl);
int dbr= sensor(trigpin_br,echopin_br);
int dbl= sensor(trigpin_bl,echopin_bl);
digitalWrite(in1,LOW);
digitalWrite(in2,HIGH);
analogWrite(pwma,90);
avgr=(dfr+dbr)/2;
avgl = (dfl +dbl)/2;
input = abs(avgr-avgl);
output = computError(input);
Serial.print(" bL: ");
Serial.print(dbl);
Serial.print(" bR: ");
Serial.print(dbr);
Serial.print(" fr: ");
Serial.print(dfr);
Serial.print(" fL: ");
Serial.print(dfl);
Serial.println();
if(avgr-avgl>0)
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
if(avgl-avgr>0)
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
//if((avgr-avgl)>3)  // turn right
// {
//  digitalWrite(in3,LOW);
//  digitalWrite(in4,HIGH);
//  analogWrite(pwma,165);
// }
// else if(avgl-avgr>3) // turns left
// {
//  digitalWrite(in3,HIGH);
//  digitalWrite(in4,LOW);
//  analogWrite(pwma,165);
// }
// else
// {
//  digitalWrite(in3,HIGH);
//  digitalWrite(in4,HIGH);
//  analogWrite(pwma,150);
// }
delay(10);   
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
int computError(double inp){ 
        int Setpoint =0 ; 
        currentTime = millis();               
        elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
        
        Error = inp-Setpoint;                                // determine error
        cumError += Error * elapsedTime;
        if(cumError>100)
            cumError = int(cumError)%100;
        rateError = (Error - lastError)/elapsedTime;                
        previousTime = currentTime; 
        lastError = Error;  
        double out = kp*Error + ki*cumError + kd*rateError; 
       if(out>=20){
          out = 20;
         }
       return int(out);                          
}
