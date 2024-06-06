#include <bits/stdc++.h>
using namespace std;

void initBoard(vector<vector<string>> &board, int boardSize) {
    board = vector<vector<string>>(boardSize, vector<string>(boardSize));
    int numDigits = to_string(boardSize * boardSize).length();

    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            int num = i * boardSize + j + 1;
            string numStr = to_string(num);
            while (numStr.length() < numDigits) {
                numStr = "0" + numStr; 
            }
            board[i][j] = numStr;
        }
    }
}

void printBoard(const vector<vector<string>> &board, int boardSize, int lineForWin) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            cout << board[i][j];
            if (j < boardSize - 1) cout << " | ";
        }
        cout << endl;
        if (i < boardSize - 1) cout << string(boardSize * boardSize - lineForWin, '-') << endl;
    }
}

bool isValidMove(const vector<vector<string>> &board, int boardSize, int input) {
    int row = (input - 1) / boardSize;
    int col = (input - 1) % boardSize;
    return input >= 1 && input <= boardSize * boardSize && board[row][col] != "X" && board[row][col] != "O";
}

void makeMove(vector<vector<string>> &board, int boardSize, int input, char currPlay) {
    int row = (input - 1) / boardSize;
    int col = (input - 1) % boardSize;
    board[row][col] = currPlay;
}


bool columnsCheckWin(const vector<vector<string>> &board, int boardSize, char currPlay, int lineForWin, int i, int j){
    for (int x = 0; x < lineForWin; x++) {
            bool rowWin = true;
            for (int y = 0; y < lineForWin; y++) {
                if (board[i + x][j + y][0] != currPlay) {
                    rowWin = false;
                    break;
                }
            }
        if (rowWin)
            return true;
    }
    return false;
}


bool rowsCheckWin(const vector<vector<string>> &board, int boardSize, char currPlay, int lineForWin, int i, int j){
    for (int y = 0; y < lineForWin; y++) {
        bool colWin = true;
        for (int x = 0; x < lineForWin; x++){
            if (board[i + x][j + y][0] != currPlay) {
                colWin = false;
                break;
            }
        }
        if (colWin)
            return true;
    }
    return false;
}


bool mainDiagonalCheckWin(const vector<vector<string>> &board, int boardSize, char currPlay, int lineForWin, int i, int j){
    bool diagWin1 = true;
    for (int x = 0; x < lineForWin; x++) {
        if (board[i + x][j + x][0] != currPlay) {
            diagWin1 = false;
            break;
        }
    }
    if (diagWin1)
        return true;
    
    return false;    
}


bool antiDiagonalCheckWin(const vector<vector<string>> &board, int boardSize, char currPlay, int lineForWin, int i, int j){
    bool diagWin2 = true;
    for (int x = 0; x < lineForWin; x++) {
        if (board[i + x][j + lineForWin - 1 - x][0] != currPlay) {
            diagWin2 = false;
            break;
        }
    }
    if (diagWin2)
        return true;
    return false;
}


bool checkWin(const vector<vector<string>> &board, int boardSize, char currPlay, int lineForWin) {
    for (int i = 0; i <= boardSize - lineForWin; i++) {
        for (int j = 0; j <= boardSize - lineForWin; j++) {
            
            if(columnsCheckWin(board, boardSize, currPlay, lineForWin, i, j))
                return true;

            if(rowsCheckWin(board, boardSize, currPlay, lineForWin, i, j))
                return true;

            if(mainDiagonalCheckWin(board, boardSize, currPlay, lineForWin, i, j))
                return true;

            if(antiDiagonalCheckWin(board, boardSize, currPlay, lineForWin, i, j))
                return true;
        }
    }
    return false;
}

void testsCheckWin(){
    
    vector<vector<string>> boardTest;
    int boardSizeTest;
    int lineForWinTest;
    char currPlayTest;

    // Test 1:
    boardSizeTest = 3;
    lineForWinTest = 3;
    currPlayTest = 'X';
    boardTest = {{"X", "X", "X"},
                 {"O", "O", "1"},
                 {"2", "3", "4"}};
    cout << "Test 1:" << endl;
    for(int i = 0; i < boardSizeTest; i++){
        for(int k = 0; k < boardSizeTest; k++){
            cout << boardTest[i][k] << " ";
        }
        cout << endl;
    }
    if (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest))
        cout << "True" << endl;
    else
        cout << "False" << endl;


    // Test 2:
    boardSizeTest = 3;
    lineForWinTest = 3;
    currPlayTest = 'X';
    boardTest = {{"1", "2", "X"},
                 {"3", "X", "1"},
                 {"X", "3", "4"}};
    cout << "Test 2:" << endl;
    for(int i = 0; i < boardSizeTest; i++){
        for(int k = 0; k < boardSizeTest; k++){
            cout << boardTest[i][k] << " ";
        }
        cout << endl;
    }
    if (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest))
        cout << "True" << endl;
    else
        cout << "False" << endl;
    
    
    // Test 3:
    boardSizeTest = 3;
    lineForWinTest = 3;
    currPlayTest = 'X';
    boardTest = {{"X", "2", "1"},
                 {"3", "X", "1"},
                 {"1", "3", "X"}};
    cout << "Test 3:" << endl;
    for(int i = 0; i < boardSizeTest; i++){
        for(int k = 0; k < boardSizeTest; k++){
            cout << boardTest[i][k] << " ";
        }
        cout << endl;
    }
    if (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest))
        cout << "True" << endl;
    else
        cout << "False" << endl;

    // Test 4:
    boardSizeTest = 3;
    lineForWinTest = 3;
    currPlayTest = 'X';
    boardTest = {{"O", "2", "1"},
                 {"3", "O", "1"},
                 {"1", "3", "O"}};
    cout << "Test 4:" << endl;
    for(int i = 0; i < boardSizeTest; i++){
        for(int k = 0; k < boardSizeTest; k++){
            cout << boardTest[i][k] << " ";
        }
        cout << endl;
    }
    if (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest))
        cout << "True" << endl;
    else
        cout << "False" << endl;
}

bool isBoardFull(const vector<vector<string>> &board, int boardSize) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") return false;
        }
    }
    return true;
}

void changePlayer(char &currPlay) {
    if(currPlay == 'X'){
        currPlay = 'O';
    }
    else{
        currPlay = 'X';
    }
}

void game()
{
    int input;
    int boardSize = 4;
    int lineForWin = 3;
    vector<vector<string>> board;
    char currPlay = 'X';

    initBoard(board, boardSize);
    printBoard(board, boardSize, lineForWin);

    while (true) {
        cout << "Player " << currPlay << ", enter your move: ";
        cin >> input;

        if (!isValidMove(board, boardSize, input)) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }

        makeMove(board, boardSize, input, currPlay);
        printBoard(board, boardSize, lineForWin);

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
}

int main() {
    
    testsCheckWin();

    return 0;
}
