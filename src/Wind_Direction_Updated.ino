#include <ModbusMaster.h>
#include <SoftwareSerial.h>

/* 
    Using the RS485 transceiver module, Rx/Tx is hooked up to the hardware
    serial port at 'Serial" to DI (driver input, connected to Tx) and RO 
    (receiver ouput, connected to Rx). 
    DE and RE are shorted together and connected to pin 4
*/
// shorting DE and RE together 
#define MAX485_DE_RE 4

// instantiate the ModbusMaster object
ModbusMaster node;

// configure serial communication pins for wind vane
SoftwareSerial mySerial(10,11); // RX, TX

void preTransmission()
{
  digitalWrite(MAX485_DE_RE, 1);
}

void postTransmission() 
{
  digitalWrite(MAX485_DE_RE, 0);
}

void setup()
{
  pinMode(MAX485_DE_RE, OUTPUT);
  // initialize in receive mode
  digitalWrite(MAX485_DE_RE, 0);

  // Serial communication for printing to serial monitor
  Serial.begin(9600);

  // Serial communication for transmitting and receiving data from wind vane
  mySerial.begin(9600);

  // Modbus slave ID of wind vane is 2 
  node.begin(2, mySerial);

  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}


void loop() 
{
  uint8_t result;

  // read wind direction value at register 0x0000
  result = node.readHoldingRegisters(0x0000, 1);
  if (result == node.ku8MBSuccess) 
  {
    Serial.print("Wind direction (0 is due north): "); 
    Serial.print(node.getResponseBuffer(0x0)/10.0f); 
    Serial.println(" degrees");
  }

  Serial.println();
  delay(1000);



}



