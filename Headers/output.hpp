#pragma once

#include "responses.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

namespace Output {
    void Dashboard();
    void Log(char key, const std::pair<char,char>& tile, bool failed_input);
    void Reveal(const Responses::Tap& response);
    void Flag(const std::pair<char,char>& flagged_tile, const Responses::Flag& response);
}