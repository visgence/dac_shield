
/////////////////////////////////////////////////////////////////////////////////
//Sine Table Lookup
//This program creates a sine wave using a lookup table
///2014 Visgence Inc
/////////////////////////////////////////////////////////////////////////////////
#include <SPI.h>

#define SS0 (1 << 2) //Slave Select 0 PORTB
#define SS0PORT &PORTB   

#define SS1 (1 << 1) //Slave Select 1 PORTB
#define SS1PORT &PORTB

#define SS2 (1 << 0) //Slave Select 2 PORTB
#define SS2PORT &PORTB

#define SS3 (1 << 7) //Slave Select 2 PORTD
#define SS3PORT &PORTD

prog_uchar sinetable[256] PROGMEM = {
  128,131,134,137,140,143,146,149,152,156,159,162,165,168,171,174,
  176,179,182,185,188,191,193,196,199,201,204,206,209,211,213,216,
  218,220,222,224,226,228,230,232,234,236,237,239,240,242,243,245,
  246,247,248,249,250,251,252,252,253,254,254,255,255,255,255,255,
  255,255,255,255,255,255,254,254,253,252,252,251,250,249,248,247,
  246,245,243,242,240,239,237,236,234,232,230,228,226,224,222,220,
  218,216,213,211,209,206,204,201,199,196,193,191,188,185,182,179,
  176,174,171,168,165,162,159,156,152,149,146,143,140,137,134,131,
  128,124,121,118,115,112,109,106,103,99, 96, 93, 90, 87, 84, 81, 
  79, 76, 73, 70, 67, 64, 62, 59, 56, 54, 51, 49, 46, 44, 42, 39, 
  37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 18, 16, 15, 13, 12, 10, 
  9,  8,  7,  6,  5,  4,  3,  3,  2,  1,  1,  0,  0,  0,  0,  0,  
  0,  0,  0,  0,  0,  0,  1,  1,  2,  3,  3,  4,  5,  6,  7,  8,  
  9,  10, 12, 13, 15, 16, 18, 19, 21, 23, 25, 27, 29, 31, 33, 35, 
  37, 39, 42, 44, 46, 49, 51, 54, 56, 59, 62, 64, 67, 70, 73, 76, 
  79, 81, 84, 87, 90, 93, 96, 99, 103,106,109,112,115,118,121,124
};



void setup() {
  // set the slaveSelectPin as an output:

  pinMode (10, OUTPUT);
  pinMode (11, OUTPUT);
  
  // initialize SPI:
  SPI.begin(); 
  SPI.setDataMode(SPI_MODE0);
}

unsigned char i;
void loop() {
  
  for(i=0;i<255;i++) {
     //Digital Write
     //writeMCP492x((int) pgm_read_word_near(sinetable + i) * 16,10);
     
     //DriectPort  
     writeMCP492x((int) pgm_read_word_near(sinetable + i) * 16,SS0,SS0PORT);

  }

}

//Method to write to the DAC,using  digitalWrite for slave select
void writeMCP492x(uint16_t data,uint8_t slave_select) {
  // Take the top 4 bits of config and the top 4 valid bits (data is actually a 12 bit number) 
  //and OR them together
  uint8_t top_msg = (0x30 & 0xF0) | (0x0F & (data >> 8));

  // Take the bottom octet of data
  uint8_t lower_msg = (data & 0x00FF);

  // Select our DAC, Active LOW
  digitalWrite(slave_select, LOW);

  // Send first 8 bits
  SPI.transfer(top_msg);
  // Send second 8 bits
  SPI.transfer(lower_msg);

  //Deselect DAC
  digitalWrite(slave_select, HIGH);
}


//Method to write to the DAC, using direct port for slave select
void writeMCP492x(uint16_t data,uint8_t ss,volatile uint8_t* slave_port) {
  // Take the top 4 bits of config and the top 4 valid bits (data is actually a 12 bit number) 
  //and OR them together
  uint8_t top_msg = (0x30 & 0xF0) | (0x0F & (data >> 8));

  // Take the bottom octet of data
  uint8_t lower_msg = (data & 0x00FF);

  // Select our DAC, Active LOW
  *slave_port &= ~ss;

  // Send first 8 bits
  SPI.transfer(top_msg);
  // Send second 8 bits
  SPI.transfer(lower_msg);

  //Deselect DAC
  *slave_port |= ss;
}


