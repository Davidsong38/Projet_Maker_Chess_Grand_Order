//
// Created by david on 25/02/2025.
//

#include "piece_moves.h"

#include <board_pattern.h>
#include <Chessboard.h>
#include <log.h>
#include <GameEngine.h>


piece_move* knight_default_moves = new piece_move();
piece_move* rook_default_moves = new piece_move();
piece_move* bishop_default_moves = new piece_move();
piece_move* queen_default_moves = new piece_move();
piece_move* king_default_moves = new piece_move();
piece_move* pawn_default_moves = new piece_move();

piece_move* alternate_pawn_moves = new piece_move();
piece_move* super_pawn_moves = new piece_move();
piece_move* ushiwakamaru_moves = new piece_move();
piece_move* kintoki_rider_buff_moves = new piece_move();
piece_move* arceuid_buff_move = new piece_move();
piece_move* small_cross_moves = new piece_move();
piece_move* small_cross_3_range_moves = new piece_move();

piece_move* inverted_shinji_moves = new piece_move();
piece_move* shinji_moves = new piece_move();


void init_moves() {
    knight_default_moves->name = "knight_default_moves";
    knight_default_moves->ignoresObstacles = true;
    knight_default_moves->get_positions = [](const glm::ivec2 pos) {
        return knight_pattern->get_positions(pos);
    };

    rook_default_moves->name = "rook_default_moves";
    rook_default_moves->get_positions = [](const glm::ivec2 pos) {
        return cross_pattern->get_positions(pos);
    };

    bishop_default_moves->name = "bishop_default_moves";
    bishop_default_moves->get_positions = [](const glm::ivec2 pos) {
        return x_cross_pattern->get_positions(pos);
    };

    queen_default_moves->name = "queen_default_moves";
    queen_default_moves->get_positions = [](const glm::ivec2 pos) {
        return star_pattern->get_positions(pos);
    };

    king_default_moves->name = "king_default_moves";
    king_default_moves->get_positions = [](const glm::ivec2 pos) {
        return square_1_pattern->get_positions(pos);
    };

    pawn_default_moves->name = "pawn_default_moves";
    pawn_default_moves->pathAlwaysClear = true;
    pawn_default_moves->get_positions = [](const glm::ivec2 pos) {
        std::vector<glm::ivec2> moves;
        const auto* piece = Chessboard::getInstance()->getPieceAt(pos.x, pos.y);
        if (piece == nullptr) {
            ltr_log_error("Request pawn_default_moves from nullptr piece position : ", pos.x, pos.y);
            return moves;
        }
        const int pawnDirection = piece->getIsWhite() ? -1 : 1;
        const bool FirstMove = piece->getIsFirstMove();
        const int board_size = Chessboard::getInstance()->getSize();
        const int board_limit = piece->getIsWhite() ? 0 : board_size - 1;
        const int en_passant_pos = board_limit - 3 * pawnDirection;
        if (
            (pos.x + pawnDirection) * pawnDirection <= board_limit * pawnDirection
            && Chessboard::getInstance()->getPieceAt(pos.x + pawnDirection, pos.y) == nullptr
        ) moves.emplace_back(pos.x + pawnDirection, pos.y );
        if (
            FirstMove
            && (pos.x + 2 * pawnDirection) * pawnDirection <= board_limit * pawnDirection
            && Chessboard::getInstance()->isPathClear(pos.x, pos.y,pos.x + 2 * pawnDirection,pos.y, piece)
            && Chessboard::getInstance()->getPieceAt(pos.x + 2 * pawnDirection, pos.y) == nullptr
        ) moves.emplace_back(pos.x + 2 * pawnDirection, pos.y);
        if (
            pos.x == en_passant_pos
            && Chessboard::getInstance()->isInGrid(pos.x, pos.y - 1)
            && Chessboard::getInstance()->getPieceAt(pos.x, pos.y - 1) != nullptr
            && Chessboard::getInstance()->getPieceAt(pos.x, pos.y - 1)->isPawn()
            && !Chessboard::getInstance()->getPieceAt(pos.x, pos.y - 1)->getIsWhite()
            && GameEngine::getInstance()->getPhaseNumber() - Chessboard::getInstance()->getPieceAt(pos.x, pos.y - 1)->getFirstNormalMovePhase() < 3
            ) moves.emplace_back(pos.x + pawnDirection, pos.y - 1);
        if (
            pos.x == en_passant_pos
            && Chessboard::getInstance()->isInGrid(pos.x, pos.y + 1)
            && Chessboard::getInstance()->getPieceAt(pos.x, pos.y + 1) != nullptr
            && Chessboard::getInstance()->getPieceAt(pos.x, pos.y + 1)->isPawn()
            && !Chessboard::getInstance()->getPieceAt(pos.x, pos.y + 1)->getIsWhite()
            && GameEngine::getInstance()->getPhaseNumber() - Chessboard::getInstance()->getPieceAt(pos.x, pos.y + 1)->getFirstNormalMovePhase() < 3
        ) moves.emplace_back(pos.x + pawnDirection, pos.y + 1);
        for (
            const vector<glm::ivec2> diagonalAttack = {{pawnDirection, -1}, {pawnDirection, 1}};
            const auto& offset : diagonalAttack
        ) {
            const int diagX = pos.x + offset.x;
            const int diagY = pos.y + offset.y;
            if (
                Chessboard::getInstance()->isInGrid(diagX, diagY)
                && Chessboard::getInstance()->getPieceAt(diagX, diagY) != nullptr
                && !Chessboard::isAlly(piece, Chessboard::getInstance()->getPieceAt(diagX, diagY))
            ) {
                moves.emplace_back(diagX, diagY);
            }
        }
        return moves;
    };

    alternate_pawn_moves->name = "alternate_pawn_moves";
    alternate_pawn_moves->get_positions = [](const glm::ivec2 pos) {
        std::vector<glm::ivec2> moves;
        const auto* piece = Chessboard::getInstance()->getPieceAt(pos.x, pos.y);
        if (piece == nullptr) {
            ltr_log_error("Request alternate_pawn_moves from nullptr piece position : ", pos.x, pos.y);
            return moves;
        }
        if (piece->getIsWhite()){
            return line_N_generic_pattern(pos, 2);
        }
        return line_S_generic_pattern(pos, 2);
    };

    super_pawn_moves->name = "super_pawn_moves";
    super_pawn_moves->canKillKing = false;
    super_pawn_moves->get_positions = [](const glm::ivec2 pos) {
        std::vector<glm::ivec2> moves;
        const auto* piece = Chessboard::getInstance()->getPieceAt(pos.x, pos.y);
        if (piece == nullptr) {
            ltr_log_error("Request super_pawn_moves from nullptr piece position : ", pos.x, pos.y);
            return moves;
        }
        if (piece->getIsWhite()){
            return scatter_N_generic_pattern(pos, 2);
        }
        return scatter_S_generic_pattern(pos, 2);
    };
    
    ushiwakamaru_moves->name = "ushiwakamaru_moves";
    ushiwakamaru_moves->ignoresObstacles = true;
    ushiwakamaru_moves->get_positions = [](const glm::ivec2 pos) {
        return inverse_checker_2_pattern->get_positions(pos);
    };

    arceuid_buff_move->name = "arceuid_buff_move";
    arceuid_buff_move->get_positions = [](const glm::ivec2 pos) {
        return arcueid_buff_pattern->get_positions(pos);
    };

    kintoki_rider_buff_moves->name = "kintoki_rider_buff_move";
    kintoki_rider_buff_moves->ignoresObstacles = true;
    kintoki_rider_buff_moves->get_positions = [](const glm::ivec2 pos) {
        return kintoki_rider_buff_pattern->get_positions(pos);
    };

    small_cross_moves->name = "small_cross_moves";
    small_cross_moves->get_positions = [](const glm::ivec2 pos) {
        return cross_1_pattern->get_positions(pos);
    };
    
    small_cross_3_range_moves->name = "small_cross_3_range_moves";
    small_cross_3_range_moves->get_positions = [](const glm::ivec2 pos) {
        return cross_3_pattern->get_positions(pos);
    };

    inverted_shinji_moves->name = "inverted_shinji_moves";
    inverted_shinji_moves->ignoresObstacles = true;
    inverted_shinji_moves->get_positions = [](const glm::ivec2 pos) {
        return square_pattern->get_positions(pos);
    };

    shinji_moves->name = "shinji_moves";
    shinji_moves->get_positions = [](const glm::ivec2) {
        std::vector<glm::ivec2> moves;
        return moves;
    };
}