#include "bots.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include "../core/board.hpp"

using namespace std;

void randomBot(int boardSize, string &inpStr, string currSign, vector<vector<string>> &board) {
    srand(time(0));
    int r;
    do {
        r = rand() % (boardSize * boardSize) + 1;
    } while (!isValidMove(board, boardSize, r));
    inpStr = to_string(r);
}

void beginnerBot(string &inpStr, string currSign, int boardSize, int lineForWin, vector<vector<string>> &board) {
    int maxScore = -1, bestMove[2] = {-1, -1};
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") {
                int score = 0;
                for (int k = 0; k < boardSize; ++k) {
                    if (board[i][k] == currSign) score++;
                    if (board[k][j] == currSign) score++;
                }
                for (int k = 0; k < boardSize; ++k) {
                    if (i + k < boardSize && j + k < boardSize && board[i + k][j + k] == currSign) score++;
                    if (i + k < boardSize && j - k >= 0 && board[i + k][j - k] == currSign) score++;
                }
                if (score > maxScore) {
                    maxScore = score;
                    bestMove[0] = i;
                    bestMove[1] = j;
                }
            }
        }
    }
    if (bestMove[0] != -1 && bestMove[1] != -1) {
        inpStr = to_string(bestMove[0] * boardSize + bestMove[1] + 1);
    } else {
        randomBot(boardSize, inpStr, currSign, board);
    }
}

int minimax(vector<vector<string>> &board, int boardSize, int lineForWin, bool isMax, string botSign, string playerSign, int depth, int maxDepth) {
    if (checkWin(board, boardSize, botSign, lineForWin)) return 10 - depth;
    if (checkWin(board, boardSize, playerSign, lineForWin)) return depth - 10;
    if (checkDraw(board, boardSize)) return 0;

    if (depth >= maxDepth) return 0;

    int best;
    if (isMax) {
        best = -1000;
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (board[i][j] != "X" && board[i][j] != "O") {
                    board[i][j] = botSign;
                    best = max(best, minimax(board, boardSize, lineForWin, !isMax, botSign, playerSign, depth + 1, maxDepth));
                    board[i][j] = to_string(i * boardSize + j + 1);
                }
            }
        }
    } else {
        best = 1000;
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (board[i][j] != "X" && board[i][j] != "O") {
                    board[i][j] = playerSign;
                    best = min(best, minimax(board, boardSize, lineForWin, !isMax, botSign, playerSign, depth + 1, maxDepth));
                    board[i][j] = to_string(i * boardSize + j + 1);
                }
            }
        }
    }
    return best;
}

void dpTTToeBot(vector<vector<string>> &board, int boardSize, int lineForWin, string botSign, string &inpStr, int maxDepth) {
    int bestVal = -1000, bestMove[2] = {-1, -1};
    string playerSign = (botSign == "X") ? "O" : "X";

    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") {
                board[i][j] = playerSign;
                if (checkWin(board, boardSize, playerSign, lineForWin)) {
                    board[i][j] = botSign;
                    inpStr = to_string(i * boardSize + j + 1);
                    return;
                }
                board[i][j] = to_string(i * boardSize + j + 1);
            }
        }
    }

    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") {
                board[i][j] = botSign;
                int moveVal = minimax(board, boardSize, lineForWin, false, botSign, playerSign, 0, maxDepth);
                board[i][j] = to_string(i * boardSize + j + 1);
                if (moveVal > bestVal) {
                    bestVal = moveVal;
                    bestMove[0] = i;
                    bestMove[1] = j;
                }
            }
        }
    }

    if (bestMove[0] == -1) {
        randomBot(boardSize, inpStr, botSign, board);
    } else {
        inpStr = to_string(bestMove[0] * boardSize + bestMove[1] + 1);
    }
}