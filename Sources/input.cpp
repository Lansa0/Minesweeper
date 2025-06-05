#include "input.hpp"

namespace {

    // Constants
    const char* SET_CURSOR = "\033[33;12H";
    const char* FILLER = "                                                                         ";

    std::string parseInput() {
        std::string Input;
        do {
            std::cout << SET_CURSOR << FILLER << SET_CURSOR;
            std::getline(std::cin, Input);
        } while (Input.empty());
        return Input;
    }

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
     * @param tile: Input tile string
     *
     * @return Formatted tile
     */
    std::pair<char,char> parseTile(std::string& tile) {
        const std::vector<std::string> Tokens = split(tile,':');
        if (Tokens.size() != 2) {
            return InvalidTile;
        }

        const std::string RowComponent = Tokens[0];
        const std::string ColumnComponent = Tokens[1];
        if ((RowComponent.size() == 1 && std::isalpha(RowComponent[0])) &&
            (ColumnComponent.size() == 1 && std::isalpha(ColumnComponent[0]))
        ) {
            const std::pair<char,char> Tile = {std::toupper(RowComponent[0]),std::toupper(ColumnComponent[0])};
            return Tile;
        }
        return InvalidTile;
    }

}

namespace Input {

    /**
     * Input Handler
     */
    Responses::Input Get() {

        std::string Input = parseInput();
        std::vector<std::string> Tokens = split(Input);

        Responses::Input Response;
        switch (Tokens.size()) {
            case 1: {
                std::string Key = Tokens[0];

                if (Key == "quit") {
                    Response.Key = States::Input::Quit;
                } else if (Key == "reset") {
                    Response.Key = States::Input::Reset;
                } else {
                    Response.Key = States::Input::Bad;
                }

                return Response;
            }

            case 2: {
                std::string Key = Tokens[0];
                std::pair<char,char> Tile = parseTile(Tokens[1]);

                if (Tile == InvalidTile) {
                    Response.Key = States::Input::Bad;
                    return Response;
                }

                if (Key == "tap") {
                    Response.Key = States::Input::Tap;
                    Response.Tile = Tile;
                } else if (Key == "flag") {
                    Response.Key = States::Input::Flag;
                    Response.Tile = Tile;
                } else {
                    Response.Key = States::Input::Bad;
                }

                return Response;
            }

            default: {
                Response.Key = States::Input::Bad;
                return Response;
            }
        }
    }
}