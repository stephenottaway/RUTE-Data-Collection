#include <ModbusMaster.h>

/* 
    Using the RS485 transceiver module, Rx/Tx is hooked up to the hardware
    serial port at 'Serial" to DI (driver input, connected to Tx) and RO 
    (receiver ouput, connected to Rx). 
    The data enable and receiver enable pins are connected as follows:
*/
#define MAX485_DE 4
#define MAX485_RE_NEG 5

// instantiate the ModbusMaster object
ModbusMaster node;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission() 
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  piMode(MAX_485_RE_NEG, OUTPUT);
  // initialize in receive mode
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);

  // Modbus communication runs at 9600 baud
  Serial.begin(9600);

  // Modbus slave ID default is 2 according to data sheet
  node.begin(2, Serial);
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

bool state = true;

void loop() 
{
  uint8_t result;
  uint16_t data[7]; // slave response frame for buffer is 7 bytes according to data sheet

  // Toggle the coil at address 0x10 (Manual Load Control, INT16 writing)
  result = node.writeSingleCoil(0x10, state);
  state = !state;

  // read wind direction value at register 0x0000
  result = node.readInputRegisters(0x0000, 1);
  if (result == node.ku8MBSuccess) 
  {
    Serial.print("Slave Response Frame: ") // not sure if this is even what will get outputted
    Serial.println(node.getResponseBuffer(0x0000));
  }






}



