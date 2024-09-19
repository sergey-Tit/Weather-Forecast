#pragma once

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/mouse.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/loop.hpp"
#include "ftxui/screen/color.hpp"
#include "Cities.h"
#include <iostream>

class Weather {
public:
    Weather(Cities cities, uint32_t frequency);

    void RunProgram();

private:
    Cities cities_;
    uint32_t frequency_;
};
