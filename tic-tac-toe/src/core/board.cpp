#include "board.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

void initBoard(vector<vector<string>>& board, int boardSize) {
    board = vector<vector<string>>(boardSize, vector<string>(boardSize));
    int numDigits = to_string(boardSize * boardSize).length();
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            int num = i * boardSize + j + 1;
            string numStr = to_string(num);
            while ((int)numStr.length() < numDigits) {
                numStr = "0" + numStr;
            }
            board[i][j] = numStr;
        }
    }
}

void printBoard(const vector<vector<string>>& board, int boardSize, int lineForWin) {
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
}

bool isValidMove(const vector<vector<string>>& board, int boardSize, int input) {
    if (input < 1 || input > boardSize * boardSize) return false;
    int row = (input - 1) / boardSize;
    int col = (input - 1) % boardSize;
    return board[row][col] != "X" && board[row][col] != "O";
}

void insertMove(vector<vector<string>>& board, int boardSize, int input, string currSign) {
    int row = (input - 1) / boardSize;
    int col = (input - 1) % boardSize;
    board[row][col] = currSign;
}

bool columnsCheckWin(const vector<vector<string>>& board, int boardSize, string currSign, int lineForWin, int i, int j) {
    if (i + lineForWin > boardSize) return false;
    for (int x = 0; x < lineForWin; ++x) {
        if (board[i + x][j] != currSign) return false;
    }
    return true;
}

bool rowsCheckWin(const vector<vector<string>>& board, int boardSize, string currSign, int lineForWin, int i, int j) {
    if (j + lineForWin > boardSize) return false;
    for (int y = 0; y < lineForWin; ++y) {
        if (board[i][j + y] != currSign) return false;
    }
    return true;
}

bool mainDiagonalCheckWin(const vector<vector<string>>& board, int boardSize, string currSign, int lineForWin, int i, int j) {
    if (i + lineForWin > boardSize || j + lineForWin > boardSize) return false;
    for (int x = 0; x < lineForWin; ++x) {
        if (board[i + x][j + x] != currSign) return false;
    }
    return true;
}

bool antiDiagonalCheckWin(const vector<vector<string>>& board, int boardSize, string currSign, int lineForWin, int i, int j) {
    if (i + lineForWin > boardSize || j < lineForWin - 1) return false;
    for (int x = 0; x < lineForWin; ++x) {
        if (board[i + x][j - x] != currSign) return false;
    }
    return true;
}

bool checkWin(const vector<vector<string>>& board, int boardSize, string currSign, int lineForWin) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (columnsCheckWin(board, boardSize, currSign, lineForWin, i, j)) return true;
            if (rowsCheckWin(board, boardSize, currSign, lineForWin, i, j)) return true;
            if (mainDiagonalCheckWin(board, boardSize, currSign, lineForWin, i, j)) return true;
            if (antiDiagonalCheckWin(board, boardSize, currSign, lineForWin, i, j)) return true;
        }
    }
    return false;
}

bool checkDraw(vector<vector<string>>& board, int boardSize) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") return false;
        }
    }
    return true;
}

bool isBoardFull(const vector<vector<string>>& board, int boardSize) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") return false;
        }
    }
    return true;
}