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

    struct {
        std::map<std::pair<char,char>,int> Tiles;
        std::set<std::pair<char,char>> Mines;
        std::set<std::pair<char,char>> Revealed;
        std::set<std::pair<char,char>> Flagged;

        /**
        * Cleans board data
        */
        void clean() {
            Tiles.clear();
            Mines.clear();
            Revealed.clear();
            Flagged.clear();
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
                int Row = (Tile.first - CHAR_OFFSET) - 1;       // -1 so that it starts at the top left
                int Column = (Tile.second - CHAR_OFFSET) - 1;   // adjacent tile
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

        /**
         * Searches the entire board, starting at the input tile, for all tiles. If a tile found in the search is
         * blank (0 adjacent mines), the tile will be added to the return vector as well as a search stack where
         * further searches will occur. If the tile found is adjacent to at least 1 mine, it will be added to the
         * return vector but not the search stack.
         *
         * Since searches only apply to blank tiles, no mines will be encountered
         *
         * @param starting_tile: Input blank tile where the search starts
         *
         * @return Vector containing individual tile data which includes the tile itself and the number of 
         * adjacent mines
         *
         */
        std::vector<std::pair<std::pair<char,char>,int>> findAllAdjacent(
            const std::pair<char,char>& starting_tile
        ) {
            std::vector<std::pair<std::pair<char,char>,int>> ret;

            // Fixes situations where the input tile isn't adjacent to any
            // other blank tile
            ret.push_back({starting_tile,Tiles[starting_tile]});

            std::stack<std::pair<char,char>,std::vector<std::pair<char,char>>> Stack;
            Stack.push(starting_tile);

            do {
                auto CurrentTile = Stack.top();
                Stack.pop();

                int Row = (CurrentTile.first - CHAR_OFFSET) - 1;
                int Column = (CurrentTile.second - CHAR_OFFSET) - 1;

                for (int i=1; i<=9; i++) {
                    if (IN_BOUNDS(Row,Column,i)) {
                        std::pair<char,char> AdjacentTile = {CHAR_OFFSET + Row,CHAR_OFFSET + Column};

                        if (Revealed.find(AdjacentTile) == Revealed.end()) {
                            Revealed.insert(AdjacentTile);

                            const int AdjacentMines = Tiles[AdjacentTile];
                            ret.push_back({AdjacentTile,AdjacentMines});
                            if (AdjacentMines == 0) {
                                Stack.push(AdjacentTile);
                            }

                            // For situations where there is a false flag
                            Flagged.erase(AdjacentTile);
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

            return ret;
        }

        // idk why i have some getter functions even though i could just directly get these values
        // would make more sense to make this a class rather than a struct
        // too lazy to change now

        /**
         * Returns the number of adjacent mines to a given tile
         *
         * @param tile: Input tile
         *
         */
        inline int getNumAdjacentMines(const std::pair<char,char>& tile) {
            return Tiles[tile];
        }

        /**
         * Returns if given tile is currently flagged
         *
         * @param tile: Input tile
         *
         */
        inline bool tileFlagged(const std::pair<char,char>& tile) {
            return Flagged.find(tile) != Flagged.end();
        }

        /**
         * Returns if given tile is currently revealed
         *
         * @param tile: Input tile
         *
         */
        inline bool tileRevealed(const std::pair<char,char>& tile) {
            return Revealed.find(tile) != Revealed.end();
        }

        /**
         * Returns the number of unrevealed tiles
         */
        inline int getRemaining() {
            return TOTAL_TILES - Revealed.size();
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
    Responses::Tap Tap(const std::pair<char,char>& tapped_tile) {

        Responses::Tap Response;

        // Do nothing pretty much
        if (Board.tileFlagged(tapped_tile)) {
            Response.State = States::Game::Playon;
            Response.NumTilesLeft = Board.getRemaining();
            Response.NumFlags = Board.Flagged.size();
            return Response;
        }

        const int AdjacentMines = Board.getNumAdjacentMines(tapped_tile);

        switch (AdjacentMines) {
            case 9: { // Tile is a mine
                for (const auto& Mine : Board.Mines) {
                    Response.Tiles.push_back({Mine,9});
                }
                Response.State = States::Game::Lose;
                Response.NumTilesLeft = Board.getRemaining();
                Response.NumFlags = Board.Flagged.size();
                return Response;
            }

            case 0: { // Tile is blank (0 adjacent mines)
                Response.Tiles = Board.findAllAdjacent(tapped_tile);
                Response.NumTilesLeft = Board.getRemaining();
                Response.NumFlags = Board.Flagged.size();
                break;
            }

            default: { // Tile has at least 1 adjacent mine
                Board.Revealed.insert(tapped_tile);
                Response.Tiles.push_back({tapped_tile,AdjacentMines});
                break;
            }
        }

        Response.State = Board.getRemaining() == NUM_MINES ? States::Game::Win : States::Game::Playon;
        Response.NumTilesLeft = Board.getRemaining();
        Response.NumFlags = Board.Flagged.size();
        return Response;
    }

    /**
     * Handles flagging/unflagging tiles
     *
     * Flagging a tile that doesn't have a flag will add it to the set of flagged tiles
     *
     * Flagging a tile that does have a flag will remove it from the set of flagged tiles
     *
     * Flagging a tile that is already revealed will do nothing
     *
     * @param flagged_tile: Input Tile
     *
     * @return
     * true: If the input tile is not already flagged
     *
     * false: If the input tile is already flagged or revealed
     *
     */
    Responses::Flag Flag(const std::pair<char,char>& flagged_tile) {

        Responses::Flag Response;

        if (Board.tileRevealed(flagged_tile)) {     // Tile is revealed
            Response.State = States::Flag::Nothing;
        }

        else if (Board.tileFlagged(flagged_tile)) { // Tile is currently flagged
            Board.Flagged.erase(flagged_tile);
            Response.State = States::Flag::Remove;
        }

        else {                                      // Tile is not currently flagged
            Board.Flagged.insert(flagged_tile);
            Response.State = States::Flag::Add;
        }

        Response.NumFlags = Board.Flagged.size();
        return Response;
    };

}