#include "player.hpp"
#include "../bots/bots.hpp"
#include "board.hpp"
#include <iostream>
#include <exception>  // Добавлен

Player::Player(const std::string& sign) : sign(sign) {}

std::string Player::getSign() const {
    return sign;
}

HumanPlayer::HumanPlayer(const std::string& sign) : Player(sign) {}

int HumanPlayer::getMove(const Board& board) {
    std::cout << "Player " << sign << ", enter your move: ";
    std::string input;
    std::cin >> input;
    try {
        int move = std::stoi(input);
        return move;
    } catch (const std::exception&) {
        std::cout << "Invalid input. Please enter a number.\n";
        return getMove(board);
    }
}

RandomBotPlayer::RandomBotPlayer(const std::string& sign) : Player(sign) {}

int RandomBotPlayer::getMove(const Board& board) {
    return randomBot(board, sign);
}

BeginnerBotPlayer::BeginnerBotPlayer(const std::string& sign) : Player(sign) {}

int BeginnerBotPlayer::getMove(const Board& board) {
    return beginnerBot(board, sign);
}

AdvancedBotPlayer::AdvancedBotPlayer(const std::string& sign, int maxDepth) : Player(sign), maxDepth(maxDepth) {}

int AdvancedBotPlayer::getMove(const Board& board) {
    return dpTTToeBot(board, sign, maxDepth);
}