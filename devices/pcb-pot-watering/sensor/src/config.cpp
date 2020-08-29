#include "config.h"
#include "EEPROM.h"
#include <assert.h>
#include "CRC32.h"

#define EEPROM_ADDRESS_4_BLOCK(block) ((block) * EEPROM_BLOCK_SIZE)
#define CONFIG_BLOCK 0

device_config_t device_config;
external_data_block_t external_data[EEPROM_BLOCKS];

int read_device_config() {
    read_external_data(CONFIG_BLOCK);
    assert(sizeof(device_config) <= EEPROM_BLOCK_SIZE);
    memcpy(&device_config, external_data[CONFIG_BLOCK].block, sizeof(device_config));

    if ((device_config.version_config != DEVICE_CONFIG_VERSION) ||
        (bool(device_config.is_debug) != bool(DEBUG)))
        return -1;
    else
        return 0;
}

void write_device_config(device_config_t &device_config_new) {
    if (memcmp(&device_config, &device_config_new, sizeof(device_config_t))) {
        device_config = device_config_new;
        memcpy(external_data[CONFIG_BLOCK].block, &device_config, sizeof(device_config));
        CRC32 crcTool;
        crcTool.reset();
        external_data[CONFIG_BLOCK].parts.crc = crcTool.calculate(external_data[CONFIG_BLOCK].parts.data,
                                                                  sizeof(external_data[CONFIG_BLOCK].parts.data));
        write_external_data(CONFIG_BLOCK);
        log_warning_ln(F("Settings written"));
    } else {
        log_warning_ln(F("Settings unchanged"));
    }
}

void init_device_config(bool reset) {
    bool not_valid_seeting = read_device_config();
    UNUSED(not_valid_seeting);
    device_config_t device_config_new = device_config;
#ifndef RESET_SETTINGS
    if (reset || not_valid_seeting )
#endif
    {
        device_config_new.version_config = DEVICE_CONFIG_VERSION;
        device_config_new.version = DEVICE_VERSION;
        device_config_new.measure_average = DEFAULT_AVERAGE_MEASUREMENTS;
        device_config_new.min_s = DEFAULT_MIN_S;
        device_config_new.max_s = DEFAULT_MAX_S;
  
    }
    device_config_new.version = DEVICE_VERSION;
    write_device_config(device_config_new);

}

void print_hex_mem(uint8_t *mem, size_t len, bool bigEndian=true) {
    log_info   (F("0x"));
    for (size_t i=0; i < len; i++) {
        size_t ii = bigEndian ? i : len-(i+1);
        uint8_t byte = *(mem + ii);
        if (byte < 16)
            log_info('0');
        log_info (byte, HEX);
    }
    log_info_ln("");
}

void print_buildinfo() {
    log_info(F("DEVICE VERSION: "));
        log_info_ln(DEVICE_VERSION);
    log_info(F("DEVICE CONFIG: "));
        print_hex_mem((uint8_t*)&device_config, sizeof(device_config));
    log_info(F("Device UID: "));
        print_hex_mem((uint8_t*)UID_BASE, UID_BYTES_LENGTH);
    log_info(F("BUILD DATE: "));
        log_info_ln(__DATE__);
    log_info(F("BUILD TIME: "));
        log_info_ln(__TIME__);
    log_info(F("COMMIT ID: "));
        log_info_ln(GIT_COMMIT_ID);
}


void read_from_eeprom(int eeprom_address, uint8_t* data, size_t size) {
    eeprom_buffer_fill();
    for (size_t i=0; i < size; i++) {
        data[i] = eeprom_buffered_read_byte(eeprom_address+i);
    }
}
void write_to_eeprom(int eeprom_address, uint8_t* data, size_t size) {
    for (size_t i=0; i < size; i++) {
        eeprom_buffered_write_byte(eeprom_address+i, data[i]);
    }
    eeprom_buffer_flush();
}


crc_t get_external_data_crc(size_t block) {
    assert(block < EEPROM_BLOCKS);
    return external_data[block].parts.crc;
}

bool is_external_data_ok(size_t block) {
    assert(block < EEPROM_BLOCKS);
    CRC32 crcTool;
    crcTool.reset();
    crc_t crc = crcTool.calculate(external_data[block].parts.data, sizeof(external_data[block].parts.data));
    return (crc == get_external_data_crc(block));
}

static crc_t last_external_data_crc[EEPROM_BLOCKS];

void read_external_data(size_t block) {
    assert(block < EEPROM_BLOCKS);
    //EEPROM.get(EEPROM_ADDRESS_4_BLOCK(block), external_data[block].block);
    read_from_eeprom(EEPROM_ADDRESS_4_BLOCK(block), external_data[block].block, sizeof(external_data[block].block));
    last_external_data_crc[block] = get_external_data_crc(block);
}

void write_external_data(size_t block) {
    assert(block < EEPROM_BLOCKS);
    log_info(block);
    if ( ! is_external_data_ok(block)) {
        log_warning_ln(F(" - External data corrupted - skip write and repair from EEPROM"));
        read_external_data(block);
    } else if (last_external_data_crc[block] == get_external_data_crc(block)) {
        log_info_ln(F(" - External data unchnaged - skip write to EEPROM"));
    } else {
        log_info_ln(F(" - Writting external data to EEPROM"));
        //EEPROM.put(EEPROM_ADDRESS_4_BLOCK(block), external_data[block].block);
        write_to_eeprom(EEPROM_ADDRESS_4_BLOCK(block), external_data[block].block, sizeof(external_data[block].block));
        last_external_data_crc[block] = get_external_data_crc(block);
        log_info_ln(F("    Writting external data to EEPROM - DONE"));
        if (block == CONFIG_BLOCK) {
            read_device_config();
        }
    }
}