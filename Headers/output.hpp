#pragma once

#include "responses.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

namespace Output {
    void Dashboard();
    void Log(States::Log key, const std::pair<char,char>& tile);
    void Reveal(const Responses::Tap& response);
    void Flag(const std::pair<char,char>& flagged_tile, const Responses::Flag& response);
    void Reset();
    void Quit(const Responses::SessionStats& stats);
}