#include "input.hpp"

namespace
{
    const char* SET_CURSOR = "\033[33;12H";
}

namespace Input
{
    /**
     * Input response format
     */
    struct Response;

    /**
     * Input parser
     */
    Response Get()
    {
        // Clears the input line
        // Will mess with the UI if user input exceeds outside the borders
        std::cout << SET_CURSOR << "                                                                         " << SET_CURSOR;

        std::string Input;
        std::cin >> Input;

        Response R;
        R.Key = Input[0];
        R.Tile = {'A','B'};

        return R;
    }
}