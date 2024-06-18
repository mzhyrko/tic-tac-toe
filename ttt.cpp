#include <bits/stdc++.h>
#include <random>
#include <stdlib.h>
#include <time.h>
using namespace std;


void initBoardSize(int &boardSize, int &lineForWin)
{
    cout << "Board size (3 - 1024): " << endl;
    cin >> boardSize;

    if(boardSize < 3 || boardSize > 1024) {
        cout << "Invalid board size. Please enter a value between 3 and 1024." << endl;
        return;
    }

    if (boardSize <= 3) {
        lineForWin = 3;
    } else if (boardSize <= 5) {
        lineForWin = 3;
    } else if (boardSize <= 14) {
        lineForWin = 4;
    } else if (boardSize <= 19) {
        lineForWin = 5;
    } else {
        lineForWin = 5 + ceil((boardSize - 19) / 20.0);
    }
}


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


void selectGameMode(int &selectMode, string &firstOpponent, string &secondOpponent)
{
    int localInput;

    cout << "Game mods:" << endl;
    cout << "1 - Human vs Human" << endl;
    cout << "2 - Human vs Bot" << endl;
    cout << "3 - Bot vs Bot" << endl;
    cout << "Select game mode: ";

    cin >> selectMode;

    if(selectMode ==  1)
    {
        firstOpponent = "man";
    }

    if(selectMode ==  2)
    {
        firstOpponent = "man";

        cout << "Select bot type: " << endl;
        cout << "1 - Random move bot" << endl;
        cout << "2 - Beginner bot" << endl;
        cout << "3 - Advanced bot" << endl;
        cout << "Select bot type: ";
        cin >> localInput;
        if(localInput == 1 )
        {
            secondOpponent = "rbot";
        }
        else if(localInput == 2)
        {
            secondOpponent = "bbot";
        }
        else if(localInput == 3)
        {
            secondOpponent = "abot";
        }
    }

    if(selectMode ==  3)
    {
        cout << "Select first bot:" << endl;
        cout << "1 - Random move bot" << endl;
        cout << "2 - Beginner bot" << endl;
        cout << "3 - Advanced bot" << endl;


        cin >> localInput;

        if(localInput == 1){
            firstOpponent = "rbot";
        }
        else if(localInput == 2){
            firstOpponent = "bbot";
        }
        else if(localInput == 3)
        {
            firstOpponent = "abot";
        }


        cout << "Select second bot:" << endl;
        cout << "1 - Random move bot" << endl;
        cout << "2 - Beginner bot" << endl;
        cout << "3 - Advanced bot" << endl;

        cin >> localInput;

        if(localInput == 1){
            secondOpponent = "rbot";
        }
        else if(localInput == 2){
            secondOpponent = "bbot";
        }
        else if(localInput == 3)
        {
            secondOpponent = "abot";
        }
    }
}


void printBoard(const vector<vector<string>>& board, int boardSize, int lineForWin) {
    const int cellWidth = 5;

    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] == "X")
                cout << "\t" << "\033[1;31m" << board[i][j] << "\033[0m";
            else if (board[i][j] == "O")
                cout << "\t" << "\033[1;34m" << board[i][j] << "\033[0m";
            else
                cout << "\t" << board[i][j];

            //if (j < boardSize - 1) cout << " | ";
        }
        cout << endl;
        //if (i < boardSize - 1) cout << string((cellWidth + 3) * boardSize - 1, '-') << endl;
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


void insertMove(vector<vector<string>> &board, int boardSize, int input, string currSign) {
    int row = (input - 1) / boardSize;
    int col = (input - 1) % boardSize;
    board[row][col] = currSign;
}


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

bool isBoardFull(const vector<vector<string>> &board, int boardSize) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O")
                return false;
        }
    }
    return true;
}


void changeSign(string &currSign) {

    if(currSign == "X"){
        currSign = "O";
    }
    else{
        currSign = "X";
    }


}

void changePlayer(int &selectMode, string &firstOpponent, string &secondOpponent){
    swap(firstOpponent, secondOpponent);
}

bool isNumber(string inpStr){
    for (char c : inpStr) {
        if (c < char(48) || c > char(57)) {
            return false;
        }
    }
    return true;
}


void randomBot(int boardSize, string &inpStr, string currSign, vector<vector<string>> &board){
    int r;
    srand(time(NULL));
    while(true)
    {
        r = rand()%(boardSize*boardSize) + 1;
        if(isValidMove(board, boardSize, r))
            break;
    }
    inpStr = to_string(r);
}

void beginnerBot(string &inpStr, string currSign, int boardSize, int lineForWin, vector<vector<string>> &board) {
    int maxScore = -1;
    int bestMove[2];
    bestMove[0] = -1;
    bestMove[1] = -1;

    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != "X" && board[i][j] != "O") {
                int score = 0;

                for (int k = 0; k < boardSize; ++k) {
                    if (board[i][k] == currSign) {
                        score++;
                    }
                }


                for (int k = 0; k < boardSize; ++k) {
                    if (board[k][j] == currSign) {
                        score++;
                    }
                }


                for (int k = 0; k < boardSize; ++k) {
                    if (i + k < boardSize && j + k < boardSize && board[i + k][j + k] == currSign) {
                        score++;
                    }
                }


                for (int k = 0; k < boardSize; ++k) {
                    if (i + k < boardSize && j - k >= 0 && board[i + k][j - k] == currSign) {
                        score++;
                    }
                }


                if (score > maxScore) {
                    maxScore = score;
                    bestMove[0] = i;
                    bestMove[1] = j;
                }
            }
        }
    }

    if (bestMove[0] != -1 && bestMove[1] != -1) {
        inpStr = to_string(bestMove[0] * boardSize + bestMove[1] + 1);
    }
}

int minimax(vector<vector<string>> &board, int boardSize, int lineForWin, bool isMax, string botSign, string playerSign, int depth, int maxDepth) {
    if (checkWin(board, boardSize, botSign, lineForWin)) return 10 - depth;;
    if (checkWin(board, boardSize, playerSign, lineForWin)) return depth - 10;
    if (checkDraw(board, boardSize)) return 0;

    if (isMax == true) {
       
        int best = -1000;
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] != "X" && board[i][j] != "O") {
                    string temp = board[i][j];
                    board[i][j] = botSign;
                    best = max(best, minimax(board, boardSize, lineForWin, !isMax, botSign, playerSign, depth+1, maxDepth));
                    
                    board[i][j] = temp;
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] != "X" && board[i][j] != "O") {
                    
                    string temp = board[i][j];
                    board[i][j] = playerSign;
                    
                    best = min(best, minimax(board, boardSize, lineForWin, !isMax, botSign, playerSign, depth+1, maxDepth));
                    
                    board[i][j] = temp;
                }
            }
        }
        return best;
    }
}

void dpTTToeBot(vector<vector<string>> &board, int boardSize, int lineForWin, string botSign, string &inpStr, int maxDepth) {
    int bestVal = -1000;
    int bestMove[2];
    string playerSign = (botSign == "X") ? "O" : "X";
    bestMove[0] = -1;
    bestMove[1] = -1;

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] != "X" && board[i][j] != "O") {
                
                string temp = board[i][j];
                board[i][j] = botSign;
                
                int moveVal = minimax(board, boardSize, lineForWin, false, botSign, playerSign, 0, maxDepth);
        
                board[i][j] = temp;

                
                if (moveVal > bestVal) {
                    bestMove[0] = i; 
                    bestMove[1] = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    inpStr = to_string(bestMove[0] * boardSize + bestMove[1] + 1);
}


void makeMove(string &inpStr, string firstOpponent, string currSign, int boardSize, int lineForWin, vector<vector<string>> &board, int maxDepth){

    if(firstOpponent == "rbot"){
        randomBot(boardSize, inpStr, currSign, board);
        cout << "Random bot " << currSign << " entered move: " << inpStr << endl;
    }

    if(firstOpponent == "bbot"){
        beginnerBot(inpStr, currSign, boardSize, lineForWin, board);
        cout << "Beginner bot " << currSign << " entered move: " << inpStr << endl;
    }

    if(firstOpponent == "abot"){
        dpTTToeBot(board, boardSize, lineForWin, firstOpponent, inpStr, maxDepth);
        cout << "Advanced bot " << currSign << " entered move: " << inpStr << endl;
    }

    if(firstOpponent == "man"){
        cout << "Player " << currSign << ", enter your move: ";
        cin >> inpStr;
    }
}


void game(){

    int input;
    int boardSize;
    int lineForWin;
    int selectMode;
    int maxDepth = 6;
    string currSign = "X";
    string inpStr;
    string firstOpponent = "man";
    string secondOpponent = "man";
    vector<vector<string>> board;

    initBoardSize(boardSize, lineForWin);
    selectGameMode(selectMode, firstOpponent, secondOpponent);
    initBoard(board, boardSize);
    printBoard(board, boardSize, lineForWin);


    while (true) {
        //cout << "Player " << currSign << ", enter your move: ";
        //cin >> inpStr;

        makeMove(inpStr, firstOpponent, currSign,  boardSize, lineForWin, board, maxDepth);

        if(isNumber(inpStr) == false){
            continue;
        }

        input = stoi(inpStr);

        if (!isValidMove(board, boardSize, input)) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }

        insertMove(board, boardSize, input, currSign);
        printBoard(board, boardSize, lineForWin);

        if(checkWin(board, boardSize, currSign, lineForWin)) {
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


int main() {

    game();

    return 0;
}
