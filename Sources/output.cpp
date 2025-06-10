#include "output.hpp"

namespace {

    // Constants
    const int CHAR_OFFSET = 64;

    const int LOG_MAX_SIZE = 21;
    const int LOG_OFFSET_X = 3;
    const int LOG_OFFSET_Y = 9;

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

    const int INFO_OFFSET_X             = 19;
    const int TIMER_INFO_OFFSET_Y       = 3;
    const int FLAG_INFO_OFFSET_Y        = 5;
    const int TILES_LEFT_INFO_OFFSET_Y  = 6;

    const char* BLANK   = "\033[38;5;245m.\033[0m";
    const char* MINE    = "\033[38;5;196m▇\033[0m";
    const char* FLAG    = "\033[38;5;208m▶\033[0m";

    // Macros
    inline std::string formatTile(const std::pair<char,char>& tile) {
        return {'\'',tile.first,':',tile.second,'\''};
    }

    inline std::string setCursor(int y, int x) {
        return "\033[" + std::to_string(y) + ';' + std::to_string(x) + 'H';
    }

    std::string formatStatLine(const std::string& temp, const int stat_count) {
        std::string Formatted = "┃                               ";
        Formatted += temp + std::to_string(stat_count);

        const int Length = Formatted.length();
        Formatted.append(85 - Length + 1,' ');
        Formatted.append("┃\n");
        return Formatted;
    }

    struct {
        bool TimerOn = false;
        std::thread TimerThread;
    } Timer;

    void setTime() {
        int Count = 0;
        const auto s = std::chrono::seconds(1);
        while (Timer.TimerOn) {
            if (Count > 999) {break;}
            std::stringstream ss;
            ss << std::setw(3) << std::setfill('0') << Count;
            std::string Formatted = ss.str();
            std::cout << setCursor(TIMER_INFO_OFFSET_Y,INFO_OFFSET_X) << Formatted << std::flush;
            Count++;
            std::this_thread::sleep_for(s);
        }
    }

    /**
     * Inserts log message into UI
     *
     * @param message: Log message
     */
    void insertLog(const std::string& message) {
        static std::queue<std::string> Logs;

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
        {std::cout << MINE;}
        else if (n == 0)
        {std::cout << BLANK;}
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

    /**
     * Resets board section of the UI
     */
    void resetBoard() {
        const int TOTAL_TILES = 676;
        int Row = 1;
        int Column = 1;
        for (int i = 1; i <= TOTAL_TILES; i++) {
            std::cout << setCursor(TILE_OFFSET_Y + Row, TILE_OFFSET_X + (Column*2)) << '-';
            if (i%26==0) { // 26 = number of columns
                Row++;
                Column = 1;
            } else {
                Column++;
            }
        }
    }

    /**
     * Resets the Info section of the UI
     */
    void resetInfo() {
        std::cout
        << setCursor(TIMER_INFO_OFFSET_Y,INFO_OFFSET_X) << "$@#"
        << setCursor(FLAG_INFO_OFFSET_Y,INFO_OFFSET_X) << "&*^"
        << setCursor(TILES_LEFT_INFO_OFFSET_Y,INFO_OFFSET_X) << "!~%";
    }

}

namespace Output {

    /**
     * Outputs the main UI
     */
    void Dashboard() {
        std::cout << "\033[?25l\033[2J\033[H"
        <<"┏━INFO━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
        <<"┃                      ┃┃     A B C D E F G H I J K L M N O P Q R S T U V W X Y Z   ┃\n"
        <<"┃ Timer           $@#  ┃┃   ┌─────────────────────────────────────────────────────┐ ┃\n"
        <<"┃ Total Bombs     100  ┃┃ A │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃ Flags Used      &*^  ┃┃ B │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃ Tiles Left      !~%  ┃┃ C │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┃                      ┃┃ D │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┗━━━━━━━━━━━━━━━━━━━━━━┛┃ E │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
        <<"┏━LOG━━━━━━━━━━━━━━━━━━┓┃ F │ - - - - - - - - - - - - - - - - - - - - - - - - - - │ ┃\n"
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

    void startTimer() {
        Timer.TimerOn = true;
        Timer.TimerThread = std::thread(setTime);
    }

    void endTimer() {
        Timer.TimerOn = false;
        if (Timer.TimerThread.joinable()) {
            Timer.TimerThread.join();
        }
    }

    /**
     * Log Handler
     *
     * @param key: The type of log to output
     * @param tile: Input tile
     * @param failed_input: Status of the input
     *
     */
    void Log(States::Log key, const std::pair<char,char>& tile) {

        std::string Message;

        switch (key)  {
            case States::Log::Tap:
                Message = "Tapped Tile " + formatTile(tile);
                break;
            case States::Log::Flag:
                Message = "Flagged Tile " + formatTile(tile);
                break;
            case States::Log::Unflag:
                Message = "Unflagged Tile " + formatTile(tile);
                break;
            case States::Log::Flagfail:
                Message = "Can't Flag Tile " + formatTile(tile);
                break;
            case States::Log::Reset:
                Message = "Reset Board";
                break;
            case States::Log::Failed:
                Message = "Failed Input";
                break;
            case States::Log::Bad:
                Message = "Bad Input";
                break;
            case States::Log::Win:
                Message = "Game Won";
                break;
            case States::Log::Lose:
                Message = "Game Lost";
                break;
        }
            insertLog(Message);
    }

    /**
     * Handles revealing tiles on the UI
     *
     * @param response: Response data determining what to render
     */
    void Reveal(const Responses::Tap& response) {
        setFlagCount(response.NumFlags);
        setRemainingCount(response.NumTilesLeft);

        for (const auto& TileData : response.Tiles) {
            const std::pair<char,char> Tile = TileData.first;
            const int AdjacentMines = TileData.second;
            revealTile(Tile,AdjacentMines);
        }
    }

    /**
     * Renders flag onto the given tile slot in the UI
     *
     * @param flagged_tile: Tile where the flag will go on
     * @param response: Additional data to determine what to render
     */
    void Flag(const std::pair<char,char>& flagged_tile, const Responses::Flag& response) {
        setFlagCount(response.NumFlags);

        const char* c;
        if (response.State == States::Flag::Add)     {c = FLAG;}
   else if (response.State == States::Flag::Remove)  {c = "-";}
   else if (response.State == States::Flag::Nothing) {return;}

        const int Row = flagged_tile.first - CHAR_OFFSET;
        const int Column = (flagged_tile.second - CHAR_OFFSET)*2;

        std::cout << setCursor(TILE_OFFSET_Y + Row,TILE_OFFSET_X + Column) << c;
    }

    /**
     * Clears/Resets the board and info UI
     */
    void Reset() {
        resetBoard();
        resetInfo();
    }

    /**
     * Handles clearing the dashboard and outputting the session stats UI
     *
     * @param stats: Session stats
     */
    void Quit(const Responses::SessionStats& stats) {

        const std::string BoardsPlayed = formatStatLine("Boards Played    ", stats.BoardsPlayed);
        const std::string GamesWonStat = formatStatLine("Games Won        ", stats.GamesWon) ;
        const std::string GamesLostStat = formatStatLine("Games Lost       ", stats.GamesLost);
        const std::string TilesRevealedStat = formatStatLine("Tiles Revealed   ", stats.TilesRevealed);

        std::cout << setCursor(1,1) << "\033[?25h"
        << "┏━Session-Stats━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << BoardsPlayed
        << GamesWonStat
        << GamesLostStat
        << TilesRevealedStat
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┃                                                                                   ┃\n"
        << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
    }

}