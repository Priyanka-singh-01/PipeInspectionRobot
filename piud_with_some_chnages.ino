#include <Servo.h> 
int servoPin = 10; 
Servo Servo1; 
int trigpin_fr = 3;
int echopin_fr = 2;
int trigpin_fl = 8;
int echopin_fl = 9;
int trigpin_br = 5;
int echopin_br = 4;
int trigpin_bl = 7;
int echopin_bl = 6;
const int in1 = 12;
const int in2 = 13;
int pwma=11;
int l =0;
//const int in3 = 9;
//const int in4 = 8;
//int pwmb=5;
long duration;
double ki = 0;
double kd = .5;
int kp = 1.6;
unsigned long currentTime, previousTime;
double elapsedTime;
double lastError = 0;
double input, setPoint;
int output;
int Error;
double cumError, rateError;
int avgl;
int avgr;
int sum = 0 ;
int ovalue[3]={0,0,0};
int ovalue1[3]={0,0,0};
int nvalue1=0;
int nvalue2=0;
int nvalue3 =0;
int nvalue4 =0;
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
    analogWrite(pwma,95);
    Servo1.write(150); 

}

void loop(){ 
int dfr= sensor(trigpin_fr,echopin_fr);
int dfl= sensor(trigpin_fl,echopin_fl);
int dbr= sensor(trigpin_br,echopin_br);
int dbl= sensor(trigpin_bl,echopin_bl);
nvalue1 = dfr;
nvalue2 = dbr;
nvalue3 = dfl;
nvalue4= dbl;
//right side
int oval_mean = (ovalue[0]+ovalue[1]+ovalue[2])/3;

if(abs(oval_mean-nvalue1)>abs(oval_mean-nvalue2))
{ 
  for (int i=0; i<2;i++)
{
    ovalue[i]=ovalue[i+1];
}
  ovalue[2]= nvalue2;
}

if(abs(oval_mean-nvalue1)<abs(oval_mean-nvalue2))
{
  for (int i=0; i<2;i++)
{
    ovalue[i]=ovalue[i+1];
}
  ovalue[2]= nvalue1;
}
//  left side
int oval_mean1 = (ovalue1[0]+ovalue1[1]+ovalue1[2])/3;

if(abs(oval_mean1-nvalue3)>abs(oval_mean1-nvalue4))
{ 
  for (int i=0; i<2;i++)
{
    ovalue1[i]=ovalue1[i+1];
}
  ovalue1[2]= nvalue4;
}

if(abs(oval_mean1-nvalue3)<abs(oval_mean1-nvalue4)){
  for (int i=0; i<2;i++)
{
    ovalue1[i]=ovalue1[i+1];
}
  ovalue1[2]= nvalue3;}
digitalWrite(in1,LOW);
digitalWrite(in2,HIGH);
analogWrite(pwma,95);
avgr=ovalue[2];
avgl =ovalue1[2] ;
input = abs(avgr-avgl);
output = computError(input);
Serial.print(" bL: ");
Serial.print(avgl);
Serial.print(" bR: ");
Serial.print(avgr);
Serial.print(" fr: ");
Serial.print(dbr);
Serial.print(" fL: ");
Serial.print(dfl);

Serial.println(" ");
if(avgr-avgl>0)
{
   if(output>25)
     {
        output=25;
     }
  Servo1.write(150-output);
  if(output>15)
  {
    analogWrite(pwma,130);
  } 
}
if(avgl-avgr>0)
{
   if(output>25)
   {
      output= 25;
   }
     if(output>15)
  {
    analogWrite(pwma,130);
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
}
int sensor(int trig,int echo){
 int distance = 0;
 digitalWrite(trig,LOW);
 delay(5);
 digitalWrite(trig,HIGH);
 delay(5);
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
       if(out>=25){
          out = 25;
         }
       return int(out);                          
}
