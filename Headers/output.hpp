#pragma once
#include <iostream>
#include <vector>

namespace Output
{
    void Dashboard();
    void Log(char key, const std::pair<char,char>& tile);
}