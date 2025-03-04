//
// Created by boolean_sniper on 26/02/2025.
//

#include "board_pattern.h"

#include <Chessboard.h>
#include <GameEngine.h>
#include <cstdarg>

board_pattern* cross_pattern = new board_pattern();
board_pattern* cross_1_pattern = new board_pattern();
board_pattern* cross_2_pattern = new board_pattern();
board_pattern* cross_3_pattern = new board_pattern();

board_pattern* x_cross_pattern = new board_pattern();
board_pattern* x_cross_1_pattern = new board_pattern();
board_pattern* x_cross_2_pattern = new board_pattern();
board_pattern* x_cross_3_pattern = new board_pattern();

board_pattern* star_pattern = new board_pattern();
board_pattern* star_1_pattern = new board_pattern();
board_pattern* star_2_pattern = new board_pattern();
board_pattern* star_3_pattern = new board_pattern();

board_pattern* square_pattern = new board_pattern();
board_pattern* square_1_pattern = new board_pattern();
board_pattern* square_2_pattern = new board_pattern();
board_pattern* square_3_pattern = new board_pattern();

board_pattern* checker_pattern = new board_pattern();
board_pattern* checker_1_pattern = new board_pattern();
board_pattern* checker_2_pattern = new board_pattern();
board_pattern* checker_3_pattern = new board_pattern();
board_pattern* checker_4_pattern = new board_pattern();


board_pattern* inverse_checker_2_pattern = new board_pattern();
board_pattern* inverse_checker_3_pattern = new board_pattern();
board_pattern* inverse_checker_4_pattern = new board_pattern();

board_pattern* knight_pattern = new board_pattern();

board_pattern* arcueid_buff_pattern = new board_pattern();
board_pattern* kintoki_rider_buff_pattern = new board_pattern();
board_pattern* nitocris_alter_spawn_pattern = new board_pattern();

board_pattern* scatter_N_pattern = new board_pattern();
board_pattern* scatter_S_pattern = new board_pattern();
board_pattern* scatter_E_pattern = new board_pattern();
board_pattern* scatter_W_pattern = new board_pattern();

board_pattern* scatter_NE_pattern = new board_pattern();
board_pattern* scatter_NW_pattern = new board_pattern();
board_pattern* scatter_SE_pattern = new board_pattern();
board_pattern* scatter_SW_pattern = new board_pattern();

void init_board_patterns() {
    cross_pattern->name = "cross_pattern";
    cross_pattern->get_positions = [](const glm::ivec2 pos) {
        return cross_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };

    cross_1_pattern->name = "cross_1_pattern";
    cross_1_pattern->nextSizePattern = cross_2_pattern;
    cross_1_pattern->get_positions = [](const glm::ivec2 pos) {
        return cross_generic_pattern(pos, 1);
    };

    cross_2_pattern->name = "cross_2_pattern";
    cross_2_pattern->nextSizePattern = cross_3_pattern;
    cross_2_pattern->get_positions = [](const glm::ivec2 pos) {
        return cross_generic_pattern(pos, 2);
    };

    cross_3_pattern->name = "cross_3_pattern";
    cross_3_pattern->get_positions = [](const glm::ivec2 pos) {
        return cross_generic_pattern(pos, 3);
    };

    x_cross_pattern->name = "x_cross_pattern";
    x_cross_pattern->get_positions = [](const glm::ivec2 pos) {
        return x_cross_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };

    x_cross_1_pattern->name = "x_cross_1_pattern";
    x_cross_1_pattern->nextSizePattern = x_cross_2_pattern;
    x_cross_1_pattern->get_positions = [](const glm::ivec2 pos) {
        return x_cross_generic_pattern(pos, 1);
    };

    x_cross_2_pattern->name = "x_cross_2_pattern";
    x_cross_2_pattern->nextSizePattern = x_cross_3_pattern;
    x_cross_2_pattern->get_positions = [](const glm::ivec2 pos) {
        return x_cross_generic_pattern(pos, 2);
    };

    x_cross_3_pattern->name = "x_cross_3_pattern";
    x_cross_3_pattern->get_positions = [](const glm::ivec2 pos) {
        return x_cross_generic_pattern(pos, 3);
    };

    star_pattern->name = "star_pattern";
    star_pattern->get_positions = [](const glm::ivec2 pos) {
        return star_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };

    star_1_pattern->name = "star_1_pattern";
    star_1_pattern->nextSizePattern = star_2_pattern;
    star_1_pattern->get_positions = [](const glm::ivec2 pos) {
        return star_generic_pattern(pos, 1);
    };

    star_2_pattern->name = "star_2_pattern";
    star_2_pattern->nextSizePattern = star_3_pattern;
    star_2_pattern->get_positions = [](const glm::ivec2 pos) {
        return star_generic_pattern(pos, 2);
    };

    star_3_pattern->name = "star_3_pattern";
    star_3_pattern->get_positions = [](const glm::ivec2 pos) {
        return star_generic_pattern(pos, 3);
    };

    square_pattern->name = "square_pattern";
    square_pattern->get_positions = [](const glm::ivec2 pos) {
        return square_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };

    square_1_pattern->name = "square_1_pattern";
    square_1_pattern->nextSizePattern = square_2_pattern;
    square_1_pattern->get_positions = [](const glm::ivec2 pos) {
        return square_generic_pattern(pos, 1);
    };

    square_2_pattern->name = "square_2_pattern";
    square_2_pattern->nextSizePattern = square_3_pattern;
    square_2_pattern->get_positions = [](const glm::ivec2 pos) {
        return square_generic_pattern(pos, 2);
    };

    square_3_pattern->name = "square_3_pattern";
    square_3_pattern->get_positions = [](const glm::ivec2 pos) {
        return square_generic_pattern(pos, 3);
    };

    checker_pattern->name = "checker_pattern";
    checker_pattern->get_positions = [](const glm::ivec2 pos) {
        return checker_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };

    checker_1_pattern->name = "checker_1_pattern";
    checker_1_pattern->nextSizePattern = inverse_checker_2_pattern;
    checker_1_pattern->get_positions = [](const glm::ivec2 pos) {
        return checker_generic_pattern(pos, 1);
    };

    checker_2_pattern->name = "checker_2_pattern";
    checker_2_pattern->nextSizePattern = inverse_checker_3_pattern;
    checker_2_pattern->get_positions = [](const glm::ivec2 pos) {
        return checker_generic_pattern(pos, 2);
    };

    checker_3_pattern->name = "checker_3_pattern";
    checker_3_pattern->nextSizePattern = inverse_checker_4_pattern;
    checker_3_pattern->get_positions = [](const glm::ivec2 pos) {
        return checker_generic_pattern(pos, 3);
    };

    checker_4_pattern->name = "checker_4_pattern";
    checker_4_pattern->get_positions = [](const glm::ivec2 pos) {
        return checker_generic_pattern(pos, 4);
    };

    inverse_checker_2_pattern->name = "inverse_checker_2_pattern";
    inverse_checker_2_pattern->nextSizePattern = checker_3_pattern;
    inverse_checker_2_pattern->get_positions = [](const glm::ivec2 pos) {
        return inverse_checker_generic_pattern(pos, 2);
    };

    inverse_checker_3_pattern->name = "inverse_checker_3_pattern";
    inverse_checker_3_pattern->nextSizePattern = checker_4_pattern;
    inverse_checker_3_pattern->get_positions = [](const glm::ivec2 pos) {
        return inverse_checker_generic_pattern(pos, 3);
    };

    inverse_checker_4_pattern->name = "inverse_checker_4_pattern";
    inverse_checker_4_pattern->get_positions = [](const glm::ivec2 pos) {
        return inverse_checker_generic_pattern(pos, 4);
    };

    knight_pattern->name = "knight_pattern";
    knight_pattern->get_positions = [](const glm::ivec2 pos) {
        return knight_global_pattern(pos);
    };
    arcueid_buff_pattern->name = "arcueid_buff_pattern";
    arcueid_buff_pattern->get_positions = [](const glm::ivec2 pos) {
        return merge_patterns(
            cross_generic_pattern(pos, Chessboard::getInstance()->getSize()),
            x_cross_generic_pattern(pos, 2)
        );
    };

    kintoki_rider_buff_pattern->name = "kintoki_rider_buff_pattern";
    kintoki_rider_buff_pattern->get_positions = [](const glm::ivec2 pos) {
        return merge_patterns(
            knight_global_pattern(pos),
            x_cross_generic_pattern(pos, Chessboard::getInstance()->getSize())
        );
    };

    nitocris_alter_spawn_pattern->name = "nitocris_alter_spawn_pattern";
    nitocris_alter_spawn_pattern->get_positions = [](const glm::ivec2 pos) {
        return merge_patterns(
            x_cross_generic_pattern(pos,1),
            cross_generic_pattern(pos,2)
        );
    };

    scatter_N_pattern->name = "scatter_N_pattern";
    scatter_N_pattern->get_positions = [](const glm::ivec2 pos) {
        return scatter_N_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };

    scatter_S_pattern->name = "scatter_S_pattern";
    scatter_S_pattern->get_positions = [](const glm::ivec2 pos) {
        return scatter_S_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };

    scatter_E_pattern->name = "scatter_E_pattern";
    scatter_E_pattern->get_positions = [](const glm::ivec2 pos) {
        return scatter_E_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };

    scatter_W_pattern->name = "scatter_W_pattern";
    scatter_W_pattern->get_positions = [](const glm::ivec2 pos) {
        return scatter_W_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };

    scatter_NE_pattern->name = "scatter_NE_pattern";
    scatter_NE_pattern->get_positions = [](const glm::ivec2 pos) {
        return scatter_NE_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };

    scatter_NW_pattern->name = "scatter_NW_pattern";
    scatter_NW_pattern->get_positions = [](const glm::ivec2 pos) {
        return scatter_NW_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };

    scatter_SE_pattern->name = "scatter_SE_pattern";
    scatter_SE_pattern->get_positions = [](const glm::ivec2 pos) {
        return scatter_SE_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };

    scatter_SW_pattern->name = "scatter_SE_pattern";
    scatter_SW_pattern->get_positions = [](const glm::ivec2 pos) {
        return scatter_SW_generic_pattern(pos, Chessboard::getInstance()->getSize());
    };
}







std::vector<glm::ivec2> merge_patterns(const std::vector<glm::ivec2> &first, const std::vector<glm::ivec2> &second) {
    return merge_patterns(2, first, second);
}

std::vector<glm::ivec2> merge_patterns(const int n_args, const std::vector<glm::ivec2>& patterns...) {
    va_list args;
    va_start(args, patterns);
    std::vector<glm::ivec2> merged_patterns;
    merged_patterns.insert(merged_patterns.end(), patterns.begin(), patterns.end());
    for (int i = 0; i < n_args - 1; i++) {
        auto pattern = va_arg(args, std::vector<glm::ivec2>);
        merged_patterns.insert(merged_patterns.end(), pattern.begin(), pattern.end());
    }
    return merged_patterns;
}







std::vector<glm::ivec2> line_N_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    std::vector<glm::ivec2> out_positions;
    for (int i = 1; i < 1 + pattern_size; ++i)
        if (pos.x - i >= 0)
            out_positions.emplace_back(pos.x - i, pos.y);
    return out_positions;
}

std::vector<glm::ivec2> line_S_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    std::vector<glm::ivec2> out_positions;
    for (int i = 1; i < 1 + pattern_size; ++i)
        if (pos.x + i < Chessboard::getInstance()->getSize())
            out_positions.emplace_back(pos.x + i, pos.y);
    return out_positions;
}

std::vector<glm::ivec2> line_E_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    std::vector<glm::ivec2> out_positions;
    for (int i = 1; i < 1 + pattern_size; ++i)
        if (pos.y + i < Chessboard::getInstance()->getSize())
            out_positions.emplace_back(pos.x, pos.y + i);
    return out_positions;
}

std::vector<glm::ivec2> line_W_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    std::vector<glm::ivec2> out_positions;
    for (int i = 1; i < 1 + pattern_size; ++i)
        if (pos.y - i >= 0)
            out_positions.emplace_back(pos.x, pos.y - i);
    return out_positions;
}



std::vector<glm::ivec2> line_NE_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    std::vector<glm::ivec2> out_positions;
    for (int i = 1; i < 1 + pattern_size; ++i)
        if (pos.x - i >= 0 && pos.y + i < Chessboard::getInstance()->getSize())
            out_positions.emplace_back(pos.x - i, pos.y + i);
    return out_positions;
}

std::vector<glm::ivec2> line_NW_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    std::vector<glm::ivec2> out_positions;
    for (int i = 1; i < 1 + pattern_size; ++i)
        if (pos.x - i >= 0 && pos.y - i >= 0)
            out_positions.emplace_back(pos.x - i, pos.y - i);
    return out_positions;
}

std::vector<glm::ivec2> line_SE_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    std::vector<glm::ivec2> out_positions;
    for (int i = 1; i < 1 + pattern_size; ++i)
        if (pos.x + i < Chessboard::getInstance()->getSize() && pos.y + i < Chessboard::getInstance()->getSize())
            out_positions.emplace_back(pos.x + i, pos.y + i);
    return out_positions;
}

std::vector<glm::ivec2> line_SW_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    std::vector<glm::ivec2> out_positions;
    for (int i = 1; i < 1 + pattern_size; ++i)
        if (pos.x + i < Chessboard::getInstance()->getSize() && pos.y - i >= 0)
            out_positions.emplace_back(pos.x + i, pos.y - i);
    return out_positions;
}





std::vector<glm::ivec2> square_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    std::vector<glm::ivec2> out_positions;
    for (int x = -pattern_size; x <= pattern_size; ++x)
        for (int y = -pattern_size; y <= pattern_size; ++y)
            if (const auto sel_pos = glm::ivec2(pos.x + x, pos.y + y);
                (pos.x != sel_pos.x || pos.y != sel_pos.y)
                && sel_pos.x < Chessboard::getInstance()->getSize()
                && sel_pos.y < Chessboard::getInstance()->getSize()
                && sel_pos.x >= 0
                && sel_pos.y >= 0
            ) out_positions.emplace_back(sel_pos);
    return out_positions;

}

std::vector<glm::ivec2> checker_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    std::vector<glm::ivec2> out_positions;
    for (int x = -pattern_size; x <= pattern_size; ++x)
        for (int y = -pattern_size; y <= pattern_size; ++y)
            if (const auto sel_pos = glm::ivec2(pos.x + x, pos.y + y);
                (pos.x != sel_pos.x || pos.y != sel_pos.y)
                && sel_pos.x < Chessboard::getInstance()->getSize()
                && sel_pos.y < Chessboard::getInstance()->getSize()
                && sel_pos.x >= 0
                && sel_pos.y >= 0
                && (x + y + 2 * Chessboard::getInstance()->getSize()) % 2 == 0
            ) out_positions.emplace_back(sel_pos);;
    return out_positions;
}

std::vector<glm::ivec2> inverse_checker_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    std::vector<glm::ivec2> out_positions;
    for (int x = -pattern_size; x <= pattern_size; ++x)
        for (int y = -pattern_size; y <= pattern_size; ++y)
            if (const auto sel_pos = glm::ivec2(pos.x + x, pos.y + y);
                (pos.x != sel_pos.x || pos.y != sel_pos.y)
                && sel_pos.x < Chessboard::getInstance()->getSize()
                && sel_pos.y < Chessboard::getInstance()->getSize()
                && sel_pos.x >= 0
                && sel_pos.y >= 0
                && (x + y + 2 * Chessboard::getInstance()->getSize()) % 2 == 1
            ) out_positions.emplace_back(sel_pos);;
    return out_positions;
}

std::vector<glm::ivec2> knight_global_pattern(const glm::ivec2 pos) {
    std::vector<glm::ivec2> out_positions;
    if (pos.x + 1 < Chessboard::getInstance()->getSize() && pos.y + 2 < Chessboard::getInstance()->getSize())
        out_positions.emplace_back(pos.x + 1, pos.y + 2);
    if (pos.x - 1 >= 0 && pos.y + 2 < Chessboard::getInstance()->getSize())
        out_positions.emplace_back(pos.x - 1, pos.y + 2);
    if (pos.x + 1 < Chessboard::getInstance()->getSize() && pos.y- 2 >= 0)
        out_positions.emplace_back(pos.x + 1, pos.y - 2);
    if (pos.x - 1 >= 0 && pos.y - 2 >= 0)
        out_positions.emplace_back(pos.x - 1, pos.y - 2);
    if (pos.x + 2 < Chessboard::getInstance()->getSize() && pos.y + 1 < Chessboard::getInstance()->getSize())
        out_positions.emplace_back(pos.x + 2, pos.y + 1);
    if (pos.x - 2 >= 0 && pos.y + 1 < Chessboard::getInstance()->getSize())
        out_positions.emplace_back(pos.x - 2, pos.y + 1);
    if (pos.x + 2 < Chessboard::getInstance()->getSize() && pos.y- 1 >= 0)
        out_positions.emplace_back(pos.x + 2, pos.y - 1);
    if (pos.x - 2 >= 0 && pos.y - 1 >= 0)
        out_positions.emplace_back(pos.x - 2, pos.y - 1);
    return out_positions;
}




std::vector<glm::ivec2> cross_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    return merge_patterns(
        4,
        line_N_generic_pattern(pos, pattern_size),
        line_S_generic_pattern(pos, pattern_size),
        line_E_generic_pattern(pos, pattern_size),
        line_W_generic_pattern(pos, pattern_size)
    );
}

std::vector<glm::ivec2> x_cross_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    return merge_patterns(
        4,
        line_NE_generic_pattern(pos, pattern_size),
        line_NW_generic_pattern(pos, pattern_size),
        line_SE_generic_pattern(pos, pattern_size),
        line_SW_generic_pattern(pos, pattern_size)
    );
}

std::vector<glm::ivec2> star_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    return merge_patterns(
        cross_generic_pattern(pos, pattern_size),
        x_cross_generic_pattern(pos, pattern_size)
    );
}

std::vector<glm::ivec2> scatter_N_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    return merge_patterns(
        3,
        line_NE_generic_pattern(pos, pattern_size),
        line_N_generic_pattern(pos, pattern_size),
        line_NW_generic_pattern(pos, pattern_size)
    );
}

std::vector<glm::ivec2> scatter_S_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    return merge_patterns(
        3,
        line_S_generic_pattern(pos, pattern_size),
        line_SE_generic_pattern(pos, pattern_size),
        line_SW_generic_pattern(pos, pattern_size)
    );
}

std::vector<glm::ivec2> scatter_E_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    return merge_patterns(
        3,
        line_E_generic_pattern(pos, pattern_size),
        line_NE_generic_pattern(pos, pattern_size),
        line_SE_generic_pattern(pos, pattern_size)
    );
}

std::vector<glm::ivec2> scatter_W_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    return merge_patterns(
        3,
        line_W_generic_pattern(pos, pattern_size),
        line_NW_generic_pattern(pos, pattern_size),
        line_SW_generic_pattern(pos, pattern_size)
    );
}



std::vector<glm::ivec2> scatter_NE_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    return merge_patterns(
        3,
        line_NE_generic_pattern(pos, pattern_size),
        line_N_generic_pattern(pos, pattern_size),
        line_E_generic_pattern(pos, pattern_size)
    );
}

std::vector<glm::ivec2> scatter_NW_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    return merge_patterns(
        3,
        line_NW_generic_pattern(pos, pattern_size),
        line_N_generic_pattern(pos, pattern_size),
        line_W_generic_pattern(pos, pattern_size)
    );
}

std::vector<glm::ivec2> scatter_SE_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    return merge_patterns(
        3,
        line_SE_generic_pattern(pos, pattern_size),
        line_S_generic_pattern(pos, pattern_size),
        line_E_generic_pattern(pos, pattern_size)
    );
}

std::vector<glm::ivec2> scatter_SW_generic_pattern(const glm::ivec2 pos, const int pattern_size) {
    return merge_patterns(
        3,
        line_SW_generic_pattern(pos, pattern_size),
        line_S_generic_pattern(pos, pattern_size),
        line_W_generic_pattern(pos, pattern_size)
    );
}
