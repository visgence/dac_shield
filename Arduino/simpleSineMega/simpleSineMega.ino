/////////////////////////////////////////////////////////////////////////////////
//Sine Table Lookup
//This program creates a sine wave using the sin function
///2014 Visgence Inc
/////////////////////////////////////////////////////////////////////////////////
#include <SPI.h>

//Use direct port write for aditional speed over DigitalWrite
#define SS0 (1 << 4) //Slave Select 0 PORTB
#define SS0PORT &PORTB   

#define SS1 (1 << 6) //Slave Select 1 PORTH
#define SS1PORT &PORTH

#define SS2 (1 << 5) //Slave Select 2 PORTH
#define SS2PORT &PORTH

#define SS3 (1 << 4) //Slave Select 2 PORTH
#define SS3PORT &PORTH

//Pins needed for Soft SPI
#define MOSI 11
#define CLK  13

int i;


void setup() {
  // set the slaveSelectPin as an output:

  pinMode (MOSI,OUTPUT);
  pinMode (CLK,OUTPUT);
  pinMode (10, OUTPUT);
  pinMode (11, OUTPUT);
  i=0;
}

void loop() {
  
  for(i=0;i<0x4096;i++) {
     //Digital Write
     //writeMCP492x((int)(sin(2.0 * 3.14159 * double(i)/double(4095)) *(4096/2) + (4096/2)),10);
     
     //DriectPort  
     writeMCP492x((int)(sin(2.0 * 3.14159 * double(i)/double(4095)) *(4096/2) + (4096/2)),SS0,SS0PORT);

  }
}


//Method to write to the DAC
void writeMCP492x(uint16_t data,uint8_t ss,volatile uint8_t* slave_port) {
  // Take the top 4 bits of config and the top 4 valid bits (data is actually a 12 bit number) and or them together
  uint8_t top_msg = (0x30 & 0xF0) | (0x0F & (data >> 8));
  
  // Take the bottom octet of data
  uint8_t lower_msg = (data & 0x00FF);
   
  // Select our DAC
  *slave_port &= ~ss;
  // Send first 8 bits
  shiftOut(MOSI,CLK,MSBFIRST,top_msg);
  // Send second 8 bits
  shiftOut(MOSI,CLK,MSBFIRST,lower_msg);
  *slave_port |= ss;
}


