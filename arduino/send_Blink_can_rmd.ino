// demo: CAN-BUS Shield, send data
#include <SPI.h>

#define CAN_2515
// #define CAN_2518FD

// Set SPI CS Pin according to your hardware

#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else

// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;
#endif


#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif

const int ledHIGH    = 1;
const int ledLOW     = 0;

byte readByte;

void printHex(uint8_t num, bool last) {
  char hexCar[2];

  if(last) {
    sprintf(hexCar, "%02X", num);
  }
  else {
    sprintf(hexCar, "%02X, ", num);
  }
  Serial.print(hexCar);
}

void setup() {
    SERIAL_PORT_MONITOR.begin(1000000);

    while (CAN_OK != CAN.begin(CAN_1000KBPS)) {
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        char test[1] = {0x88};
        CAN.sendMsgBuf(0x70, 0, 1, test);
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}

byte stmp[8] = {0x9A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void loop() {
    if(SERIAL_PORT_MONITOR.available()>0) {
      // send data once per second
      readByte = SERIAL_PORT_MONITOR.read();
      delay(100);
      if(readByte==70) {        
        SERIAL_PORT_MONITOR.println("");
        SERIAL_PORT_MONITOR.println("-------------");
        SERIAL_PORT_MONITOR.println("Command:");
        SERIAL_PORT_MONITOR.print(readByte);
        CAN.sendMsgBuf(0xDF, 0, 8, stmp);
        
        unsigned char len = 0;
        char buf[8];
        CAN.readMsgBuf(&len, buf);
        unsigned long canId = CAN.getCanId();

        SERIAL_PORT_MONITOR.println("");
        SERIAL_PORT_MONITOR.print("Get data from ID: 0x");
        SERIAL_PORT_MONITOR.print(canId, HEX);
        
        // Print in HEX
        SERIAL_PORT_MONITOR.println("");
        SERIAL_PORT_MONITOR.println("Received in HEX:");
        SERIAL_PORT_MONITOR.print("{");
        for(int i=0; i<sizeof(buf); i++){
          if(i+1==sizeof(buf)) {
            printHex(buf[i], true);
          }
          else {
            printHex(buf[i], false);
          }
        }
        SERIAL_PORT_MONITOR.print("}");
        
        // Print in DEC
        SERIAL_PORT_MONITOR.println("");
        SERIAL_PORT_MONITOR.println("Received in DEC:");
        SERIAL_PORT_MONITOR.print("{");
        for(int i=0; i<sizeof(buf); i++){
          if(i+1==sizeof(buf)) {
            SERIAL_PORT_MONITOR.print((byte)buf[i]);
          }
          else {
            SERIAL_PORT_MONITOR.print((byte)buf[i]);
            SERIAL_PORT_MONITOR.print(", ");
          }
        }
        SERIAL_PORT_MONITOR.print("}");
      }
    }
}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
