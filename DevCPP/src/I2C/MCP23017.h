//
// Created by david on 31/03/2025.
//

#ifndef MCP23017_H
#define MCP23017_H


#include "I2C_Interface.h"

/**
 * @brief Classe pour gérer l'extenseur d'E/S MCP23017 via I2C.
 * Cette classe permet de configurer les broches du MCP23017 comme sorties
 * et de contrôler les CD4051 pour sélectionner les antennes RFID.
 */
class MCP23017 {
public:
    /**
     * @brief Constructeur.
     * @param i2c Référence à l'interface I2C pour communiquer avec le MCP23017.
     * @param address Adresse I2C du MCP23017 (par exemple, 0x20).
     */
    MCP23017(I2C_Interface& i2c, uint8_t address);

    /**
     * @brief Initialise le MCP23017.
     * Configure les broches GPA0-GPA5 comme sorties et les met à LOW.
     * @return true si l'initialisation réussit, false sinon.
     */
    bool begin();

    /**
     * @brief Sélectionne une antenne RFID.
     * Configure les broches de sélection (A, B, C) des CD4051 (U3 ou U4)
     * pour activer l'antenne spécifiée.
     * @param antenna Numéro de l'antenne (0 à 15).
     * @return true si la sélection réussit, false sinon.
     */
    bool selectAntenna(uint8_t antenna);

private:
    I2C_Interface& i2c_;  // Interface I2C pour la communication
    uint8_t address_;    // Adresse I2C du MCP23017

    // Registres du MCP23017
    static constexpr uint8_t IODIRA = 0x00;  // Direction des broches GPA (0 = sortie, 1 = entrée)
    static constexpr uint8_t IODIRB = 0x01;  // Direction des broches GPB
    static constexpr uint8_t GPIOA = 0x12;   // État des broches GPA
    static constexpr uint8_t GPIOB = 0x13;   // État des broches GPB

    // Configuration des broches pour les CD4051
    static constexpr uint8_t U3_A_PIN = 0;   // GPA0 : A de U3
    static constexpr uint8_t U3_B_PIN = 1;   // GPA1 : B de U3
    static constexpr uint8_t U3_C_PIN = 2;   // GPA2 : C de U3
    static constexpr uint8_t U4_A_PIN = 3;   // GPA3 : A de U4
    static constexpr uint8_t U4_B_PIN = 4;   // GPA4 : B de U4
    static constexpr uint8_t U4_C_PIN = 5;   // GPA5 : C de U4
};



#endif //MCP23017_H
