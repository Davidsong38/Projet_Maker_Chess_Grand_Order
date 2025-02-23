//
// Created by sebas on 19/02/2025.
//

#ifndef CLICKABLES_H
#define CLICKABLES_H

#include <RadioButton.h>

extern std::vector<ClickableArea*> clickable_areas_lmb;
extern std::vector<ClickableArea*> clickable_areas_rmb;
extern std::vector<ClickableArea*> clickable_areas_mmb;

extern RadioButton* radio_button_test;
extern Button* screen_blocker;

void initClickables();

void addClickableAreaLMB(ClickableArea* clickable_area);
void addClickableAreaRMB(ClickableArea* clickable_area);
void addClickableAreaMMB(ClickableArea* clickable_area);

bool compareClickOrder(const ClickableArea* a, const ClickableArea* b);

#endif //CLICKABLES_H
