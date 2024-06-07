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
    if(input == 0) 
    return false;
    if (input < 1 || input > boardSize * boardSize) {
        return false;
    }
    int row = (input - 1) / boardSize;
    int col = (input - 1) % boardSize;
    return board[row][col] != "X" && board[row][col] != "O";
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


void testsCheckWin() {
    vector<vector<string>> boardTest;
    int boardSizeTest;
    int lineForWinTest;
    char currPlayTest;

    // 3x3 Board with Winning Line of 3 Cells
    // Test 1: Horizontal win in the first row
    boardSizeTest = 3;
    lineForWinTest = 3;
    currPlayTest = 'X';
    boardTest = {{"X", "X", "X"},
                 {"O", "O", "1"},
                 {"2", "3", "4"}};
    cout << "Test 1:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 2: Vertical win in the first column
    boardSizeTest = 3;
    lineForWinTest = 3;
    currPlayTest = 'X';
    boardTest = {{"X", "2", "3"},
                 {"X", "O", "O"},
                 {"X", "4", "5"}};
    cout << "Test 2:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 3: Diagonal win from top-left to bottom-right
    boardSizeTest = 3;
    lineForWinTest = 3;
    currPlayTest = 'X';
    boardTest = {{"X", "2", "3"},
                 {"O", "X", "O"},
                 {"4", "5", "X"}};
    cout << "Test 3:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 4: Diagonal win from bottom-left to top-right
    boardSizeTest = 3;
    lineForWinTest = 3;
    currPlayTest = 'X';
    boardTest = {{"4", "2", "X"},
                 {"3", "X", "O"},
                 {"X", "5", "O"}};
    cout << "Test 4:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // 4x4 Board with Winning Line of 3 Cells
    // Test 5: Horizontal win in the first row
    boardSizeTest = 4;
    lineForWinTest = 3;
    currPlayTest = 'X';
    boardTest = {{"X", "X", "X", "4"},
                 {"O", "O", "1", "2"},
                 {"2", "3", "4", "5"},
                 {"6", "7", "8", "9"}};
    cout << "Test 5:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 6: Vertical win in the first column
    boardSizeTest = 4;
    lineForWinTest = 3;
    currPlayTest = 'X';
    boardTest = {{"X", "2", "3", "4"},
                 {"X", "O", "O", "5"},
                 {"X", "4", "5", "6"},
                 {"2", "7", "8", "9"}};
    cout << "Test 6:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 7: Diagonal win from top-left to bottom-right
    boardSizeTest = 4;
    lineForWinTest = 3;
    currPlayTest = 'X';
    boardTest = {{"X", "2", "3", "4"},
                 {"O", "X", "O", "5"},
                 {"4", "5", "X", "6"},
                 {"6", "7", "8", "9"}};
    cout << "Test 7:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 8: Diagonal win from bottom-left to top-right
    boardSizeTest = 4;
    lineForWinTest = 3;
    currPlayTest = 'X';
    boardTest = {{"4", "2", "X", "4"},
                 {"3", "X", "O", "5"},
                 {"X", "4", "5", "6"},
                 {"6", "7", "8", "9"}};
    cout << "Test 8:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // 4x4 Board with Winning Line of 4 Cells
    // Test 9: Horizontal win in the first row
    boardSizeTest = 4;
    lineForWinTest = 4;
    currPlayTest = 'X';
    boardTest = {{"X", "X", "X", "X"},
                 {"O", "O", "1", "2"},
                 {"2", "3", "4", "5"},
                 {"6", "7", "8", "9"}};
    cout << "Test 9:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 10: Vertical win in the first column
    boardSizeTest = 4;
    lineForWinTest = 4;
    currPlayTest = 'X';
    boardTest = {{"X", "2", "3", "4"},
                 {"X", "O", "O", "5"},
                 {"X", "4", "5", "6"},
                 {"X", "7", "8", "9"}};
    cout << "Test 10:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 11: Diagonal win from top-left to bottom-right
    boardSizeTest = 4;
    lineForWinTest = 4;
    currPlayTest = 'X';
    boardTest = {{"X", "2", "3", "4"},
                 {"O", "X", "O", "5"},
                 {"4", "5", "X", "6"},
                 {"6", "7", "8", "X"}};
    cout << "Test 11:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 12: Diagonal win from bottom-left to top-right
    boardSizeTest = 4;
    lineForWinTest = 4;
    currPlayTest = 'X';
    boardTest = {{"6", "7", "8", "X"},
                 {"3", "4", "X", "2"},
                 {"2", "X", "3", "1"},
                 {"X", "O", "O", "4"}};
    cout << "Test 12:" << endl;
    for (const auto& row : boardTest) {
        for (const auto& cell : row) cout << cell << " ";
        cout << endl;
    }
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // 5x5 Board with Winning Line of 3 Cells
    // Test 13: Horizontal win in the first row
    boardSizeTest = 5;
    lineForWinTest = 3;
    currPlayTest = 'X';
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
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 14: Vertical win in the first column
    boardSizeTest = 5;
    lineForWinTest = 3;
    currPlayTest = 'X';
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
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 15: Diagonal win from top-left to bottom-right
    boardSizeTest = 5;
    lineForWinTest = 3;
    currPlayTest = 'X';
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
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 16: Diagonal win from bottom-left to top-right
    boardSizeTest = 5;
    lineForWinTest = 3;
    currPlayTest = 'X';
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
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // 5x5 Board with Winning Line of 4 Cells
    // Test 17: Horizontal win in the first row
    boardSizeTest = 5;
    lineForWinTest = 4;
    currPlayTest = 'X';
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
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 18: Vertical win in the first column
    boardSizeTest = 5;
    lineForWinTest = 4;
    currPlayTest = 'X';
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
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 19: Diagonal win from top-left to bottom-right
    boardSizeTest = 5;
    lineForWinTest = 4;
    currPlayTest = 'X';
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
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // Test 20: Diagonal win from bottom-left to top-right
    boardSizeTest = 5;
    lineForWinTest = 4;
    currPlayTest = 'X';
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
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;

    // 15x15 Board with Winning Line of 5 Cells
    // Test 21: Horizontal win in the first row
    boardSizeTest = 15;
    lineForWinTest = 5;
    currPlayTest = 'X';
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
    cout << (checkWin(boardTest, boardSizeTest, currPlayTest, lineForWinTest) ? "True" : "False") << endl;
}


bool isBoardFull(const vector<vector<string>> &board, int boardSize) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") 
                return false;
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

bool isNumber(string inpStr){
    for (char c : inpStr) {
        if (c < char(48) || c > char(57)) {
            return false;
        }
    }
    return true;
}

void game(){   
    int input;
    int boardSize = 4;
    int lineForWin = 3;
    vector<vector<string>> board;
    char currPlay = 'X';

    initBoard(board, boardSize);
    printBoard(board, boardSize, lineForWin);

    while (true) {
        cout << "Player " << currPlay << ", enter your move: ";
        string inpStr;
        cin >> inpStr;

        if(isNumber(inpStr) == false){
            continue;
        }

        input = stoi(inpStr);

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
    
    game();
    //testsCheckWin();
    //cout << isNumber("f");

    return 0;
}
