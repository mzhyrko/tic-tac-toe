#include "bots.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include <chrono>
#include <random>
#include <cmath>
#include <exception>
#include <omp.h>

using namespace std;

// Cash for minimax memoization
unordered_map<uint64_t, int> memo;
const size_t MAX_CACHE_SIZE = 10000000;
unordered_map<uint64_t, bool> winCache;

// Implementation of ZobristHash methods
ZobristHash::ZobristHash(int boardSize) {
    srand(time(0));
    xKeys = vector<vector<uint64_t>>(boardSize, vector<uint64_t>(boardSize));
    oKeys = vector<vector<uint64_t>>(boardSize, vector<uint64_t>(boardSize));
    emptyKeys = vector<vector<uint64_t>>(boardSize, vector<uint64_t>(boardSize));
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            xKeys[i][j] = ((uint64_t)rand() << 32) | rand();
            oKeys[i][j] = ((uint64_t)rand() << 32) | rand();
            emptyKeys[i][j] = ((uint64_t)rand() << 32) | rand();
        }
    }
}

uint64_t ZobristHash::operator()(const vector<vector<string>>& board) const {
    uint64_t hash = 0;
    for (size_t i = 0; i < board.size(); ++i) {
        for (size_t j = 0; j < board[i].size(); ++j) {
            if (board[i][j] == "X") hash ^= xKeys[i][j];
            else if (board[i][j] == "O") hash ^= oKeys[i][j];
            else hash ^= emptyKeys[i][j];
        }
    }
    return hash;
}

int getEmptyCells(const vector<vector<string>>& board, int boardSize) {
    int count = 0;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") count++;
        }
    }
    return count;
}

vector<pair<int, int>> getCandidateMoves(const vector<vector<string>>& board, int boardSize, int lineForWin, string botSign, string playerSign) {
    vector<pair<int, int>> candidates;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") {
                bool hasNeighbor = false;
                int radius = (getEmptyCells(board, boardSize) < boardSize * boardSize / 2) ? 1 : 2;
                for (int di = -radius; di <= radius && !hasNeighbor; ++di) {
                    for (int dj = -radius; dj <= radius; ++dj) {
                        int ni = i + di, nj = j + dj;
                        if (ni >= 0 && ni < boardSize && nj >= 0 && nj < boardSize &&
                            (board[ni][nj] == "X" || board[ni][nj] == "O")) {
                            hasNeighbor = true;
                            break;
                        }
                    }
                }
                bool isCritical = false;
                for (int di = -1; di <= 1 && !isCritical; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        if (di == 0 && dj == 0) continue;
                        int botCount = 0, playerCount = 0, emptyCount = 0;
                        for (int k = -lineForWin + 1; k < lineForWin; ++k) {
                            int ni = i + k * di, nj = j + k * dj;
                            if (ni >= 0 && ni < boardSize && nj >= 0 && nj < boardSize) {
                                if (board[ni][nj] == botSign) botCount++;
                                else if (board[ni][nj] == playerSign) playerCount++;
                                else emptyCount++;
                            }
                        }
                        if ((botCount >= lineForWin - 2 && playerCount == 0) ||
                            (playerCount >= lineForWin - 3 && emptyCount >= 3)) {
                            isCritical = true;
                        }
                    }
                }
                if (hasNeighbor || isCritical) candidates.emplace_back(i, j);
                else if (boardSize >= 10 && getEmptyCells(board, boardSize) > boardSize * boardSize - 10 &&
                         i >= boardSize/3 && i < 2*boardSize/3 && j >= boardSize/3 && j < 2*boardSize/3) {
                    candidates.emplace_back(i, j);
                }
            }
        }
    }
    return candidates;
}

// Optimized evristic evaluation
int evaluateBoard(const vector<vector<string>>& board, int boardSize, string botSign, string playerSign, int lineForWin) {
    int score = 0;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue;
                    int botCount = 0, playerCount = 0, emptyCount = 0;
                    bool botBlocked = false, playerBlocked = false;
                    for (int k = 0; k < lineForWin; ++k) {
                        int ni = i + k * di, nj = j + k * dj;
                        if (ni >= 0 && ni < boardSize && nj >= 0 && nj < boardSize) {
                            if (board[ni][nj] == botSign) botCount++;
                            else if (board[ni][nj] == playerSign) playerCount++;
                            else emptyCount++;
                        } else {
                            botBlocked = true;
                            playerBlocked = true;
                            break;
                        }
                    }
                    if (botCount > 0 && playerCount == 0 && !botBlocked) {
                        if (botCount == lineForWin - 1 && emptyCount == 1) score += 100000;
                        else if (botCount == lineForWin - 2 && emptyCount == 2) score += 20000;
                        else if (botCount == lineForWin - 3 && emptyCount == 3) score += 1000;
                        else score += botCount * botCount * 100;
                    }
                    if (playerCount > 0 && botCount == 0 && !playerBlocked) {
                        if (playerCount == lineForWin - 1 && emptyCount == 1) score -= 100000;
                        else if (playerCount == lineForWin - 2 && emptyCount == 2) score -= 20000;
                        else if (playerCount == lineForWin - 3 && emptyCount == 3) score -= 1000;
                        else score -= playerCount * playerCount * 100;
                    }
                }
            }
        }
    }
    return score;
}

// Move evaluation for sorting
int scoreMove(const vector<vector<string>>& board, int boardSize, int i, int j, string currSign, int lineForWin) {
    int score = 0;
    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0) continue;
            int currCount = 0, emptyCount = 0;
            for (int k = 0; k < lineForWin; ++k) {
                int ni = i + k * di, nj = j + k * dj;
                if (ni >= 0 && ni < boardSize && nj >= 0 && nj < boardSize) {
                    if (board[ni][nj] == currSign) currCount++;
                    else if (board[ni][nj] != (currSign == "X" ? "O" : "X")) emptyCount++;
                }
            }
            if (currCount + emptyCount >= lineForWin) {
                if (currCount >= lineForWin - 1) score += 100000;
                else if (currCount >= lineForWin - 2) score += 20000;
                else if (currCount >= lineForWin - 3) score += 1000;
                else score += currCount * currCount * 100;
            }
        }
    }
    if (boardSize >= 10 && i >= boardSize/3 && i < 2*boardSize/3 && j >= boardSize/3 && j < 2*boardSize/3) {
        score += 50;
    }
    return score;
}

// Minimax with alpha-beta pruning
int minimax(Board& board, int lineForWin, bool isMax, string botSign, string playerSign, int depth,
            int maxDepth, int alpha, int beta, const chrono::steady_clock::time_point& startTime) {
    static ZobristHash hasher(board.getBoardSize());
    uint64_t hash = hasher(board.getBoardState());

    if (depth > 0 && chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - startTime).count() > 30000) {
        return evaluateBoard(board.getBoardState(), board.getBoardSize(), botSign, playerSign, lineForWin);
    }

    if (winCache.count(hash ^ (botSign == "X" ? 1ULL : 2ULL))) {
        if (winCache[hash ^ (botSign == "X" ? 1ULL : 2ULL)]) return 100000 - depth;
    }
    if (winCache.count(hash ^ (playerSign == "X" ? 1ULL : 2ULL))) {
        if (winCache[hash ^ (playerSign == "X" ? 1ULL : 2ULL)]) return depth - 100000;
    }

    if (board.checkWin(botSign)) {
        winCache[hash ^ (botSign == "X" ? 1ULL : 2ULL)] = true;
        return 100000 - depth;
    }
    if (board.checkWin(playerSign)) {
        winCache[hash ^ (playerSign == "X" ? 1ULL : 2ULL)] = true;
        return depth - 100000;
    }
    if (getEmptyCells(board.getBoardState(), board.getBoardSize()) == 0) {
        return 0;
    }
    if (depth >= maxDepth) {
        return evaluateBoard(board.getBoardState(), board.getBoardSize(), botSign, playerSign, lineForWin);
    }

    if (memo.count(hash)) return memo[hash];

    auto candidates = getCandidateMoves(board.getBoardState(), board.getBoardSize(), board.getLineForWin(), botSign, (botSign == "X" ? "O" : "X"));
    
    if (candidates.empty()) 
        return evaluateBoard(board.getBoardState(), board.getBoardSize(), botSign, playerSign, lineForWin);

    vector<pair<int, pair<int, int>>> scoredCandidates;
    for (const auto& [i, j] : candidates) {
        scoredCandidates.emplace_back(scoreMove(board.getBoardState(), board.getBoardSize(), i, j, isMax ? botSign : playerSign, lineForWin), make_pair(i, j));
    }
    sort(scoredCandidates.rbegin(), scoredCandidates.rend());

    int best;
    bool cancel = false;
    if (isMax) {
        best = -1000000;
        #pragma omp parallel for shared(best, alpha, beta, cancel)
        for (size_t idx = 0; idx < scoredCandidates.size() && !cancel; ++idx) {
            auto [_, pos] = scoredCandidates[idx];
            auto [i, j] = pos;
            string original;
            int val;
            #pragma omp critical
            {
                if (!cancel) {
                    original = board.getCell(i, j);
                    board.setCell(i, j, botSign);
                }
            }
            if (!cancel) {
                val = minimax(board, lineForWin, false, botSign, playerSign, depth + 1, maxDepth, alpha, beta, startTime);
                #pragma omp critical
                {
                    if (!cancel) {
                        board.setCell(i, j, original);
                        best = max(best, val);
                        alpha = max(alpha, best);
                        if (beta <= alpha) cancel = true;
                    }
                }
            }
        }
    } else {
        best = 1000000;
        #pragma omp parallel for shared(best, alpha, beta, cancel)
        for (size_t idx = 0; idx < scoredCandidates.size() && !cancel; ++idx) {
            auto [_, pos] = scoredCandidates[idx];
            auto [i, j] = pos;
            string original;
            int val;
            #pragma omp critical
            {
                if (!cancel) {
                    original = board.getCell(i, j);
                    board.setCell(i, j, playerSign);
                }
            }
            if (!cancel) {
                val = minimax(board, lineForWin, true, botSign, playerSign, depth + 1, maxDepth, alpha, beta, startTime);
                #pragma omp critical
                {
                    if (!cancel) {
                        board.setCell(i, j, original);
                        best = min(best, val);
                        beta = min(beta, best);
                        if (beta <= alpha) cancel = true;
                    }
                }
            }
        }
    }

    if (memo.size() < MAX_CACHE_SIZE) memo[hash] = best;
    return best;
}

// Auxiliary function
void dpTTToeBotImpl(Board& board, string botSign, string& inpStr, int maxDepth) {
    static mt19937 gen(random_device{}());
    int boardSize = board.getBoardSize();
    int lineForWin = board.getLineForWin();

    if (memo.size() > MAX_CACHE_SIZE) memo.clear();
    if (winCache.size() > MAX_CACHE_SIZE) winCache.clear();

    int dynamicMaxDepth = maxDepth;
    if (boardSize == 3) {
        dynamicMaxDepth = 9;
    } else if (boardSize <= 5) {
        dynamicMaxDepth = 19;
    } else if (boardSize <= 7) {
        dynamicMaxDepth = 29;
    } else if (boardSize <= 10) {
        dynamicMaxDepth = 39;
    } else if (boardSize <= 14) {
        dynamicMaxDepth = 49;
    } else if (boardSize <= 20) {
        dynamicMaxDepth = 59;
    } else {
        dynamicMaxDepth = 69; 
    }

    // First Move: center priority
    if (getEmptyCells(board.getBoardState(), board.getBoardSize()) == boardSize * boardSize) {
        vector<int> centerMoves;
        int mid = boardSize / 2;
        vector<pair<int, int>> centers = {{mid, mid}};
        if (boardSize % 2 == 0) {
            centers.push_back({mid - 1, mid - 1});
            centers.push_back({mid - 1, mid});
            centers.push_back({mid, mid - 1});
        }
        for (const auto& [r, c] : centers) {
            int move = r * boardSize + c + 1;
            int row = r, col = c;
            if (board.getCell(row, col) != "X" && board.getCell(row, col) != "O") centerMoves.push_back(move);
        }
        if (!centerMoves.empty()) {
            inpStr = to_string(centerMoves[gen() % centerMoves.size()]);
            return;
        }
    }

    // Check immediate win
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board.getCell(i, j) != "X" && board.getCell(i, j) != "O") {
                string original = board.getCell(i, j);
                board.setCell(i, j, botSign);
                if (board.checkWin(botSign)) {
                    inpStr = to_string(i * boardSize + j + 1);
                    board.setCell(i, j, original);
                    return;
                }
                board.setCell(i, j, original);
            }
        }
    }

    // Block opponent's win
    string playerSign = (botSign == "X") ? "O" : "X";
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board.getCell(i, j) != "X" && board.getCell(i, j) != "O") {
                string original = board.getCell(i, j);
                board.setCell(i, j, playerSign);
                if (board.checkWin(playerSign)) {
                    board.setCell(i, j, original);
                    inpStr = to_string(i * boardSize + j + 1);
                    return;
                }
                board.setCell(i, j, original);
            }
        }
    }

    auto candidates = getCandidateMoves(board.getBoardState(), board.getBoardSize(), board.getLineForWin(), botSign, (botSign == "X" ? "O" : "X"));
    if (candidates.empty()) {
        // Using randomBot as a fallback
        vector<int> available = board.getAvailableMoves();
        if (available.empty()) {
            inpStr = "-1";
            return;
        }
        inpStr = to_string(available[gen() % available.size()]);
        return;
    }

    auto startTime = chrono::steady_clock::now();
    vector<pair<int, pair<int, int>>> scoredCandidates;
    for (const auto& [i, j] : candidates) {
        scoredCandidates.emplace_back(scoreMove(board.getBoardState(), board.getBoardSize(), i, j, botSign, lineForWin), make_pair(i, j));
    }
    sort(scoredCandidates.rbegin(), scoredCandidates.rend());

    int bestVal = -1000000;
    vector<pair<int, int>> bestMoves;
    bool cancel = false;
    for (int depth = 1; depth <= dynamicMaxDepth && !cancel; ++depth) {
        bestVal = -1000000;
        bestMoves.clear();
        #pragma omp parallel for shared(bestVal, bestMoves, cancel)
        for (size_t idx = 0; idx < scoredCandidates.size() && !cancel; ++idx) {
            auto [_, pos] = scoredCandidates[idx];
            auto [i, j] = pos;
            string original;
            int moveVal;
            #pragma omp critical
            {
                if (!cancel) {
                    original = board.getCell(i, j);
                    board.setCell(i, j, botSign);
                }
            }
            if (!cancel) {
                moveVal = minimax(board, lineForWin, false, botSign, playerSign, 0, depth, -1000000, 1000000, startTime);
                #pragma omp critical
                {
                    if (!cancel) {
                        board.setCell(i, j, original);
                        if (moveVal > bestVal) {
                            bestVal = moveVal;
                            bestMoves = {{i, j}};
                        } else if (moveVal == bestVal) {
                            bestMoves.emplace_back(i, j);
                        }
                    }
                }
            }
            if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - startTime).count() > 30000) {
                #pragma omp critical
                {
                    cancel = true;
                }
            }
        }
        if (!bestMoves.empty() && 
            chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - startTime).count() <= 30000) {
            auto [bestRow, bestCol] = bestMoves[gen() % bestMoves.size()];
            inpStr = to_string(bestRow * boardSize + bestCol + 1);
            return;
        }
    }
    
    // If no candidates found, return first candidate
    if (!scoredCandidates.empty()) {
        auto [i, j] = scoredCandidates[0].second;
        inpStr = to_string(i * boardSize + j + 1);
    } else {
        // Using randomBot as a fallback
        vector<int> available = board.getAvailableMoves();
        if (available.empty()) {
            inpStr = "-1";
            return;
        }
        inpStr = to_string(available[gen() % available.size()]);
    }
}

// The main bot function (corresponds to the declaration in bots.hpp)
int dpTTToeBot(const Board& board, const string& botSign, int maxDepth) {
    // Create a copy of the board for manipulation
    Board tempBoard = board;
    string moveStr;
    
    // (dpTTToeBotImpl is the implementation of the bot logic)
    dpTTToeBotImpl(tempBoard, botSign, moveStr, maxDepth);
    
    try {
        return stoi(moveStr);
    } catch (...) {
        // In case of error, return a random move
        vector<int> available = board.getAvailableMoves();
        if (available.empty()) return -1;
        static mt19937 gen(random_device{}());
        return available[gen() % available.size()];
    }
}