#include <SPI.h>          //Library for using SPI Communication 

#include <mcp2515.h>      //Library for using CAN Communication


struct can_frame canMsg;

MCP2515 mcp2515(10);

int incomingByte = 1; // 1 стоп; 2 пуск;


void setup()

{

  while (!Serial);

  Serial.begin(115200);


  mcp2515.reset();

  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);

  mcp2515.setNormalMode();


  Serial.print("Setup done\n\n");

}


void printserial(unsigned long ID, unsigned char buf[8]) {

  Serial.print(ID, HEX);

  Serial.print(": ");

  for (int i = 0; i < 8; i++)

  {

    Serial.print(buf[i], HEX);

    Serial.print("  ");

  }

}


void loop()

{

  canMsg.can_id  = 0x141;           

  canMsg.can_dlc = 0x08;


  if (2 == incomingByte) {

    canMsg.data[0] = 0xA2;

    canMsg.data[1] = 0x00;

    canMsg.data[2] = 0x00;

    canMsg.data[3] = 0x00;

    canMsg.data[4] = 0xA0;

    canMsg.data[5] = 0x86;

    canMsg.data[6] = 0x01;

    canMsg.data[7] = 0x00;

    Serial.print("Send(2): ");

  } else {

    if (3 == incomingByte) {

      canMsg.data[0] = 0xA2;

      canMsg.data[1] = 0x00;

      canMsg.data[2] = 0x00;

      canMsg.data[3] = 0x00;

      canMsg.data[4] = 0x40;

      canMsg.data[5] = 0xD0;

      canMsg.data[6] = 0x03;

      canMsg.data[7] = 0x00;

      Serial.print("Send(3): ");

    } else {

      canMsg.data[0] = 0x81;

      canMsg.data[1] = 0x00;

      canMsg.data[2] = 0x00;

      canMsg.data[3] = 0x00;

      canMsg.data[4] = 0x00;

      canMsg.data[5] = 0x00;

      canMsg.data[6] = 0x00;

      canMsg.data[7] = 0x00;

      Serial.print("Send(1): ");

    }

  }

  mcp2515.sendMessage(&canMsg); 

  printserial(canMsg.can_id, canMsg.data);

  Serial.print("\n");


  int len = 10;

  while ((mcp2515.readMessage(&canMsg) != MCP2515::ERROR_OK))

  {

   

    delay(1);

    len--;

    if ((len <= 0)) {

      break;

    }

  }


  if (len > 0) {

    Serial.print("Recv   : ");

    printserial(canMsg.can_id, canMsg.data);

    Serial.print("\n");

  } else {

   

    Serial.print("Recv   : NO ANSWER");

    Serial.print("\n");

  }

  Serial.print("\n");


  int pause_3000 = 30;

  while (pause_3000) {

    if (Serial.available() > 0) {

      // read the incoming byte:

      incomingByte = Serial.read();

      incomingByte = incomingByte - 0x30; // "1" -> 1


      while (Serial.available() > 0) {

        Serial.read();

      }

      break;

    }


    pause_3000--;

    delay(100);

  }

}
