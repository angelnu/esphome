#pragma once

#include "esphome/core/defines.h"

#define LOG_SIEDLE_MESSAGE(prefix, msg) \
  if ((msg) != nullptr) { \
    ESP_LOGCONFIG(TAG, prefix "Message:"); \
    ESP_LOGCONFIG(TAG, prefix "  Command: %s", format_hex_pretty((msg)->get_command()).c_str()); \
    ESP_LOGCONFIG(TAG, prefix "  Destination: %s", format_hex_pretty((msg)->get_destination()).c_str()); \
    ESP_LOGCONFIG(TAG, prefix "    Bus: %s", format_hex_pretty((msg)->get_destination_bus()).c_str()); \
    ESP_LOGCONFIG(TAG, prefix "  Source: %s", format_hex_pretty((msg)->get_source()).c_str()); \
    ESP_LOGCONFIG(TAG, prefix "    Bus: %s", format_hex_pretty((msg)->get_source_bus()).c_str()); \
  }

namespace esphome {
namespace siedle_inhome_bus {

  class SiedleInhomeBusMessage {
    public:
      SiedleInhomeBusMessage (uint8_t command,
                              uint8_t destination,
                              uint8_t destination_bus,
                              uint8_t source,
                              uint8_t source_bus);
      
      SiedleInhomeBusMessage (uint32_t raw_msg) { raw_msg_ = raw_msg; }

      uint32_t get_raw() { return raw_msg_; }
      std::string get_string();
      bool log_unexpected_bits();
      
      uint8_t get_command();
      uint8_t get_destination();
      uint8_t get_destination_bus();
      uint8_t get_source();
      uint8_t get_source_bus();

    protected:
      uint32_t raw_msg_;

      uint8_t get_msg_prolog();
      uint8_t get_msg_middle();
      uint8_t get_msg_epilog();
  };

}  // namespace siedle_inhome_bus
}  // namespace esphome