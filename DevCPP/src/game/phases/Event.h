//
// Created by david on 23/02/2025.
//

#ifndef EVENT_H
#define EVENT_H

#include <log.h>
#include <PhaseHandler.h>
#include <event_type.h>
#include <Pieces.h>
#include <glm.hpp>
#include <Chessboard.h>

class Event {
public:
  virtual ~Event() = default;
  explicit Event(const int eventType)
  : eventType(eventType), eventTurn(PhaseHandler::getTurnNumber()), eventPhase(PhaseHandler::getPhaseNumber()) {}
  const int eventType;
  const int eventTurn;
  const int eventPhase;
  virtual std::vector<Pieces*> getAllConcernedPieces() {
    std::vector<Pieces*> concernedPieces;
    return concernedPieces;
  }
};

class EventMove final : public Event {
public:
  explicit EventMove(Pieces* piece) : Event(EVENT_MOVE), piece(piece), moveType(MOVE_NORMAL) {
    this->startPos.x = this->piece->getCoordX();
    this->startPos.y = this->piece->getCoordY();
  }
  EventMove(Pieces* piece, const glm::ivec2 startPos) : Event(EVENT_MOVE), piece(piece), moveType(MOVE_NORMAL) {
    this->startPos = startPos;
    this->endPos.x = this->piece->getCoordX();
    this->endPos.y = this->piece->getCoordY();
  }
  EventMove(Pieces* piece, const int moveType) : Event(EVENT_MOVE), piece(piece), moveType(moveType) {
    this->startPos.x = this->piece->getCoordX();
    this->startPos.y = this->piece->getCoordY();
  }
  EventMove(Pieces* piece, const glm::ivec2 startPos, const int moveType) : Event(EVENT_MOVE), piece(piece), moveType(moveType) {
    this->startPos = startPos;
    this->endPos.x = this->piece->getCoordX();
    this->endPos.y = this->piece->getCoordY();
  }
  void setEndPos(const glm::ivec2 endPos) {
    this->endPos = endPos;
  }
  Pieces* piece;
  const int moveType;
  [[nodiscard]] glm::ivec2 getStartPos() const {return startPos;}
  [[nodiscard]] glm::ivec2 getEndPos() const {return endPos;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(piece);
    return concernedPieces;
  }
protected:
  glm::ivec2 startPos{};
  glm::ivec2 endPos{};
};

class EventKill final : public Event {
public:
  EventKill(Pieces* killedPiece, Pieces* killerPiece, const glm::ivec2 killPos, const int killType)
  : Event(EVENT_KILL), killedPiece(killedPiece), killerPiece(killerPiece), killType(killType) {
    this->killPos = killPos;
  }
  Pieces* killedPiece;
  Pieces* killerPiece;
  const int killType;
  [[nodiscard]] glm::ivec2 getKillPos() const {return killPos;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(killedPiece);
    concernedPieces.emplace_back(killerPiece);
    return concernedPieces;
  }
protected:
  glm::ivec2 killPos{};
};

class EventSpellUsed final : public Event {
public:
  explicit EventSpellUsed(const EffectInstance *effect_instance)
  : Event(EVENT_SPELL_USED), casterPiece(static_cast<Pieces*>(effect_instance->caster_piece)), spellType(effect_instance->effect) {
    this->casterPosition.x = casterPiece->getCoordX();
    this->casterPosition.y = casterPiece->getCoordY();
    for (auto &target: effect_instance->target_pieces) {
      const auto piece = static_cast<Pieces*>(target);
      targetPieces.emplace_back(piece);
      spellPositions.emplace_back(piece->getCoordX(), piece->getCoordY());
    }
    for (auto &target: effect_instance->target_cells) {
      const auto cell = static_cast<chessboard_cell*>(target);
      targetCells.emplace_back(cell);
      spellPositions.emplace_back(cell->pos);
    }
  }
  void setSuccess(const bool success) {
    if (this->success != -1)
      return;
    this->success = success ? 1 : 0;
  }
  Pieces* casterPiece;
  const int spellType;
  [[nodiscard]] glm::ivec2 getCasterPosition() const {return casterPosition;}
  [[nodiscard]] std::vector<Pieces *> getTargetPieces() const {return targetPieces;}
  [[nodiscard]] std::vector<glm::ivec2> getSpellPositions() const {return spellPositions;}
  [[nodiscard]] bool getSuccess() const {
    return success == 1;
  }
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(casterPiece);
    for (auto &target: targetPieces)
      if (target != casterPiece)
        concernedPieces.emplace_back(target);
    return concernedPieces;
  }
protected:
  glm::ivec2 casterPosition{};
  std::vector<Pieces*> targetPieces;
  std::vector<chessboard_cell*> targetCells{};
  std::vector<glm::ivec2> spellPositions;
  int success = -1;
};

class EventEffectTriggered final : public Event {
public:
  explicit EventEffectTriggered(const EffectInstance *effect_instance)
  : Event(EVENT_EFFECT_TRIGGERED), casterPiece(static_cast<Pieces*>(effect_instance->caster_piece)), spellType(effect_instance->effect) {
    this->casterPosition.x = casterPiece->getCoordX();
    this->casterPosition.y = casterPiece->getCoordY();
  }
  void addTargetPiece(Pieces* targetPiece) {
    this->targetPieces.emplace_back(targetPiece);
    this->spellPositions.emplace_back(targetPiece->getCoordX(), targetPiece->getCoordY());
  }
  void addTargetCells(chessboard_cell* targetCell) {
    this->targetCells.emplace_back(targetCell);
    this->spellPositions.emplace_back(targetCell->pos);
  }
  Pieces* casterPiece;
  const int spellType;
  [[nodiscard]] glm::ivec2 getCasterPosition() const {return casterPosition;}
  [[nodiscard]] std::vector<Pieces *> getTargetPieces() const {return targetPieces;}
  [[nodiscard]] std::vector<chessboard_cell *> getTargetCells() const {return targetCells;}
  [[nodiscard]] std::vector<glm::ivec2> getSpellPositions() const {return spellPositions;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(casterPiece);
    for (auto &target: targetPieces)
      if (target != casterPiece)
        concernedPieces.emplace_back(target);
    return concernedPieces;
  }
protected:
  glm::ivec2 casterPosition{};
  std::vector<Pieces*> targetPieces;
  std::vector<chessboard_cell*> targetCells{};
  std::vector<glm::ivec2> spellPositions;
};

class EventEffectApply final : public Event {
public:
  explicit EventEffectApply(const EffectInstance *effect_instance)
  : Event(EVENT_EFFECT_APPLY), casterPiece(static_cast<Pieces*>(effect_instance->caster_piece)), spellType(effect_instance->effect), duration(effect_instance->effect_duration), amount(effect_instance->effect_amount) {
    this->casterPosition.x = casterPiece->getCoordX();
    this->casterPosition.y = casterPiece->getCoordY();
  }
  void addTargetPiece(Pieces* targetPiece) {
    this->targetPieces.emplace_back(targetPiece);
    this->spellPositions.emplace_back(targetPiece->getCoordX(), targetPiece->getCoordY());
  }
  void addTargetCells(chessboard_cell* targetCell) {
    this->targetCells.emplace_back(targetCell);
    this->spellPositions.emplace_back(targetCell->pos);
  }
  Pieces* casterPiece;
  const int spellType;
  const int duration;
  const int amount;
  [[nodiscard]] glm::ivec2 getCasterPosition() const {return casterPosition;}
  [[nodiscard]] std::vector<Pieces *> getTargetPieces() const {return targetPieces;}
  [[nodiscard]] std::vector<chessboard_cell *> getTargetCells() const {return targetCells;}
  [[nodiscard]] std::vector<glm::ivec2> getSpellPositions() const {return spellPositions;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(casterPiece);
    for (auto &target: targetPieces)
      if (target != casterPiece)
        concernedPieces.emplace_back(target);
    return concernedPieces;
  }
protected:
  glm::ivec2 casterPosition{};
  std::vector<Pieces*> targetPieces;
  std::vector<chessboard_cell*> targetCells{};
  std::vector<glm::ivec2> spellPositions;
};

class EventEffectUpdate final : public Event {
public:
  explicit EventEffectUpdate(const EffectInstance *effect_instance)
  : Event(EVENT_EFFECT_UPDATE), casterPiece(static_cast<Pieces*>(effect_instance->caster_piece)), spellType(effect_instance->effect), amount(effect_instance->effect_amount) {
    this->casterPosition.x = casterPiece->getCoordX();
    this->casterPosition.y = casterPiece->getCoordY();
  }
  void addTargetPiece(Pieces* targetPiece) {
    this->targetPieces.emplace_back(targetPiece);
    this->spellPositions.emplace_back(targetPiece->getCoordX(), targetPiece->getCoordY());
  }
  void addTargetCells(chessboard_cell* targetCell) {
    this->targetCells.emplace_back(targetCell);
    this->spellPositions.emplace_back(targetCell->pos);
  }
  Pieces* casterPiece;
  const int spellType;
  const int amount;
  [[nodiscard]] glm::ivec2 getCasterPosition() const {return casterPosition;}
  [[nodiscard]] std::vector<Pieces *> getTargetPieces() const {return targetPieces;}
  [[nodiscard]] std::vector<chessboard_cell *> getTargetCells() const {return targetCells;}
  [[nodiscard]] std::vector<glm::ivec2> getSpellPositions() const {return spellPositions;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(casterPiece);
    for (auto &target: targetPieces)
      if (target != casterPiece)
        concernedPieces.emplace_back(target);
    return concernedPieces;
  }
protected:
  glm::ivec2 casterPosition{};
  std::vector<Pieces*> targetPieces;
  std::vector<chessboard_cell*> targetCells{};
  std::vector<glm::ivec2> spellPositions;
};

class EventEffectEnd final : public Event  {
public:
  explicit EventEffectEnd(const EffectInstance *effect_instance)
  : Event(EVENT_EFFECT_END), casterPiece(static_cast<Pieces*>(effect_instance->caster_piece)), spellType(effect_instance->effect), duration(effect_instance->effect_duration), amount(effect_instance->effect_amount) {
    this->casterPosition.x = casterPiece->getCoordX();
    this->casterPosition.y = casterPiece->getCoordY();
  }
  void addTargetPiece(Pieces* targetPiece) {
    this->targetPieces.emplace_back(targetPiece);
    this->spellPositions.emplace_back(targetPiece->getCoordX(), targetPiece->getCoordY());
  }
  void addTargetCells(chessboard_cell* targetCell) {
    this->targetCells.emplace_back(targetCell);
    this->spellPositions.emplace_back(targetCell->pos);
  }
  Pieces* casterPiece;
  const int spellType;
  const int duration;
  const int amount;
  [[nodiscard]] glm::ivec2 getCasterPosition() const {return casterPosition;}
  [[nodiscard]] std::vector<Pieces *> getTargetPieces() const {return targetPieces;}
  [[nodiscard]] std::vector<chessboard_cell *> getTargetCells() const {return targetCells;}
  [[nodiscard]] std::vector<glm::ivec2> getSpellPositions() const {return spellPositions;}
  [[nodiscard]] bool gotDeleted() const {return duration > 0 && amount > 0;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(casterPiece);
    for (auto &target: targetPieces)
      if (target != casterPiece)
        concernedPieces.emplace_back(target);
    return concernedPieces;
  }
protected:
  glm::ivec2 casterPosition{};
  std::vector<Pieces*> targetPieces;
  std::vector<chessboard_cell*> targetCells{};
  std::vector<glm::ivec2> spellPositions;
};

class EventEvolved : public Event {
public:
  explicit EventEvolved(Pieces* evolvedPiece)
  : Event(EVENT_EVOLVED), evolvedPiece(evolvedPiece), modifier(0) {
    this->evolvedPosition.x = evolvedPiece->getCoordX();
    this->evolvedPosition.y = evolvedPiece->getCoordY();
  }
  EventEvolved(Pieces* evolvedPiece, const int modifier)
  : Event(EVENT_EVOLVED), evolvedPiece(evolvedPiece), modifier(modifier) {
    this->evolvedPosition.x = evolvedPiece->getCoordX();
    this->evolvedPosition.y = evolvedPiece->getCoordY();
  }
  Pieces* evolvedPiece;
  const int modifier;
  [[nodiscard]] glm::ivec2 getEvolvedPosition() const {return evolvedPosition;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(evolvedPiece);
    return concernedPieces;
  }
protected:
  glm::ivec2 evolvedPosition{};
};

class EventEvolvedKillList final : public EventEvolved {
public:
  explicit EventEvolvedKillList(Pieces* evolvedPiece)
  : EventEvolved(evolvedPiece) {}
  EventEvolvedKillList(Pieces* evolvedPiece, const int modifier)
  : EventEvolved(evolvedPiece, modifier) {}
  void addKilledPiece(Pieces* killedPiece) { this->killList.emplace_back(killedPiece); }
  [[nodiscard]] std::vector<Pieces *> getKillList() const {return killList;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(evolvedPiece);
    for (auto &killedPiece: killList)
      concernedPieces.emplace_back(killedPiece);
    return concernedPieces;
  }
protected:
  std::vector<Pieces*> killList;
};

class EventEvolvedFacingDeath final : public EventEvolved {
public:
  explicit EventEvolvedFacingDeath(Pieces* evolvedPiece)
  : EventEvolved(evolvedPiece) {}
  EventEvolvedFacingDeath(Pieces* evolvedPiece, const int modifier)
  : EventEvolved(evolvedPiece, modifier) {}
  void addKilledPiece(Pieces* killedPiece) { this->facedKills.emplace_back(killedPiece); }
  [[nodiscard]] std::vector<Pieces *> getKillList() const {return facedKills;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(evolvedPiece);
    for (auto &killedPiece: facedKills)
      concernedPieces.emplace_back(killedPiece);
    return concernedPieces;
  }
protected:
  std::vector<Pieces*> facedKills;
};

class EventEvolvedGavePieces final : public EventEvolved {
public:
  explicit EventEvolvedGavePieces(Pieces* evolvedPiece)
  : EventEvolved(evolvedPiece) {}
  EventEvolvedGavePieces(Pieces* evolvedPiece, const int modifier)
  : EventEvolved(evolvedPiece, modifier) {}
  void addKilledPiece(Pieces* pieceGiven) { this->piecesGiven.emplace_back(pieceGiven); }
  [[nodiscard]] std::vector<Pieces *> getPiecesGiven() const {return piecesGiven;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(evolvedPiece);
    for (auto &givenPiece: piecesGiven)
      concernedPieces.emplace_back(givenPiece);
    return concernedPieces;
  }
protected:
  std::vector<Pieces*> piecesGiven;
};

class EventEvolvedEffect : public EventEvolved {
public:
  explicit EventEvolvedEffect(Pieces* evolvedPiece)
  : EventEvolved(evolvedPiece) {}
  EventEvolvedEffect(Pieces* evolvedPiece, const int modifier)
  : EventEvolved(evolvedPiece, modifier) {}
  ///TODO plus tared
protected:
};

class EventEvolvedTakingDamage final : public EventEvolved {
public:
  EventEvolvedTakingDamage(Pieces* evolvedPiece, Pieces* attackerPiece)
  : EventEvolved(evolvedPiece), attackerPiece(attackerPiece) {
    this->attackerPosition.x = attackerPiece->getCoordX();
    this->attackerPosition.y = attackerPiece->getCoordY();
  }
  EventEvolvedTakingDamage(Pieces* evolvedPiece, const int modifier, Pieces* attackerPiece)
  : EventEvolved(evolvedPiece, modifier), attackerPiece(attackerPiece) {
    this->attackerPosition.x = attackerPiece->getCoordX();
    this->attackerPosition.y = attackerPiece->getCoordY();
  }
  Pieces* attackerPiece;
  [[nodiscard]] glm::ivec2 getAttackerPosition() const {return attackerPosition;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(evolvedPiece);
    concernedPieces.emplace_back(attackerPiece);
    return concernedPieces;
  }
protected:
  glm::ivec2 attackerPosition{};
};

class EventEvolvedAllySwap final : public EventEvolved {
public:
  explicit EventEvolvedAllySwap(Pieces* evolvedPiece)
  : EventEvolved(evolvedPiece), allyPiece(nullptr) {}
  EventEvolvedAllySwap(Pieces* evolvedPiece, const int modifier)
  : EventEvolved(evolvedPiece, modifier), allyPiece(nullptr) {}
  EventEvolvedAllySwap(Pieces* evolvedPiece, Pieces* allyPiece)
  : EventEvolved(evolvedPiece), allyPiece(allyPiece) {
      if (allyPiece != nullptr) {
        this->allyPosition.x = allyPiece->getCoordX();
        this->allyPosition.y = allyPiece->getCoordY();
      } else {
        ltr_log_warn("EventEvolvedAllySwap::EventEvolvedAllySwap: allyPiece is nullptr");
      }
  }
  EventEvolvedAllySwap(Pieces* evolvedPiece, const int modifier, Pieces* allyPiece)
  : EventEvolved(evolvedPiece, modifier), allyPiece(allyPiece) {
    if (allyPiece != nullptr) {
      this->allyPosition.x = allyPiece->getCoordX();
      this->allyPosition.y = allyPiece->getCoordY();
    } else {
      ltr_log_warn("EventEvolvedAllySwap::EventEvolvedAllySwap: allyPiece is nullptr");
    }
  }
  Pieces* allyPiece;
  [[nodiscard]] glm::ivec2 getAllyPosition() const {return allyPosition;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(evolvedPiece);
    concernedPieces.emplace_back(allyPiece);
    return concernedPieces;
  }
protected:
  glm::ivec2 allyPosition{};
};

class EventEvolvedFromRelativePosition final : public EventEvolved {
public:
  EventEvolvedFromRelativePosition(Pieces* evolvedPiece, Pieces* referencePiece)
  : EventEvolved(evolvedPiece), referencePiece(referencePiece) {
    this->referencePosition.x = referencePiece->getCoordX();
    this->referencePosition.y = referencePiece->getCoordY();
  }
  EventEvolvedFromRelativePosition(Pieces* evolvedPiece, const int modifier, Pieces* referencePiece)
  : EventEvolved(evolvedPiece, modifier), referencePiece(referencePiece) {
    this->referencePosition.x = referencePiece->getCoordX();
    this->referencePosition.y = referencePiece->getCoordY();
  }
  Pieces* referencePiece;
  [[nodiscard]] glm::ivec2 getReferencePosition() const {return referencePosition;}
  std::vector<Pieces*> getAllConcernedPieces() override {
    std::vector<Pieces*> concernedPieces;
    concernedPieces.emplace_back(evolvedPiece);
    concernedPieces.emplace_back(referencePiece);
    return concernedPieces;
  }
protected:
  glm::ivec2 referencePosition{};
};



#endif //EVENT_H
