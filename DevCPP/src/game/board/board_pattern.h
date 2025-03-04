//
// Created by boolean_sniper on 26/02/2025.
//

#ifndef BOARD_PATTERN_H
#define BOARD_PATTERN_H

#include <functional>
#include <string>
#include <vector>
#include "glm.hpp"

struct board_pattern {
    std::function<std::vector<glm::ivec2>(const glm::ivec2)> get_positions;
    std::string name{"Unnamed board_pattern"};
    board_pattern* nextSizePattern = nullptr;
};

extern board_pattern* cross_pattern;
extern board_pattern* cross_1_pattern;
extern board_pattern* cross_2_pattern;
extern board_pattern* cross_3_pattern;

extern board_pattern* x_cross_pattern;
extern board_pattern* x_cross_1_pattern;
extern board_pattern* x_cross_2_pattern;
extern board_pattern* x_cross_3_pattern;

extern board_pattern* star_pattern;
extern board_pattern* star_1_pattern;
extern board_pattern* star_2_pattern;
extern board_pattern* star_3_pattern;

extern board_pattern* square_pattern;
extern board_pattern* square_1_pattern;
extern board_pattern* square_2_pattern;
extern board_pattern* square_3_pattern;

extern board_pattern* checker_pattern;
extern board_pattern* checker_1_pattern;
extern board_pattern* checker_2_pattern;
extern board_pattern* checker_3_pattern;
extern board_pattern* checker_4_pattern;


extern board_pattern* inverse_checker_2_pattern;
extern board_pattern* inverse_checker_3_pattern;
extern board_pattern* inverse_checker_4_pattern;



extern board_pattern* knight_pattern;

extern board_pattern* arcueid_buff_pattern;

extern board_pattern* kintoki_rider_buff_pattern;

extern board_pattern* nitocris_alter_spawn_pattern;


extern board_pattern* scatter_N_pattern;
extern board_pattern* scatter_S_pattern;
extern board_pattern* scatter_E_pattern;
extern board_pattern* scatter_W_pattern;

extern board_pattern* scatter_NE_pattern;
extern board_pattern* scatter_NW_pattern;
extern board_pattern* scatter_SE_pattern;
extern board_pattern* scatter_SW_pattern;

void init_board_patterns();



std::vector<glm::ivec2> merge_patterns(const std::vector<glm::ivec2> &first, const std::vector<glm::ivec2> &second);
std::vector<glm::ivec2> merge_patterns(int n_args, const std::vector<glm::ivec2>& patterns...);



std::vector<glm::ivec2> line_N_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> line_S_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> line_E_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> line_W_generic_pattern(glm::ivec2 pos, int pattern_size);

std::vector<glm::ivec2> line_NE_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> line_NW_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> line_SE_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> line_SW_generic_pattern(glm::ivec2 pos, int pattern_size);

std::vector<glm::ivec2> square_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> checker_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> inverse_checker_generic_pattern(glm::ivec2 pos, int pattern_size);

std::vector<glm::ivec2> knight_global_pattern(glm::ivec2 pos);


std::vector<glm::ivec2> cross_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> x_cross_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> star_generic_pattern(glm::ivec2 pos, int pattern_size);

std::vector<glm::ivec2> scatter_N_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> scatter_S_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> scatter_E_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> scatter_W_generic_pattern(glm::ivec2 pos, int pattern_size);

std::vector<glm::ivec2> scatter_NE_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> scatter_NW_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> scatter_SE_generic_pattern(glm::ivec2 pos, int pattern_size);
std::vector<glm::ivec2> scatter_SW_generic_pattern(glm::ivec2 pos, int pattern_size);

#endif //BOARD_PATTERN_H
