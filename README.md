# Build
Create a binary folder

`clang++ -std=c++11 -IHeaders -o binary/minesweeper main.cpp Sources/output.cpp Sources/input.cpp Sources/tiles.cpp`

# User Manual
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

- Quit

  **Usage** : Quit

  Properly quits the program
