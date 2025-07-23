#pragma once

#include <vector>
#include <string>

using namespace std;

// Board functions
void initBoard(vector<vector<string>>& board, int boardSize);
void printBoard(const vector<vector<string>>& board, int boardSize, int lineForWin);
bool isValidMove(const vector<vector<string>>& board, int boardSize, int input);
void insertMove(vector<vector<string>>& board, int boardSize, int input, string currSign);
bool checkWin(const vector<vector<string>>& board, int boardSize, string currSign, int lineForWin);
bool isBoardFull(const vector<vector<string>>& board, int boardSize);
bool checkDraw(vector<vector<string>>& board, int boardSize);

// Helper functions
bool columnsCheckWin(const vector<vector<string>>& board, int boardSize, string currSign, int lineForWin, int i, int j);
bool rowsCheckWin(const vector<vector<string>>& board, int boardSize, string currSign, int lineForWin, int i, int j);
bool mainDiagonalCheckWin(const vector<vector<string>>& board, int boardSize, string currSign, int lineForWin, int i, int j);
bool antiDiagonalCheckWin(const vector<vector<string>>& board, int boardSize, string currSign, int lineForWin, int i, int j);