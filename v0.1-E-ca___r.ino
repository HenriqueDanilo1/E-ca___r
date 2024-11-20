#include <Servo.h>

#define d_ang 28
#define rot_ang 28 //cant be smaller than ang_d

Servo d_axle; //directional axle
int tl_led = 2; // turn left led
int tr_led = 3; //turn right led
int tl_inp = 10; //turn left inp
int tr_inp = 11; //turn right inp
int a_inp = 12; //alert inp
int a_stage = 0; //alert stage

int cts = 0; //current turn signal
int tss = 0; //turn signal stage
int ctsir = 0;//current turn signal for input right
int ctsil = 0; //current turn signal for input left

void setup() {
  Serial.begin(9600);
  pinMode(tl_led, OUTPUT); //2
  pinMode(tr_led, OUTPUT); //3
  d_axle.attach(4); //4
  pinMode (tl_inp, INPUT); //5
  pinMode (tr_inp, INPUT); //6
  pinMode(8, INPUT); //8 turn right
  pinMode(9, INPUT); //9 turn left
  
  d_axle.write(d_ang);
}

//activate turn signal
void t_signal() {
  //activate turn left signal
  if(digitalRead(tl_inp) && cts != 1 && ctsil == 0) { 
    cts = 1;
    digitalWrite(tl_led, HIGH);
    digitalWrite(tr_led, LOW);
    ctsil = 1;
    ctsir = 0;
  } else if (digitalRead(tl_inp) && cts == 1 && ctsil == 2) {
    cts = 0;
    digitalWrite(tl_led, LOW);
  }
  //activate turn right signal
  if(digitalRead(tr_inp) && cts != 2 && ctsir == 0) { 
    cts = 2;
    digitalWrite(tl_led, LOW);
    digitalWrite(tr_led, HIGH);
    ctsir = 1;
    ctsil = 0;
  } else if (digitalRead(tr_inp) && cts == 2 && ctsir == 2) {
    cts = 0;
    digitalWrite(tr_led, LOW);
  }
  //anti-flick system
  if(!digitalRead(tr_inp) && ctsir == 1) { ctsir = 2;}
  if(!digitalRead(tr_inp) && cts == 0) {ctsir = 0;}
  if(!digitalRead(tl_inp) && ctsil == 1) { ctsil = 2;}
  if(!digitalRead(tl_inp) && cts == 0) {ctsil = 0;}
  
  //disactivate turn signal
  if(!digitalRead(9) && !digitalRead(8) && tss == 0 && cts != 0) {tss = 1;}
  if(digitalRead(8) && tss == 1 && cts == 2) {tss = 2;}
  if(digitalRead(9) && tss == 1 && cts == 1) {tss = 3;}
  if(!digitalRead(9) && !digitalRead(8) && tss == 2) {
    digitalWrite(tr_led, LOW);
    cts = 0;
    tss = 0;
    ctsil = 0;
    ctsir = 0;
  }
  if(!digitalRead(9) && !digitalRead(8) && tss == 3) {
    digitalWrite(tl_led, LOW);
    cts = 0;
    tss = 0;
    ctsil = 0;
    ctsir = 0;
  }
}
//Servo rotation for directional
void directional() {
  if(digitalRead(8) && !digitalRead(9)) {
    d_axle.write(d_ang+rot_ang); //turn right
  }
  if(digitalRead(9) && !digitalRead(8)) {
    d_axle.write(d_ang-rot_ang); //turn left
  }
  if(!digitalRead(8) && !digitalRead(9)) {
    d_axle.write(d_ang); //back to default pos
  }
}

//alert
void alert() {
  if(digitalRead(a_inp) && a_stage == 0) {
    int cts = 0;
    int tss = 0;
    int ctsir = 0;
    int ctsil = 0; 
    digitalWrite(tl_led, HIGH);
    digitalWrite(tr_led, HIGH);
   a_stage = 1;
  }
  if(!digitalRead(a_inp) && a_stage == 1) {
  	a_stage = 2;
  }
  if(digitalRead(a_inp) && a_stage == 2) {
    digitalWrite(tl_led, LOW);
    digitalWrite(tr_led, LOW);
	a_stage = 3;
  }
  if(!digitalRead(a_inp) && a_stage == 3) {
    a_stage = 0;
  }
}

void loop() {
  directional();
  t_signal();
  alert();
}
