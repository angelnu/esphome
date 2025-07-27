#pragma once

#include <cstdint>
#include <cassert>
#include <format>
#include "esphome/core/defines.h"

#define LOG_SIEDLE_COMMAND(prefix, cmd) \
  if ((cmd) != nullptr) { \
    ESP_LOGCONFIG(TAG, prefix "Command: %s", format_hex_pretty((cmd)->get_command()).c_str()); \
    ESP_LOGCONFIG(TAG, prefix "Destination: %s", format_hex_pretty((cmd)->get_destination()).c_str()); \
    ESP_LOGCONFIG(TAG, prefix "Destination Bus: %s", format_hex_pretty((cmd)->get_destination_bus()).c_str()); \
    ESP_LOGCONFIG(TAG, prefix "Source: %s", format_hex_pretty((cmd)->get_source()).c_str()); \
    ESP_LOGCONFIG(TAG, prefix "Source Bus: %s", format_hex_pretty((cmd)->get_source_bus()).c_str()); \
  }

namespace esphome {
namespace siedle_inhome_bus {

  class SiedleInhomeBusCommand {
    public:
      SiedleInhomeBusCommand (uint8_t command, uint8_t destination, uint8_t destination_bus, uint8_t source, uint8_t source_bus) {
        raw_cmd_ = 0b010;

        assert(command < (1<<7));
        raw_cmd_ = (raw_cmd_<<6) + command;

        assert(destination < (1<<6));
        raw_cmd_ = (raw_cmd_<<5) + destination;

        assert(destination_bus < (1<<5));
        raw_cmd_ = (raw_cmd_<<4) + destination_bus;

        raw_cmd_ = (raw_cmd_<<3) + 0b010;

        assert(source < (1<<6));
        raw_cmd_ = (raw_cmd_<<5) + source;

        assert(source_bus < (1<<5));
        raw_cmd_ = (raw_cmd_<<4) + source_bus;

        raw_cmd_ = (raw_cmd_<<2) + 0b00;
      };
      SiedleInhomeBusCommand (uint32_t raw_cmd) { raw_cmd_ = raw_cmd; }

      uint32_t get_raw() { return raw_cmd_; }
      std::string get_string() {
        return std::format(
          "Command: 0x{}, Destination: 0x{}, Source: 0x{}, Destination Bus: 0x{}, SourceBus: 0x{}",
          format_hex_pretty(get_command()),
          format_hex_pretty(get_destination()),
          format_hex_pretty(get_destination_bus()),
          format_hex_pretty(get_source()),
          format_hex_pretty(get_source_bus())
        );
      }
      
      uint8_t get_command()         { return (bitRead(raw_cmd_,28)<<5) + (bitRead(raw_cmd_,27)<<4) + (bitRead(raw_cmd_,26)<<3) + (bitRead(raw_cmd_,25)<<2) + (bitRead(raw_cmd_,24)<<1) + bitRead(raw_cmd_,23); }
      uint8_t get_destination()     { return (bitRead(raw_cmd_,22)<<4) + (bitRead(raw_cmd_,21)<<3) + (bitRead(raw_cmd_,20)<<2) + (bitRead(raw_cmd_,19)<<1) + bitRead(raw_cmd_,18); }
      uint8_t get_destination_bus() { return (bitRead(raw_cmd_,17)<<3) + (bitRead(raw_cmd_,16)<<2) + (bitRead(raw_cmd_,15)<<1) + bitRead(raw_cmd_,14); }
      uint8_t get_source()          { return (bitRead(raw_cmd_,10)<<4) + (bitRead(raw_cmd_, 9)<<3) + (bitRead(raw_cmd_, 8)<<2) + (bitRead(raw_cmd_, 7)<<1) + (bitRead(raw_cmd_, 6)); }
      uint8_t get_source_bus()      { return (bitRead(raw_cmd_, 5)<<3) + (bitRead(raw_cmd_, 4)<<2) + (bitRead(raw_cmd_, 3)<<1) + bitRead(raw_cmd_, 2); }
    protected:
      uint32_t raw_cmd_;

  };

}  // namespace siedle_inhome_bus
}  // namespace esphome