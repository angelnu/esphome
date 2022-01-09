#pragma once

#include "Stream.h"
#include "esphome/components/uart/uart.h"

class ESPHome_Stream: public Stream {
  public:
    ESPHome_Stream(esphome::uart::UARTDevice *esphome_uartdevice):
    esphome_uartdevice_(esphome_uartdevice) {};

    size_t write(uint8_t c){
        return esphome_uartdevice_->write(c);
    }
    int available(){
        return esphome_uartdevice_->available();
    }
    int read() {
        return esphome_uartdevice_->read();
    }
    int peek() {
        return esphome_uartdevice_->peek();
    }
    void flush() {
        esphome_uartdevice_->flush();
    }
  private:
    esphome::uart::UARTDevice *esphome_uartdevice_;

};
