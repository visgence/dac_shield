/////////////////////////////////////////////////////////////////////////////////
//slowSquare
//This program generates a 0.5Hz square wave
//It can be used for testing the DAC shield using a multimeter
//Set on DC Volts, the multimeter should alternately read V+ and V-,
//changing every 2 seconds
//
///(c) 2015 Visgence Inc
/////////////////////////////////////////////////////////////////////////////////
#include <SPI.h>
#define SS1 (1 << 2) //Slave Select 1 PORTB
#define SS1PORT &PORTB   

#define SS2 (1 << 1) //Slave Select 2 PORTB
#define SS2PORT &PORTB

#define SS3 (1 << 0) //Slave Select 3 PORTB
#define SS3PORT &PORTB

#define SS4 (1 << 7) //Slave Select 4 PORTD
#define SS4PORT &PORTD

const int ss1 = 10;
const int ss2 = 9;
const int ss3 = 8;
const int ss4 = 7;

void setup() {
  // set the slaveSelectPins as an output:
  pinMode (ss1, OUTPUT);
  pinMode (ss2, OUTPUT);
  pinMode (ss3, OUTPUT);
  pinMode (ss4, OUTPUT);
  
  // initialize SPI:
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
}

void loop() {
  //Generate 1khz square wave using digitalWrite Slave Select
  
  //writeMCP492x(4095,ss1); //Write max voltage to shield 1
  //delayMicroseconds(500);
  //writeMCP492x(0,ss1); //Write min voltage to shield 1
  //delayMicroseconds(500);
  
  //Generate 1khz square wave using direct port writing
  writeMCP492x(4095,SS1,SS1PORT); //Write max voltage to shield 0
  delaySeconds(2);
  writeMCP492x(0,SS1,SS1PORT); //Write min voltage to shield 0
  delaySeconds(2);
}

//method to delay for a certain number of seconds
void delaySeconds(uint16_t seconds) {
  uint16_t i = 0;
  uint16_t j = 0;
  for(i = 0; i < seconds; i++) {
    for(j = 0; j < 1000; j++) {
      delayMicroseconds(1000);
    } 
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
