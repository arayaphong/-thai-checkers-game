# Thai Checkers Game

## Overview
Thai Checkers is a two-player board game that involves strategic movement of pieces on a board. This project implements the game using modern C++17 features and follows a modular structure for easy testing and maintenance.

## Project Structure
The project is organized as follows:

```
thai-checkers
├── CMakeLists.txt          # Build configuration file for CMake
├── src                     # Source files for the game logic
│   ├── main.cpp            # Entry point of the application
│   ├── Board.cpp           # Implementation of the Board class
│   ├── Piece.cpp           # Implementation of the Piece class
│   └── THCheckers.cpp      # Implementation of the THCheckers class
├── include                  # Header files for the game classes
│   ├── Board.h             # Declaration of the Board class
│   ├── Piece.h             # Declaration of the Piece class
│   └── THCheckers.h        # Declaration of the THCheckers class
├── tests                   # Unit tests for the game
│   ├── CMakeLists.txt      # Build configuration for the test suite
│   ├── BoardTests.cpp      # Unit tests for the Board class
│   ├── PieceTests.cpp      # Unit tests for the Piece class
│   └── THCheckersTests.cpp  # Unit tests for the THCheckers class
└── README.md               # Documentation for the project
```

## Setup Instructions
1. **Clone the repository**:
   ```
   git clone <repository-url>
   cd thai-checkers
   ```

2. **Build the project**:
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run the game**:
   ```
   ./thai-checkers
   ```

## Usage
- The game is played on a board with pieces that players move according to the rules of Thai Checkers.
- Players take turns to make their moves until one player wins or the game ends in a draw.

## Game Rules
- Each player starts with a set of pieces placed on their side of the board.
- Players can move their pieces diagonally and capture opponent pieces by jumping over them.
- The objective is to capture all of the opponent's pieces or block them from making any legal moves.

## Testing
- Unit tests are provided for each class to ensure the functionality of the game logic.
- To run the tests, navigate to the `tests` directory and follow the build instructions similar to the main project.

## Contributions
Contributions to improve the game or add new features are welcome. Please submit a pull request with your changes.