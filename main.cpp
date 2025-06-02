#include "output.hpp"
#include "input.hpp"
#include "tiles.hpp"

// Set to 85x34
// clang++ -std=c++11 -IHeaders -o binary/minesweeper main.cpp Sources/output.cpp Sources/input.cpp Sources/tiles.cpp && ./binary/minesweeper

int main() {
    Output::Dashboard();

    bool NewBoard = true;

    do
    {
        const Input::Response Response = Input::Get();

        bool FailedInput = false;
        switch (Response.Key) {
            case 'T': {
                if (NewBoard) {
                    NewBoard = false;
                    Tiles::generateBoard(Response.Tile);
                }

                Tiles::TapResponse TR = Tiles::Tap(Response.Tile);
                Output::Reveal(TR.Tiles);

                break;
            }

            case 'F':
                if (NewBoard) {
                    FailedInput = true;
                    break;
                }

            case 'U':
                if (NewBoard) {
                    FailedInput = true;
                    break;
                }

            case 'R':
                if (NewBoard) {
                    FailedInput = true;
                    break;
                }
                // Reset Board

            default: break;
        }

        Output::Log(Response.Key,Response.Tile,FailedInput);

    } while (true);

}
