//
// Created by sebas on 23/02/2025.
//

#ifndef PHASE_CONTEXT_H
#define PHASE_CONTEXT_H

#include "Pieces.h"
#include "Event.h"

struct phase_context_type {
    Pieces* firstSelectedPiece;
    std::vector<Event*> events;
    int phaseNumber{0}, turnNumber{1};
};

#endif //PHASE_CONTEXT_H
