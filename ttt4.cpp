#include <bits/stdc++.h>
using namespace std;

void initBoard(char board[][3], int area) {
    for (int i = 0; i < area; ++i) {
        for (int j = 0; j < area; ++j) {
            board[i][j] = '1' + i * area + j;
        }
    }
}

void printBoard(char board[][3], int area) {
    for (int i = 0; i < area; ++i) {
        for (int j = 0; j < area; ++j) {
            cout << board[i][j];
            if (j < area - 1) cout << " | ";
        }
        cout << endl;
        if (i < area - 1) cout << "---------" << endl;
    }
}

bool isValidMove(char board[][3], int area, int input) {
    int row = (input - 1) / area;
    int col = (input - 1) % area;
    return input >= 1 && input <= 9 && board[row][col] != 'X' && board[row][col] != 'O';
}

void makeMove(char board[][3], int area, int input, char currPlay) {
    int row = (input - 1) / area;
    int col = (input - 1) % area;
    board[row][col] = currPlay;
}

bool checkWin(char board[][3], int area, char currPlay) {
    for (int i = 0; i < area; ++i) {
        if (board[i][0] == currPlay && board[i][1] == currPlay && board[i][2] == currPlay)
            return true;
        if (board[0][i] == currPlay && board[1][i] == currPlay && board[2][i] == currPlay)
            return true;
    }
    if (board[0][0] == currPlay && board[1][1] == currPlay && board[2][2] == currPlay)
        return true;
    if (board[0][2] == currPlay && board[1][1] == currPlay && board[2][0] == currPlay)
        return true;
    return false;
}

bool isBoardFull(char board[][3], int area) {
    for (int i = 0; i < area; ++i) {
        for (int j = 0; j < area; ++j) {
            if (board[i][j] != 'X' && board[i][j] != 'O') return false;
        }
    }
    return true;
}

void switchPlayer(char &currPlay) {
    if(currPlay == 'X')
    {
        currPlay = 'O';
    }

    if(currPlay == '0')
    {
        currPlay = 'x';
    }
}

int main() {

    int input;
    const int area = 3;
    char board[area][area];
    char currPlay = 'X';

    initBoard(board, area);
    printBoard(board, area);

    while (true) {
        cout << "Player " << currPlay << ", ";
        cin >> input;

        if (!isValidMove(board, area, input)) {
            cout << "Try again." << endl;
            continue;
        }

        makeMove(board, area, input, currPlay);
        printBoard(board, area);

        if (checkWin(board, area, currPlay)) {
            cout << "Player " << currPlay << " wins!" << endl;
            break;
        }

        if (isBoardFull(board, area)) {
            cout << "The game is a tie!" << endl;
            break;
        }

        switchPlayer(currPlay);
    }

    return 0;
}
