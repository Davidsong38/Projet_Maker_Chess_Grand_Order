//
// Created by david on 31/03/2025.
//

#ifndef I2C_INTERFACE_H
#define I2C_INTERFACE_H

#include <cstdint>
#include <string>

class I2C_Interface {
public:
    virtual ~I2C_Interface() = default;
    virtual bool writeRegister(uint8_t address, uint8_t reg, uint8_t value) = 0;
    virtual bool readRegister(uint8_t address, uint8_t reg, uint8_t& value) = 0;
    virtual bool writeData(uint8_t address, const uint8_t* data, size_t length) = 0;
    virtual bool readData(uint8_t address, uint8_t* buffer, size_t length) = 0;
};

#endif //I2C_INTERFACE_H
