#include <SPI.h>
#include <EasyTransferSPI.h>

//create object
EasyTransferSPI ET; 

struct SPIDATA {
  int data1;
  int data2;
};


//give a name to the group of data
SPIDATA mydata;

void setup(){
  ET.begin(details(mydata), &SPI);
  
}

void loop(){
  mydata.data1 = 10;
  mydata.data2 = random(5);
  //send the data
  ET.sendData();
  delay(5000);
}
