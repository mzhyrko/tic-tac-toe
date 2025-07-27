#pragma once

#include "../core/board.hpp"
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <cstdint> 

using namespace std;

// Bots initerface
// Each bot function should return the position to place its sign
// If no moves are possible, return -1 
int randomBot(const Board& board, const string& currSign);
int beginnerBot(const Board& board, const string& currSign);
int dpTTToeBot(const Board& board, const string& botSign, int maxDepth); 

// Zobrist hashing for board state 
struct ZobristHash {
    vector<vector<uint64_t>> xKeys;
    vector<vector<uint64_t>> oKeys;
    vector<vector<uint64_t>> emptyKeys;
    ZobristHash(int boardSize);
    uint64_t operator()(const vector<vector<string>>& board) const;
};
