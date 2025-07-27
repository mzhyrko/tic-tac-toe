#include "bots.hpp"
#include <random>
#include <vector>
#include <algorithm>
#include <cmath>

int beginnerBot(const Board& board, const std::string& currSign) {
    std::vector<int> available = board.getAvailableMoves();
    std::string opponent = (currSign == "X") ? "O" : "X";

    // Check for winning move
    for (int move : available) {
        Board tempBoard = board;
        tempBoard.makeMove(move, currSign);
        if (tempBoard.checkWin(currSign)) {
            return move;
        }
    }

    // Check for blocking move
    for (int move : available) {
        Board tempBoard = board;
        tempBoard.makeMove(move, opponent);
        if (tempBoard.checkWin(opponent)) {
            return move;
        }
    }

    // Otherwise, move randomly
    return randomBot(board, currSign);
}