//
// Created by david on 31/03/2025.
//

#include "MCP23017.h"
#include <iostream>

/**
 * @brief Constructeur de la classe MCP23017.
 * @param i2c Référence à l'interface I2C.
 * @param address Adresse I2C du MCP23017.
 */
MCP23017::MCP23017(I2C_Interface& i2c, uint8_t address) : i2c_(i2c), address_(address) {}

/**
 * @brief Initialise le MCP23017.
 * Configure les broches GPA0-GPA5 comme sorties et les met à LOW.
 * Les autres broches (GPA6-GPA7, GPB0-GPB7) sont également configurées comme sorties
 * pour éviter des comportements indéfinis.
 * @return true si l'initialisation réussit, false sinon.
 */
bool MCP23017::begin() {
    // Configurer toutes les broches comme sorties (IODIRA et IODIRB à 0x00)
    if (!i2c_.writeRegister(address_, IODIRA, 0x00)) { // GPA0-GPA7
        std::cerr << "MCP23017: Failed to set IODIRA" << std::endl;
        return false;
    }
    if (!i2c_.writeRegister(address_, IODIRB, 0x00)) { // GPB0-GPB7
        std::cerr << "MCP23017: Failed to set IODIRB" << std::endl;
        return false;
    }

    // Mettre toutes les broches à LOW
    if (!i2c_.writeRegister(address_, GPIOA, 0x00)) { // GPA0-GPA7
        std::cerr << "MCP23017: Failed to set GPIOA to LOW" << std::endl;
        return false;
    }
    if (!i2c_.writeRegister(address_, GPIOB, 0x00)) { // GPB0-GPB7
        std::cerr << "MCP23017: Failed to set GPIOB to LOW" << std::endl;
        return false;
    }

    return true;
}

/**
 * @brief Sélectionne une antenne RFID.
 * Configure les broches de sélection (A, B, C) des CD4051 (U3 ou U4)
 * pour activer l'antenne spécifiée.
 * U3 contrôle les antennes A0 à A7, U4 contrôle les antennes A8 à A15.
 * @param antenna Numéro de l'antenne (0 à 15).
 * @return true si la sélection réussit, false sinon.
 */
bool MCP23017::selectAntenna(uint8_t antenna) {
    if (antenna > 15) {
        std::cerr << "MCP23017: Invalid antenna number: " << static_cast<int>(antenna) << std::endl;
        return false;
    }

    // Déterminer quel CD4051 utiliser (U3 pour A0-A7, U4 pour A8-A15)
    bool useU3 = (antenna < 8);
    uint8_t value = useU3 ? antenna : (antenna - 8);

    // Décomposer la valeur en bits (A, B, C) pour la sélection
    uint8_t a = (value & 0x01);       // Bit 0
    uint8_t b = (value & 0x02) >> 1;  // Bit 1
    uint8_t c = (value & 0x04) >> 2;  // Bit 2

    // Lire l'état actuel de GPIOA
    uint8_t gpioa = 0;
    if (!i2c_.readRegister(address_, GPIOA, gpioa)) {
        std::cerr << "MCP23017: Failed to read GPIOA" << std::endl;
        return false;
    }

    // Réinitialiser les bits utilisés (GPA0-GPA5)
    gpioa &= 0xC0; // Conserver GPA6 et GPA7, réinitialiser GPA0-GPA5

    // Configurer les broches de sélection
    if (useU3) {
        // U3 : GPA0 (A), GPA1 (B), GPA2 (C)
        gpioa |= (a << U3_A_PIN) | (b << U3_B_PIN) | (c << U3_C_PIN);
    } else {
        // U4 : GPA3 (A), GPA4 (B), GPA5 (C)
        gpioa |= (a << U4_A_PIN) | (b << U4_B_PIN) | (c << U4_C_PIN);
    }

    // Mettre à jour GPIOA
    if (!i2c_.writeRegister(address_, GPIOA, gpioa)) {
        std::cerr << "MCP23017: Failed to write to GPIOA" << std::endl;
        return false;
    }

    return true;
}