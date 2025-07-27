#include "bots.hpp"
#include <random>
#include <vector>
#include <algorithm>
#include <cmath>

// Function to select a random move from available moves on the board
int randomBot(const Board& board, const std::string& currSign) {
    std::vector<int> available = board.getAvailableMoves();
    if (available.empty()) return -1;
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, available.size() - 1);
    return available[dist(gen)];
}