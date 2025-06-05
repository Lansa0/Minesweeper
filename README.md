# Build
Create a binary folder

`clang++ -std=c++11 -IHeaders -o binary/minesweeper main.cpp Sources/output.cpp Sources/input.cpp Sources/tiles.cpp`

# User Manual

## Setup

For a proper experience set the dimensions of your terminal of choice to 85x34 

## Commands

> [!Note]
> Commands and tile inputs are not case sensitive 

> [!Important]
> Tile inputs are read as **ROW**:**COLUMN** / **Y-AXIS**:**X-AXIS** (i.e **A:Z**)

- Tap

  **Usage** : Tap *TILE*

  Reveals the given tile on the board

- Flag

  **Usage** : Flag *TILE*

  Flags the given tile

  If the tile is already flagged, it will instead be unflagged

- Reset

  **Usage** : Reset

  Clears/Resets the board for a new game

  Use this after a win/loss or when you just don't like what you've got

- Quit

  **Usage** : Quit

  Properly quits the program
