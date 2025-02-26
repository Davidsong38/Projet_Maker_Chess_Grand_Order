//
// Created by david on 24/01/2025.
//

#include "Pieces.h"

#include <GameEngine.h>
#include <iostream>
#include <utility>
#include "effects.h"
#include "Event.h"

glm::vec3 Pieces::getFilterColor() {
    if (isWhite)
        return {};
    return {-0.2f,-0.2f,-0.2f};
}

glm::vec4 Pieces::getDefaultColor() {
    if (selected) {
        if (isWhite)
            return {0.25f, 0.875f, 1.0f, 0.3f};
        return {1.0f, 0.3f, 0.3f, 0.3f};
    }
    return {};
}

bool Pieces::hasThisEffect(const Effect_List chosenEffect) const {
    for (const auto& e : activeEffects)
        if (e->effect == chosenEffect)
            return true;
    return false;
}

void Pieces::updateEffectStatus() {
    for (EffectInstance* effect : activeEffects) {
        if (effect->effect == CHANGE_CONTROL && (effect->effect_duration == 1 || effect->effect_amount == 0))
            this->setIsWhite(not this->getIsWhite());
        if ((effect->effect == MOVE_CHANGING || effect->effect == SUPP_RANGE) && effect->effect_amount == 0)
            override_piece_move = nullptr;
        effect->decrement_duration();
        if (effect->isExpired())
            std::erase(activeEffects, effect);
    }
}

void Pieces::deleteEffect(const Effect_List effect) {
    for (auto& e : activeEffects){
        if (e->effect == effect && !e->isExpired()){
            e->effect_amount = 0;
        }
    }
}

void Pieces::activateEffect(const Effect_List effect) {
    for (const auto& e : activeEffects) {
        if (e->caster_piece != nullptr && (e->effect == IMMUNITY_AOE || e->effect == IMMUNITY_EFFECT)){
            static_cast<Pieces*>(e->caster_piece)->evolved = true;
        }
        if (e->effect == effect && !e->isExpired()) {
            e->activation();
        }
    }
}

void Pieces::displayEffects() const {
    for (const auto& e : activeEffects) {
        ltr_log_info(
            CONSOLE_COLOR_MAGENTA,
            "Effect: ",
            Effect_List_to_string[e->effect],
            ", Duration: ",
            e->effect_duration == -1 ? "Infinite" : std::to_string(e->effect_duration),
            ", Activations: ",
            e->effect_amount == -1 ? "Infinite" : std::to_string(e->effect_amount)
        );
    }
}

void Pieces::goToPosition(const int x, const int y) {
    ltr_log_info(
        CONSOLE_COLOR_YELLOW,
        isWhite? "White " : "Black ",
        name,
        " moved from (",
        coordX,
        ", ",
        coordY,
        ") to (",
        x,
        ", ",
        y,
        ")."
    );
    AllMovesDoneBefore.emplace_back(coordX, coordY);
    Chessboard::getInstance()->placePiece(x, y, this);
    Chessboard::getInstance()->deletePiece(this);
    auto* moveEvent = new EventMove(this);
    coordX = x;
    coordY = y;
    isFirstMove = false;
    moveEvent->setEndPos(glm::ivec2(x, y));
    GameEngine::getInstance()->addEvent(moveEvent);
}

void Pieces::goToPosition(const int x, const int y, const bool increaseCountMove) {
    goToPosition(x, y);
    if (increaseCountMove && !isOnAMove)
        CNTMove++;
}

void Pieces::gotUnalivedBy(Pieces* killer, const int killType) {
    auto* killEvent = new EventKill(
        this,
        killer,
        glm::ivec2(coordX, coordY),
        killType
    );
    isAlive = false;
    GameEngine::getInstance()->addEvent(killEvent);
    this->events.emplace_back(killEvent);
    Chessboard::getInstance()->addToDeadList(this);
    if (killer != nullptr)
        killer->setHasJustKilled(true);
    if (isWhite)
        GameEngine::getInstance()->NB_WhiteDead++;
    else
        GameEngine::getInstance()->NB_BlackDead++;
    if (Chessboard::getInstance()->getPieceAt(coordX, coordY) == this)
        Chessboard::getInstance()->deletePiece(this);
    if (killer != nullptr)
        ltr_log_info(
            CONSOLE_COLOR_RED,
            killer->getIsWhite() ? "White " : "Black ",
            killer->getName(),
            " killed ",
            isWhite ? "White " : "Black ",
            name
        );
    else
        ltr_log_info(
            CONSOLE_COLOR_RED,
            isWhite ? "White " : "Black ",
            name,
            " somehow died"
        );
}

void Pieces::gotResurrectedAt(const Pieces* caster, const glm::ivec2 pos) {
    if (Chessboard::getInstance()->getPieceAt(pos.x, pos.y) != nullptr)
        ltr_log_info("Pieces::gotResurrectedAt::error couldn't resurrect at ", pos.x, " ", pos.y);
    isAlive = true;
    Chessboard::getInstance()->removeFromDeadList(this);
    coordX = pos.x;
    coordY = pos.y;
    Chessboard::getInstance()->placePiece(pos.x, pos.y, this);
    if (caster != nullptr)
        ltr_log_info(
            CONSOLE_COLOR_MAGENTA,
            isWhite ? "White " : "Black ",
            name,
            " got resurrected by ",
            caster->getName(),
            " and deadList is now of size ",
            Chessboard::getInstance()->getDeadList().size()
        );
    else
        ltr_log_info(
            CONSOLE_COLOR_MAGENTA,
            isWhite ? "White " : "Black ",
            name,
            " got resurrected by a miracle"
        );
}

piece_move* Pieces::getCurrentPieceMove() const {
    if (override_piece_move == nullptr)
        return default_piece_move;
    return override_piece_move;
}

void Pieces::setPiecesOrigin(const Pieces_List pieces_origin) {
    this->pieces_origin = pieces_origin;
    switch (pieces_origin) {
        case QUEEN:
            this->default_piece_move = queen_default_moves;
        break;
        case KNIGHT:
            this->default_piece_move = knight_default_moves;
        break;
        case KING:
            this->default_piece_move = king_default_moves;
        break;
        case ROOK:
            this->default_piece_move = rook_default_moves;
        break;
        case BISHOP:
            this->default_piece_move = bishop_default_moves;
        break;
        case PAWN:
            this->default_piece_move = pawn_default_moves;
        break;
        default:
            ltr_log_error("Pieces::setPiecesOrigin: Unknown piece type : ", pieces_origin);
            this->default_piece_move = shinji_moves;
        break;
    }
}

void Pieces::addEvent(void* event) {
    this->events.emplace_back(event);
    const auto* real_event = static_cast<Event*>(event);
    ltr_log_debug(
        CONSOLE_COLOR_LIGHT_BLUE,
        "Event added : ", event_type_to_string[real_event->eventType],
        " to piece : ", isWhite ? "White " : "Black ", name,
        ", at turn : ",
        real_event->eventTurn
    );
}