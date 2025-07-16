#include <bits/stdc++.h>

using namespace std;


bool columnsCheckWin(const vector<vector<string>> &board, int boardSize, string currSign, int lineForWin, int i, int j){
    for (int x = 0; x < lineForWin; x++) {
            bool rowWin = true;
            for (int y = 0; y < lineForWin; y++) {
                if (board[i + x][j + y] != currSign) {
                    rowWin = false;
                    break;
                }
            }
        if (rowWin)
            return true;
    }
    return false;
}

bool rowsCheckWin(const vector<vector<string>> &board, int boardSize, string currSign, int lineForWin, int i, int j){
    for (int y = 0; y < lineForWin; y++) {
        bool colWin = true;
        for (int x = 0; x < lineForWin; x++){
            if (board[i + x][j + y] != currSign) {
                colWin = false;
                break;
            }
        }
        if (colWin)
            return true;
    }
    return false;
}

bool mainDiagonalCheckWin(const vector<vector<string>> &board, int boardSize, string currSign, int lineForWin, int i, int j){
    bool diagWin1 = true;
    for (int x = 0; x < lineForWin; x++) {
        if (board[i + x][j + x] != currSign) {
            diagWin1 = false;
            break;
        }
    }
    if (diagWin1)
        return true;

    return false;
}

bool antiDiagonalCheckWin(const vector<vector<string>> &board, int boardSize, string currSign, int lineForWin, int i, int j){
    bool diagWin2 = true;
    for (int x = 0; x < lineForWin; x++) {
        if (board[i + x][j + lineForWin - 1 - x] != currSign) {
            diagWin2 = false;
            break;
        }
    }
    if (diagWin2)
        return true;
    return false;
}

bool checkWin(const vector<vector<string>> &board, int boardSize, string currSign, int lineForWin) {
    for (int i = 0; i <= boardSize - lineForWin; i++) {
        for (int j = 0; j <= boardSize - lineForWin; j++) {

            if(columnsCheckWin(board, boardSize, currSign, lineForWin, i, j))
                return true;

            if(rowsCheckWin(board, boardSize, currSign, lineForWin, i, j))
                return true;

            if(mainDiagonalCheckWin(board, boardSize, currSign, lineForWin, i, j))
                return true;

            if(antiDiagonalCheckWin(board, boardSize, currSign, lineForWin, i, j))
                return true;
        }
    }
    return false;
}

bool checkDraw(vector<vector<string>> &board, int boardSize) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] != "X" && board[i][j] != "O") {
                return false;
            }
        }
    }
    return true;
}

void testsCheckWin(){
    vector<vector<string>> boardTest;
    int boardSizeTest;
    int lineForWinTest;
    string currSignTest;

    // 3x3 Board with Winning Line of 3 Cells
    // Test 1: Horizontal win in the first row
    boardSizeTest = 3;
    lineForWinTest = 3;
    currSignTest = 'X';
    boardTest = {{"X", "X", "X"},
                 {"O", "O", "1"},
                 {"2", "3", "4"}};
    cout << "Test 1:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 2: Vertical win in the first column
    boardSizeTest = 3;
    lineForWinTest = 3;
    currSignTest = 'X';
    boardTest = {{"X", "2", "3"},
                 {"X", "O", "O"},
                 {"X", "4", "5"}};
    cout << "Test 2:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 3: Diagonal win from top-left to bottom-right
    boardSizeTest = 3;
    lineForWinTest = 3;
    currSignTest = 'X';
    boardTest = {{"X", "2", "3"},
                 {"O", "X", "O"},
                 {"4", "5", "X"}};
    cout << "Test 3:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 4: Diagonal win from bottom-left to top-right
    boardSizeTest = 3;
    lineForWinTest = 3;
    currSignTest = 'X';
    boardTest = {{"4", "2", "X"},
                 {"3", "X", "O"},
                 {"X", "5", "O"}};
    cout << "Test 4:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // 4x4 Board with Winning Line of 3 Cells
    // Test 5: Horizontal win in the first row
    boardSizeTest = 4;
    lineForWinTest = 3;
    currSignTest = 'X';
    boardTest = {{"X", "X", "X", "4"},
                 {"O", "O", "1", "2"},
                 {"2", "3", "4", "5"},
                 {"6", "7", "8", "9"}};
    cout << "Test 5:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 6: Vertical win in the first column
    boardSizeTest = 4;
    lineForWinTest = 3;
    currSignTest = 'X';
    boardTest = {{"X", "2", "3", "4"},
                 {"X", "O", "O", "5"},
                 {"X", "4", "5", "6"},
                 {"2", "7", "8", "9"}};
    cout << "Test 6:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 7: Diagonal win from top-left to bottom-right
    boardSizeTest = 4;
    lineForWinTest = 3;
    currSignTest = 'X';
    boardTest = {{"X", "2", "3", "4"},
                 {"O", "X", "O", "5"},
                 {"4", "5", "X", "6"},
                 {"6", "7", "8", "9"}};
    cout << "Test 7:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 8: Diagonal win from bottom-left to top-right
    boardSizeTest = 4;
    lineForWinTest = 3;
    currSignTest = 'X';
    boardTest = {{"4", "2", "X", "4"},
                 {"3", "X", "O", "5"},
                 {"X", "4", "5", "6"},
                 {"6", "7", "8", "9"}};
    cout << "Test 8:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // 4x4 Board with Winning Line of 4 Cells
    // Test 9: Horizontal win in the first row
    boardSizeTest = 4;
    lineForWinTest = 4;
    currSignTest = 'X';
    boardTest = {{"X", "X", "X", "X"},
                 {"O", "O", "1", "2"},
                 {"2", "3", "4", "5"},
                 {"6", "7", "8", "9"}};
    cout << "Test 9:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 10: Vertical win in the first column
    boardSizeTest = 4;
    lineForWinTest = 4;
    currSignTest = 'X';
    boardTest = {{"X", "2", "3", "4"},
                 {"X", "O", "O", "5"},
                 {"X", "4", "5", "6"},
                 {"X", "7", "8", "9"}};
    cout << "Test 10:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 11: Diagonal win from top-left to bottom-right
    boardSizeTest = 4;
    lineForWinTest = 4;
    currSignTest = 'X';
    boardTest = {{"X", "2", "3", "4"},
                 {"O", "X", "O", "5"},
                 {"4", "5", "X", "6"},
                 {"6", "7", "8", "X"}};
    cout << "Test 11:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 12: Diagonal win from bottom-left to top-right
    boardSizeTest = 4;
    lineForWinTest = 4;
    currSignTest = 'X';
    boardTest = {{"6", "7", "8", "X"},
                 {"3", "4", "X", "2"},
                 {"2", "X", "3", "1"},
                 {"X", "O", "O", "4"}};
    cout << "Test 12:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // 5x5 Board with Winning Line of 3 Cells
    // Test 13: Horizontal win in the first row
    boardSizeTest = 5;
    lineForWinTest = 3;
    currSignTest = 'X';
    boardTest = {{"X", "X", "X", "4", "5"},
                 {"O", "O", "1", "2", "3"},
                 {"2", "3", "4", "5", "6"},
                 {"6", "7", "8", "9", "10"},
                 {"11", "12", "13", "14", "15"}};
    cout << "Test 13:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 14: Vertical win in the first column
    boardSizeTest = 5;
    lineForWinTest = 3;
    currSignTest = 'X';
    boardTest = {{"X", "2", "3", "4", "5"},
                 {"X", "O", "O", "6", "7"},
                 {"X", "4", "5", "8", "9"},
                 {"2", "6", "7", "10", "11"},
                 {"12", "13", "14", "15", "16"}};
    cout << "Test 14:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 15: Diagonal win from top-left to bottom-right
    boardSizeTest = 5;
    lineForWinTest = 3;
    currSignTest = 'X';
    boardTest = {{"X", "2", "3", "4", "5"},
                 {"O", "X", "O", "6", "7"},
                 {"4", "5", "X", "8", "9"},
                 {"6", "7", "8", "10", "11"},
                 {"12", "13", "14", "15", "16"}};
    cout << "Test 15:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 16: Diagonal win from bottom-left to top-right
    boardSizeTest = 5;
    lineForWinTest = 3;
    currSignTest = 'X';
    boardTest = {{"12", "13", "14", "15", "X"},
                 {"11", "10", "9", "X", "8"},
                 {"8", "X", "5", "4", "3"},
                 {"6", "7", "O", "2", "1"},
                 {"O", "O", "O", "O", "O"}};
    cout << "Test 16:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // 5x5 Board with Winning Line of 4 Cells
    // Test 17: Horizontal win in the first row
    boardSizeTest = 5;
    lineForWinTest = 4;
    currSignTest = 'X';
    boardTest = {{"X", "X", "X", "X", "5"},
                 {"O", "O", "1", "2", "3"},
                 {"2", "3", "4", "5", "6"},
                 {"6", "7", "8", "9", "10"},
                 {"11", "12", "13", "14", "15"}};
    cout << "Test 17:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 18: Vertical win in the first column
    boardSizeTest = 5;
    lineForWinTest = 4;
    currSignTest = 'X';
    boardTest = {{"X", "2", "3", "4", "5"},
                 {"X", "O", "O", "6", "7"},
                 {"X", "4", "5", "8", "9"},
                 {"X", "6", "7", "10", "11"},
                 {"12", "13", "14", "15", "16"}};
    cout << "Test 18:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 19: Diagonal win from top-left to bottom-right
    boardSizeTest = 5;
    lineForWinTest = 4;
    currSignTest = 'X';
    boardTest = {{"X", "2", "3", "4", "5"},
                 {"O", "X", "O", "6", "7"},
                 {"4", "5", "X", "8", "9"},
                 {"6", "7", "8", "X", "11"},
                 {"12", "13", "14", "15", "16"}};
    cout << "Test 19:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 20: Diagonal win from bottom-left to top-right
    boardSizeTest = 5;
    lineForWinTest = 4;
    currSignTest = 'X';
    boardTest = {{"12", "13", "14", "15", "X"},
                 {"11", "10", "9", "X", "8"},
                 {"8", "X", "5", "4", "3"},
                 {"X", "7", "O", "2", "1"},
                 {"O", "O", "O", "O", "O"}};
    cout << "Test 20:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;

    // 15x15 Board with Winning Line of 5 Cells
    // Test 21: Horizontal win in the first row
    boardSizeTest = 15;
    lineForWinTest = 5;
    currSignTest = 'X';
    boardTest = {{"X", "X", "X", "X", "X", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"}};
    cout << "Test 21:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currSignTest, lineForWinTest) ? "True" : "False") << endl;
}



int main() {

    testsCheckWin();

    return 0;
}

