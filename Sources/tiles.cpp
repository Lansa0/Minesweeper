#include "tiles.hpp"

namespace {

    // Constants
    const int CHAR_OFFSET = 64;
    const int ROWS = 26;
    const int COLUMNS = 26;
    const int TOTAL_TILES = 676;
    const int NUM_MINES = 100;

    // Macros
    inline bool IN_BOUNDS(int Row, int Column, int i) {
        return (Row > 0 && Row <= ROWS) && (Column > 0 && Column <= COLUMNS) && (i != 5);
    }

    struct
    {
        std::map<std::pair<char,char>,int> Tiles;
        std::set<std::pair<char,char>> Mines;
        std::set<std::pair<char,char>> Revealed;

        /**
        * Cleans board data
        */
        void clean() {
            Tiles.clear();
            Mines.clear();
            Revealed.clear();
        }

        /**
         * Randomly assigns tiles to be mines
         *
         * @param all_tiles: All the tiles on the board
         * @param safe_tiles: Tiles that cannot be a mines
         *
         */
        void generateMines(
            std::vector<std::pair<char,char>>& all_tiles,
            const std::set<std::pair<char,char>>& safe_tiles
        ) {
            int Converted = 0;

            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(all_tiles.begin(),all_tiles.end(),g);

            for (const auto Tile : all_tiles) {
                if (safe_tiles.find(Tile) == safe_tiles.end()) {
                    Mines.insert(Tile);
                    Converted++;
                }
                if (Converted == NUM_MINES) {break;}
            }
        }

        /**
         * Assigns every tile a number based on how many mines are adjacent to it
         *
         * 0 indicates no adjacent mines
         *
         * 9 indicates tile itself is a mine
         *
         * @param all_tiles: All the tiles on the board
         */
        void setTiles(const std::vector<std::pair<char,char>>& all_tiles) {
            for (const auto& Tile : all_tiles) {

                // If tile itself is a mine
                if (Mines.find(Tile) != Mines.end()) {
                    Tiles[Tile] = 9;
                    continue;
                }

                int MinesFound = 0;
                int Row = (Tile.first - CHAR_OFFSET) - 1;
                int Column = (Tile.second - CHAR_OFFSET) - 1;
                for (int i = 1; i <= 9; i++) {
                    if (IN_BOUNDS(Row,Column,i)) {

                        const std::pair<char,char> AdjacentTile = {CHAR_OFFSET + Row,CHAR_OFFSET + Column};
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

    /**
     * Creates vector containing every tile
     *
     * could be done hard coded tbh since output is always the same
     *
     * @return All the tiles on the board
     */
    std::vector<std::pair<char,char>> getAllTiles() {
        std::vector<std::pair<char,char>> AllTiles;
        AllTiles.reserve(TOTAL_TILES);

        int Row = 1;
        int Column = 1;
        for (int i = 1; i <= TOTAL_TILES; i++) {
            const std::pair<char,char> Tile = {CHAR_OFFSET + Row,CHAR_OFFSET + Column};
            AllTiles.push_back(Tile);

            if (i%COLUMNS==0) {
                Row++;
                Column = 1;
            } else {
                Column++;
            }
        }
        return AllTiles;
    }

    /**
     * Gets all tiles that cannot be a mine
     *
     * @param safe_tile: Given safe tile
     *
     * @return All tiles adjacent to and including the safe tile
     */
    std::set<std::pair<char,char>> getSafeTiles(const std::pair<char,char> safe_tile) {
        std::set<std::pair<char,char>> SafeTiles = {safe_tile};
        int Row = (safe_tile.first - CHAR_OFFSET) - 1;
        int Column = (safe_tile.second - CHAR_OFFSET) - 1;
        for (int i = 1; i <= 9; i++) {
            if (IN_BOUNDS(Row,Column,i)) {
                const std::pair<char,char> SafeTile = {CHAR_OFFSET + Row,CHAR_OFFSET + Column};
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

}

namespace Tiles {

    /**
     * Generates a new Minesweeper board
     *
     * @param safe_tile: Starting tile, all tiles adjacent and including will not be a mine
     */
    void generateBoard(const std::pair<char,char>& safe_tile) {
        Board.clean();

        // auto cause i'm lazy
        auto AllTiles = getAllTiles();
        auto SafeTiles = getSafeTiles(safe_tile);

        Board.generateMines(AllTiles,SafeTiles);
        Board.setTiles(AllTiles);
    }

    /**
     * Handles which tiles should be revealed based on tile input
     *
     * @param tapped_tile: Input tile
     *
     * @return WIP
     */
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
            if (Board.Revealed.size() == TOTAL_TILES - NUM_MINES) {
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

                int Row = (CurrentTile.first - CHAR_OFFSET) - 1;
                int Column = (CurrentTile.second - CHAR_OFFSET) - 1;

                for (int i=1; i<=9; i++) {
                    if (IN_BOUNDS(Row,Column,i)) {
                        std::pair<char,char> AdjacentTile = {CHAR_OFFSET + Row,CHAR_OFFSET + Column};

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

            if (Board.Revealed.size() == TOTAL_TILES - NUM_MINES) {
                Response.GameState = 'F';
            } else {
                Response.GameState = 'O';
            }

            return Response;
        }

    }

}