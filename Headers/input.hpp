#pragma once
#include <utility>  // For pair
#include <iostream> // For setting the cursor

namespace Input
{
    struct Response { char Key; std::pair<char,char> Tile; };
    Response Get();
}