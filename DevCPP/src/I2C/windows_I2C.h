//
// Created by david on 31/03/2025.
//

#ifndef WINDOWS_I2C_H
#define WINDOWS_I2C_H



#include "I2C_Interface.h"

class Windows_I2C : public I2C_Interface {
public:
    Windows_I2C() = default;

    bool writeRegister(uint8_t address, uint8_t reg, uint8_t value) override;
    bool readRegister(uint8_t address, uint8_t reg, uint8_t& value) override;
};


#endif //WINDOWS_I2C_H
