# Thai Checkers Game

## Overview
Thai Checkers is a two-player board game implemented in modern C++17. This project features a modular architecture with support for both Pion (regular) and Dame (king) pieces, providing a robust engine for analytics, AI, and scenario testing. The codebase ensures accurate rule enforcement and move generation for each piece type.

## Features
- **Full Thai Checkers Engine**: Implements all rules, including forced captures, promotion, and multi-capture sequences.
- **Analytics & Simulation**: Cloneable game states, move history, and position evaluation for AI and analysis.
- **Custom Board Initialization**: Supports both standard and custom board setups.
- **Comprehensive Testing**: Extensive unit tests for all game scenarios and rules.
- **Modern C++ Design**: Clean, modular code with clear separation of concerns and memory management.

## Project Structure
```
thai-checkers/
├── CMakeLists.txt              # Build configuration for CMake
├── src/                        # Source files
│   ├── main.cpp                # Application entry point and demonstrations
│   ├── Board.cpp               # Board interface and display
│   ├── GameModel.cpp           # Core game engine
│   └── Piece.cpp               # Piece representation (Pion/Dame)
├── include/                    # Header files
│   ├── Board.h                 # Board interface
│   ├── GameModel.h             # Game engine
│   ├── Move.h                  # Move structure
│   ├── Piece.h                 # Piece class (Pion/Dame)
│   └── Position.h              # Position struct
├── tests/                      # Unit tests
│   ├── CMakeLists.txt          # Test build config
│   ├── BoardTests.cpp          # Board logic tests
│   ├── BoardDisplayTests.cpp   # Board display tests
│   ├── GameLogicTests.cpp      # Game rules and move logic tests
│   ├── GameScenariosTests.cpp  # Full scenario and integration tests
│   └── PieceTests.cpp          # Piece class tests
├── PionRules.md                # Pion movement and capture rules
├── DameRules.md                # Dame movement and capture rules
└── README.md                   # Project documentation
```

## Setup Instructions
1. **Clone the repository**:
   ```bash
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

4. **Run tests**:
   ```bash
   ctest --output-on-failure
   ```

## Architecture Overview

### Piece Types

There are two types of pieces:
- **Pion**: The regular piece, moves and captures forward diagonally.
- **Dame**: The promoted piece (king), moves and captures diagonally in all directions and multiple squares.

### GameModel - Core Engine
The heart of the system is the `GameModel` class which handles:
- **Game State Management**: Board representation, current player, move history
- **Move Generation**: Valid moves, capture sequences, forced captures
  - Uses explicit functions for each piece type:
    - `generatePionSimpleMoves`, `generatePionCaptureMoves`
    - `generateDameSimpleMoves`, `generateDameCaptureMoves`
- **Rule Enforcement**: Move validation, win conditions, game termination
- **Analytics Support**: Position evaluation, move simulation, game cloning

### Board - Interface Layer
The `Board` class provides a high-level interface for:
- Game initialization (standard setup or custom positions)
- Move execution and validation
- Display and visualization
- Legacy compatibility

### Move System
Moves are represented as structured data containing:
- Starting position
- Path of positions (for multi-step captures)
- Captured pieces
- Player information

## API Usage Examples

### Basic Game Setup
```cpp
#include "Board.h"

int main() {
    Board board;
    board.initialize("Player1", "Player2");
    board.display();  // Shows the initial board state
    return 0;
}
```

### Move Generation and Execution
```cpp
#include "Board.h"
#include "GameModel.h"

int main() {
    Board board;
    board.initialize("Player1", "Player2");
    auto allMoves = board.getAllValidMoves();
    // Example: execute the first available move for the current player
    for (const auto& [pos, moves] : allMoves) {
        if (!moves.empty()) {
            GameModel* model = board.getModel();
            model->executeMove(moves[0]);
            break;
        }
    }
    return 0;
}
```

### Custom Board Scenarios
```cpp
#include "Board.h"
#include "Piece.h"
#include <vector>

int main() {
    std::vector<std::vector<Piece*>> customGrid(8, std::vector<Piece*>(8, nullptr));
    customGrid[3][3] = new Piece("Player1", {3, 3});
    customGrid[4][4] = new Piece("Player2", {4, 4});
    Board board;
    board.initialize(customGrid);
    board.display();
    // Clean up allocated pieces
    delete customGrid[3][3];
    delete customGrid[4][4];
    return 0;
}
```

## Game Rules
Thai Checkers follows these rules:
- **Board Setup**: 8x8 board with pieces on dark squares of first two rows for each player
- **Movement**:
  - **Pion**: Moves one square diagonally forward to empty squares
  - **Dame**: Moves any number of squares diagonally in any direction
- **Capturing**:
  - **Pion**: Jumps diagonally forward over opponent pieces to capture
  - **Dame**: Jumps diagonally over opponent pieces in any direction, any distance
- **Multiple Captures**: If multiple captures are possible, they must all be taken in sequence
- **Forced Captures**: If a capture is available, it must be taken (simple moves are not allowed)
- **Promotion**: A Pion reaching the last row is promoted to Dame
- **Win Conditions**: Win by capturing all opponent pieces or blocking all their moves

## Testing Framework
The project includes comprehensive unit tests covering:

### BoardTests
- Initial board setup and piece placement
- Move generation and validation (for both Pion and Dame)
- Custom board scenarios
- Multi-capture sequences
- Edge cases and blocked pieces

### GameModelTests  
- Standard game initialization
- Move execution and game state updates
- Capture mechanics and forced captures
- Game termination and winner detection
- Position evaluation and cloning

### Integration Tests
- Board display functionality
- Move history tracking
- Statistics and analytics

### Running Tests
```bash
cd build
ctest --output-on-failure        # Run all tests with detailed output
ctest -V                         # Verbose output for debugging
```

## Development and Extension
The modular architecture makes it easy to extend the system:

## Performance Considerations
- Move generation is optimized for typical game positions and distinguishes between Pion and Dame logic
- Memory management handles piece allocation/deallocation
- Game cloning supports efficient tree search algorithms
- Lazy evaluation where possible to minimize computation

## Contributing
When contributing to this project:
1. Maintain the existing architecture patterns
2. Add comprehensive tests for new features
3. Update documentation for API changes
4. Follow C++17 best practices
5. Ensure all tests pass before submitting