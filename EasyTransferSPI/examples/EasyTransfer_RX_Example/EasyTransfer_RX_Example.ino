#include <EasyTransferSPI.h>
#include <SPI.h>

EasyTransferSPI ET;

struct SPIDATA {
  float data1;
  float data2;
  float data3;
  float data4;
  float data5;
};

float sum1 = 0;
float sum2 = 0;
float sum3 = 0;
float sum4 = 0;
float sum5 = 0;

int counter = 0;

SPIDATA data;

void setup() {
  Serial.begin(115200);
  ET.beginSlave(details(data), &SPI);
}

void loop() {
  if (ET.messageAvailable()) {
    data = *((SPIDATA*) ET.getEarliestMessage());

    sum1 += data.data1;
    sum2 += data.data2;
    sum3 += data.data3;
    sum4 += data.data4;
    sum5 += data.data5;

    counter++;

    if (counter % 100 == 0) {
      Serial.println("****************************************************************");
      Serial.print("Data 1: ");
      Serial.println(sum1);
      Serial.print("Data 2: ");
      Serial.println(sum2);
      Serial.print("Data 3: ");
      Serial.println(sum3);
      Serial.print("Data 4: ");
      Serial.println(sum4);
      Serial.print("Data 5: ");
      Serial.println(sum5);
      Serial.println("****************************************************************");
    }
  }
}
