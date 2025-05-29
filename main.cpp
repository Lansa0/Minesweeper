#include "output.hpp"
#include "input.hpp"


std::string F = "ABCD";

// Set to 85x34
// clang++ -std=c++11 -IHeaders -o Binary/minesweeper main.cpp Sources/output.cpp Sources/input.cpp && ./Binary/minesweeper

int main() 
{
    Output::Dashboard();

    do
    {
        const Input::Response Response = Input::Get();
        Output::Log(Response.Key,Response.Tile);

    } while (true);

}
