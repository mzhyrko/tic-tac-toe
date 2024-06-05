#include <bits/stdc++.h>

using namespace std;

void initBoard(vector<vector<char>> &board, int boardSize) {
    board.resize(boardSize, vector<char>(boardSize));
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            board[i][j] = char('1' + i * boardSize + j);
        }
    }
}

void printBoard(const vector<vector<char>> &board, int boardSize) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            cout << board[i][j];
            if (j < boardSize - 1) cout << " | ";
        }
        cout << endl;
        if (i < boardSize - 1) cout << string(boardSize * 4 - 3, '-') << endl;
    }
}

bool isValidMove(const vector<vector<char>> &board, int boardSize, int input) {
    int row = (input - 1) / boardSize;
    int col = (input - 1) % boardSize;
    return input >= 1 && input <= boardSize * boardSize && board[row][col] != 'X' && board[row][col] != 'O';
}

void makeMove(vector<vector<char>> &board, int boardSize, int input, char currPlay) {
    int row = (input - 1) / boardSize;
    int col = (input - 1) % boardSize;
    board[row][col] = currPlay;
}


bool checkWin(const vector<vector<char>> &board, int boardSize, char currPlay, int lineForWin) {
    vector<vector<char>> currBoard(lineForWin, vector<char>(lineForWin));

    for(int i = 0; i <= boardSize - lineForWin; i++) {
        for(int k = 0; k <= boardSize - lineForWin; k++) {
            for(int m = 0; m < lineForWin; m++) {
                for(int n = 0; n < lineForWin; n++) {
                    currBoard[m][n] = board[i + m][k + n];
                }
            }

            // Check rows
            for(int m = 0; m < lineForWin; m++) {
                int counter = 0;
                for(int n = 0; n < lineForWin; n++) {
                    if(currBoard[m][n] == currPlay) {
                        counter++;
                        if(counter == lineForWin) return true;
                    } else {
                        counter = 0;
                        break;
                    }
                }
            }

            // Check columns
            for(int n = 0; n < lineForWin; n++) {
                int counter = 0;
                for(int m = 0; m < lineForWin; m++) {
                    if(currBoard[m][n] == currPlay) {
                        counter++;
                        if(counter == lineForWin) return true;
                    } else {
                        counter = 0;
                        break;
                    }
                }
            }

            // Check main diagonal
            int counter = 0;
            for(int m = 0; m < lineForWin; m++) {
                if(currBoard[m][m] == currPlay) {
                    counter++;
                    if(counter == lineForWin) return true;
                } else {
                    counter = 0;
                    break;
                }
            }

            // Check anti-diagonal
            counter = 0;
            for(int m = 0; m < lineForWin; m++) {
                if(currBoard[m][lineForWin - 1 - m] == currPlay) {
                    counter++;
                    if(counter == lineForWin) return true;
                } else {
                    counter = 0;
                    break;
                }
            }
        }
    }
    return false;
}


bool isBoardFull(const vector<vector<char>> &board, int boardSize) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != 'X' && board[i][j] != 'O') return false;
        }
    }
    return true;
}

void changePlayer(char &currPlay) {
    if(currPlay == 'X')
    {
        currPlay = 'O';
    }
    else
    {
        currPlay = 'X';
    }
}

int main() {
    
    int input;
    const int boardSize = 4;
    const int lineForWin = 3;
    vector<vector<char>> board;
    char currPlay = 'X';

    initBoard(board, boardSize);
    printBoard(board, boardSize);

    while (true) {
        cout << "Player " << currPlay << ":";
        cin >> input;

        if (isValidMove(board, boardSize, input) == false) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }

        makeMove(board, boardSize, input, currPlay);
        printBoard(board, boardSize);

        if (checkWin(board, boardSize, currPlay, lineForWin)) {
            cout << "Player " << currPlay << " wins!" << endl;
            break;
        }

        if (isBoardFull(board, boardSize)) {
            cout << "The game is a tie!" << endl;
            break;
        }

        changePlayer(currPlay);
    }

    return 0;
}
