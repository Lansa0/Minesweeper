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
        Win,
        Lose
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

    // idk where else to put this
    struct SessionStats {
        int BoardsPlayed = 0;
        int GamesWon = 0;
        int GamesLost = 0;
        int TilesRevealed = 0;
    };

}