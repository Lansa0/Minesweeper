#include "responses.hpp"
#include "output.hpp"
#include "input.hpp"
#include "tiles.hpp"

// Set to 85x34
// clang++ -std=c++11 -IHeaders -o binary/minesweeper main.cpp Sources/output.cpp Sources/input.cpp Sources/tiles.cpp && ./binary/minesweeper

// TODO
// recreate and fix that one bug, somehow

int main() {
    Output::Dashboard();

    bool NewBoard = true;

    States::Game GameState;

    do {
        const Responses::Input Response = Input::Get();
        const auto Tile = Response.Tile;
        const auto Key = Response.Key;

        switch (Key) {
            case States::Input::Tap: {
                if (NewBoard) {
                    NewBoard = false;
                    Tiles::generateBoard(Tile);
                }

                const Responses::Tap TR = Tiles::Tap(Tile);
                GameState = TR.State;

                Output::Reveal(TR);
                Output::Log(States::Log::Tap,Tile);
                break;
            }

            case States::Input::Flag: {
                if (NewBoard) {
                    Output::Log(States::Log::Failed,Tile);
                    break;
                }

                const Responses::Flag FR = Tiles::Flag(Tile);

                States::Log LogState;
                if (FR.State == States::Flag::Add) {
                    LogState = States::Log::Flag;
                } else if (FR.State == States::Flag::Remove) {
                    LogState = States::Log::Unflag;
                } else {
                    LogState = States::Log::Flagfail;
                }

                Output::Flag(Tile,FR);
                Output::Log(LogState,Tile);
                break;
            }

            case States::Input::Reset: {
                if (NewBoard) {
                    Output::Log(States::Log::Failed,Tile);
                    break;
                }
                NewBoard = true;

                Output::Reset();
                Output::Log(States::Log::Reset,Tile);
                break;
            }

            case States::Input::Bad: {
                Output::Log(States::Log::Bad,Tile);
                break;
            }

            case States::Input::Quit: {
                Output::Log(States::Log::Quit,Tile);
                // Add a proper exit
                return 1;
            }
        }

        // Output::Log(Key,Response.Tile,FailedInput);

        switch (GameState) {
            case States::Game::Playon: {
                break;
            }

            case States::Game::Win: {
                break;
            }

            case States::Game::Lose: {
                break;
            }
        }

    } while (true);

}
