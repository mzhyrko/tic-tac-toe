#include "game.hpp"
#include "board.hpp"
#include "player.hpp"
#include <iostream>
#include <string>

namespace game {

int getBoardSizeFromUser() {
    int size;
    std::cout << "Enter board size (3-1024): ";
    std::cin >> size;
    while (size < 3 || size > 1024) {
        std::cout << "Invalid size. Enter again: ";
        std::cin >> size;
    }
    return size;
}

int getWinConditionFromUser(int boardSize) {
    int winCondition;
    std::cout << "Enter win condition (3-" << boardSize << "): ";
    std::cin >> winCondition;
    while (winCondition < 3 || winCondition > boardSize) {
        std::cout << "Invalid win condition. Enter again: ";
        std::cin >> winCondition;
    }
    return winCondition;
}

std::string getPlayerTypeFromUser(const std::string& playerName) {
    std::cout << "Select type for " << playerName << ":\n";
    std::cout << "1 - Human\n";
    std::cout << "2 - Random Bot\n";
    std::cout << "3 - Beginner Bot\n";
    std::cout << "4 - Advanced Bot\n";
    int choice;
    std::cin >> choice;
    while (choice < 1 || choice > 4) {
        std::cout << "Invalid choice. Enter again: ";
        std::cin >> choice;
    }
    switch (choice) {
        case 1: return "human";
        case 2: return "randombot";
        case 3: return "beginnerbot";
        case 4: return "advancedbot";
        default: return "human";
    }
}

Player* createPlayer(const std::string& type, const std::string& sign) {
    if (type == "human") return new HumanPlayer(sign);
    else if (type == "randombot") return new RandomBotPlayer(sign);
    else if (type == "beginnerbot") return new BeginnerBotPlayer(sign);
    else if (type == "advancedbot") return new AdvancedBotPlayer(sign, 5); // Default maxDepth=5
    else throw std::invalid_argument("Invalid player type");
}

void play() {
    // Get game parameters from user
    int boardSize = getBoardSizeFromUser();
    int winCondition = getWinConditionFromUser(boardSize);
    std::string player1Type = getPlayerTypeFromUser("Player 1");
    std::string player2Type = getPlayerTypeFromUser("Player 2");

    // Create players and board
    Player* p1 = createPlayer(player1Type, "X");
    Player* p2 = createPlayer(player2Type, "O");
    Board board(boardSize, winCondition);

    // Game loop
    std::string currentSign = "X";
    Player* currentPlayer = p1;
    while (true) {
        board.print();
        int move = currentPlayer->getMove(board);
        while (!board.makeMove(move, currentSign)) {
            std::cout << "Invalid move. Try again.\n";
            move = currentPlayer->getMove(board);
        }

        if (board.checkWin(currentSign)) {
            board.print();
            std::cout << "Player " << currentSign << " wins!\n";
            break;
        }
        if (board.isFull()) {
            board.print();
            std::cout << "The game is a tie!\n";
            break;
        }

        currentSign = (currentSign == "X") ? "O" : "X";
        currentPlayer = (currentPlayer == p1) ? p2 : p1;
    }

    // Clean up
    delete p1;
    delete p2;
}

}