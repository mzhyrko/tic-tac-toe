#include "board.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

Board::Board(int size, int winCondition) : boardSize(size), lineForWin(winCondition) {
    init();
}

void Board::init() {
    // Resize the board and fill it with position numbers
    board = std::vector<std::vector<std::string>>(boardSize, std::vector<std::string>(boardSize));
    int numDigits = std::to_string(boardSize * boardSize).length();
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            int num = i * boardSize + j + 1;
            std::string numStr = std::to_string(num);
            while (numStr.length() < numDigits) {
                numStr = "0" + numStr;
            }
            board[i][j] = numStr;
        }
    }
}

bool Board::makeMove(int position, const std::string& sign) {
    // Convert position to row and column
    int index = position - 1;
    int row = index / boardSize;
    int col = index % boardSize;
    if (row >= 0 && row < boardSize && col >= 0 && col < boardSize &&
        board[row][col] != "X" && board[row][col] != "O") {
        board[row][col] = sign;
        return true;
    }
    return false;
}

bool Board::checkWin(const std::string& sign) const {
    // Check rows
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j <= boardSize - lineForWin; ++j) {
            bool win = true;
            for (int k = 0; k < lineForWin; ++k) {
                if (board[i][j + k] != sign) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }
    // Check columns
    for (int j = 0; j < boardSize; ++j) {
        for (int i = 0; i <= boardSize - lineForWin; ++i) {
            bool win = true;
            for (int k = 0; k < lineForWin; ++k) {
                if (board[i + k][j] != sign) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }
    // Check main diagonals
    for (int i = 0; i <= boardSize - lineForWin; ++i) {
        for (int j = 0; j <= boardSize - lineForWin; ++j) {
            bool win = true;
            for (int k = 0; k < lineForWin; ++k) {
                if (board[i + k][j + k] != sign) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }
    // Check anti-diagonals
    for (int i = 0; i <= boardSize - lineForWin; ++i) {
        for (int j = lineForWin - 1; j < boardSize; ++j) {
            bool win = true;
            for (int k = 0; k < lineForWin; ++k) {
                if (board[i + k][j - k] != sign) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }
    return false;
}

bool Board::isFull() const {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") {
                return false;
            }
        }
    }
    return true;
}

void Board::print() const {
    const int cellWidth = 5;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] == "X")
                cout << "\t" << "\033[1;31m" << board[i][j] << "\033[0m";
            else if (board[i][j] == "O")
                cout << "\t" << "\033[1;34m" << board[i][j] << "\033[0m";
            else
                cout << "\t" << board[i][j];
        }
        cout << endl;
    }

    cout << endl;
}

int Board::getBoardSize() const {
    return boardSize;
}

int Board::getLineForWin() const {
    return lineForWin;
}

std::vector<int> Board::getAvailableMoves() const {
    std::vector<int> available;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") {
                int position = i * boardSize + j + 1;
                available.push_back(position);
            }
        }
    }
    return available;
}

std::string Board::getCell(int row, int col) const {
    if (row < 0 || row >= boardSize || col < 0 || col >= boardSize) {
        return "";
    }
    return board[row][col];
}

const std::vector<std::vector<std::string>>& Board::getBoardState() const {
    return board;
}
