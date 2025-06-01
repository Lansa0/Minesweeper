#pragma once

#include <algorithm>
#include <utility>
#include <random>
#include <vector>
#include <stack>
#include <map>
#include <set>

namespace Tiles
{
    struct TapResponse {
        std::vector<std::pair<std::pair<char,char>,int>> Tiles;
        char GameState;
    };

    void generateBoard(const std::pair<char,char>& safe_tile);
    TapResponse Tap(const std::pair<char,char>& tapped_tile);
}