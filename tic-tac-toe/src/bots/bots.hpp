#ifndef BOTS_HPP
#define BOTS_HPP

#include <vector>
#include <string>

using namespace std;

void randomBot(int boardSize, string &inpStr, string currSign, vector<vector<string>> &board);
void beginnerBot(string &inpStr, string currSign, int boardSize, int lineForWin, vector<vector<string>> &board);
void dpTTToeBot(vector<vector<string>> &board, int boardSize, int lineForWin, string botSign, string &inpStr, int maxDepth);
int minimax(vector<vector<string>> &board, int boardSize, int lineForWin, bool isMax, string botSign, string playerSign, int depth, int maxDepth);

#endif // BOTS_HPP