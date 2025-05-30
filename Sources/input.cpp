#include "input.hpp"

namespace {
    const char* SET_CURSOR = "\033[33;12H";
    const char* FILLER = "                                                                         ";

    /**
     * Splits string input with given delimiter
     * 
     * @param s: Input string
     * @param delimiter: Split delimiter
     * 
     * @return Vector array of strings
     */
    std::vector<std::string> split(std::string& s,const char delimiter = ' ') {
        std::vector<std::string> Strings;
        size_t Position = 0;
        std::string Str;
        while ((Position = s.find(delimiter))!=std::string::npos) {
            Str = s.substr(0,Position);
            if(Str.size() >= 1) {
                std::transform(Str.begin(),Str.end(),Str.begin(),
                    [](unsigned char c){return std::tolower(c);}
                    );
                Strings.push_back(Str);
            }
            s.erase(0, Position + 1);
        }
        if(s.size() >= 1) {
            std::transform(s.begin(),s.end(),s.begin(),
                [](unsigned char c){return std::tolower(c);}
                );
            Strings.push_back(s);
        }
        return Strings;
    }

    const std::pair<char,char> InvalidTile = {'0','0'};
    /**
     * Checks for valid tile input and returns formatted tile data
     * 
     * @param tile: Input tile
     * 
     * @return Formatted tile data
     */
    std::pair<char,char> parseTile(std::string& tile) {
        const std::vector<std::string> Tokens = split(tile,':');
        if (Tokens.size() != 2) {
            return InvalidTile;
        }

        const std::string ComponentX = Tokens[0];
        const std::string ComponentY = Tokens[1];
        if ((ComponentX.size() == 1 && std::isalpha(ComponentX[0])) &&
            (ComponentY.size() == 1 && std::isalpha(ComponentY[0]))
        ) {
            return {std::toupper(ComponentX[0]),std::toupper(ComponentY[0])};
        }
        return InvalidTile;
    }

}

namespace Input {

    /**
     * Input Handler
     */
    Response Get() {

        std::string Input;
        do {
            std::cout << SET_CURSOR << FILLER << SET_CURSOR;
            std::getline(std::cin, Input);
        } while (Input.empty());

        std::vector<std::string> Tokens = split(Input);

        Response R;
        switch (Tokens.size()) {
            case 1: {
                std::string Key = Tokens[0];
                if (Key == "quit" || Key == "reset") {
                    R.Key = std::toupper(Key[0]);
                    return R;
                }
                return R;
            }

            case 2: {
                std::string Key = Tokens[0];
                std::pair<char,char> Tile = parseTile(Tokens[1]);
                if (Tile == InvalidTile) {
                    return R;
                } else if (Key == "tap" || Key == "flag" || Key == "unflag") {
                    R.Key = std::toupper(Key[0]);
                    R.Tile = Tile;
                    return R;
                }
                return R;
            }

            default: return R;
        }
    }
}