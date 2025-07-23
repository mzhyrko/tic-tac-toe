#include "game.hpp"
#include "board.hpp"
#include "player.hpp"
#include "../bots/bots.hpp"

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

void initBoardSize(int &boardSize, int &lineForWin) {
    cout << "Board size (3 - 1024): " << endl;
    cin >> boardSize;
    if (boardSize < 3 || boardSize > 1024) {
        cout << "Invalid board size." << endl;
        return;
    }
    cout << "Line for win (3 - " << boardSize << "): " << endl;
    cin >> lineForWin;
    if (lineForWin < 3 || lineForWin > boardSize) {
        cout << "Invalid line for win. Using default value." << endl;
        if (boardSize <= 5) lineForWin = 3;
        else if (boardSize <= 14) lineForWin = 4;
        else lineForWin = 5;
    }
}

void changeSign(string &currSign) {
    currSign = (currSign == "X") ? "O" : "X";
}

void game() {
    int input;
    int boardSize;
    int lineForWin;
    int selectMode;
    int maxDepth;
    string currSign = "X";
    string inpStr;
    string firstOpponent = "man";
    string secondOpponent = "man";
    vector<vector<string>> board;

    initBoardSize(boardSize, lineForWin);
    if (boardSize < 3 || boardSize > 1024 || lineForWin < 3 || lineForWin > boardSize) {
        cout << "Invalid parameters. Exiting." << endl;
        return;
    }
    selectGameMode(selectMode, firstOpponent, secondOpponent);
    initBoard(board, boardSize);
    printBoard(board, boardSize, lineForWin);
    maxDepth = lineForWin; // Увеличено с lineForWin - 1

    while (true) {
        makeMove(inpStr, firstOpponent, currSign, boardSize, lineForWin, board, maxDepth);

        if (!isNumber(inpStr)) continue;

        input = stoi(inpStr);
        if (!isValidMove(board, boardSize, input)) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }

        insertMove(board, boardSize, input, currSign);
        printBoard(board, boardSize, lineForWin);

        if (checkWin(board, boardSize, currSign, lineForWin)) {
            cout << "Player " << currSign << " wins!" << endl;
            break;
        }

        if (isBoardFull(board, boardSize)) {
            cout << "The game is a tie!" << endl;
            break;
        }

        changeSign(currSign);
        changePlayer(selectMode, firstOpponent, secondOpponent);
    }
}