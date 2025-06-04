#include "output.hpp"

namespace {

    // Constants
    const int CHAR_OFFSET = 64;

    const int LOG_MAX_SIZE = 22;
    const int LOG_OFFSET_X = 3;
    const int LOG_OFFSET_Y = 8;

    const int TILE_OFFSET_X = 29;
    const int TILE_OFFSET_Y = 3;
    std::map<int,std::string> TILE_COLOURS = {
        {1, "\033[38;5;26m"},   // 26   : Blue
        {2, "\033[38;5;22m"},   // 22   : Green
        {3, "\033[38;5;196m"},  // 196  : Red
        {4, "\033[38;5;226m"},  // 226  : Yellow
        {5, "\033[38;5;126m"},  // 126  : Pinkish
        {6, "\033[38;5;6m"},    // 6    : Cyan
        {7, "\033[38;5;99m"},   // 99   : Purple
        {8, "\033[38;5;245m"},  // 245  : Grey
    };

    const int INFO_OFFSET_X = 19;
    const int FLAG_INFO_OFFSET_Y = 4;
    const int TILES_LEFT_INFO_OFFSET_Y = 5;

    // Macros
    inline std::string formatTile(const std::pair<char,char>& tile) {
        return {'\'',tile.first,':',tile.second,'\''};
    }

    inline std::string setCursor(int y, int x) {
        return "\033[" + std::to_string(y) + ';' + std::to_string(x) + 'H';
    }

    std::queue<std::string> Logs;
    /**
     * Inserts log message into UI
     *
     * @param message: Log message
     */
    void insertLog(const std::string& message) {
        const char* FILLER = "                     ";
        Logs.push(message);
        if (Logs.size() > LOG_MAX_SIZE) {
            Logs.pop();
            std::queue<std::string> Temp = Logs;
            while (!Temp.empty()) {
                const int Y = (LOG_MAX_SIZE - Temp.size()) + 1;
                const std::string CursorPos = setCursor(Y + LOG_OFFSET_Y,LOG_OFFSET_X);
                std::cout << CursorPos << FILLER << CursorPos << Temp.front();
                Temp.pop();
            }
        } else {
            std::cout << setCursor(Logs.size() + LOG_OFFSET_Y,LOG_OFFSET_X) << message;
        }
    }

    /**
     * Reveals the given tile on the UI
     *
     * @param tile: The tile to reveal
     * @param n: The number of mines adjacent to the tile
     */
    void revealTile(const std::pair<char,char>& tile, const int n) {
        const int Row = tile.first - CHAR_OFFSET;
        const int Column = (tile.second - CHAR_OFFSET)*2;

        std::cout << setCursor(TILE_OFFSET_Y + Row,TILE_OFFSET_X + Column);

        if (n == 9)
        {std::cout << 'M';} // TEMPORARY
        else if (n == 0)
        {std::cout << ' ';}
        else
        {std::cout << TILE_COLOURS[n] << n << "\033[0m";}

    }

    /**
     * Sets the number of flags used onto the UI
     *
     * @param num_flags: Number of flags used
     */
    void setFlagCount(const int num_flags) {
        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0') << num_flags;
        std::string Formatted = ss.str();
        std::cout << setCursor(FLAG_INFO_OFFSET_Y,INFO_OFFSET_X) << Formatted;
    }

    /**
     * Sets the number of tiles left onto the UI
     *
     * @param num_tiles_left: Number of tiles left
     */
    void setRemainingCount(const int num_tiles_left) {
        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0') << num_tiles_left;
        std::string Formatted = ss.str();
        std::cout << setCursor(TILES_LEFT_INFO_OFFSET_Y,INFO_OFFSET_X) << Formatted;
    }

}

namespace Output {

    /**
     * Outputs the main UI
     */
    void Dashboard() {
        std::cout << "\033[2J\033[H"
        <<"┏━INFO━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
        <<"┃                      ┃┃     A B C D E F G H I J K L M N O P Q R S T U V W X Y Z   ┃\n"
        <<"┃ Total Bombs     100  ┃┃   ┌─────────────────────────────────────────────────────┐ ┃\n"
        <<"┃ Flags Used      &*^  ┃┃ A │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃ Tiles Left      !~%  ┃┃ B │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ C │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┗━━━━━━━━━━━━━━━━━━━━━━┛┃ D │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┏━LOG━━━━━━━━━━━━━━━━━━┓┃ E │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ F │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ G │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ H │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ I │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ J │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ K │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ L │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ M │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ N │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ O │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ P │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ Q │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ R │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ S │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ T │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ U │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ V │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ W │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ X │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ Y │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ Z │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃   └─────────────────────────────────────────────────────┘ ┃\n"
        <<"┗━━━━━━━━━━━━━━━━━━━━━━┛┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"
        <<"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
        <<"┃ INPUT ::                                                                          ┃\n"
        <<"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
    }

    /**
     * Log Handler
     *
     * @param key: The type of log to output
     * @param tile: Input tile
     * @param failed_input: Status of the input
     *
     */
    void Log(char key, const std::pair<char,char>& tile, bool failed_input) {
        // TODO: Better logger

        if (failed_input) {
            insertLog("Failed Input");
            return;
        }

        std::string Message;

        switch (key) {
            case 'T': // Tap Tile
                Message = "Tapped Tile " + formatTile(tile);
                break;
            case 'F': // Place Flag
                Message = "Flagged Tile " + formatTile(tile);
                break;
            case 'U': // Unplace Flag
                Message = "Unflagged Tile " + formatTile(tile);
                break;
            case 'R': //Reset Board
                Message = "Reset Board";
                break;
            case 'Q': // Quit program
                exit(0);
            default:
                Message = "Bad Input";
                break;
        }

        insertLog(Message);
    }

    void Reveal(const Responses::Tap& response) {
        setFlagCount(response.NumFlags);
        setRemainingCount(response.NumTilesLeft);

        for (const auto& TileData : response.Tiles) {
            const std::pair<char,char> Tile = TileData.first;
            const int AdjacentMines = TileData.second;
            revealTile(Tile,AdjacentMines);
        }
    }

    void Flag(const std::pair<char,char>& flagged_tile, const Responses::Flag& response) {
        setFlagCount(response.NumFlags);

        char c = ' ';
        if (response.State == States::Flag::Add) {c = 'F';}
   else if (response.State == States::Flag::Remove) {c = '-';}

        const int Row = flagged_tile.first - CHAR_OFFSET;
        const int Column = (flagged_tile.second - CHAR_OFFSET)*2;

        std::cout << setCursor(TILE_OFFSET_Y + Row,TILE_OFFSET_X + Column) << c;
    }

}