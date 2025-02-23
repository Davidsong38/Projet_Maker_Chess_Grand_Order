//
// Created by sebas on 19/02/2025.
//

#include "clickables.h"

#include <Chessboard.h>
#include <GameEngine.h>
#include <game_cfg.h>
#include <log.h>
#include <RadioButton.h>
#include <RenderEngine.h>
#include <rendering_cfg.h>
#include <textures.h>

std::vector<ClickableArea*> clickable_areas_lmb;
std::vector<ClickableArea*> clickable_areas_rmb;
std::vector<ClickableArea*> clickable_areas_mmb;

RadioButton* radio_button_test;
Button* screen_blocker;

void initClickables() {
    auto* chessboard_button = new Button(
        boardTexture,
        glm::vec3(0, 0, LAYER_BOARD),
        glm::vec2(RenderEngine::getWindowInverseAspectRatio(), 1),
        glm::vec3(),
        [](const glm::vec2 pos) {
            auto posInBoard = glm::ivec2();
            posInBoard.x = floor(BOARD_SIZE * pos.x);
            posInBoard.y = floor(BOARD_SIZE * (1 -pos.y));
            GameEngine::getInstance()->clickBoardAtPos(posInBoard.y, posInBoard.x);
            return true;
        }
    );

    auto* button_test_toggle_visibility = new Button(
        ltrTexture,
        glm::vec3(-0.9, -0.9, LAYER_UI),
        glm::vec2(RenderEngine::getWindowInverseAspectRatio() * 0.1, 0.1),
        glm::vec3(0.2, 0.2, 0.2),
        [](const glm::vec2 /*pos*/) {
            static bool toggle = false;
            if (toggle)
                radio_button_test->show();
            else
                radio_button_test->hide();
            toggle = !toggle;
            return true;
        }
    );

    auto* button_test_print_value = new Button(
        artoriaTexture,
        glm::vec3(-0.75, -0.9, LAYER_UI),
        glm::vec2(RenderEngine::getWindowInverseAspectRatio() * 0.1, 0.1),
        glm::vec3(0.2, 0.2, 0.2),
        [](const glm::vec2 /*pos*/) {
            ltr_log_info("Printing value : ", radio_button_test->getIndex());
            return true;
        }
    );

    auto* button_gameEngine_rightClick = new Button(
        nullptr,
        glm::vec3(0, 0, LAYER_UI_BCK),
        glm::vec2(1, 1),
        glm::vec3(),
        [](const glm::vec2 /*pos*/) {
            GameEngine::getInstance()->inputRightClick();
            return false;
        }
    );

    screen_blocker = new Button(
        screenBlockerTexture,
        glm::vec3(0, 0, LAYER_UI_SCREEN_BLCK),
        glm::vec2(1, 1),
        glm::vec3(),
        [](const glm::vec2 /*pos*/) {
            return true;
        }
    );
    screen_blocker->hide();

    clickable_areas_lmb.push_back(chessboard_button);
    clickable_areas_lmb.push_back(button_test_toggle_visibility);
    clickable_areas_lmb.push_back(button_test_print_value);
    clickable_areas_lmb.push_back(screen_blocker);

    clickable_areas_rmb.push_back(button_gameEngine_rightClick);

    const std::vector buttonTextures{
        boardTexture,
        faridTexture,
        nemoMarxTexture,
        nemoMarxTexture,
        medusaTexture
    };
    radio_button_test = new RadioButton(
        buttonTextures,
        glm::vec3(-0.825, 0, LAYER_UI),
        glm::vec2(RenderEngine::getWindowInverseAspectRatio() * 0.1, 0.1),
        glm::vec2(0,-0.3),
        selectionTexture,
        glm::vec2(RenderEngine::getWindowInverseAspectRatio() * 0.12, 0.12),
        glm::vec3(0, 0, LAYER_UI_BCK),
        glm::vec3(0.2, 0.2, 0.2),
        true
    );

    std::sort(clickable_areas_lmb.begin(), clickable_areas_lmb.end(), compareClickOrder);
    std::sort(clickable_areas_rmb.begin(), clickable_areas_rmb.end(), compareClickOrder);
    std::sort(clickable_areas_mmb.begin(), clickable_areas_mmb.end(), compareClickOrder);
}

void addClickableAreaLMB(ClickableArea* clickable_area) {
    clickable_areas_lmb.push_back(clickable_area);
}

void addClickableAreaRMB(ClickableArea* clickable_area) {
    clickable_areas_rmb.push_back(clickable_area);
}

void addClickableAreaMMB(ClickableArea* clickable_area) {
    clickable_areas_mmb.push_back(clickable_area);
}

bool compareClickOrder(const ClickableArea* a, const ClickableArea* b) {
    return a->getDistanceToCamera() > b->getDistanceToCamera();
}
