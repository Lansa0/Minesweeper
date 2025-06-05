#include "responses.hpp"
#include "output.hpp"
#include "input.hpp"
#include "tiles.hpp"

int main() {
    Output::Dashboard();
    Responses::SessionStats SessionStats = Responses::SessionStats();

    bool NewGame = true;
    States::Game GameState = States::Game::Playon;

    do {
        const Responses::Input Response = Input::Get();
        const std::pair<char,char> Tile = Response.Tile;
        const States::Input Key = Response.Key;

        if ((GameState != States::Game::Playon) && !(Key == States::Input::Reset || Key == States::Input::Quit)) {
            Output::Log(States::Log::Failed,Tile);
            continue;
        }

        switch (Key) {
            case States::Input::Tap: {
                if (NewGame) {
                    NewGame = false;
                    SessionStats.BoardsPlayed++;
                    Tiles::generateBoard(Tile);
                }

                const Responses::Tap TR = Tiles::Tap(Tile);
                SessionStats.TilesRevealed += TR.Tiles.size();
                GameState = TR.State;

                Output::Reveal(TR);
                Output::Log(States::Log::Tap,Tile);
                break;
            }

            case States::Input::Flag: {
                if (NewGame) {
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
                if (NewGame) {
                    Output::Log(States::Log::Failed,Tile);
                    break;
                }
                NewGame = true;
                GameState = States::Game::Playon;

                Output::Reset();
                Output::Log(States::Log::Reset,Tile);
                break;
            }

            case States::Input::Bad: {
                Output::Log(States::Log::Bad,Tile);
                break;
            }

            case States::Input::Quit: {
                Output::Quit(SessionStats);
                return 0;
            }
        }

        if (NewGame) {continue;}

        switch (GameState) {
            case States::Game::Playon: {
                break; // Do nothing
            }

            case States::Game::Win: {
                SessionStats.GamesWon++;
                Output::Log(States::Log::Win,Tile);
                break;
            }

            case States::Game::Lose: {
                SessionStats.GamesLost++;
                Output::Log(States::Log::Lose,Tile);
                break;
            }
        }

    } while (true);
}