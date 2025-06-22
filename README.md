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
│   ├── KingPiece.cpp       # Implementation of the KingPiece subclass
│   └── THCheckers.cpp      # Implementation of the THCheckers class
├── include                  # Header files for the game classes
│   ├── Board.h             # Declaration of the Board class
│   ├── Piece.h             # Declaration of the Piece class
│   ├── KingPiece.h         # Declaration of the KingPiece subclass
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
   ```bash
   mkdir -p build && cd build
   cmake ..
   cmake --build .
   ```

3. **Run the game**:
   ```bash
   ./ThaiCheckers
   ```

## Usage
- The game is played on a board with pieces that players move according to the rules of Thai Checkers.
- Players take turns to make their moves until one player wins or the game ends in a draw.
  
### Game API
- Initialize and start a game with default player names:
  ```cpp
  THCheckers game;
  game.startGame();
  game.displayBoard(); // Show initial board
  ```
- Use a custom constructor to name players:
  ```cpp
  THCheckers customGame("Alice", "Bob");
  customGame.startGame();
  ```
- Retrieve valid moves for any piece using `getTargetPositions`:
  ```cpp
  Position pos{1, 1};
  auto moves = game.getTargetPositions(pos);
  for (auto m : moves) {
      std::cout << "(" << m.x << "," << m.y << ") ";
  }
  ```
- **NEW:** Retrieve all pieces that can move for a specific player using `getMoveablePieces`:
  ```cpp
  // Get all Player1 pieces that have at least one valid move
  std::vector<Position> movable = board.getMoveablePieces("Player1");
  for (const Position& pos : movable) {
      std::cout << "Player1 can move piece at (" << pos.x << ", " << pos.y << ")\n";
  }
  ```

## Game Rules
- Each player starts with a set of pieces placed on their side of the board.
- Players can move their pieces diagonally and capture opponent pieces by jumping over them.
- The objective is to capture all of the opponent's pieces or block them from making any legal moves.

## Testing
- Unit tests are provided for each class to ensure the functionality of the game logic.
- The test suite now includes comprehensive scenarios for the `getMoveablePieces` method, covering:
  - Initial board state
  - After moves
  - Capture opportunities
  - Edge and corner cases
  - Empty and custom boards
  - Consistency and correctness
- To run the tests, build the project and run:
  ```bash
  cd build
  ctest --verbose
  ```
- All tests should pass, confirming the correctness of the game logic and move generation.

## Contributions
Contributions to improve the game or add new features are welcome. Please submit a pull request with your changes.