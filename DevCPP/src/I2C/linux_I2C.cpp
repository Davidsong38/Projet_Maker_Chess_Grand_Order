//
// Created by david on 31/03/2025.
//

#include "linux_I2C.h"
#include <iostream>

Linux_I2C::Linux_I2C(const std::string& device) {
    fd_ = open(device.c_str(), O_RDWR);
    if (fd_ < 0) {
        std::cerr << "Failed to open I2C device: " << device << std::endl;
        throw std::runtime_error("I2C device open failed");
    }
}

Linux_I2C::~Linux_I2C() {
    if (fd_ >= 0) {
        close(fd_);
    }
}

bool Linux_I2C::writeRegister(uint8_t address, uint8_t reg, uint8_t value) {
    if (ioctl(fd_, I2C_SLAVE, address) < 0) {
        std::cerr << "Failed to set I2C address: " << static_cast<int>(address) << std::endl;
        return false;
    }

    uint8_t buffer[2] = {reg, value};
    if (write(fd_, buffer, 2) != 2) {
        std::cerr << "Failed to write to I2C device" << std::endl;
        return false;
    }
    return true;
}

bool Linux_I2C::readRegister(uint8_t address, uint8_t reg, uint8_t& value) {
    if (ioctl(fd_, I2C_SLAVE, address) < 0) {
        std::cerr << "Failed to set I2C address: " << static_cast<int>(address) << std::endl;
        return false;
    }

    if (write(fd_, &reg, 1) != 1) {
        std::cerr << "Failed to write register address" << std::endl;
        return false;
    }

    if (read(fd_, &value, 1) != 1) {
        std::cerr << "Failed to read from I2C device" << std::endl;
        return false;
    }
    return true;
}

// Écriture de plusieurs octets (données) sur l'I2C
bool Linux_I2C::writeData(uint8_t address, const uint8_t* data, size_t length) {
    if (ioctl(fd_, I2C_SLAVE, address) < 0) {
        std::cerr << "Failed to set I2C address: " << static_cast<int>(address) << std::endl;
        return false;
    }

    if (write(fd_, data, length) != length) {
        std::cerr << "Failed to write data to I2C device" << std::endl;
        return false;
    }
    return true;
}

// Lecture de plusieurs octets (données) depuis l'I2C
bool Linux_I2C::readData(uint8_t address, uint8_t* buffer, size_t length) {
    if (ioctl(fd_, I2C_SLAVE, address) < 0) {
        std::cerr << "Failed to set I2C address: " << static_cast<int>(address) << std::endl;
        return false;
    }

    if (read(fd_, buffer, length) != length) {
        std::cerr << "Failed to read data from I2C device" << std::endl;
        return false;
    }
    return true;
}