#include <EasyTransferSPI.h>
#include <SPI.h>

EasyTransferSPI ET;

struct SPIDATA {
  int data1;
  int data2;
};

SPIDATA data;

void setup() {
  Serial.begin(9600);
  ET.beginSlave(details(data), &SPI);

}

void loop() {
  if (ET.messageAvailable()) {
    SPIDATA* msgPtr = (SPIDATA*) ET.getEarliestMessage();
    Serial.println("Message received");
    Serial.print("Data1: ");
    Serial.println(msgPtr->data1);
    Serial.print("Data2: ");
    Serial.println(msgPtr->data2);
  }
}
