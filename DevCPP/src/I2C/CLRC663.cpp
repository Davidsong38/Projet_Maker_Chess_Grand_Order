//
// Created by david on 31/03/2025.
//


#include "CLRC663.h"
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>

/**
 * @brief Constructeur de la classe CLRC663.
 * @param i2c Référence à l'interface I2C.
 * @param address Adresse I2C du CLRC663.
 */
CLRC663::CLRC663(I2C_Interface& i2c, uint8_t address) : i2c_(i2c), address_(address) {}

/**
 * @brief Initialise le CLRC663.
 * Effectue une réinitialisation logicielle, configure les registres,
 * et active l'émetteur RFID.
 * @return true si l'initialisation réussit, false sinon.
 */
bool CLRC663::begin() {
    // Réinitialisation logicielle
    if (!i2c_.writeRegister(address_, REG_COMMAND, CMD_SOFT_RESET)) {
        std::cerr << "CLRC663: Failed to perform soft reset" << std::endl;
        return false;
    }

    // Attendre que la réinitialisation soit terminée (typiquement 1 ms)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // Configurer les registres pour le mode RFID 13,56 MHz
    // (Exemple simplifié, à adapter selon la datasheet)
    if (!i2c_.writeRegister(address_, 0x2A, 0x0D)) { // Mode ISO/IEC 14443
        std::cerr << "CLRC663: Failed to set mode" << std::endl;
        return false;
    }

    // Activer l'émetteur
    if (!enableTransmitter()) {
        std::cerr << "CLRC663: Failed to enable transmitter" << std::endl;
        return false;
    }

    return true;
}

/**
 * @brief Active l'émetteur RFID.
 * Configure le registre TxControl pour activer l'émetteur.
 * @return true si l'activation réussit, false sinon.
 */
bool CLRC663::enableTransmitter() {
    // Activer l'émetteur (Tx1 et Tx2, selon la datasheet)
    uint8_t txControl = 0;
    if (!i2c_.readRegister(address_, REG_TX_CONTROL, txControl)) {
        return false;
    }

    // Activer Tx1 et Tx2 (bit 0 et bit 1 à 1)
    txControl |= 0x03;
    if (!i2c_.writeRegister(address_, REG_TX_CONTROL, txControl)) {
        return false;
    }

    return true;
}

/**
 * @brief Détecte un tag RFID.
 * Envoie une commande REQA et vérifie si un tag est présent.
 * @return true si un tag est détecté, false sinon.
 */
bool CLRC663::detectTag() {
    // Vider le FIFO
    if (!i2c_.writeRegister(address_, REG_FIFO_LEVEL, 0x80)) { // Flush FIFO
        return false;
    }

    // Envoyer la commande REQA
    if (!i2c_.writeRegister(address_, REG_COMMAND, CMD_REQA)) {
        return false;
    }

    // Attendre que la commande soit terminée (typiquement 1 ms)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // Vérifier les interruptions (IRQ0 et IRQ1)
    uint8_t irq0 = 0, irq1 = 0;
    if (!i2c_.readRegister(address_, REG_IRQ0, irq0) ||
        !i2c_.readRegister(address_, REG_IRQ1, irq1)) {
        return false;
    }

    // Vérifier si un tag a répondu (par exemple, RxIRQ dans IRQ0)
    if (!(irq0 & 0x02)) { // RxIRQ (bit 1)
        return false;
    }

    // Vérifier les erreurs
    uint8_t error = 0;
    if (!i2c_.readRegister(address_, REG_ERROR, error)) {
        return false;
    }

    if (error != 0) {
        std::cerr << "CLRC663: Error during REQA: 0x" << std::hex << static_cast<int>(error) << std::dec << std::endl;
        return false;
    }

    return true;
}

/**
 * @brief Lit l'UID d'un tag détecté.
 * Récupère l'UID depuis le FIFO du CLRC663.
 * @return L'UID sous forme de chaîne hexadécimale, ou une chaîne vide en cas d'erreur.
 */
std::string CLRC663::readUID() {
    // Vérifier le niveau du FIFO
    uint8_t fifoLevel = 0;
    if (!i2c_.readRegister(address_, REG_FIFO_LEVEL, fifoLevel)) {
        return "";
    }

    // Lire l'UID (typiquement 4 ou 7 octets, selon le type de tag)
    std::stringstream uid;
    uid << std::hex << std::uppercase << std::setfill('0');
    for (uint8_t i = 0; i < fifoLevel && i < 7; i++) { // Limite à 7 octets
        uint8_t byte = 0;
        if (!i2c_.readRegister(address_, REG_FIFO_DATA, byte)) {
            return "";
        }
        uid << std::setw(2) << static_cast<int>(byte);
    }

    return uid.str();
}

/**
 * @brief Lit l'UID d'un tag RFID détecté.
 * @return L'UID du tag sous forme de chaîne hexadécimale, ou "None" si aucun tag n'est détecté.
 */
std::string CLRC663::readTag() {
    // Détecter un tag
    if (!detectTag()) {
        return "None";
    }

    // Lire l'UID
    std::string uid = readUID();
    if (uid.empty()) {
        return "None";
    }

    return uid;
}