const int clockPin = 2;
const int dataPin = 3;
const int latchPin = 7;
const int outputenablePin = 9;

int digittable[10];
const int dig1count=9;
const int dig2count=5;
unsigned long intervall = 1000;


int dig1value=0;
int dig2value=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(outputenablePin,OUTPUT);
  digitalWrite(clockPin, LOW);
  digitalWrite(dataPin,LOW);
  digitalWrite(latchPin, HIGH);
  digitalWrite(outputenablePin, HIGH);
  
}

void update_value()  {
  dig1value=dig1value+1;
  if (dig1value>dig1count) {
    dig1value=0;
    dig2value=dig2value+1;
    }
  if (dig2value>dig2count) {
    dig2value=0;
    }
  }

void loop() {

  //displayvalue=1;
  update_display();

  delay(500);
  
  delay(500);
  update_value();
  
  // put your main code here, to run repeatedly:

}

void update_display() {
 int encodedvalue;
 table_construct();
 encodedvalue=digittable[dig2value];
 digitalWrite(latchPin,LOW);
 
 if (dig2value==0) encodedvalue=0;
 shiftOut(dataPin, clockPin, LSBFIRST, encodedvalue);

 encodedvalue=digittable[dig1value];
 shiftOut(dataPin, clockPin, LSBFIRST, encodedvalue);

  digitalWrite(latchPin,HIGH);
  digitalWrite(outputenablePin,LOW);
  
  }
void table_construct(){
  int SegA=128;
  int SegB=64;
  int SegC=32;
  int SegD=16;
  int SegE=8;
  int SegF=4;
  int SegG=2;
  int SegDP=1;
  digittable[0]=SegA+SegB+SegC+SegD+SegE+SegF;
  digittable[1]=SegF+SegE;
  digittable[2]=SegA+SegF+SegG+SegC+SegD;
  digittable[3]=SegA+SegD+SegE+SegF+SegG;
  digittable[4]=SegB+SegG+SegF+SegE;
  digittable[5]=SegA+SegB+SegG+SegE+SegD;
  digittable[6]=255-SegF;
  digittable[7]=SegA+SegF+SegE;
  digittable[8]=255;
  digittable[9]=255-SegC;
