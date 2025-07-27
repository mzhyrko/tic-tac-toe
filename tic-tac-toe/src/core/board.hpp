#pragma once

#include <vector>
#include <string>

// Class representing the game board for Tic-Tac-Toe
class Board {
public:
    // Constructor to initialize the board with size and win condition
    Board(int size, int winCondition);

    // Initialize the board with numbers representing available positions
    void init();

    // Attempt to place a sign at the specified position; returns true if successful
    bool makeMove(int position, const std::string& sign);

    // Check if the specified sign has won the game
    bool checkWin(const std::string& sign) const;

    // Check if the board is full (no more moves possible)
    bool isFull() const;

    // Print the current state of the board to the console (can be replaced with "send" or smth like this)
    void print() const;

    // Get the size of the board
    int getBoardSize() const;

    // Get the number of consecutive signs required to win
    int getLineForWin() const;

    // Get a list of available move positions
    std::vector<int> getAvailableMoves() const;

    // Get the value of a specific cell at (row, col)
    std::string getCell(int row, int col) const;

    // Get a const reference to the current board state (safe for bots/AI)
    const std::vector<std::vector<std::string>>& getBoardState() const;

    void setCell(int i, int j, const std::string& value) {
        if (i >= 0 && i < boardSize && j >= 0 && j < boardSize) {
            board[i][j] = value;
        }
    }

private:
    std::vector<std::vector<std::string>> board; // 2D vector representing the board
    int boardSize;                               // Size of the board (e.g., 3 for 3x3)
    int lineForWin;                              // Number of signs in a row needed to win
};