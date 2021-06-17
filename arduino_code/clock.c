const int clockPin = 2; //Arduino Pin2 //Will be passed as parameter to the Shiftout function that takes care of Serializing the memorybuffy to the shift registerICs
                        //Transition from LOW to HIGH will trigger the shiftregister/led driver ic logic that is designed as follows :
                        //all registerstorage bits (16 in our case) pass their information to the next registerstorage,
                        //serial data is read (one bit at a time) from serialdatainput pin and passed to first memorybit of the register
                        //the information from the last registermemorybit is displayed on serialdata output pin for cascading multiple ICs
                        //latchespin state is not relevant for the shift logic.
                        
                       
const int dataPin = 3; //same but referes to the databit that is displayed at the first shiftregister serialdata input. will be processed when clockpin transitions from LOW to HIGH

const int latchPin = 7; //When HIGH the Shiftregister memory is stored into the latch memory. set it LOW before shifting the full 32bit display memory.
const int outputenablePin = 9; //when set to HIGH the high current led driver outputs will be turned off. (active LOW)
                               //when set to LOW the output driver pin will be active, depending on the information stored in the registers latch memory.


const int scale=50;   //the higher the faster the time is running :-) for fun and experimenting.
const int hours_offset=0; // to set the time, offsets are user  put your local time(hours) here. 0-24.
const int minutes_offset=0; // replace the 0 by whatever your local time is when you run the code.

byte truthtable[10]; //array of type bytes. 
                      //combinatorically mapping int digits(0-9) to construct a bitorder 7 corresponding segments and a zero bit
                      //will be populated by table_construct function when initializing
                      
byte memory[4]={0,0,0,0}; //(4 bytes of memory. will be shifted out when updating_display is called will be passed to the external 7segment circuit
unsigned long millis_initial; //inital timestamp in milliseconds when initializing the clock
unsigned long millis_now=0; //will be used to calculate the time that has passed sind initializing in 
unsigned int hours; // will be calculated from timestamp
unsigned int minutes;
unsigned int seconds;

void setup(){ 
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(outputenablePin,OUTPUT);
  digitalWrite(clockPin, HIGH); // to initialize the shift register logic
  digitalWrite(dataPin,LOW); // there is no data yet
  digitalWrite(latchPin, LOW); // and so is nothing to latch
  digitalWrite(outputenablePin, HIGH); //and no leds

  table_construct();

  writetomemory(0,0,0,0);
  update_display(); //to clear potential random bytes in the external display
  digitalWrite(outputenablePin, LOW); //turn on the drivers
  
  
}
void update_display() {

  digitalWrite(latchPin,LOW);// (initializing the shifting action)  
  digitalWrite(clockPin,LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, memory[0]);
  shiftOut(dataPin, clockPin, LSBFIRST, memory[1]);
  shiftOut(dataPin, clockPin, LSBFIRST, memory[2]);
  shiftOut(dataPin, clockPin, LSBFIRST, memory[3]);
 
  digitalWrite(latchPin,HIGH); //turn on the lights
}

//i didnt get the array syntax right; thats why this function is here
void writetomemory(byte wert0,byte wert1,byte wert2,byte wert3) {
  memory[0]=wert0;
  memory[1]=wert1;
  memory[2]=wert2;
  memory[3]=wert3;
}

void loop() {

//get the timestamp
  millis_now=millis();
//keeping track of time
unsigned long clocktime=((millis_now-millis_initial));

  clocktime=clocktime*scale;

//extract human format and consider the user defined offsets
  seconds=clocktime / 1000;
  minutes=seconds /60;
  hours=minutes /60;
  seconds=seconds % 60;
  minutes=(minutes+minutes_offset)% 60;
  hours=(hours+hours_offset) % 24;

//some math and type conversion to finalle have 6 values for each digit of human format
  
int hours_lo=hours % 10;
int hours_hi=hours / 10;
int seconds_lo=seconds % 10;
int seconds_hi=seconds /10;
int minutes_lo=minutes % 10;
int minutes_hi=minutes /10;
byte hours_lo2=lowByte(hours_lo);
byte hours_hi2=lowByte(hours_hi);
byte minutes_lo2=lowByte(minutes_lo);
byte minutes_hi2=lowByte(minutes_hi);
byte seconds_lo2=lowByte(seconds_lo);
byte seconds_hi2=lowByte(seconds_hi);

//map the digits to the encoding truth table

byte h_hi=truthtable[hours_hi2];
byte h_lo=truthtable[hours_lo2];
byte m_hi=truthtable[minutes_hi2];
byte m_lo=truthtable[minutes_lo2];
byte s_hi=truthtable[seconds_hi2];
byte s_lo=truthtable[seconds_lo2];
  
writetomemory(h_hi,h_lo,m_hi,m_lo); //write hours and minutes to the prepared memory
  update_display(); //update the external blabla

}

void table_construct(){
  byte SegA=128;
  byte SegB=64;
  byte alt_B=2;
  byte SegC=32;
  byte SegD=16;
  byte SegE=8;
  byte SegF=4;
  byte SegG=2;
  byte alt_G=64;
  byte SegDP=1;
  truthtable[0]=255-2;
  truthtable[1]=SegF+SegE;
  truthtable[2]=SegA+SegF+SegG+SegC+SegD;
  truthtable[3]=SegA+SegD+SegE+SegF+SegG;
  truthtable[4]=SegB+SegG+SegF+SegE;
  truthtable[5]=SegA+SegB+SegG+SegE+SegD;
  truthtable[6]=255-SegF;
  truthtable[7]=SegA+SegF+SegE;
  truthtable[8]=255;
  truthtable[9]=SegA+SegB+SegD+SegE+SegF+SegG;
}
