//
// Created by david on 31/03/2025.
//


#ifndef CLRC663_H
#define CLRC663_H

#include "I2C_Interface.h"
#include <string>
#include <vector>

/**
 * @brief Classe pour gérer le lecteur RFID CLRC663 via I2C.
 * Cette classe permet d'initialiser le CLRC663, d'activer l'émetteur RFID,
 * et de lire les UIDs des tags RFID détectés.
 */
class CLRC663 {
public:
    /**
     * @brief Constructeur.
     * @param i2c Référence à l'interface I2C pour communiquer avec le CLRC663.
     * @param address Adresse I2C du CLRC663 (par exemple, 0x28).
     */
    CLRC663(I2C_Interface& i2c, uint8_t address);

    /**
     * @brief Initialise le CLRC663.
     * Configure les registres pour activer l'émetteur RFID à 13,56 MHz.
     * @return true si l'initialisation réussit, false sinon.
     */
    bool begin();

    /**
     * @brief Lit l'UID d'un tag RFID détecté.
     * Envoie une commande REQA pour détecter un tag, puis lit son UID.
     * @return L'UID du tag sous forme de chaîne hexadécimale (par exemple, "1A2B3C4D"),
     * ou "None" si aucun tag n'est détecté.
     */
    std::string readTag();

private:
    I2C_Interface& i2c_;  // Interface I2C pour la communication
    uint8_t address_;    // Adresse I2C du CLRC663

    // Registres du CLRC663 (basés sur la datasheet, à adapter si nécessaire)
    static constexpr uint8_t REG_COMMAND = 0x00;      // Registre de commande
    static constexpr uint8_t REG_STATUS = 0x02;       // Registre de statut
    static constexpr uint8_t REG_FIFO_DATA = 0x04;    // Données du FIFO
    static constexpr uint8_t REG_FIFO_LEVEL = 0x05;   // Niveau du FIFO
    static constexpr uint8_t REG_ERROR = 0x06;        // Registre d'erreur
    static constexpr uint8_t REG_CONTROL = 0x09;      // Registre de contrôle
    static constexpr uint8_t REG_TX_CONTROL = 0x11;   // Contrôle de l'émetteur
    static constexpr uint8_t REG_IRQ0 = 0x0A;         // Interruptions IRQ0
    static constexpr uint8_t REG_IRQ1 = 0x0B;         // Interruptions IRQ1

    // Commandes du CLRC663 (basées sur la datasheet)
    static constexpr uint8_t CMD_SOFT_RESET = 0x80;   // Réinitialisation logicielle
    static constexpr uint8_t CMD_REQA = 0x26;         // Commande REQA (détecter un tag)
    static constexpr uint8_t CMD_READ_UID = 0xCA;     // Commande pour lire l'UID (simplifiée)

    /**
     * @brief Active l'émetteur RFID.
     * Configure les registres pour activer l'émetteur à 13,56 MHz.
     * @return true si l'activation réussit, false sinon.
     */
    bool enableTransmitter();

    /**
     * @brief Détecte un tag RFID.
     * Envoie une commande REQA et vérifie si un tag est présent.
     * @return true si un tag est détecté, false sinon.
     */
    bool detectTag();

    /**
     * @brief Lit l'UID d'un tag détecté.
     * Récupère l'UID depuis le FIFO du CLRC663.
     * @return L'UID sous forme de chaîne hexadécimale, ou une chaîne vide en cas d'erreur.
     */
    std::string readUID();
};


#endif //CLRC663_H
