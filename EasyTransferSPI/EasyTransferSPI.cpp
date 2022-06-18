#include "EasyTransferSPI.h"

void EasyTransferSPI::beginMaster(uint8_t *structPtr, uint8_t structSize, SPI *theSPI)
{
    address = structPtr;
    size = structSize;
    _spi = theSPI;
    _spi->begin();
    _spi_settings = SPISettings(4000000, MSBFIRST, SPI_MODE0);
}

void EasyTransferSPI::beginMaster(uint8_t *structPtr, uint8_t structSize, SPI *theSPI, SPISettings customSettings)
{
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
    digitalWrite(SS, LOW);  
    _spi->beginTransaction(_spi_settings);

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
    _spi->endTransaction();
}