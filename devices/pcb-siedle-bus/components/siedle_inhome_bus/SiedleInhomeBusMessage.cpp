#include <cstdint>
#include <cassert>
#include <format>
#include "SiedleInhomeBusButton.h"

namespace esphome {
namespace siedle_inhome_bus {

  SiedleInhomeBusMessage::SiedleInhomeBusMessage (uint8_t command, uint8_t destination, uint8_t destination_bus, uint8_t source, uint8_t source_bus) {
    raw_msg_ = 0b010;

    assert(command < (1<<7));
    raw_msg_ = (raw_msg_<<6) + command;

    assert(destination < (1<<6));
    raw_msg_ = (raw_msg_<<5) + destination;

    assert(destination_bus < (1<<5));
    raw_msg_ = (raw_msg_<<4) + destination_bus;

    raw_msg_ = (raw_msg_<<3) + 0b010;

    assert(source < (1<<6));
    raw_msg_ = (raw_msg_<<5) + source;

    assert(source_bus < (1<<5));
    raw_msg_ = (raw_msg_<<4) + source_bus;

    raw_msg_ = (raw_msg_<<2) + 0b00;
  };
    
  std::string SiedleInhomeBusMessage::get_string() {
    return std::format(
      "Command: 0x{}, Destination: 0x{}, Source: 0x{}, Destination Bus: 0x{}, SourceBus: 0x{}",
      format_hex_pretty(get_command()),
      format_hex_pretty(get_destination()),
      format_hex_pretty(get_destination_bus()),
      format_hex_pretty(get_source()),
      format_hex_pretty(get_source_bus())
    );
  };
  
  uint8_t SiedleInhomeBusMessage::get_command() {
    return (bitRead(raw_msg_,28)<<5) +
           (bitRead(raw_msg_,27)<<4) +
           (bitRead(raw_msg_,26)<<3) +
           (bitRead(raw_msg_,25)<<2) +
           (bitRead(raw_msg_,24)<<1) +
           (bitRead(raw_msg_,23)<<0);
  }
  uint8_t SiedleInhomeBusMessage::get_destination() {
    return (bitRead(raw_msg_,22)<<4) + 
           (bitRead(raw_msg_,21)<<3) + 
           (bitRead(raw_msg_,20)<<2) + 
           (bitRead(raw_msg_,19)<<1) + 
           (bitRead(raw_msg_,18)<<0);
  }

  uint8_t SiedleInhomeBusMessage::get_destination_bus() {
    return (bitRead(raw_msg_,17)<<3) +
           (bitRead(raw_msg_,16)<<2) +
           (bitRead(raw_msg_,15)<<1) +
           (bitRead(raw_msg_,14)<<0);
  }

  uint8_t SiedleInhomeBusMessage::get_source() {
    return (bitRead(raw_msg_,10)<<4) +
           (bitRead(raw_msg_, 9)<<3) +
           (bitRead(raw_msg_, 8)<<2) +
           (bitRead(raw_msg_, 7)<<1) +
           (bitRead(raw_msg_, 6)<<0);
  }
  uint8_t SiedleInhomeBusMessage::get_source_bus() {
    return (bitRead(raw_msg_, 5)<<3) +
           (bitRead(raw_msg_, 4)<<2) +
           (bitRead(raw_msg_, 3)<<1) +
           (bitRead(raw_msg_, 2)<<0);
  }

}  // namespace siedle_inhome_bus
}  // namespace esphome