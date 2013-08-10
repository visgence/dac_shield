// inslude the SPI library:
#include <SPI.h>
#include <math.h>

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;

void setup() {
  // set the slaveSelectPin as an output:

  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin(); 
  SPI.setDataMode(SPI_MODE0);
  //SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}

int i;
void loop() {
 
  int config = 0x30;
  int data = 0;
  
  
  
  for(i=0;i<0xFFF;i++) {
  
    data= (int) (sin(2.0 * 3.14159 * double(i)/double(4095)) *(4096/2) + (4096/2));
    writeMCP492x(data, 0x30);
    //delayMicroseconds(10);
}
  //for(i=0xFFF;i>0;i--) {
    //writeMCP492x(i, 0x30);
    //delayMicroseconds(10);
//}


}

  



void writeMCP492x(uint16_t data, uint8_t config) {
  // Take the top 4 bits of config and the top 4 valid bits (data is actually a 12 bit number) and or them together
  uint8_t top_msg = (config & 0xF0) | (0x0F & (data >> 8));
  
  // Take the bottom octet of data
  uint8_t lower_msg = (data & 0x00FF);
   
  // Select our DAC
  digitalWrite(slaveSelectPin,LOW);
  // Send first 8 bits
  SPI.transfer(top_msg);
  // Send second 8 bits
  SPI.transfer(lower_msg);
  digitalWrite(slaveSelectPin,HIGH);
}


