//
// Created by david on 24/01/2025.
//

#include "Pieces.h"

#include <Context.h>
#include <iostream>
#include <utility>
#include "Effect_List.h"
//Pieces::Pieces(string name) : name(std::move(name)) {}

float Pieces::getSpriteX() {
    return (-0.875f + 0.25f * static_cast<float>(coordY)) * 1080.0f / 1920.0f;
}

float Pieces::getSpriteY() {
    return 0.875f - 0.25f * static_cast<float>(coordX);
}

float Pieces::getSpriteRotation() {
    return (isWhite) ? 0.0f : 180.0f;
}

glm::vec3 Pieces::getFilterColor() {
    if (isWhite)
        return glm::vec3();
    else
        return glm::vec3(-0.2f,-0.2f,-0.2f);
}

glm::vec4 Pieces::getDefaultColor() {
    if (selected)
        if (isWhite)
            return glm::vec4(0.25f, 0.875f, 1.0f, 0.3f);
        else
            return glm::vec4(1.0f, 0.3f, 0.3f, 0.3f);
    else
            return glm::vec4();
}

string Pieces::getName() {
    return name;
}

bool Pieces::getIsEvolved() const{
    return evolved;
}

bool Pieces::getIsWhite() const {
    return isWhite;
}

void Pieces::setIsWhite(bool is_white) {
    isWhite = is_white;
}

bool Pieces::getIsFirstMove() const{
    return isFirstMove;
}

bool Pieces::getFirstMoveLastTurn() const {
    return firstMoveLastTurn;
}

int Pieces::getTurnStamp() const {
    return TurnStamp;
}

bool Pieces::getCanActivateEffects() const {
    return canActivateEffects;
}

bool Pieces::getHasJustKilled() const {
    return hasJustKilled;
}

int Pieces::getPieceGameMode() const {
    return pieceGameMode;
}

bool Pieces::getIsOnAMove() const{
    return isOnAMove;
}

int Pieces::getNB_TurnWithoutMoving() const{
    return NB_TurnWithoutMoving;
}

bool Pieces::getHasRoqued() const{
    return hasRoqued;
}

vector<pair<int, int>> Pieces::getAllMovesDoneBefore() const{
    return AllMovesDoneBefore;
}

int Pieces::getMovesMode() const{
    return movesMode;
}

function<vector<pair<int, int>>()> Pieces::getOverrideMoves() const{
    return overrideMoves;
}

void Pieces::setMovesMode(int moves_mode){
    movesMode = moves_mode;
}

void Pieces::clearOverrideMoves(){
    overrideMoves = nullptr;
}


void Pieces::setOverrideMoves(const function<vector<pair<int, int>>()>& override_moves){
    overrideMoves = override_moves;
}

void Pieces::addToAllMovesDoneBefore(int lastCoordX, int lastCoordY){
    AllMovesDoneBefore.emplace_back(lastCoordX,lastCoordY);
}

//void Pieces::setLastPosX(int last_pos_x){
//    lastPosX = last_pos_x;
//}
//
//void Pieces::setLastPosY(int last_pos_y){
//    lastPosY = last_pos_y;
//}

void Pieces::setHasRoqued(bool has_roqued){
    hasRoqued = has_roqued;
}

void Pieces::setNB_TurnWithoutMoving(int nb_turn_without_moving){
    NB_TurnWithoutMoving = nb_turn_without_moving;
}

void Pieces::setIsOnAMove(bool is_on_a_move){
    isOnAMove = is_on_a_move;
}

void Pieces::setHasJustKilled(bool has_just_killed) {
    hasJustKilled = has_just_killed;
}

void Pieces::setFirstMoveLastTurn(bool first_move_last_turn) {
    firstMoveLastTurn = first_move_last_turn;
}



void Pieces::setTurnStamp(int turn_stamp) {
    TurnStamp = turn_stamp;
}

int Pieces::getCNTMove() const {
    return CNTMove;
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
    isFirstMove = false;
}

void Pieces::setCanActivateEffects(bool can_activate_effects) {
    canActivateEffects = can_activate_effects;
}

bool Pieces::isHidden() {
    return !isAlive;
}

void Pieces::setIsAlive(bool is_alive) {
    isAlive = is_alive;
}


void Pieces::addEffectStatus(EffectInstance effect_instance) {

    activeEffects.emplace_back(effect_instance.getEffect(),effect_instance.getEffectDuration(),effect_instance.getEffectAmount(),effect_instance.getNB_Target(),effect_instance.caster_piece);
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
        if (effect->effect == CHANGE_CONTROL && (effect->effect_duration == 1 || effect->effect_amount == 0)){
            //std::cout << "allllllezzzzz" << std::endl;
            this->setIsWhite(not this->getIsWhite());
        }
        if (effect->effect == MOVE_CHANGING && effect->effect_amount == 0){
            setMovesMode(0);
            clearOverrideMoves();
            std::cout << "abracadabra" << std::endl;
        }
        effect->decrement_duration();
        if (effect->isExpired()) {
            effect= activeEffects.erase(effect);
        } else {
            ++effect;
        }
    }
}

void Pieces::deleteEffect(Effect_List effect)
{
    for (auto& e : activeEffects){
        if (e.effect == effect && !e.isExpired()){
            e.setEffectAmount(0);
        }
    }
}

void Pieces::activateEffect(Effect_List effect) {
    //std::cout << "Yaharo0000"<<std::endl;
    for ( auto& e : activeEffects) {
        //std::cout << Effect_List_to_string[e.effect] << "Yaharo"<<std::endl;
        if (e.caster_piece != nullptr && (e.effect == IMMUNITY_AOE || e.effect == IMMUNITY_EFFECT)){
            static_cast<Pieces*>(e.caster_piece)->evolved = true;
            //std::cout << static_cast<Pieces*>(e.caster_piece)->getName() << static_cast<Pieces*>(e.caster_piece)->evolved <<std::endl;
        }
        if (e.effect == effect && !e.isExpired()) {
            //std::cout << e.effect_amount << " je vais devenir fou " <<std::endl;
            //std::cout << e.effect_duration << " je vais devenir fou " <<std::endl;
            e.activation();
            //std::cout << "Effect " << Effect_List_to_string[e.effect] << " activated on piece!" << std::endl;

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

void Pieces::setPiecesOrigin(Pieces_List pieces_origin) {
            this->pieces_origin = pieces_origin;
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

