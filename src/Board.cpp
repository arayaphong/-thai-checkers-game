#include "Board.h"
#include "KingPiece.h"
#include <algorithm>  // for remove_if
#include <iostream>
#include <vector>
#include <functional>
#include <set> // Add include for set to detect player names

Board::Board() {
    grid.resize(8, std::vector<Piece*>(8, nullptr));
}

Board::~Board() {
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
        }
    }
}

void Board::initialize(const std::vector<std::vector<Piece*>>& initialGrid) {
    clearBoard();
    grid = initialGrid;
    // detect player names for custom grid initialization
    std::set<std::string> names;
    for (const auto& row : grid) {
        for (const auto& p : row) {
            if (p) names.insert(p->getColor());
        }
    }
    auto it = names.begin();
    if (names.size() >= 1) {
        player1Name = *it;
        ++it;
    }
    if (names.size() >= 2) {
        player2Name = *it;
    }
    currentPlayer = player1Name;
}

void Board::initialize(const std::string& p1, const std::string& p2) {
    // Store player names
    player1Name = p1;
    player2Name = p2;
    currentPlayer = player1Name;
    clearBoard();

    // Initialize the board with 8:8 pieces each
    grid.resize(8, std::vector<Piece*>(8, nullptr));

    // place pieces for player 1 (X)
    for (int row = 0; row < 2; ++row) {
        for (int col = (row % 2); col < 8; col += 2) {
            grid[row][col] = new Piece(player1Name, {row, col});
        }
    }

    // place pieces for player 2 (O)
    for (int row = 6; row < 8; ++row) {
        for (int col = (row % 2); col < 8; col += 2) {
            grid[row][col] = new Piece(player2Name, {row, col});
        }
    }
}

void Board::clearBoard()
{
    for (auto &row : grid)
    {
        for (auto &piece : row)
        {
            delete piece;
            piece = nullptr;
        }
    }
}

void Board::display() const {
    // Print column indices
    std::cout << "  ";
    for (int j = 0; j < 8; ++j) {
        std::cout << j << " ";
    }
    std::cout << std::endl;
    // Print each row with row index
    for (int i = 0; i < 8; ++i) {
        std::cout << i << " ";
        for (const auto& piece : grid[i]) {
            if (piece) {
                char symbol = (piece->getColor() == player1Name) ? 'X' : 'O';
                std::cout << symbol << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

// Wrapper for choice paths, delegates to getStates
std::vector<std::vector<Position>> Board::getChoices(const Piece& piece) const {
    States s = const_cast<Board*>(this)->getStates(piece);
    return s.getChoices();
}

// Generate all possible move paths wrapped in a States object
States Board::getStates(const Piece& piece) {
    States s;
    s.board = this;
    s.piece = &piece;
    // replicate getTargetPositions logic here
    Position pos = piece.getPosition();
    int x = pos.x, y = pos.y;
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return s;
    Piece* p = grid[x][y];
    if (!p) return s;
    // movement dirs
    std::vector<std::pair<int,int>> dirs;
    if (p->getType() == Piece::Type::Regular) {
        if (p->getColor() == player1Name) dirs = {{1,1},{1,-1}};
        else dirs = {{-1,1},{-1,-1}};
    } else {
        dirs = {{1,1},{1,-1},{-1,1},{-1,-1}};
    }
    // simple moves
    std::vector<std::vector<Position>>& allMoves = s.choices;
    for (auto [dx,dy] : dirs) {
        int nx = x+dx, ny = y+dy;
        if (nx>=0&&nx<8&&ny>=0&&ny<8 && !grid[nx][ny])
            allMoves.push_back({{nx,ny}});
    }
    // capture chains
    auto gridCopy = grid;
    std::vector<Position> path;
    std::vector<std::vector<Position>> capPaths;
    std::function<void(int,int)> dfs = [&](int cx,int cy){
        bool found=false;
        for(auto [dx,dy]:dirs){
            int mx=cx+dx, my=cy+dy, lx=cx+2*dx, ly=cy+2*dy;
            if(mx>=0&&mx<8&&my>=0&&my<8&&lx>=0&&lx<8&&ly>=0&&ly<8&&
               gridCopy[mx][my]&&gridCopy[mx][my]->getColor()!=p->getColor()&&
               !gridCopy[lx][ly]){
                found=true;
                Piece* cap=gridCopy[mx][my];
                gridCopy[cx][cy]=nullptr; gridCopy[mx][my]=nullptr; gridCopy[lx][ly]=p;
                path.push_back({lx,ly}); dfs(lx,ly);
                path.pop_back(); gridCopy[cx][cy]=p; gridCopy[mx][my]=cap; gridCopy[lx][ly]=nullptr;
            }
        }
        if(!found && !path.empty()) capPaths.push_back(path);
    };
    dfs(x,y);
    for(auto &seq:capPaths) allMoves.push_back(seq);
    // detect captures and filter simple if needed
    Position start = pos;
    for(auto &pathOpt:allMoves) if(!pathOpt.empty()&& std::abs(pathOpt[0].x-start.x)==2){ s.hasCapture=true; break; }
    if(s.hasCapture){
        auto &ch=allMoves;
        ch.erase(std::remove_if(ch.begin(),ch.end(),[&](auto &p){ return p.empty()|| std::abs(p[0].x-start.x)!=2;}), ch.end());
    }
    return s;
}

// Apply a selected path back to the board, moving the piece and removing captures
void States::selectMove(const std::vector<Position>& path) {
    if (!board || !piece || path.empty()) return;
    Position from = piece->getPosition();
    int x0 = from.x;
    int y0 = from.y;
    // extract the pointer to the actual Piece
    Piece* p = board->grid[x0][y0];
    board->grid[x0][y0] = nullptr;
    // follow the chosen path
    for (const auto& pos : path) {
        int x1 = pos.x;
        int y1 = pos.y;
        int dx = x1 - x0;
        int dy = y1 - y0;
        // if this is a capture jump, remove the jumped piece
        if (std::abs(dx) == 2 && std::abs(dy) == 2) {
            int mx = x0 + dx/2;
            int my = y0 + dy/2;
            delete board->grid[mx][my];
            board->grid[mx][my] = nullptr;
        }
        // move piece pointer
        p->setPosition({x1, y1});
        x0 = x1; y0 = y1;
    }
    // place on final square
    board->grid[x0][y0] = p;
    // optionally switch current player
    if (!board->player1Name.empty() && !board->player2Name.empty()) {
        board->currentPlayer = (board->currentPlayer == board->player1Name)
            ? board->player2Name : board->player1Name;
    }
}

// Get all pieces that can move for a specific player
std::vector<Piece> Board::getMoveablePieces(const std::string& playerName) const {
    std::vector<Piece> movablePieces;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Piece* piece = grid[x][y];
            if (piece && piece->getColor() == playerName) {
                if (!getChoices(*piece).empty()) {
                    movablePieces.push_back(*piece);
                }
            }
        }
    }
    return movablePieces;
}

// Return the current player's name
std::string Board::getCurrentPlayer() const {
    return currentPlayer;
}