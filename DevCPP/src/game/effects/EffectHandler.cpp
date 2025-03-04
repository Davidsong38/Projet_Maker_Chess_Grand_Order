//
// Created by david on 25/01/2025.
//

#include "EffectHandler.h"

#include <chrono>
#include <GameEngine.h>
#include <random>

#include "Chessboard.h"
#include <utility>


unordered_map<Effect_List, function<bool()>> EffectHandler::effectBehaviors;


bool EffectHandler::validTargetForEffect(const Pieces* target_piece, const EffectInstance *effect_instance) {
    if (
        target_piece != nullptr
        && target_piece->isKing()
        && (!effect_instance->isBuff()
        || effect_instance->effect == IMMUNITY_AOE
        || effect_instance->effect == IMMUNITY_EFFECT
        || effect_instance->effect == IMMORTALITY)
    ) return false;
    if (
        (
            target_piece != nullptr
            && !Chessboard::isAlly(static_cast<const Pieces*>(effect_instance->caster_piece), target_piece)
            && !effect_instance->isBuff()
            && effect_instance->check_condition(Chessboard::getInstance()->getCellAt(target_piece->getCoordX(), target_piece->getCoordY()))
        ) || (
            target_piece != nullptr
            && Chessboard::isAlly(static_cast<const Pieces*>(effect_instance->caster_piece), target_piece)
            && effect_instance->isBuff())
            && effect_instance->check_condition(Chessboard::getInstance()->getCellAt(target_piece->getCoordX(), target_piece->getCoordY()))
    ) return true;
    return false;
}

int EffectHandler::selectRandomTargetPieces(EffectInstance* effect_instance) {
    board_pattern* effect_range =
        static_cast<Pieces*>(effect_instance->caster_piece)
        ->getDefaultEffectsRangesPattern(effect_instance->effect)();
    const auto * caster = static_cast<Pieces*>(effect_instance->caster_piece);
    const unsigned rd_key = chrono::system_clock::now().time_since_epoch().count();
    std::vector<glm::ivec2> positions = effect_range->get_positions(glm::ivec2(caster->getCoordX(), caster->getCoordY()));
    shuffle(positions.begin(),positions.end(), default_random_engine(rd_key));
    int NB_targetSelected = 0;
    for (const auto &target: positions) {
        if (
            effect_instance->NB_Target != -1
            && NB_targetSelected == effect_instance->NB_Target
        ) break;
        Pieces* target_piece = Chessboard::getInstance()->getPieceAt(target.x, target.y);
        if (!validTargetForEffect(target_piece,effect_instance))
            continue;
        NB_targetSelected++;
        effect_instance->target_pieces.emplace_back(target_piece);
    }
    return NB_targetSelected;
}

int EffectHandler::selectRandomTargetDeadPieces(EffectInstance* effect_instance) {
    auto dead_list = Chessboard::getInstance()->getDeadList();
    const unsigned rd_key = chrono::system_clock::now().time_since_epoch().count();
    shuffle(dead_list.begin(), dead_list.end(), default_random_engine(rd_key));
    int NB_targetSelected = 0;
    for (const auto &target: dead_list) {
        if (
            effect_instance->NB_Target != -1
            && NB_targetSelected == effect_instance->NB_Target
        ) break;
        if (!validTargetForEffect(target, effect_instance))
            continue;
        NB_targetSelected++;
        effect_instance->target_pieces.emplace_back(target);
    }
    return NB_targetSelected;
}

int EffectHandler::selectRandomTargetCells(EffectInstance* effect_instance) {
    board_pattern* effect_range =
        static_cast<Pieces*>(effect_instance->caster_piece)
        ->getDefaultEffectsRangesPattern(effect_instance->effect)();
    const auto * caster = static_cast<Pieces*>(effect_instance->caster_piece);
    const unsigned rd_key = chrono::system_clock::now().time_since_epoch().count();
    std::vector<glm::ivec2> positions = effect_range->get_positions(glm::ivec2(caster->getCoordX(), caster->getCoordY()));
    shuffle(positions.begin(),positions.end(), default_random_engine(rd_key));
    int NB_targetSelected = 0;
    for (const auto &target: positions) {
        if (
            effect_instance->NB_Target != -1
            && NB_targetSelected == effect_instance->NB_Target
        ) break;
        chessboard_cell* target_cell =  Chessboard::getInstance()->getCellAt(target.x, target.y);
        if (!effect_instance->check_condition(target_cell))
            continue;
        NB_targetSelected++;
        effect_instance->target_cells.emplace_back(target_cell);
    }
    return NB_targetSelected;
}

int EffectHandler::selectRandomTargetEmptyCells(EffectInstance* effect_instance) {
    board_pattern* effect_range =
        static_cast<Pieces*>(effect_instance->caster_piece)
        ->getDefaultEffectsRangesPattern(effect_instance->effect)();
    const auto * caster = static_cast<Pieces*>(effect_instance->caster_piece);
    const unsigned rd_key = chrono::system_clock::now().time_since_epoch().count();
    std::vector<glm::ivec2> positions = effect_range->get_positions(glm::ivec2(caster->getCoordX(), caster->getCoordY()));
    shuffle(positions.begin(),positions.end(), default_random_engine(rd_key));
    int NB_targetSelected = 0;
    for (const auto &target: positions) {
        if (
            effect_instance->NB_Target != -1
            && NB_targetSelected == effect_instance->NB_Target
        ) break;
        chessboard_cell* target_cell =  Chessboard::getInstance()->getCellAt(target.x, target.y);
        if (target_cell->piece != nullptr || !effect_instance->check_condition(target_cell))
            continue;
        NB_targetSelected++;
        effect_instance->target_cells.emplace_back(target_cell);
    }
    return NB_targetSelected;
}

int EffectHandler::selectRandomTargetNonEmptyCells(EffectInstance* effect_instance) {
    board_pattern* effect_range =
        static_cast<Pieces*>(effect_instance->caster_piece)
        ->getDefaultEffectsRangesPattern(effect_instance->effect)();
    const auto * caster = static_cast<Pieces*>(effect_instance->caster_piece);
    const unsigned rd_key = chrono::system_clock::now().time_since_epoch().count();
    std::vector<glm::ivec2> positions = effect_range->get_positions(glm::ivec2(caster->getCoordX(), caster->getCoordY()));
    shuffle(positions.begin(),positions.end(), default_random_engine(rd_key));
    int NB_targetSelected = 0;
    for (const auto &target: positions) {
        if (
            effect_instance->NB_Target != -1
            && NB_targetSelected == effect_instance->NB_Target
        ) break;
        chessboard_cell* target_cell =  Chessboard::getInstance()->getCellAt(target.x, target.y);
        if (target_cell->piece == nullptr || !effect_instance->check_condition(target_cell))
            continue;
        NB_targetSelected++;
        effect_instance->target_cells.emplace_back(target_cell);
    }
    return NB_targetSelected;
}

bool EffectHandler::selectManualTargetCells(EffectInstance *effect_instance, selection_request_type request) {
    std::cout << "whites : " << request.whites << "blacks : " << request.blacks <<"emptys : " << request.emptys
    << "target pieces : " << effect_instance->target_pieces.size() << "target cells : " <<effect_instance->target_cells.size() << endl;
    if (
        request.whites + request.blacks == effect_instance->target_pieces.size()
        && request.emptys == effect_instance->target_cells.size()
    ) {
        return true;
    }
    ltr_log_info("Selecting manual target cells...");
    request.effect_instance = effect_instance;
    GameEngine::getInstance()->requestSelection(request);
    return false;
}

bool EffectHandler::applyToTargets(EffectInstance* effect_instance) {
    ltr_log_info(
        "Applying effect : ",
        Effect_List_to_string[effect_instance->effect],
        ", duration : ",
        effect_instance->effect_duration,
        ", amount : ",
        effect_instance->effect_amount,
        ", NB_Target : ",
        effect_instance->NB_Target
    );
    return configureEffectHandler(effect_instance);
}

bool EffectHandler::applyBuffToSelf(EffectInstance* effect_instance) {
    effect_instance->target_pieces.emplace_back(effect_instance->caster_piece);
    return configureEffectHandler(effect_instance);
}

bool EffectHandler::addEffectBehavior(const Effect_List effect, function<bool()> behavior) {
    effectBehaviors[effect] = std::move(behavior);
    return effectBehaviors[effect]();
}

bool EffectHandler::cellIsInRange(const chessboard_cell* cell, const EffectInstance* effect_instance) {
    board_pattern* effect_range =
        static_cast<Pieces*>(effect_instance->caster_piece)
        ->getDefaultEffectsRangesPattern(effect_instance->effect)();
    const auto * caster = static_cast<Pieces*>(effect_instance->caster_piece);
    std::vector<glm::ivec2> positions = effect_range->get_positions(glm::ivec2(caster->getCoordX(), caster->getCoordY()));
    for (const auto &target: positions)
        if (cell->pos.x == target.x && cell->pos.y == target.y)
            return true;
    return false;
}





bool EffectHandler::configureEffectHandler(EffectInstance* effect_instance) {
    bool success;
    switch (effect_instance->effect) {
        case STUN:
            success = addEffectBehavior(
                effect_instance->effect,
                getStunEffect(effect_instance)
            );
        break;
        case AOE:
            success = addEffectBehavior(
                effect_instance->effect,
                getAoeEffect(effect_instance)
            );
        break;
        case CHANGE_CONTROL:
        case CHANGE_CONTROL_ADVANCE:
            success = addEffectBehavior(
                effect_instance->effect,
                getChangeControlEffect(effect_instance)
            );
        break;
        case IMMUNITY_AOE:
        case IMMUNITY_EFFECT:
        case IMMORTALITY:
            success = addEffectBehavior(
                effect_instance->effect,
                getImmunityEffect(effect_instance)
            );
        break;
        case SHIELD:
            success = addEffectBehavior(
                effect_instance->effect,
                getShieldEffect(effect_instance)
            );
        break;
        case ALTERNATE_RANGE:
            success = addEffectBehavior(
                effect_instance->effect,
                getAlternateRangeEffect(effect_instance)
            );
        break;
        case SUPP_RANGE:
            success = addEffectBehavior(
                effect_instance->effect,
                getSuppRangeEffect(effect_instance)
            );
        break;
        case KILLING:
            success = addEffectBehavior(
                effect_instance->effect,
                getKillEffect(effect_instance)
            );
        break;
        case SPAWN_PIECES:
            success = addEffectBehavior(
                effect_instance->effect,
                getSpawnPieceEffect(effect_instance)
            );
        break;
        case ONE_MORE_MOVE:
            success = addEffectBehavior(
                effect_instance->effect,
                getOneMoreMoveEffect(effect_instance)
            );
        break;
        case MOVE_CHANGING:
            success = addEffectBehavior(
                effect_instance->effect,
                getMoveChangingEffect(effect_instance)
            );
        break;
        case SUPP_MOVE:
            success = addEffectBehavior(
                effect_instance->effect,
                getSuppMoveEffect(effect_instance)
            );
        break;
        case GIVING_AOE:
            success = addEffectBehavior(
                effect_instance->effect,
                getGivingAOEEffect(effect_instance)
            );
        break;
        default:
            ltr_log_fatal("EffectHandler::configureEffectHandler: Invalid effect_instance.effect : ", effect_instance->effect);
            success = false;
    }
    return success;
}






function<bool()> EffectHandler::getStunEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectApply(effect_instance);
        for (auto &target: effect_instance->target_pieces) {
            const auto piece = static_cast<Pieces*>(target);
            if (piece->isKing())
                continue;
            bool did_not_work = false;
            for (const auto& e : piece->getActive_effects()) {
                if (e->effect == IMMUNITY_EFFECT) {
                    piece->activateEffect(e->effect);
                    piece->CheckEffectAmount(e->effect);
                    did_not_work = true;
                    break;
                }
            }
            NB_targetTouched++;
            if (did_not_work)
                continue;
            piece->addEffectStatus(effect_instance);
            event_effect_applied->addTargetPiece(piece);
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}

function<bool()> EffectHandler::getAoeEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectTriggered(effect_instance);
        for (auto &target: effect_instance->target_pieces) {
            const auto piece = static_cast<Pieces*>(target);
            if (piece->isKing())
                continue;
            bool did_not_work = false;
            for (const auto& e : piece->getActive_effects()) {
                if (e->effect == SHIELD || e->effect == IMMORTALITY || e->effect == IMMUNITY_AOE) {
                    piece->activateEffect(e->effect);
                    piece->CheckEffectAmount(e->effect);
                    did_not_work = true;
                    break;
                }
            }
            NB_targetTouched++;
            if (did_not_work)
                continue;
            event_effect_applied->addTargetPiece(piece);
            piece->gotUnalivedBy(static_cast<Pieces*>(effect_instance->caster_piece), KILL_AOE_EFFECT);
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}

function<bool()> EffectHandler::getChangeControlEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectApply(effect_instance);
        for (const auto &target: effect_instance->target_pieces) {
            const auto piece = static_cast<Pieces*>(target);
            if (piece->isKing())
                continue;
            bool did_not_work = false;
            for (const auto& e : piece->getActive_effects()) {
                if (e->effect == IMMUNITY_EFFECT) {
                    piece->activateEffect(e->effect);
                    piece->CheckEffectAmount(e->effect);
                    did_not_work = true;
                    break;
                }
            }
            NB_targetTouched++;
            if (did_not_work)
                continue;
            piece->addEffectStatus(effect_instance);
            event_effect_applied->addTargetPiece(piece);
            piece->setIsWhite(!piece->getIsWhite());
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}

function<bool()> EffectHandler::getImmunityEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectApply(effect_instance);
        for (const auto &target: effect_instance->target_pieces) {
            const auto piece = static_cast<Pieces*>(target);
            if (piece->isKing())
                continue;
            NB_targetTouched++;
            piece->addEffectStatus(effect_instance);
            event_effect_applied->addTargetPiece(piece);
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}

function<bool()> EffectHandler::getShieldEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectApply(effect_instance);
        for (const auto &target: effect_instance->target_pieces) {
            const auto piece = static_cast<Pieces*>(target);
            NB_targetTouched++;
            piece->addEffectStatus(effect_instance);
            event_effect_applied->addTargetPiece(piece);
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}

function<bool()> EffectHandler::getAlternateRangeEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectApply(effect_instance);
        for (const auto &target: effect_instance->target_pieces) {
            const auto piece = static_cast<Pieces*>(target);
            NB_targetTouched++;
            piece->addEffectStatus(effect_instance);
            piece->addOverrideMove(alternate_pawn_moves);
            event_effect_applied->addTargetPiece(piece);
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}


function<bool()> EffectHandler::getSuppRangeEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectApply(effect_instance);
        for (const auto &target: effect_instance->target_pieces) {
            const auto piece = static_cast<Pieces*>(target);
            NB_targetTouched++;
            piece->addEffectStatus(effect_instance);
            piece->addOverrideMove(super_pawn_moves);
            event_effect_applied->addTargetPiece(piece);
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}

function<bool()> EffectHandler::getKillEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectTriggered(effect_instance);
        for (const auto &target: effect_instance->target_pieces) {
            const auto piece = static_cast<Pieces*>(target);
            if (piece->isKing())
                continue;
            bool did_not_work = false;
            for (const auto& e : piece->getActive_effects()) {
                if (e->effect == IMMORTALITY || e->effect == IMMUNITY_EFFECT) {
                    piece->activateEffect(e->effect);
                    piece->CheckEffectAmount(e->effect);
                    did_not_work = true;
                    break;
                }
            }
            NB_targetTouched++;
            if (did_not_work)
                continue;
            event_effect_applied->addTargetPiece(piece);
            piece->gotUnalivedBy(static_cast<Pieces*>(effect_instance->caster_piece), KILL_KILL_EFFECT);
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}

function<bool()> EffectHandler::getSpawnPieceEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectTriggered(effect_instance);
        if (effect_instance->target_pieces.size() != effect_instance->target_cells.size()) {
            ltr_log_error(
                "Wrong number of targets for SpawnPieceEffect : ",
                effect_instance->target_pieces.size(),
                " != ",
                effect_instance->target_cells.size()
            );
            return false;
        }
        for (int i = 0; i < effect_instance->target_pieces.size(); i++) {
            const auto piece = static_cast<Pieces*>(effect_instance->target_pieces[i]);
            const auto cell = static_cast<chessboard_cell*>(effect_instance->target_cells[i]);
            if (piece->isKing())
                ltr_log_warn("Spawning a king");
            NB_targetTouched++;
            event_effect_applied->addTargetPiece(piece);
            piece->gotResurrectedAt(static_cast<Pieces*>(effect_instance->caster_piece), cell->pos);
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}

function<bool()> EffectHandler::getOneMoreMoveEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectApply(effect_instance);
        for (const auto &target: effect_instance->target_pieces) {
            const auto piece = static_cast<Pieces*>(target);
            NB_targetTouched++;
            piece->addEffectStatus(effect_instance);
            GameEngine::getInstance()->getCurrentPhaseContext()->firstSelectedPiece = piece;
            piece->setIsOnAMove(true);
            event_effect_applied->addTargetPiece(piece);
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}

function<bool()> EffectHandler::getMoveChangingEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectApply(effect_instance);
        for (const auto &target: effect_instance->target_pieces) {
            const auto piece = static_cast<Pieces*>(target);
            NB_targetTouched++;
            piece->addEffectStatus(effect_instance);
            piece->addOverrideMove(static_cast<Pieces*>(effect_instance->caster_piece)->getCurrentPieceMove());
            event_effect_applied->addTargetPiece(piece);
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}

function<bool()> EffectHandler::getSuppMoveEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectApply(effect_instance);
        for (const auto &target: effect_instance->target_pieces) {
            const auto piece = static_cast<Pieces*>(target);
            NB_targetTouched++;
            piece->addEffectStatus(effect_instance);
            piece->goToPosition(static_cast<chessboard_cell*>(effect_instance->target_cells[0])->pos.x,
                static_cast<chessboard_cell*>(effect_instance->target_cells[0])->pos.y,MOVE_SUPPLEMENTARY);
            event_effect_applied->addTargetPiece(piece);
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}

function<bool()> EffectHandler::getGivingAOEEffect(EffectInstance* effect_instance) {
    return [effect_instance]() {
        int NB_targetTouched = 0;
        auto* event_spell_used = new EventSpellUsed(effect_instance);
        auto* event_effect_applied = new EventEffectApply(effect_instance);
        for (const auto &target: effect_instance->target_pieces) {
            const auto piece = static_cast<Pieces*>(target);
            NB_targetTouched++;
            piece->addEffectStatus(effect_instance);
            piece->setEffectRange(AOE, [piece]() { return cross_1_pattern; });
            event_effect_applied->addTargetPiece(piece);
        }
        const bool success = NB_targetTouched > 0 || !effect_instance->requires_hitting_something;
        event_spell_used->setSuccess(success);
        GameEngine::getInstance()->registerEvent(event_spell_used);
        if (success)
            GameEngine::getInstance()->registerEvent(event_effect_applied);
        return success;
    };
}
