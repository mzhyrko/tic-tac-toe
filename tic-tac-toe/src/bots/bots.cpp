#include "bots.hpp"
#include "../core/board.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <cstdint>
#include <chrono>
#include <random>
#include <omp.h>

using namespace std;

// Кэш для мемоизации минимакса
unordered_map<uint64_t, int> memo;
const size_t MAX_CACHE_SIZE = 10000000; // Увеличенный размер кэша

// Кэш для проверки победы
unordered_map<uint64_t, bool> winCache;

// Хэширование Зобриста
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

// Подсчет пустых клеток
int getEmptyCells(const vector<vector<string>>& board, int boardSize) {
    int count = 0;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") count++;
        }
    }
    return count;
}

// Получение кандидатов на ход (только незанятые клетки)
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

// Оптимизированная эвристическая оценка
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

// Оценка хода для сортировки
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

// Минимакс с альфа-бета отсечением и итеративным углублением
int minimax(vector<vector<string>>& board, int boardSize, int lineForWin,
            bool isMax, string botSign, string playerSign, int depth,
            int maxDepth, int alpha, int beta, const chrono::steady_clock::time_point& startTime) {
    static ZobristHash hasher(boardSize);
    uint64_t hash = hasher(board);

    if (depth > 0 && chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - startTime).count() > 10000) {
        return evaluateBoard(board, boardSize, botSign, playerSign, lineForWin);
    }

    if (winCache.count(hash ^ (botSign == "X" ? 1ULL : 2ULL))) {
        if (winCache[hash ^ (botSign == "X" ? 1ULL : 2ULL)]) return 100000 - depth;
    }
    if (winCache.count(hash ^ (playerSign == "X" ? 1ULL : 2ULL))) {
        if (winCache[hash ^ (playerSign == "X" ? 1ULL : 2ULL)]) return depth - 100000;
    }

    if (checkWin(board, boardSize, botSign, lineForWin)) {
        winCache[hash ^ (botSign == "X" ? 1ULL : 2ULL)] = true;
        return 100000 - depth;
    }
    if (checkWin(board, boardSize, playerSign, lineForWin)) {
        winCache[hash ^ (playerSign == "X" ? 1ULL : 2ULL)] = true;
        return depth - 100000;
    }
    if (checkDraw(board, boardSize)) {
        return 0;
    }
    if (depth >= maxDepth) {
        return evaluateBoard(board, boardSize, botSign, playerSign, lineForWin);
    }

    if (memo.count(hash)) return memo[hash];

    vector<pair<int, int>> candidates = getCandidateMoves(board, boardSize, lineForWin, botSign, playerSign);
    if (candidates.empty()) return evaluateBoard(board, boardSize, botSign, playerSign, lineForWin);

    vector<pair<int, pair<int, int>>> scoredCandidates;
    for (const auto& [i, j] : candidates) {
        scoredCandidates.emplace_back(scoreMove(board, boardSize, i, j, isMax ? botSign : playerSign, lineForWin), make_pair(i, j));
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
                    original = board[i][j];
                    board[i][j] = botSign;
                }
            }
            if (!cancel) {
                val = minimax(board, boardSize, lineForWin, false,
                              botSign, playerSign, depth + 1, maxDepth, alpha, beta, startTime);
                #pragma omp critical
                {
                    if (!cancel) {
                        board[i][j] = original;
                        best = max(best, val);
                        alpha = max(alpha, best);
                        if (beta <= alpha) {
                            cancel = true;
                        }
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
                    original = board[i][j];
                    board[i][j] = playerSign;
                }
            }
            if (!cancel) {
                val = minimax(board, boardSize, lineForWin, true,
                              botSign, playerSign, depth + 1, maxDepth, alpha, beta, startTime);
                #pragma omp critical
                {
                    if (!cancel) {
                        board[i][j] = original;
                        best = min(best, val);
                        beta = min(beta, best);
                        if (beta <= alpha) {
                            cancel = true;
                        }
                    }
                }
            }
        }
    }

    if (memo.size() < MAX_CACHE_SIZE) {
        memo[hash] = best;
    }
    return best;
}

// Случайный бот
void randomBot(int boardSize, string &inpStr, [[maybe_unused]] string currSign, vector<vector<string>> &board) {
    static mt19937 gen(random_device{}());
    vector<int> validMoves;
    for (int i = 1; i <= boardSize * boardSize; ++i) {
        if (isValidMove(board, boardSize, i)) {
            validMoves.push_back(i);
        }
    }
    if (!validMoves.empty()) {
        inpStr = to_string(validMoves[gen() % validMoves.size()]);
    }
}

// Бот начального уровня
void beginnerBot(string &inpStr, string currSign, int boardSize, int lineForWin, vector<vector<string>> &board) {
    for (int i = 1; i <= boardSize * boardSize; ++i) {
        if (isValidMove(board, boardSize, i)) {
            insertMove(board, boardSize, i, currSign);
            if (checkWin(board, boardSize, currSign, lineForWin)) {
                inpStr = to_string(i);
                return;
            }
            insertMove(board, boardSize, i, "0" + to_string(i));
        }
    }
    string opponent = (currSign == "X") ? "O" : "X";
    for (int i = 1; i <= boardSize * boardSize; ++i) {
        if (isValidMove(board, boardSize, i)) {
            insertMove(board, boardSize, i, opponent);
            if (checkWin(board, boardSize, opponent, lineForWin)) {
                inpStr = to_string(i);
                insertMove(board, boardSize, i, currSign);
                return;
            }
            insertMove(board, boardSize, i, "0" + to_string(i));
        }
    }
    randomBot(boardSize, inpStr, currSign, board);
}

// Улучшенный непобедимый бот
void dpTTToeBot(vector<vector<string>>& board, int boardSize, int lineForWin,
                string botSign, string& inpStr, int maxDepth) {
    static mt19937 gen(random_device{}());
    if (memo.size() > MAX_CACHE_SIZE) memo.clear();
    if (winCache.size() > MAX_CACHE_SIZE) winCache.clear();

    // Динамическая настройка глубины
    int dynamicMaxDepth;
    if (boardSize == 3) {
        dynamicMaxDepth = 9; // Полное дерево для 3x3
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
        dynamicMaxDepth = 69; // Для больших досок
    }

    // Проверка пустой доски
    bool isBoardEmpty = getEmptyCells(board, boardSize) == boardSize * boardSize;

    // Первый ход: приоритет центру
    if (isBoardEmpty) {
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
            if (isValidMove(board, boardSize, move)) {
                centerMoves.push_back(move);
            }
        }
        if (!centerMoves.empty()) {
            inpStr = to_string(centerMoves[gen() % centerMoves.size()]);
            //cout << "Улучшенный бот " << botSign << " выбрал центральный ход: " << inpStr << endl;
            return;
        }
    }

    // Проверка немедленной победы
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") {
                string original = board[i][j];
                board[i][j] = botSign;
                if (checkWin(board, boardSize, botSign, lineForWin)) {
                    inpStr = to_string(i * boardSize + j + 1);
                    board[i][j] = original;
                    //cout << "Улучшенный бот " << botSign << " выбрал выигрышный ход: " << inpStr << endl;
                    return;
                }
                board[i][j] = original;
            }
        }
    }

    // Блокировка победы противника
    string playerSign = (botSign == "X") ? "O" : "X";
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") {
                string original = board[i][j];
                board[i][j] = playerSign;
                if (checkWin(board, boardSize, playerSign, lineForWin)) {
                    board[i][j] = original;
                    inpStr = to_string(i * boardSize + j + 1);
                    //cout << "Улучшенный бот " << botSign << " выбрал блокирующий ход: " << inpStr << endl;
                    return;
                }
                board[i][j] = original;
            }
        }
    }

    // Получение кандидатов на ход
    vector<pair<int, int>> candidates = getCandidateMoves(board, boardSize, lineForWin, botSign, playerSign);
    if (candidates.empty()) {
        randomBot(boardSize, inpStr, botSign, board);
        //cout << "Улучшенный бот " << botSign << " выбрал случайный ход: " << inpStr << endl;
        return;
    }

    // Итеративное углубление с минимаксом
    auto startTime = chrono::steady_clock::now();
    vector<pair<int, pair<int, int>>> scoredCandidates;
    for (const auto& [i, j] : candidates) {
        scoredCandidates.emplace_back(scoreMove(board, boardSize, i, j, botSign, lineForWin), make_pair(i, j));
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
                    original = board[i][j];
                    board[i][j] = botSign;
                }
            }
            if (!cancel) {
                moveVal = minimax(board, boardSize, lineForWin, false,
                                  botSign, playerSign, 0, depth, -1000000, 1000000, startTime);
                #pragma omp critical
                {
                    if (!cancel) {
                        board[i][j] = original;
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
            //cout << "Улучшенный бот " << botSign << " выбрал минимакс ход: " << inpStr << " (оценка: " << bestVal << ")" << endl;
            return;
        }
    }
}