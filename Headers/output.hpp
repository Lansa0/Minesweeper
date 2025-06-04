#pragma once

#include <iostream>
#include <map>

namespace Output {
    void Dashboard();
    void Log(char key, const std::pair<char,char>& tile, bool failed_input);
    void Reveal(const std::vector<std::pair<std::pair<char,char>,int>>& tiles);
    void Flag(const std::pair<char,char>& flagged_tile, const bool add);
}