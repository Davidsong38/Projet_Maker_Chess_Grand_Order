//
// Created by david on 24/01/2025.
//

#include "Pieces.h"

#include <EffectHandler.h>
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
    for (auto effect : activeEffects) {
        effect->decrement_duration();
        if (effect->isExpired()) {
            if (effect->effect == CHANGE_CONTROL)
                this->setIsWhite(not this->getIsWhite());
            if (effect->effect == ONE_MORE_MOVE)
                this->isOnAMove = false;
            auto* event_effect_delete = new EventEffectEnd(effect);
            event_effect_delete->addTargetPiece(this);
            GameEngine::getInstance()->registerEvent(event_effect_delete);
            std::erase(activeEffects, effect);
        }
    }
}

void Pieces::deleteEffect(const Effect_List effect) {
    for (auto e : activeEffects){
        if (e->effect == effect && !e->isExpired()) {
            auto* event_effect_delete = new EventEffectEnd(e);
            event_effect_delete->addTargetPiece(this);
            GameEngine::getInstance()->registerEvent(event_effect_delete);
            e->effect_amount = 0;
            std::erase(activeEffects, e);
        }
    }
}

void Pieces::activateEffect(const Effect_List effect) {
    for (const auto& e : activeEffects) {
        if (e->effect == effect && !e->isExpired()) {
            e->activation();
            auto* event_effect_update = new EventEffectUpdate(e);
            event_effect_update->addTargetPiece(this);
            GameEngine::getInstance()->registerEvent(event_effect_update);
        }
    }
}

void Pieces::displayEffects() const {
    ltr_log_info(CONSOLE_COLOR_RED, "Displaying effects");
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
    Chessboard::getInstance()->placePiece(x, y, this);
    Chessboard::getInstance()->deletePiece(this);
    auto* moveEvent = new EventMove(this);
    coordX = x;
    coordY = y;
    isFirstMove = false;
    moveEvent->setEndPos(glm::ivec2(x, y));
    GameEngine::getInstance()->registerEvent(moveEvent);
}

void Pieces::goToPosition(const int x, const int y, const int moveType) {
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
    Chessboard::getInstance()->placePiece(x, y, this);
    Chessboard::getInstance()->deletePiece(this);
    auto* moveEvent = new EventMove(this, moveType);
    coordX = x;
    coordY = y;
    isFirstMove = false;
    if (isOnAMove) {
        activateEffect(ONE_MORE_MOVE);
        isOnAMove = false;
    }
    moveEvent->setEndPos(glm::ivec2(x, y));
    GameEngine::getInstance()->registerEvent(moveEvent);
}

void Pieces::gotUnalivedBy(Pieces* killer, const int killType) {
    auto* killEvent = new EventKill(
        this,
        killer,
        glm::ivec2(coordX, coordY),
        killType
    );
    isAlive = false;
    GameEngine::getInstance()->registerEvent(killEvent);
    this->events.emplace_back(killEvent);
    Chessboard::getInstance()->addToDeadList(this);
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

void Pieces::activateSpecialEffect() {
    if (hasThisEffect(GIVING_AOE) && static_cast<Event*>(events.back())->eventType == EVENT_KILL) {
        const int killType = static_cast<EventKill*>(events.back())->killType;
        if (killType == KILL_NORMAL || killType == KILL_EN_PASSANT) {
            auto *  effect_instance = new EffectInstance(
            AOE,
            this,
            1,
            1,
            -1
            );
            EffectHandler::selectRandomTargetPieces(effect_instance);
            EffectHandler::applyToTargets(effect_instance);
            activateEffect(AOE);
            CheckEffectAmount(AOE);
            defaultEffectsRanges[AOE] = [this](){return square_pattern; };
        }
    }
}

void Pieces::CheckEffectAmount(const Effect_List effect) {
    for (const auto & eff : activeEffects) {
        if (eff->effect == effect && eff->isExpired()) {
            if (eff->effect == CHANGE_CONTROL)
                this->setIsWhite(not this->getIsWhite());
            if ((eff->effect == MOVE_CHANGING || eff->effect == SUPP_RANGE))
                override_piece_move = nullptr;
            auto* event_effect_delete = new EventEffectEnd(eff);
            event_effect_delete->addTargetPiece(this);
            GameEngine::getInstance()->registerEvent(event_effect_delete);
            eff->effect_amount = 0;
            std::erase(activeEffects, eff);
        }
    }
}

void Pieces::addEvent(void* event) {
    this->events.emplace_back(event);
    const auto* real_event = static_cast<Event*>(event);
    ltr_log_debug(
        CONSOLE_COLOR_DARK_GREEN,
        "Event added : ", event_type_to_string[real_event->eventType],
        " to piece : ", isWhite ? "White " : "Black ", name,
        ", at turn : ",
        real_event->eventTurn
    );
}

std::vector<void*> Pieces::getAllMoveEvents() {
    std::vector<void*> selected_events;
    for (auto* event_ptr : events) {
        auto event = static_cast<Event*>(event_ptr);
        if (event == nullptr) ltr_log_error("Pieces::getAllMoveEvents: Event nullptr");
        if (event->eventType == EVENT_MOVE) selected_events.emplace_back(event);
    }
    return selected_events;
}




void* Pieces::getLastNormalMoveEvent() {
    for (int i = static_cast<int>(getAllMoveEvents().size()) - 1; i > -1; i--)
        if (const int moveType = static_cast<EventMove*>(getAllMoveEvents()[i])->moveType; moveType == MOVE_NORMAL || moveType == MOVE_ROQUED || moveType == MOVE_EN_PASSANT)
            return getAllMoveEvents()[i];
    return nullptr;
}

int Pieces::getLastNormalMovePhase() {
    const auto last_normal_move = static_cast<EventMove*>(getLastNormalMoveEvent());
    if (last_normal_move == nullptr)
        return 1;
    return last_normal_move->eventPhase;
}

int Pieces::getLastNormalMoveEventType() {
    const auto last_normal_move = static_cast<EventMove*>(getLastNormalMoveEvent());
    if (last_normal_move == nullptr)
        return -69420;
    return last_normal_move->moveType;
}

void* Pieces::getSecondLastNormalMoveEvent() {
    int CNT = 0;
    for (int i = static_cast<int>(getAllMoveEvents().size()) - 1; i > -1; i--)
        if (const int moveType = static_cast<EventMove*>(getAllMoveEvents()[i])->moveType; moveType == MOVE_NORMAL || moveType == MOVE_ROQUED || moveType == MOVE_EN_PASSANT) {
            CNT++;
            if (CNT == 2)
                return getAllMoveEvents()[i];
        }
    return nullptr;
}

int Pieces::getSecondLastNormalMovePhase() {
    const auto second_last_normal_move = static_cast<EventMove*>(getSecondLastNormalMoveEvent());
    if (second_last_normal_move == nullptr)
        return 1;
    return second_last_normal_move->eventPhase;
}

int Pieces::getSecondLastNormalMoveEventType() {
    const auto second_last_normal_move = static_cast<EventMove*>(getSecondLastNormalMoveEvent());
    if (second_last_normal_move == nullptr)
        return -69420;
    return second_last_normal_move->moveType;
}

void* Pieces::getFirstNormalMoveEvent() {
    for (auto* event_ptr : getAllMoveEvents())
        if (const int moveType = static_cast<EventMove*>(event_ptr)->moveType; moveType == MOVE_NORMAL || moveType == MOVE_ROQUED || moveType == MOVE_EN_PASSANT)
            return event_ptr;
    return nullptr;
}

int Pieces::getFirstNormalMovePhase() {
    const auto first_normal_move = static_cast<EventMove*>(getFirstNormalMoveEvent());
    if (first_normal_move == nullptr)
        return -69420;
    return first_normal_move->eventPhase;
}

std::vector<void*> Pieces::getAllKillEvents() {
    std::vector<void*> selected_events;
    for (auto* event_ptr : events) {
        auto event = static_cast<Event*>(event_ptr);
        if (event == nullptr) ltr_log_error("Pieces::getAllKillEvents: Event nullptr");
        if (event->eventType == EVENT_KILL) selected_events.emplace_back(event);
    }
    return selected_events;
}

void* Pieces::getLastKillKillEvent() {
    for (int i = static_cast<int>(getAllKillEvents().size()) - 1; i > -1; i--) {
        if (static_cast<EventKill*>(getAllKillEvents()[i])->killerPiece == this)
            return getAllKillEvents()[i];
    }
    return nullptr;
}

void* Pieces::getSecondLastKillKillEvent() {
    int CNT = 0;
    for (int i = static_cast<int>(getAllKillEvents().size()) - 1; i > -1; i--) {
        if (static_cast<EventKill*>(getAllKillEvents()[i])->killerPiece == this) {
            CNT++;
            if (CNT == 2)
                return getAllKillEvents()[i];
        }
    }
    return nullptr;
}

int Pieces::getLastKillTurn() {
    const auto last_kill = static_cast<EventKill*>(getLastKillKillEvent());
    if (last_kill == nullptr)
        return -69420;
    return last_kill->eventTurn;
}

bool Pieces::gotKillAtTurn(const int turn) {
    for (const auto event_ptr : getAllKillEvents())
        if (const auto event = static_cast<const EventKill*>(event_ptr); event->eventTurn == turn)
            return true;
    return false;
}

void* Pieces::getLastDeathKillEvent() {
    for (int i = static_cast<int>(getAllKillEvents().size()) - 1; i > -1; i--) {
        if (static_cast<EventKill*>(getAllKillEvents()[i])->killedPiece == this)
            return getAllKillEvents()[i];
    }
    return nullptr;
}

void* Pieces::getSecondLastDeathKillEvent() {
    int CNT = 0;
    for (int i = static_cast<int>(getAllKillEvents().size()) - 1; i > -1; i--) {
        if (static_cast<EventKill*>(getAllKillEvents()[i])->killedPiece == this) {
            CNT++;
            if (CNT == 2)
                return getAllKillEvents()[i];
        }
    }
    return nullptr;
}

void* Pieces::getLastKillKillEvent(const int killType) {
    for (int i = static_cast<int>(getAllKillEvents().size()) - 1; i > -1; i--) {
        if (
            static_cast<EventKill*>(getAllKillEvents()[i])->killerPiece == this
            && killType == static_cast<EventKill*>(getAllKillEvents()[i])->killType
        ) return getAllKillEvents()[i];
    }
    return nullptr;
}

void* Pieces::getLastDeathKillEvent(const int killType) {
    for (int i = static_cast<int>(getAllKillEvents().size()) - 1; i > -1; i--) {
        if (
            static_cast<EventKill*>(getAllKillEvents()[i])->killedPiece == this
            && killType == static_cast<EventKill*>(getAllKillEvents()[i])->killType
        ) return getAllKillEvents()[i];
    }
    return nullptr;
}

std::vector<void*> Pieces::getAllSpellUsedEvents() {
    std::vector<void*> selected_events;
    for (auto* event_ptr : events) {
        auto event = static_cast<Event*>(event_ptr);
        if (event == nullptr) ltr_log_error("Pieces::getAllSpellUsedEvents: Event nullptr");
        if (event->eventType == EVENT_SPELL_USED) selected_events.emplace_back(event);
    }
    return selected_events;
}

void* Pieces::getLastSpellUsedByMeEvent() {
    for (int i = static_cast<int>(getAllSpellUsedEvents().size()) - 1; i > -1; i--) {
        if (static_cast<EventSpellUsed*>(getAllSpellUsedEvents()[i])->casterPiece == this)
            return getAllSpellUsedEvents()[i];
    }
    return nullptr;
}

std::vector<void*> Pieces::getAllEffectAppliedEvents() {
    std::vector<void*> selected_events;
    for (auto* event_ptr : events) {
        auto event = static_cast<Event*>(event_ptr);
        if (event == nullptr) ltr_log_error("Pieces::getAllEffectApplyEvents: Event nullptr");
        if (event->eventType == EVENT_EFFECT_APPLY) selected_events.emplace_back(event);
    }
    return selected_events;
}

std::vector<void*> Pieces::getAllEffectAppliedCastedByMeEvent() {
    std::vector<void*> selected_events;
    for (auto* event_ptr : getAllEffectAppliedEvents()) {
        if (
            auto event = static_cast<EventEffectApply*>(event_ptr);
            event->casterPiece == this
        ) selected_events.emplace_back(event);
    }
    return selected_events;
}

std::vector<void*> Pieces::getAllEffectUpdateEvents() {
    std::vector<void*> selected_events;
    for (auto* event_ptr : events) {
        auto event = static_cast<Event*>(event_ptr);
        if (event == nullptr) ltr_log_error("Pieces::getAllEffectUpdateEvents: Event nullptr");
        if (event->eventType == EVENT_EFFECT_UPDATE) selected_events.emplace_back(event);
    }
    return selected_events;
}

std::vector<void*> Pieces::getAllEffectUpdateCastedByMeEvent() {
    std::vector<void*> selected_events;
    for (auto* event_ptr : getAllEffectUpdateEvents()) {
        if (
            auto event = static_cast<EventEffectUpdate*>(event_ptr);
            event->casterPiece == this
        ) selected_events.emplace_back(event);
    }
    return selected_events;
}

std::vector<void*> Pieces::getAllDeathWithEffectCastedByMe() {
    std::vector<void*> dead_list;
    for (auto* event_ptr : getAllEffectAppliedCastedByMeEvent()) {
        for (auto * dead : Chessboard::getInstance()->getDeadList()) {
            auto event = static_cast<EventEffectApply*>(event_ptr);
            for (auto * target : event->getTargetPieces()) {
                if (target == dead)
                    dead_list.emplace_back(dead);
            }
        }
    }
    return dead_list;
}