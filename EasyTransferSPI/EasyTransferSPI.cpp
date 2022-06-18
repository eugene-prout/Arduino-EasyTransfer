#include "EasyTransferSPI.h"

void EasyTransferSPI::beginSlave(uint8_t *structPtr, uint8_t structSize, SPI *theSPI) 
{ 
  address = structPtr;
  size = structSize;
  _spi = theSPI;

  SPCR |= bit(SPE);
  pinMode(MISO, OUTPUT);
  SPI.attachInterrupt();
  rx_buffer_idx = 0;
  
  rx_buffer = (uint8_t *) malloc(structSize + 4);
  // TODO: Raise exception if malloc fails
}

ISR (SPI_STC_vect)
{
  uint8_t data = SPDR;
  rx_buffer[rx_buffer_idx] = data;


  if (rx_buffer_idx == 0 && data == 0x06) 
  {
    // First byte of header
    rx_buffer_idx++;
  }
  else if (rx_buffer_idx == 1 && data == 0x85)
  {
    // Second byte of header
    rx_buffer_idx++;
  }
  else if (rx_buffer_idx == 2)
  {
    // Size of payload
    size = data;
    rx_buffer_idx++;
  }
  else if (rx_buffer_idx == size + 3) 
  {
    // Checksum
    
    // Calculate checksum
    uint8_t checksum = size;   
    for (int i = 3; i < rx_len + 3; i++)
    {
      checksum ^= rx_buffer[i];
    }

    if (checksum == data)
    {
      // Correct checksum
      uint8_t *new_message = (uint8_t *) malloc(structSize);
      memcpy(new_message, buffer+3, structSize);
      messages_received.push(new_message)
    }

    rx_buffer_idx = 0;
  }
}

boolean EasyTransferSPI::messageAvailable()
{
  return !messages_received.empty();
}

uint8_t *getEarliestMessage()
{
  if (messages_received.empty()) {
    return NULL;
  } else {
    messages_received.pop();
  }
}
