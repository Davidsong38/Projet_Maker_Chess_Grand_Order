//
// Created by david on 31/03/2025.
//

#include "windows_I2C.h"
#include <iostream>

bool Windows_I2C::writeRegister(uint8_t address, uint8_t reg, uint8_t value) {
    std::cout << "[Simulated] Write to I2C: Address=0x" << std::hex << static_cast<int>(address)
              << ", Reg=0x" << static_cast<int>(reg) << ", Value=0x" << static_cast<int>(value) << std::dec << std::endl;
    return true;
}

bool Windows_I2C::readRegister(uint8_t address, uint8_t reg, uint8_t& value) {
    std::cout << "[Simulated] Read from I2C: Address=0x" << std::hex << static_cast<int>(address)
              << ", Reg=0x" << static_cast<int>(reg) << std::dec << std::endl;
    value = 0xFF; // Valeur simulée
    return true;
}