#include <algorithm>
#include <iostream>
#include <utility>
#include <random>
#include <vector>
#include <stack>
#include <map>
#include <set>

// clang++ -std=c++11 -o binary/test_board tests/test_board.cpp && ./binary/test_board

struct {
    std::map<std::pair<char,char>,int> Tiles;
    std::set<std::pair<char,char>> Mines;
    std::set<std::pair<char,char>> Revealed;

    void generateMines(
        std::vector<std::pair<char,char>>& all_tiles,
        const std::set<std::pair<char,char>>& safe_tiles
    ) {
        int Converted = 0;

        std::mt19937 g(175); // Set Seed
        std::shuffle(all_tiles.begin(),all_tiles.end(),g);

        for (const auto Tile : all_tiles) {
            if (safe_tiles.find(Tile) == safe_tiles.end()) {
                Mines.insert(Tile);
                Converted++;
            }
            if (Converted == 100) {break;}
        }
    }

    void setTiles(const std::vector<std::pair<char,char>>& all_tiles) {
        for (const auto& Tile : all_tiles) {

            if (Mines.find(Tile) != Mines.end()) {
                Tiles[Tile] = 9;
                continue;
            }

            int MinesFound = 0;
            int Row = (Tile.first - 64) - 1;
            int Column = (Tile.second - 64) - 1;
            for (int i = 1; i <= 9; i++) {
                if ((Row > 0 && Row <= 26) && (Column > 0 && Column <= 26) && (i != 5)) {

                    const std::pair<char,char> AdjacentTile = {64 + Row,64 + Column};
                    if (Mines.find(AdjacentTile) != Mines.end()) {
                        MinesFound++;
                    }
                }
                if (i%3==0) {
                    Column -= 2;
                    Row++;
                } else {
                    Column++;
                }
            }

            Tiles[Tile] = MinesFound;
        }
    }

} Board;

std::vector<std::pair<char,char>> getAllTiles() {
    std::vector<std::pair<char,char>> AllTiles;
    AllTiles.reserve(676);

    int Row = 1;
    int Column = 1;
    for (int i = 1; i <= 676; i++) {
        const std::pair<char,char> Tile = {64 + Row,64 + Column};
        AllTiles.push_back(Tile);

        if (i%26==0) {
            Row++;
            Column = 1;
        } else {
            Column++;
        }
    }
    return AllTiles;
}

std::set<std::pair<char,char>> getSafeTiles(const std::pair<char,char> safe_tile) {
    std::set<std::pair<char,char>> SafeTiles = {safe_tile};
    int Row = (safe_tile.first - 64) - 1;
    int Column = (safe_tile.second - 64) - 1;
    for (int i = 1; i <= 9; i++) {
        if ((Row > 0 && Row <= 26) && (Column > 0 && Column <= 26) && (i != 5)) {
            const std::pair<char,char> SafeTile = {64 + Row,64 + Column};
            SafeTiles.insert(SafeTile);
        }
        if (i%3==0) {
            Row++;
            Column -= 2;
        } else {
            Column++;
        }
    }
    return SafeTiles;
}

struct TapResponse {
    std::vector<std::pair<std::pair<char,char>,int>> Tiles;
    char GameState;
};

TapResponse Tap(const std::pair<char,char>& tapped_tile) {

    TapResponse Response;

    // Tile is a mine
    if (Board.Tiles[tapped_tile] == 9) {
        for (const auto& Tile : Board.Mines) {
            Response.Tiles.push_back({Tile,9});
        }
        Response.GameState = 'X';
        return Response;
    }

    // Tile has at least 1 adjacent mine
    else if (Board.Tiles[tapped_tile] != 0) {
        Board.Revealed.insert(tapped_tile);
        const int AdjacentMines = Board.Tiles[tapped_tile];
        Response.Tiles.push_back({tapped_tile,AdjacentMines});
        if (Board.Revealed.size() == 676 - 100) {
            Response.GameState = 'F';
        } else {
            Response.GameState = 'O';
        }

        return Response;
    }

    // Tile has 0 adjacent mines
    else {
        std::stack<std::pair<char,char>,std::vector<std::pair<char,char>>> Stack;
        Stack.push(tapped_tile);


        do {
            std::pair<char, char> CurrentTile = Stack.top();
            Stack.pop();

            int Row = (CurrentTile.first - 64) - 1;
            int Column = (CurrentTile.second - 64) - 1;

            for (int i=1; i<=9; i++) {
                if ((Row > 0 && Row <= 26) && (Column > 0 && Column <= 26) && (i != 5)) {
                    std::pair<char,char> AdjacentTile = {64 + Row,64 + Column};

                    if (Board.Revealed.find(AdjacentTile) == Board.Revealed.end()) {
                        Board.Revealed.insert(AdjacentTile);

                        const int AdjacentMines = Board.Tiles[AdjacentTile];
                        Response.Tiles.push_back({AdjacentTile,AdjacentMines});
                        if (AdjacentMines == 0) {
                            Stack.push(AdjacentTile);
                        }
                    }
                }
                if (i%3==0) {
                    Row++;
                    Column -= 2;
                } else {
                    Column++;
                }
            }
        } while (!Stack.empty());

        if (Board.Revealed.size() == 676 - 100) {
            Response.GameState = 'F';
        } else {
            Response.GameState = 'O';
        }

        return Response;
    }

}

void Test() {
    std::map<int,const char*> Colours {
        {0, "\033[34m"},
        {1, "\033[37m"},
        {2, "\033[37m"},
        {3, "\033[37m"},
        {4, "\033[37m"},
        {5, "\033[37m"},
        {6, "\033[37m"},
        {7, "\033[37m"},
        {8, "\033[37m"},
        {9, "\033[31m"}
    };

    int Count = 0;
    std::vector<std::pair<char,char>> AllTiles = getAllTiles();
    for (const auto& Tile : AllTiles) {
        std::cout << Colours[Board.Tiles[Tile]] << Tile.first << ':' << Tile.second << "\033[0m ";

        Count++;
        if (Count%26==0) {
            std::cout << '\n';
        }
    }

    std::pair<char,char> TappedTile = {'D','J'};
    TapResponse TR = Tap(TappedTile);

    std::cout << TR.Tiles.size() << '\n';
    for (const auto& TileData : TR.Tiles) {
        const auto Tile = TileData.first;
        std::cout << Tile.first << ':' << Tile.second << " : " << TileData.second << '\n';
    }

}

int main() {
    std::pair<char,char> SafeTile = {'G','T'};

    std::vector<std::pair<char,char>> AllTiles = getAllTiles();
    std::set<std::pair<char,char>> SafeTiles = getSafeTiles(SafeTile);
    Board.generateMines(AllTiles,SafeTiles);
    Board.setTiles(AllTiles);

    Test();
}