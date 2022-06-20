#include "EasyTransferSPI.h"

SPIClass *_spi; 
SPISettings _spi_settings;
uint8_t *address;     // address of struct
uint8_t size;         // size of struct
uint8_t *rx_buffer;   // address for temporary storage and parsing buffer
uint8_t rx_buffer_idx; // index for RX parsing buffer
uint8_t rx_len;       // RX packet length according to the packet
uint8_t checksum;

CircularQueue messages_received(100); // data structure to hold fully read messages

void EasyTransferSPI::beginSlave(uint8_t *structPtr, uint8_t structSize, SPIClass *theSPI) 
{ 
  address = structPtr;
  size = structSize;
  _spi = theSPI;

  SPCR |= bit(SPE);
  pinMode(MISO, OUTPUT);
  SPI.attachInterrupt();
  rx_buffer_idx = 0;
  
  rx_buffer = (uint8_t *) malloc(structSize + 4);
}

ISR (SPI_STC_vect)
{
  uint8_t data = SPDR;
  Serial.println(data);
  rx_buffer[rx_buffer_idx] = data;

  if (rx_buffer_idx == 0) 
  {
    // Header 1: 0x06
    if (data != 0x06)
      rx_buffer_idx = 0;
    else {
      rx_buffer_idx++;
    }
  }
  else if (rx_buffer_idx == 1)
  {
    // Header 2: 0x85
    if (data != 0x85)
      rx_buffer_idx = 0;
    else {
      rx_buffer_idx++;
    }
  }
  else if (rx_buffer_idx == 2) 
  {
    // Header 3: size
    size = data;
    rx_buffer_idx++;
    checksum = size;
  }
  else if (rx_buffer_idx == size + 3)
  {
    // Checksum
    
    // Calculate checksum
    if (checksum == data)
    {
      // Correct checksum
      uint8_t *new_message = (uint8_t *) malloc(size);
      memcpy(new_message, rx_buffer+3, size);
      messages_received.enqueue(new_message);
    }

    rx_buffer_idx = 0;
  }
  else
  {
    // Payload
    checksum ^= data;
    rx_buffer_idx++;
  }
}

boolean EasyTransferSPI::messageAvailable()
{
  return !messages_received.isEmpty();
}

uint8_t* EasyTransferSPI::getEarliestMessage()
{
  if (messages_received.isEmpty()) {
    return NULL;
  } else {
    return messages_received.dequeue();
  }
}

void EasyTransferSPI::beginMaster(uint8_t *structPtr, uint8_t structSize, SPIClass *theSPI)
{
    _spi->setClockDivider(SPI_CLOCK_DIV8);
    address = structPtr;
    size = structSize;
    _spi = theSPI;
    _spi->begin();
    _spi_settings = SPISettings(4000000, MSBFIRST, SPI_MODE0);
}

void EasyTransferSPI::beginMaster(uint8_t *structPtr, uint8_t structSize, SPIClass *theSPI, SPISettings customSettings)
{
    _spi->setClockDivider(SPI_CLOCK_DIV8);
    address = structPtr;
    size = structSize;
    _spi = theSPI;
    _spi->begin();
    _spi_settings = customSettings;
}

//Sends out struct with header, length info and checksum
void EasyTransferSPI::sendData()
{
    uint8_t CS = size;

    // Take control of SPI bus
    _spi->beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(SS, LOW);  

    _spi->transfer(0x06);
    _spi->transfer(0x85);
    _spi->transfer(size);

    for (int i=0; i<size; i++) {
         CS^=*(address+i);
        _spi->transfer(*(address+i));
    }

    _spi->transfer(CS);

    // Release control of SPI bus
    digitalWrite(SS, HIGH);
    /* _spi->endTransaction(); */
}
