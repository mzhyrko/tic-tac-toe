#pragma once

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <cstdint> // Добавлено для uint64_t

using namespace std;

// Random move bot
void randomBot(int boardSize, string &inpStr, string currSign, vector<vector<string>> &board);

// Beginner бот (использует эвристическое оценивание)
void beginnerBot(string &inpStr, string currSign, int boardSize, int lineForWin, vector<vector<string>> &board);

// Advanced бот (использует минимакс с итеративным углублением)
void dpTTToeBot(vector<vector<string>> &board, int boardSize, int lineForWin, string botSign, string &inpStr, int maxDepth);

// Алгоритм минимакса для продвинутого бота
int minimax(vector<vector<string>> &board, int boardSize, int lineForWin, bool isMax, string botSign, string playerSign, int depth, int maxDepth, int alpha, int beta);

// Получение кандидатов на ходы (только клетки рядом с занятыми)
vector<pair<int, int>> getCandidateMoves(const vector<vector<string>>& board, int boardSize);

// Эвристическая оценка доски
int evaluateBoard(const vector<vector<string>>& board, int boardSize, string botSign, string playerSign, int lineForWin);

// Оценка хода для сортировки кандидатов
int scoreMove(const vector<vector<string>>& board, int boardSize, int i, int j, string currSign);

// Zobrist hashing для доски
struct ZobristHash {
    vector<vector<uint64_t>> xKeys;
    vector<vector<uint64_t>> oKeys;
    vector<vector<uint64_t>> emptyKeys;
    ZobristHash(int boardSize);
    uint64_t operator()(const vector<vector<string>>& board) const;
};