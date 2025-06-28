#include <iostream>
#include <bitset>
#include <iomanip>
#include <vector>
#include <cassert>
#include <optional>     // for std::optional
#include <cstdint>      // for uint64_t, uint32_t, uint8_t
#include <unordered_set>// for std::unordered_set
#include <bit>          // C++20: std::popcount
#include <compare>      // C++20: operator<=>
#include <format>       // C++20: std::format


// ===== Constants and Enums =====
constexpr int MAX_POSITIONS = 32;
constexpr int MAX_PIECES = 16;


enum class PieceType : uint8_t {
    WHITE_PION = 0,
    WHITE_DAME = 1,
    BLACK_PION = 2,
    BLACK_DAME = 3
};


// ===== Main Class: SparseGameState =====
class SparseGameState {
private:
    uint64_t state;
    
public:
    // Constructors
    SparseGameState() : state(0) {}
    explicit SparseGameState(uint64_t raw_state) : state(raw_state) {}
    
    // Set piece at position
    void setPiece(int position, PieceType type) {
        assert(position >= 0 && position < MAX_POSITIONS);
        
        // Remove existing piece (if any)
        removePiece(position);
        
        // Set occupancy bit
        state |= (1ULL << position);
        
        // Calculate piece index
        uint32_t occupancy_mask = static_cast<uint32_t>(state);
        uint32_t positions_before = (1ULL << position) - 1;
        int piece_index = std::popcount(occupancy_mask & positions_before);
        
        // Set type bits
        uint64_t type_shift = 32 + piece_index * 2;
        state |= (static_cast<uint64_t>(type) << type_shift);
    }
    
    // Remove piece from position
    void removePiece(int position) {
        assert(position >= 0 && position < MAX_POSITIONS);
        if (!isOccupied(position)) return;
        
        // Calculate piece index
        uint32_t occupancy_mask = static_cast<uint32_t>(state);
        uint32_t positions_before = (1ULL << position) - 1;
        int piece_index = std::popcount(occupancy_mask & positions_before);
        
        // Clear occupancy bit
        state &= ~(1ULL << position);
        
        // Shift type bits
        uint64_t type_mask = state >> 32;
        uint64_t lower_mask = (1ULL << (piece_index * 2)) - 1;
        uint64_t upper_mask = type_mask >> ((piece_index + 1) * 2);
        uint64_t new_types = (upper_mask << (piece_index * 2)) | lower_mask;
        
        // Reconstruct state
        state = (new_types << 32) | static_cast<uint32_t>(state);
    }
    
    // Get piece at position
    std::optional<PieceType> getPiece(int position) const {
        assert(position >= 0 && position < MAX_POSITIONS);
        if (!isOccupied(position)) return std::nullopt;
        
        uint32_t occupancy_mask = static_cast<uint32_t>(state);
        uint32_t positions_before = (1ULL << position) - 1;
        int piece_index = std::popcount(occupancy_mask & positions_before);
        return static_cast<PieceType>((state >> (32 + piece_index * 2)) & 3);
    }
    
    // Check if position is occupied
    bool isOccupied(int position) const {
        return (state & (1ULL << position)) != 0;
    }
    
    // Count total pieces
    int getPieceCount() const {
        return std::popcount(static_cast<uint32_t>(state));
    }
    
    // Get raw state value
    uint64_t getRawState() const { 
        return state; 
    }
    
    // Get occupancy mask (lower 32 bits)
    uint32_t getOccupancyMask() const {
        return static_cast<uint32_t>(state);
    }
    
    // Get type encoding (upper 32 bits)
    uint32_t getTypeEncoding() const {
        return static_cast<uint32_t>(state >> 32);
    }
    
    // Get all pieces as position-type pairs
    std::vector<std::pair<int, PieceType>> getAllPieces() const {
        std::vector<std::pair<int, PieceType>> pieces;
        
        for (int pos = 0; pos < MAX_POSITIONS; pos++) {
            if (auto piece = getPiece(pos); piece.has_value()) {
                pieces.push_back({pos, piece.value()});
            }
        }
        return pieces;
    }
    
    // Print board state
    void printState() const {
        std::cout << std::format("=== SPARSE GAME STATE ===\n");
        std::cout << std::format("Raw state: 0x{:016X}\n", state);
        std::cout << std::format("Occupancy: {:032b}\n", getOccupancyMask());
        std::cout << std::format("Types:     {:032b}\n", getTypeEncoding());
        std::cout << std::format("Pieces: {}\n\n", getPieceCount());
        std::cout << "Board layout:\n";
        for (int row = 3; row >= 0; row--) {
            for (int col = 0; col < 8; col++) {
                int pos = row * 8 + col;
                auto piece = getPiece(pos);
                
                char symbol = '.';
                if (piece.has_value()) {
                    using enum PieceType;
                    switch (piece.value()) {
                        case WHITE_PION: symbol = 'P'; break;
                        case WHITE_DAME: symbol = 'D'; break;
                        case BLACK_PION: symbol = 'p'; break;
                        case BLACK_DAME: symbol = 'd'; break;
                    }
                }
                std::cout << symbol << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\nLegend: P=White Pion, D=White Dame, p=Black Pion, d=Black Dame\n\n";
    }
    
    // Validate state consistency
    bool isValid() const {
        return getPieceCount() <= MAX_PIECES;
    }
    
    // Default comparison (C++20)
    auto operator<=>(const SparseGameState& other) const = default;
};


// ===== Arithmetic Operations Class =====
class GameStateArithmetic {
public:
    // Compare equality
    static bool areEqual(const SparseGameState& a, const SparseGameState& b) {
        return a == b;  // Using operator== from C++20
    }
    
    // Calculate difference
    static uint64_t getDifference(const SparseGameState& a, const SparseGameState& b) {
        return a.getRawState() ^ b.getRawState();
    }
    
    // Count differences
    static int countDifferences(const SparseGameState& a, const SparseGameState& b) {
        return std::popcount(getDifference(a, b));
    }
    
    // Hash function
    static uint64_t hash(const SparseGameState& state) {
        uint64_t x = state.getRawState();
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        return (x >> 16) ^ x;
    }
    
    // Copy state
    static SparseGameState copy(const SparseGameState& source) {
        return source;  // Using copy constructor
    }
};


// ===== Hash Support for Standard Containers =====
namespace std {
    template<>
    struct hash<SparseGameState> {
        size_t operator()(const SparseGameState& state) const noexcept {
            return GameStateArithmetic::hash(state);
        }
    };
}


// ===== Usage Examples =====
void runTests() {
    SparseGameState game;
    
    // Test adding pieces
    game.setPiece(10, PieceType::WHITE_DAME);
    assert(game.getPiece(10).value() == PieceType::WHITE_DAME);
    
    // Test removing pieces
    game.removePiece(10);
    assert(!game.getPiece(10).has_value());
    
    // Test comparison operators
    SparseGameState game2;
    game2.setPiece(5, PieceType::BLACK_PION);
    assert(game != game2);
    
    std::cout << "All tests passed!\n";
}


void demonstrateArithmetic() {
    std::cout << std::format("=== ARITHMETIC OPERATIONS DEMO ===\n");
    
    SparseGameState game1, game2;
    
    // Create different states
    game1.setPiece(0, PieceType::WHITE_PION);
    game1.setPiece(5, PieceType::BLACK_DAME);
    
    game2.setPiece(0, PieceType::WHITE_PION);
    game2.setPiece(7, PieceType::BLACK_DAME);
    
    // Demonstrate fast operations
    std::cout << std::format("State 1: 0x{:016X}\n", game1.getRawState());
    std::cout << std::format("State 2: 0x{:016X}\n", game2.getRawState());
    std::cout << std::format("Equal: {}\n", game1 == game2 ? "YES" : "NO");
    std::cout << std::format("Difference: 0x{:X}\n", GameStateArithmetic::getDifference(game1, game2));
    std::cout << std::format("Hash 1: 0x{:X}\n", GameStateArithmetic::hash(game1));
    std::cout << std::format("Hash 2: 0x{:X}\n", GameStateArithmetic::hash(game2));
    
    // Demonstrate hash table usage
    std::unordered_set<SparseGameState> visited_states;
    visited_states.insert(game1);
    visited_states.insert(game2);
    std::cout << std::format("Unique states in hash set: {}\n", visited_states.size());
    std::cout << "\n";
}


int main() {
    std::cout << "SPARSE GAME STATE - MODERN C++20 IMPLEMENTATION\n";
    std::cout << "================================================\n\n";
    
    runTests();
    
    // Create example game state
    SparseGameState game;
    game.setPiece(0, PieceType::WHITE_PION);
    game.setPiece(5, PieceType::WHITE_DAME);
    game.setPiece(10, PieceType::BLACK_PION);
    game.setPiece(15, PieceType::BLACK_DAME);
    
    game.printState();
    
    demonstrateArithmetic();
    
    std::cout << "=== SUMMARY ===\n";
    std::cout << "✓ Complete game state in single uint64_t\n";
    std::cout << "✓ Modern C++20 features for type safety\n";
    std::cout << "✓ Optimal bit efficiency for sparse boards\n";
    std::cout << "✓ Fast arithmetic operations\n";
    std::cout << "✓ Hash table support included\n";
    std::cout << "✓ Zero-overhead abstractions\n";
    
    return 0;
}
