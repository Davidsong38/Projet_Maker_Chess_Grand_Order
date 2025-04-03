

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
        std::cerr << "CLRC663: Échec du reset" << std::endl;
        return false;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Attente

    // Configuration des registres
    if (!i2c_.writeRegister(address_, 0x2A, 0x0D)) {
        std::cerr << "CLRC663: Échec de la configuration ISO/IEC 14443" << std::endl;
        return false;
    }

    return enableTransmitter();
}

/**
 * @brief Active l'émetteur RFID.
 * @return true si l'activation réussit, false sinon.
 */
bool CLRC663::enableTransmitter() {
    uint8_t txControl = 0;
    if (!i2c_.readRegister(address_, REG_TX_CONTROL, txControl)) return false;

    txControl |= 0x03; // Active Tx1 et Tx2
    return i2c_.writeRegister(address_, REG_TX_CONTROL, txControl);
}

/**
 * @brief Lit un bloc spécifique sur un tag MIFARE Classic.
 * @param blockNum Numéro du bloc à lire.
 * @param buffer Vecteur pour stocker les 16 octets lus.
 * @return true si la lecture réussit, false sinon.
 */
bool CLRC663::readBlock(int blockNum, std::vector<uint8_t>& buffer) {
    if (blockNum < 0 || blockNum > 63) {
        std::cerr << "Bloc invalide !" << std::endl;
        return false;
    }

    // Authentification avec clé A par défaut (0xFFFFFFFFFFFF)
    std::vector<uint8_t> key = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    if (!authenticateBlock(blockNum, key, true)) {
        std::cerr << "Échec de l'authentification sur le bloc " << blockNum << std::endl;
        return false;
    }

    // Commande de lecture (0x30 suivi du numéro du bloc)
    uint8_t command[2] = {0x30, static_cast<uint8_t>(blockNum)};
    if (!i2c_.writeData(address_, command, 2)) return false;

    buffer.resize(16);
    return i2c_.readData(address_, buffer.data(), 16);
}

/**
 * @brief Écrit un bloc spécifique sur un tag MIFARE Classic.
 * @param blockNum Numéro du bloc à écrire.
 * @param data Données (16 octets).
 * @return true si l'écriture réussit, false sinon.
 */
bool CLRC663::writeBlock(uint8_t blockNum, const std::vector<uint8_t>& data) {
    if (data.size() != 16) {
        std::cerr << "Erreur: 16 octets requis !" << std::endl;
        return false;
    }

    std::vector<uint8_t> key = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    if (!authenticateBlock(blockNum, key, true)) {
        std::cerr << "Erreur d'authentification sur le bloc " << (int)blockNum << std::endl;
        return false;
    }

    uint8_t command[2] = {0xA0, blockNum};
    if (!i2c_.writeData(address_, command, 2)) return false;

    return i2c_.writeData(address_, data.data(), 16);
}

/**
 * @brief Authentifie un bloc MIFARE Classic avec une clé.
 * @param blockNum Numéro du bloc.
 * @param key Clé de 6 octets.
 * @param useKeyA true pour clé A, false pour clé B.
 * @return true si l'authentification réussit, false sinon.
 */
bool CLRC663::authenticateBlock(uint8_t blockNum, const std::vector<uint8_t>& key, bool useKeyA) {
    if (key.size() != 6) {
        std::cerr << "Clé invalide !" << std::endl;
        return false;
    }

    uint8_t authCmd = useKeyA ? 0x60 : 0x61;
    std::vector<uint8_t> cmd = {authCmd, blockNum};

    if (!i2c_.writeData(address_, cmd.data(), cmd.size())) return false;
    return i2c_.writeData(address_, key.data(), 6);
}

/**
 * @brief Lit l'UID stocké sur un bloc précis.
 * @param blockNum Numéro du bloc contenant l'UID.
 * @return UID sous forme hexadécimale.
 */
std::string CLRC663::readUID(int blockNum) {
    std::vector<uint8_t> buffer;
    if (!readBlock(blockNum, buffer)) return "";

    std::stringstream uid;
    uid << std::hex << std::uppercase << std::setfill('0');
    for (uint8_t byte : buffer) {
        uid << std::setw(2) << static_cast<int>(byte);
    }

    return uid.str();
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
 * @brief Fonction principale pour tester la lecture et l'écriture.
 */
void testRFID(CLRC663& rfid) {
    if (!rfid.begin()) {
        std::cerr << "Erreur d'initialisation du lecteur RFID !" << std::endl;
        return;
    }

    if (!rfid.detectTag()) {
        std::cerr << "Aucun tag détecté !" << std::endl;
        return;
    }

    std::string uid = rfid.readUID(4);
    std::cout << "UID du bloc 4 : " << uid << std::endl;

    std::vector<uint8_t> data = {0x31, 0x30, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (rfid.writeBlock(4, data)) {
        std::cout << "Écriture de l'UID 1001 réussie sur le bloc 4 !" << std::endl;
    }

    uid = rfid.readUID(4);
    std::cout << "Nouvel UID du bloc 4 : " << uid << std::endl;
}