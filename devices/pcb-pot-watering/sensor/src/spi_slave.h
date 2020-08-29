#ifndef _SPI__SLAVE_H_INCLUDED
#define _SPI__SLAVE_H_INCLUDED

#include "Arduino.h"

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

#define SPI_DEFAULT_TIMEOUT 10000 //10 s

class SPI_Slave {
  public:
    SPI_Slave();
    SPI_Slave(uint8_t mosi, uint8_t miso, uint8_t sclk, uint8_t ssel = (uint8_t)NC);

    // setMISO/MOSI/SCLK/SSEL have to be called before begin()
    void setMISO(uint32_t miso)
    {
      _spi.pin_miso = digitalPinToPinName(miso);
    };
    void setMOSI(uint32_t mosi)
    {
      _spi.pin_mosi = digitalPinToPinName(mosi);
    };
    void setSCLK(uint32_t sclk)
    {
      _spi.pin_sclk = digitalPinToPinName(sclk);
    };
    void setSSEL(uint32_t ssel)
    {
      _spi.pin_ssel = digitalPinToPinName(ssel);
    };

    void setMISO(PinName miso)
    {
      _spi.pin_miso = (miso);
    };
    void setMOSI(PinName mosi)
    {
      _spi.pin_mosi = (mosi);
    };
    void setSCLK(PinName sclk)
    {
      _spi.pin_sclk = (sclk);
    };
    void setSSEL(PinName ssel)
    {
      _spi.pin_ssel = (ssel);
    };
    void setBitOrder(BitOrder order)
    {
      _spi.order = (order);
    }
    void setDataMode(uint8_t mode)
    {
      _spi.mode = (mode);
    }

    void setTimeout(uint32_t timeout)
    {
      _spi.timeout = (timeout);
    }

    void begin();
    void end();

    void abort(){
      HAL_SPI_Abort(&_spi.handle);
      HAL_SPI_Abort_IT(&_spi.handle);
      end();
    }

    uint8_t transfer8(uint8_t data_out);
    uint16_t transfer16(uint16_t data_out);
    uint32_t transfer32(uint32_t data_out);
    void transfer(size_t count, uint8_t *buffer_out=0, uint8_t *buffer_in=0);
    void transfer_IT(size_t count,
                     uint8_t *buffer_out=0, void (*tx_callback)()=0,
                     uint8_t *buffer_in=0, void (*rx_callback)()=0);

  private:
    
    struct spi_s {
        SPI_HandleTypeDef handle;
        SPI_TypeDef *spi;
        PinName pin_miso;
        PinName pin_mosi;
        PinName pin_sclk;
        PinName pin_ssel;
        BitOrder order;
        uint8_t mode;
        uint32_t timeout;
    } _spi;
};
#endif //_SPI__SLAVE_H_INCLUDED