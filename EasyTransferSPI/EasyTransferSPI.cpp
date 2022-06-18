#include "EasyTransferSPI.h"

void EasyTransferSPI::beginMaster(uint8_t *structPtr, uint8_t structSize, SPI *theSPI)
{
    address = structPtr;
    size = structSize;
    _spi = theSPI;
    _spi.begin()
    _spi.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
}

void EasyTransferSPI::beginMasterbeginMaster(uint8_t *structPtr, uint8_t structSize, SPI *theSPI, SPISettings customSettings)
{
    address = structPtr;
    size = structSize;
    _spi = theSPI;
    _spi.begin()
    _spi.beginTransaction(customSettings);
}