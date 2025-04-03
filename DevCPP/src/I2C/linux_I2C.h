//
// Created by david on 31/03/2025.
//

#ifndef LINUX_I2C_H
#define LINUX_I2C_H



#include "I2C_Interface.h"
//#include <linux/i2c-dev.h>
//#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

class Linux_I2C : public I2C_Interface {
public:
    Linux_I2C(const std::string& device = "/dev/i2c-1");
    ~Linux_I2C() override;

    bool writeRegister(uint8_t address, uint8_t reg, uint8_t value) override;
    bool readRegister(uint8_t address, uint8_t reg, uint8_t& value) override;
    bool writeData(uint8_t address, const uint8_t* data, size_t length) override;
    bool readData(uint8_t address, uint8_t* buffer, size_t length) override;
private:
    int fd_;
};


#endif //LINUX_I2C_H
