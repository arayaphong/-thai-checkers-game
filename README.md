# Thai Checkers Game

## Overview
Thai Checkers is a two-player board game that involves strategic movement of pieces on a board. This project implements the game using modern C++17 features with a sophisticated architecture designed for game analytics, AI development, and strategic analysis.

## Features
- **Complete Game Engine**: Full implementation of Thai Checkers rules including move validation, capture sequences, and win conditions
- **Analytics Framework**: Built-in position evaluation, move generation, and game state analysis
- **Simulation Capabilities**: Clone game states for AI lookahead and scenario testing
- **Comprehensive Testing**: Extensive unit tests covering all game scenarios
- **Modern C++ Design**: Clean, modular architecture with separation of concerns

## Project Structure
The project is organized as follows:

```
thai-checkers/
├── CMakeLists.txt              # Build configuration file for CMake
├── src/                        # Source files for the game logic
│   ├── main.cpp                # Entry point of the application
│   ├── Board.cpp               # Board interface and display functionality
│   ├── GameModel.cpp           # Core game engine and rules implementation
│   └── Piece.cpp               # Piece representation and behavior
├── include/                    # Header files for the game classes
│   ├── Board.h                 # Board interface declaration
│   ├── GameModel.h             # Game engine and analytics interface
│   ├── Move.h                  # Move structure definitions
│   ├── Piece.h                 # Piece class declaration
│   └── Position.h              # Position coordinate structure
├── tests/                      # Comprehensive unit test suite
│   ├── CMakeLists.txt          # Build configuration for tests
│   ├── BoardTests.cpp          # Board functionality tests
│   ├── BoardDisplayTests.cpp   # Display and visualization tests
│   ├── GameModelTests.cpp      # Core game logic tests
│   └── PieceTests.cpp          # Piece behavior tests
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

### GameModel - Core Engine
The heart of the system is the `GameModel` class which handles:
- **Game State Management**: Board representation, current player, move history
- **Move Generation**: Valid moves, capture sequences, forced captures
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

Board board;
board.initialize("Player1", "Player2");
board.display();  // Shows the initial board state
```

### Move Generation and Execution
```cpp
// Get all valid moves for current player
auto allMoves = board.getAllValidMoves();

// Get moves for a specific piece
auto moves = board.getValidMovesFor({2, 1});

// Execute a move through the model
if (!moves.empty()) {
    GameModel* model = board.getModel();
    model->executeMove(moves[0]);
}
```

### Analytics and Evaluation
```cpp
GameModel* model = board.getModel();

// Evaluate position strength
double score = model->evaluatePosition("Player1");

// Clone for simulation
GameModel* simulation = model->clone();
simulation->executeMove(someMove);
// ... analyze hypothetical position
delete simulation;
```

### Custom Board Scenarios
```cpp
// Create custom board position
std::vector<std::vector<Piece*>> customGrid(8, std::vector<Piece*>(8, nullptr));
customGrid[3][3] = new Piece("Player1", {3, 3});
customGrid[4][4] = new Piece("Player2", {4, 4});

board.initialize(customGrid);
```

## Game Rules
Thai Checkers follows these rules:
- **Board Setup**: 8x8 board with pieces on dark squares of first two rows for each player
- **Movement**: Pieces move diagonally forward one square to empty squares
- **Capturing**: Jump diagonally over opponent pieces to capture them
- **Multiple Captures**: If multiple captures are possible, they must all be taken in sequence
- **Forced Captures**: If a capture is available, it must be taken (simple moves are not allowed)
- **Win Conditions**: Win by capturing all opponent pieces or blocking all their moves

## Testing Framework
The project includes comprehensive unit tests covering:

### BoardTests
- Initial board setup and piece placement
- Move generation and validation
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

### Adding AI Players
```cpp
// Implement using the analytics framework
class AIPlayer {
    GameModel* model;
    
    Move selectBestMove() {
        auto moves = model->getAllPossibleMoves();
        Move bestMove;
        double bestScore = -INFINITY;
        
        for (const auto& move : moves) {
            GameModel* sim = model->clone();
            sim->executeMove(move);
            double score = sim->evaluatePosition(currentPlayer);
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
            delete sim;
        }
        return bestMove;
    }
};
```

### Custom Evaluation Functions
```cpp
// Override or extend the evaluation system
double customEvaluate(const GameModel& model, const std::string& player) {
    double score = model.evaluatePosition(player);
    // Add custom strategic factors
    score += centerControlBonus(model, player);
    score += mobilityBonus(model, player);
    return score;
}
```

## Performance Considerations
- Move generation is optimized for typical game positions
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