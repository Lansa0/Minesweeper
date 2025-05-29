#include "output.hpp"

namespace {

    std::string formatTile(const std::pair<char,char>& tile) {
        return {'\'',tile.first,':',tile.second,'\''};
    }

    std::queue<std::string> Logs;
    const int LOG_MAX_SIZE = 22;
    const int LOG_OFFSET_Y = 8;
    const int LOG_OFFSET_X = 3;
    std::string setLogCursor(int y) {
        return "\033[" + std::to_string(y + LOG_OFFSET_Y) + ";" + std::to_string(LOG_OFFSET_X) + "H";
    }

    const char* FILLER = "                     ";

}

namespace Output {

    /**
     * Outputs the main UI
     */
    void Dashboard() {
        std::cout << "\033[2J\033[H"
        <<"┏━INFO━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
        <<"┃                      ┃┃     A B C D E F G H I J K L M N O P Q R S T U V W X Y Z   ┃\n"
        <<"┃ Total Bombs     @#$  ┃┃   ┌─────────────────────────────────────────────────────┐ ┃\n"
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
     * Adds user input to the log
     */
    void Log(char key, const std::pair<char,char>& tile) {

        std::string Message;

        switch (key)
        {
            case 'R': // Tile Revealed

            // case '???': // Remove Flag
            case 'F': // Flag placed
                Message = "Flagged Tile " + formatTile(tile);
                break;

            case 'Q': // Quit program
                exit(0);

            case 'N': // New Board/Reset
            break;

            default:
                Message = "Invalid Input";
                break;
        }

        // Insert the message onto the log
        Logs.push(Message);
        if (Logs.size() > LOG_MAX_SIZE) {
            Logs.pop();
            std::queue<std::string> Temp = Logs;
            while (!Temp.empty()) {
                const int X = (LOG_MAX_SIZE - Temp.size()) + 1;
                const std::string CursorPos = setLogCursor(X);
                std::cout << CursorPos << FILLER << CursorPos << Temp.front();
                Temp.pop();
            }
        } else {
            std::cout << setLogCursor(Logs.size()) << Message;
        }

    }


}