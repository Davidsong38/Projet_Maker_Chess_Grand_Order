//
// Created by sebas on 23/02/2025.
//

#ifndef EVENT_H
#define EVENT_H

#include <log.h>
#include <PhaseHandler.h>
#include <event_type.h>
#include <Pieces.h>
#include <glm.hpp>

class Event {
public:
  explicit Event(const int eventType) : eventType(eventType), eventTurn(PhaseHandler::getTurnNumber()) {}
  const int eventType;
  const int eventTurn;
};

class EventMove : public Event {
public:
  explicit EventMove(const Pieces* piece) : Event(EVENT_MOVE), piece(piece) {
    this->startPos.x = this->piece->getCoordX();
    this->startPos.y = this->piece->getCoordY();
  }
  EventMove(const Pieces* piece, const glm::ivec2 startPos) : Event(EVENT_MOVE), piece(piece) {
    this->startPos = startPos;
    this->endPos.x = this->piece->getCoordX();
    this->endPos.y = this->piece->getCoordY();
  }
  void setEndPos(const glm::ivec2 endPos) {
    this->endPos = endPos;
  }
  const Pieces* piece;
  [[nodiscard]] glm::ivec2 getStartPos() const {return startPos;}
  [[nodiscard]] glm::ivec2 getEndPos() const {return endPos;}
protected:
  glm::ivec2 startPos{};
  glm::ivec2 endPos{};
};

class EventKill : public Event {
public:
  EventKill(const Pieces* killedPiece, const Pieces* killerPiece, const glm::ivec2 killPos, const int killType)
  : Event(EVENT_KILL), killedPiece(killedPiece), killerPiece(killerPiece), killType(killType) {
    this->killPos = killPos;
  }
  const Pieces* killedPiece;
  const Pieces* killerPiece;
  const int killType;
  [[nodiscard]] glm::ivec2 getKillPos() const {return killPos;}
protected:
  glm::ivec2 killPos{};
};

class EventSpellUsed : public Event {
public:
  EventSpellUsed(const Pieces* casterPiece, const int spellType, const bool success)
  : Event(EVENT_SPELL_USED), casterPiece(casterPiece), spellType(spellType), success(success) {
    this->casterPosition.x = casterPiece->getCoordX();
    this->casterPosition.y = casterPiece->getCoordY();
  }
  void addTargetPiece(Pieces* targetPiece) { this->targetPieces.emplace_back(targetPiece); }
  void addSpellPosition(glm::ivec2 spellPosition) { this->spellPositions.emplace_back(spellPosition); }
  const Pieces* casterPiece;
  const int spellType;
  const bool success;
  [[nodiscard]] glm::ivec2 getCasterPosition() const {return casterPosition;}
  [[nodiscard]] std::vector<Pieces *> getTargetPieces() const {return targetPieces;}
  [[nodiscard]] std::vector<glm::ivec2> getSpellPositions() const {return spellPositions;}
protected:
  glm::ivec2 casterPosition{};
  std::vector<Pieces*> targetPieces;
  std::vector<glm::ivec2> spellPositions;
};

class EventEffectTriggered {
  ///TODO
};

class EventEffectApply {
  ///TODO
};

class EventEffectUpdate {
  ///TODO
};

class EventEffectEnd {
  ///TODO
};

class EventEvolved : public Event {
public:
  explicit EventEvolved(const Pieces* evolvedPiece)
  : Event(EVENT_EVOLVED), evolvedPiece(evolvedPiece), modifier(0) {
    this->evolvedPosition.x = evolvedPiece->getCoordX();
    this->evolvedPosition.y = evolvedPiece->getCoordY();
  }
  EventEvolved(const Pieces* evolvedPiece, const int modifier)
  : Event(EVENT_EVOLVED), evolvedPiece(evolvedPiece), modifier(modifier) {
    this->evolvedPosition.x = evolvedPiece->getCoordX();
    this->evolvedPosition.y = evolvedPiece->getCoordY();
  }
  const Pieces* evolvedPiece;
  const int modifier;
  [[nodiscard]] glm::ivec2 getEvolvedPosition() const {return evolvedPosition;}
protected:
  glm::ivec2 evolvedPosition{};
};

class EventEvolvedKillList : public EventEvolved {
public:
  explicit EventEvolvedKillList(const Pieces* evolvedPiece)
  : EventEvolved(evolvedPiece) {}
  EventEvolvedKillList(const Pieces* evolvedPiece, const int modifier)
  : EventEvolved(evolvedPiece, modifier) {}
  void addKilledPiece(Pieces* killedPiece) { this->killList.emplace_back(killedPiece); }
  [[nodiscard]] std::vector<Pieces *> getKillList() const {return killList;}
protected:
  std::vector<Pieces*> killList;
};

class EventEvolvedFacingDeath : public EventEvolved {
public:
  explicit EventEvolvedFacingDeath(const Pieces* evolvedPiece)
  : EventEvolved(evolvedPiece) {}
  EventEvolvedFacingDeath(const Pieces* evolvedPiece, const int modifier)
  : EventEvolved(evolvedPiece, modifier) {}
  void addKilledPiece(Pieces* killedPiece) { this->facedKills.emplace_back(killedPiece); }
  [[nodiscard]] std::vector<Pieces *> getKillList() const {return facedKills;}
protected:
  std::vector<Pieces*> facedKills;
};

class EventEvolvedGavePieces : public EventEvolved {
public:
  explicit EventEvolvedGavePieces(const Pieces* evolvedPiece)
  : EventEvolved(evolvedPiece) {}
  EventEvolvedGavePieces(const Pieces* evolvedPiece, const int modifier)
  : EventEvolved(evolvedPiece, modifier) {}
  void addKilledPiece(Pieces* pieceGiven) { this->piecesGiven.emplace_back(pieceGiven); }
  [[nodiscard]] std::vector<Pieces *> getPiecesGiven() const {return piecesGiven;}
protected:
  std::vector<Pieces*> piecesGiven;
};

class EventEvolvedEffect : public EventEvolved {
public:
  explicit EventEvolvedEffect(const Pieces* evolvedPiece)
  : EventEvolved(evolvedPiece) {}
  EventEvolvedEffect(const Pieces* evolvedPiece, const int modifier)
  : EventEvolved(evolvedPiece, modifier) {}
  ///TODO plus tared
protected:
};

class EventEvolvedTakingDamage : public EventEvolved {
public:
  EventEvolvedTakingDamage(const Pieces* evolvedPiece, const Pieces* attackerPiece)
  : EventEvolved(evolvedPiece), attackerPiece(attackerPiece) {
    this->attackerPosition.x = attackerPiece->getCoordX();
    this->attackerPosition.y = attackerPiece->getCoordY();
  }
  EventEvolvedTakingDamage(const Pieces* evolvedPiece, const int modifier, const Pieces* attackerPiece)
  : EventEvolved(evolvedPiece, modifier), attackerPiece(attackerPiece) {
    this->attackerPosition.x = attackerPiece->getCoordX();
    this->attackerPosition.y = attackerPiece->getCoordY();
  }
  const Pieces* attackerPiece;
  [[nodiscard]] glm::ivec2 getAttackerPosition() const {return attackerPosition;}
protected:
  glm::ivec2 attackerPosition{};
};

class EventEvolvedAllySwap : public EventEvolved {
public:
  explicit EventEvolvedAllySwap(const Pieces* evolvedPiece)
  : EventEvolved(evolvedPiece), allyPiece(nullptr) {}
  EventEvolvedAllySwap(const Pieces* evolvedPiece, const int modifier)
  : EventEvolved(evolvedPiece, modifier), allyPiece(nullptr) {}
  EventEvolvedAllySwap(const Pieces* evolvedPiece, const Pieces* allyPiece)
  : EventEvolved(evolvedPiece), allyPiece(allyPiece) {
      if (allyPiece != nullptr) {
        this->allyPosition.x = allyPiece->getCoordX();
        this->allyPosition.y = allyPiece->getCoordY();
      } else {
        ltr_log_warn("EventEvolvedAllySwap::EventEvolvedAllySwap: allyPiece is nullptr");
      }
  }
  EventEvolvedAllySwap(const Pieces* evolvedPiece, const int modifier, const Pieces* allyPiece)
  : EventEvolved(evolvedPiece, modifier), allyPiece(allyPiece) {
    if (allyPiece != nullptr) {
      this->allyPosition.x = allyPiece->getCoordX();
      this->allyPosition.y = allyPiece->getCoordY();
    } else {
      ltr_log_warn("EventEvolvedAllySwap::EventEvolvedAllySwap: allyPiece is nullptr");
    }
  }
  const Pieces* allyPiece;
  [[nodiscard]] glm::ivec2 getAllyPosition() const {return allyPosition;}
protected:
  glm::ivec2 allyPosition{};
};

class EventEvolvedFromRelativePosition : public EventEvolved {
public:
  EventEvolvedFromRelativePosition(const Pieces* evolvedPiece, const Pieces* referencePiece)
  : EventEvolved(evolvedPiece), referencePiece(referencePiece) {
    this->referencePosition.x = referencePiece->getCoordX();
    this->referencePosition.y = referencePiece->getCoordY();
  }
  EventEvolvedFromRelativePosition(const Pieces* evolvedPiece, const int modifier, const Pieces* referencePiece)
  : EventEvolved(evolvedPiece, modifier), referencePiece(referencePiece) {
    this->referencePosition.x = referencePiece->getCoordX();
    this->referencePosition.y = referencePiece->getCoordY();
  }
  const Pieces* referencePiece;
  [[nodiscard]] glm::ivec2 getReferencePosition() const {return referencePosition;}
protected:
  glm::ivec2 referencePosition{};
};



#endif //EVENT_H
