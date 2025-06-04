#pragma once

#include "responses.hpp"

#include <algorithm>
#include <utility>
#include <random>
#include <vector>
#include <stack>
#include <map>
#include <set>

namespace Tiles
{

    void generateBoard(const std::pair<char,char>& safe_tile);
    Responses::Tap Tap(const std::pair<char,char>& tapped_tile);
    Responses::Flag Flag(const std::pair<char,char>& flagged_tile);
}