#include "player.hpp"

#include <iostream>
#include <string>
#include <vector>
#include "../bots/bots.hpp"

using namespace std;

void selectGameMode(int &selectMode, string &firstOpponent, string &secondOpponent) {
    int localInput;
    cout << "Game modes:" << endl;
    cout << "1 - Human vs Human" << endl;
    cout << "2 - Human vs Bot" << endl;
    cout << "3 - Bot vs Bot" << endl;
    cout << "Select game mode: ";
    cin >> selectMode;

    if (selectMode == 1) {
        firstOpponent = "man";
    } else if (selectMode == 2) {
        firstOpponent = "man";
        cout << "Select bot type: " << endl;
        cout << "1 - Random bot" << endl;
        cout << "2 - Beginner bot" << endl;
        cout << "3 - Advanced bot" << endl;
        cin >> localInput;
        secondOpponent = (localInput == 1) ? "rbot" :
                         (localInput == 2) ? "bbot" : "abot";
    } else if (selectMode == 3) {
        cout << "Select first bot:" << endl;
        cout << "1 - Random bot" << endl;
        cout << "2 - Beginner bot" << endl;
        cout << "3 - Advanced bot" << endl;
        cin >> localInput;
        firstOpponent = (localInput == 1) ? "rbot" :
                        (localInput == 2) ? "bbot" : "abot";

        cout << "Select second bot:" << endl;
        cout << "1 - Random bot" << endl;
        cout << "2 - Beginner bot" << endl;
        cout << "3 - Advanced bot" << endl;
        cin >> localInput;
        secondOpponent = (localInput == 1) ? "rbot" :
                         (localInput == 2) ? "bbot" : "abot";
    }
}

void changePlayer(int &selectMode, string &firstOpponent, string &secondOpponent) {
    swap(firstOpponent, secondOpponent);
}

bool isNumber(string inpStr) {
    for (char c : inpStr) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

void makeMove(string &inpStr, string firstOpponent, string currSign, int boardSize,
              int lineForWin, vector<vector<string>> &board, int maxDepth) {
    if (firstOpponent == "rbot") {
        randomBot(boardSize, inpStr, currSign, board);
        cout << "Random bot " << currSign << " entered move: " << inpStr << endl;
    } else if (firstOpponent == "bbot") {
        beginnerBot(inpStr, currSign, boardSize, lineForWin, board);
        cout << "Beginner bot " << currSign << " entered move: " << inpStr << endl;
    } else if (firstOpponent == "abot") {
        dpTTToeBot(board, boardSize, lineForWin, currSign, inpStr, maxDepth);
        cout << "Advanced bot " << currSign << " entered move: " << inpStr << endl;
    } else {
        cout << "Player " << currSign << ", enter your move: ";
        cin >> inpStr;
    }
}