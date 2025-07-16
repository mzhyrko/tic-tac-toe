#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>

void game();

void initBoardSize(int &boardSize, int &lineForWin);
void changeSign(std::string &currSign);

#endif // GAME_HPP