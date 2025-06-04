#pragma once

#include <utility>
#include <vector>

namespace States {
    enum class Input {
        Tap,
        Flag,
        Reset,
        Quit,
        Bad
    };

    enum class Game {
        Playon,
        Win,
        Lose,
    };

    enum class Flag {
        Add,
        Remove,
        Nothing
    };

    enum class Log {
        Tap,
        Flag,
        Unflag,
        Flagfail,
        Reset,
        Failed,
        Bad,
        Quit
    };

}

namespace Responses {

    struct Input {
        std::pair<char,char> Tile;
        States::Input Key;
    };

    struct Tap {
        std::vector<std::pair<std::pair<char,char>,int>> Tiles;
        States::Game State;
        int NumTilesLeft;
        int NumFlags;
    };

    struct Flag {
        States::Flag State;
        int NumFlags;
    };

}