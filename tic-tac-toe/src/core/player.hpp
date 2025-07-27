#pragma once

#include "board.hpp"
#include <string>

// Abstract base class for all players
class Player {
public:
    // Constructor to set the player's sign
    Player(const std::string& sign);

    // Virtual destructor for proper cleanup in derived classes
    virtual ~Player() = default;

    // Pure virtual method to get the player's move
    virtual int getMove(const Board& board) = 0;

    // Get the player's sign ("X" or "O")
    std::string getSign() const;

protected:
    std::string sign; // Player's sign
};

// Human player class
class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& sign);
    int getMove(const Board& board) override;
};

// Random bot player class
class RandomBotPlayer : public Player {
public:
    RandomBotPlayer(const std::string& sign);
    int getMove(const Board& board) override;
};

// Beginner bot player class
class BeginnerBotPlayer : public Player {
public:
    BeginnerBotPlayer(const std::string& sign);
    int getMove(const Board& board) override;
};

// Advanced bot player class using minimax
class AdvancedBotPlayer : public Player {
private:
    int maxDepth; // Maximum depth for minimax search
public:
    AdvancedBotPlayer(const std::string& sign, int maxDepth = 5);
    int getMove(const Board& board) override;
};