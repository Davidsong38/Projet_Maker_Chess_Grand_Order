//
// Created by david on 24/01/2025.
//

#include "Pieces.h"
#include <iostream>
#include <utility>
#include "Effect_List.h"
//Pieces::Pieces(string name) : name(std::move(name)) {}

float Pieces::getSpriteX() {
    return (-0.875f + 0.25f * static_cast<float>(coordY)) * 1080.0f / 1920.0f;
}

float Pieces::getSpriteY() {
    return -0.875f + 0.25f * static_cast<float>(coordX);
}

string Pieces::getName() {
    return name;
}

bool Pieces::getIsWhite() const {
    return isWhite;
}

int Pieces::getCoordX() const {
    return coordX;
}
int Pieces::getCoordY() const {
    return coordY;
}
void Pieces::setPosition(int newX, int newY) {
    coordX = newX;
    coordY = newY;
}

void Pieces::addEffectStatus(EffectInstance effect_instance) {
    activeEffects.emplace_back(effect_instance.getEffect(),effect_instance.getEffectDuration(),effect_instance.getEffectDuration());
}

Characters_List Pieces::getCharacters() const {
    return characters;
}
Pieces_List Pieces::getPiecesOrigin() const {
    return pieces_origin;
}
//void Pieces::affectCharacter(const Character_Instance& character_instance) {
//    characters.emplace_back(character_instance);
//}


bool Pieces::hasEffectStatus(Effect_List effect) const {
    for (const auto& e : activeEffects) {
        if (e.effect == effect && !e.isExpired()) {
            return true;
        }
    }
    return false;

}

void Pieces::updateEffectStatus() {
    for (auto effect=activeEffects.begin(); effect!=activeEffects.end();) {
        effect->decrement_duration();
        if (effect->isExpired()) {
            effect= activeEffects.erase(effect);
        } else {
            ++effect;
        }
    }
}
void Pieces::activateEffect(Effect_List effect) {
    std::cout << "Yaharo0000"<<std::endl;
    for ( auto& e : activeEffects) {
        std::cout << Effect_List_to_string[e.effect] << "Yaharo"<<std::endl;
        if (e.effect == effect && !e.isExpired()) {
            std::cout << Effect_List_to_string[e.effect] << std::endl;
            e.activation();
            std::cout << "Effect " << Effect_List_to_string[e.effect] << " activated on piece!" << std::endl;

        }
    }
}

void Pieces::displayEffect() {
    for (const auto& e : activeEffects) {
        std::cout << "Effect: " << Effect_List_to_string[e.effect]
                  << ", Duration: " << (e.effect_duration == -1 ? "Infinite" : std::to_string(e.effect_duration))
                  << ", Activations: " << (e.effect_amount == -1 ? "Infinite" : std::to_string(e.effect_amount))
                  << std::endl;
    }
}

vector<EffectInstance> Pieces::getActive_effects() const {
    return activeEffects;
}


bool Pieces::isPawn() const {
    if (pieces_origin == PAWN){
        return true;
        }
    return false;
}

bool Pieces::isKnight() const {
    if (pieces_origin == KNIGHT) {
        return true;
    }
    return false;
}

bool Pieces::isBishop() const {
    if (pieces_origin == BISHOP) {
        return true;
    }
    return false;
}

bool Pieces::isRook() const {
    if (pieces_origin == ROOK) {
        return true;
    }
    return false;
}

bool Pieces::isQueen() const {
    if (pieces_origin == QUEEN) {
        return true;
    }
    return false;
}

bool Pieces::isKing() const {
    if (pieces_origin == KING) {
        return true;
    }
    return false;
}

