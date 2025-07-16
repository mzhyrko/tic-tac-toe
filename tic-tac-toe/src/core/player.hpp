#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>

using namespace std;

void selectGameMode(int &selectMode, string &firstOpponent, string &secondOpponent);
void changePlayer(int &selectMode, string &firstOpponent, string &secondOpponent);
bool isNumber(string inpStr);
void makeMove(string &inpStr, string firstOpponent, string currSign, int boardSize,
              int lineForWin, vector<vector<string>> &board, int maxDepth);
void changeSign(string &currSign);

#endif // PLAYER_HPP