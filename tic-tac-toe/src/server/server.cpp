#include "crow_all.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include "../core/board.hpp"
#include "../core/player.hpp"
#include "../core/game.hpp"
#include "../bots/bots.hpp"

// CORS middleware
struct CORSHandler {
    struct context {};
    void before_handle(crow::request& /*req*/, crow::response& res, context&) {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        res.add_header("Access-Control-Max-Age", "86400");
    }
    void after_handle(crow::request& /*req*/, crow::response& /*res*/, context&) {}
};

crow::App<CORSHandler> app;
std::mutex gamesMutex;
const int MAX_GAMES = 100;

// Class for managing a single game instance
class ServerGame {
public:
    std::shared_ptr<Board> board;
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    std::string player1Id;
    std::string player2Id;
    std::string currentSign = "X";
    std::vector<crow::websocket::connection*> connections;
    
    ServerGame(int boardSize, int winCondition, 
             const std::string& p1Type, const std::string& p2Type) {
        
        board = std::make_shared<Board>(boardSize, winCondition);
        board->init();
        
        player1 = createPlayer(p1Type, "X");
        player2 = createPlayer(p2Type, "O");
    }
    
    bool makeMove(int position, const std::string& sign) {
        return board->makeMove(position, sign);
    }
    
    bool checkWin(const std::string& sign) {
        return board->checkWin(sign);
    }
    
    bool isFull() {
        return board->isFull();
    }
    
    int getBotMove(const std::string& sign) {
        if (sign == "X") {
            return player1->getMove(*board);
        } else {
            return player2->getMove(*board);
        }
    }
    
    bool isBot(const std::string& sign) {
        if (sign == "X") {
            return dynamic_cast<HumanPlayer*>(player1.get()) == nullptr;
        } else {
            return dynamic_cast<HumanPlayer*>(player2.get()) == nullptr;
        }
    }
    
private:
    std::unique_ptr<Player> createPlayer(const std::string& type, const std::string& sign) {
        if (type == "human") {
            return std::make_unique<HumanPlayer>(sign);
        } else if (type == "randombot" || type == "easy") {
            return std::make_unique<RandomBotPlayer>(sign);
        } else if (type == "beginnerbot" || type == "medium") {
            return std::make_unique<BeginnerBotPlayer>(sign);
        } else if (type == "advancedbot" || type == "hard" || type == "bot") {
            return std::make_unique<AdvancedBotPlayer>(sign, 5);
        } else {
            return std::make_unique<AdvancedBotPlayer>(sign, 3);
        }
    }
};


std::unordered_map<std::string, std::shared_ptr<ServerGame>> games;


std::string generateUniqueId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1000, 9999);
    static int counter = 0;
    
    return "game_" + std::to_string(counter++) + "_" + std::to_string(dis(gen));
}


void broadcast(const std::string& gameId, const crow::json::wvalue& message) {
    std::lock_guard<std::mutex> lock(gamesMutex);
    auto it = games.find(gameId);
    if (it != games.end() && it->second) {
        for (auto conn : it->second->connections) {
            if (conn) {
                try {
                    conn->send_text(message.dump());
                } catch (const std::exception& e) {
                    std::cerr << "Error sending message: " << e.what() << std::endl;
                }
            }
        }
    }
}


void handleBotMove(std::shared_ptr<ServerGame> game, const std::string& gameId) {
    std::cout << "Bot's turn (" << game->currentSign << ")" << std::endl;
    
    
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    
    
    int botMove = game->getBotMove(game->currentSign);
    
    if (botMove != -1 && game->makeMove(botMove, game->currentSign)) {
        std::cout << "Bot made move: " << botMove << std::endl;
        
        crow::json::wvalue botMsg;
        if (game->checkWin(game->currentSign)) {
            botMsg["type"] = "win";
            botMsg["sign"] = game->currentSign;
            broadcast(gameId, botMsg);
        } else if (game->isFull()) {
            botMsg["type"] = "tie";
            broadcast(gameId, botMsg);
        } else {
            game->currentSign = (game->currentSign == "X") ? "O" : "X";
            botMsg["type"] = "update";
            botMsg["board"] = game->board->getBoardState();
            botMsg["currentSign"] = game->currentSign;
            broadcast(gameId, botMsg);
            
            
            if (game->isBot(game->currentSign)) {
                handleBotMove(game, gameId);
            }
        }
    } else {
        std::cout << "Bot failed to make a move" << std::endl;
    }
}

void startServer() {
    
    CROW_ROUTE(app, "/<path>")
    .methods("OPTIONS"_method)
    ([](const crow::request&, std::string) {
        crow::response res(204);
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        return res;
    });

    CROW_ROUTE(app, "/create_game")
    .methods("POST"_method, "OPTIONS"_method)
    ([](const crow::request& req) {
        if (req.method == "OPTIONS"_method) {
            crow::response res(204);
            return res;
        }

        std::cout << "Creating game via HTTP, received: " << req.body << std::endl;
        
        auto json = crow::json::load(req.body);
        if (!json) {
            crow::response res(400);
            res.body = "Invalid JSON";
            return res;
        }

        
        int boardSize = json.has("boardSize") ? json["boardSize"].i() : 3;
        int winCondition = json.has("winCondition") ? json["winCondition"].i() : 3;
        std::string player1Type = json.has("player1Type") ? std::string(json["player1Type"].s()) : "human";
        std::string player2Type = json.has("player2Type") ? std::string(json["player2Type"].s()) : "bot";


        if (boardSize < 3) boardSize = 3;
        if (boardSize > 10) boardSize = 10;
        if (winCondition < 3) winCondition = 3;
        if (winCondition > boardSize) winCondition = boardSize;

        std::lock_guard<std::mutex> lock(gamesMutex);
        if (games.size() >= MAX_GAMES) {
            crow::response res(503);
            res.body = "Maximum number of games reached";
            return res;
        }

        
        std::string gameId = generateUniqueId();
        auto game = std::make_shared<ServerGame>(boardSize, winCondition, player1Type, player2Type);
        games[gameId] = game;

      
        crow::json::wvalue response;
        response["gameId"] = gameId;
        response["boardSize"] = boardSize;
        response["message"] = "Game created successfully";

        crow::response res(200);
        res.body = response.dump();
        res.set_header("Content-Type", "application/json");
        
        std::cout << "Game created via HTTP: " << gameId << " with board size " << boardSize << std::endl;
        
        return res;
    });

  
    CROW_ROUTE(app, "/join_game")
    .methods("POST"_method, "OPTIONS"_method)
    ([](const crow::request& req) {
        if (req.method == "OPTIONS"_method) {
            crow::response res(204);
            return res;
        }

        std::cout << "Join request received: " << req.body << std::endl;
        
        auto json = crow::json::load(req.body);
        if (!json || !json.has("gameId")) {
            crow::response res(400);
            res.body = "Missing gameId";
            return res;
        }

        std::string gameId = json["gameId"].s();
        std::string playerId = json.has("playerId") ? json["playerId"].s() : "player_" + std::to_string(rand());

        std::lock_guard<std::mutex> lock(gamesMutex);
        auto it = games.find(gameId);
        if (it == games.end()) {
            crow::response res(404);
            res.body = "Game not found";
            return res;
        }

        auto& game = it->second;

        
        if (game->player1Id.empty() && dynamic_cast<HumanPlayer*>(game->player1.get())) {
            game->player1Id = playerId;
            std::cout << "Player 1 joined: " << playerId << " in game " << gameId << std::endl;
        } else if (game->player2Id.empty() && dynamic_cast<HumanPlayer*>(game->player2.get())) {
            game->player2Id = playerId;
            std::cout << "Player 2 joined: " << playerId << " in game " << gameId << std::endl;
        } else if (game->player1Id == playerId || game->player2Id == playerId) {
            std::cout << "Player reconnected: " << playerId << " in game " << gameId << std::endl;
        } else {
            crow::response res(403);
            res.body = "Game is full";
            return res;
        }

        
        crow::json::wvalue response;
        response["message"] = "Joined game";
        response["boardSize"] = game->board->getBoardSize();
        response["gameId"] = gameId;
        response["currentSign"] = game->currentSign;

        crow::response res(200);
        res.body = response.dump();
        res.set_header("Content-Type", "application/json");
        return res;
    });

    
    CROW_WEBSOCKET_ROUTE(app, "/ws")
    .onopen([&](crow::websocket::connection& /* conn */) {
        std::cout << "INFO: New WebSocket connection established" << std::endl;
    })
    .onclose([&](crow::websocket::connection& conn, const std::string& reason, uint16_t code) {
        auto gameIdPtr = static_cast<std::string*>(conn.userdata());
        if (gameIdPtr) {
            std::string gameId = *gameIdPtr;
            std::lock_guard<std::mutex> lock(gamesMutex);
            auto it = games.find(gameId);
            if (it != games.end() && it->second) {
                auto& game = it->second;
                game->connections.erase(
                    std::remove(game->connections.begin(), game->connections.end(), &conn),
                    game->connections.end()
                );
                std::cout << "INFO: WebSocket closed for game " << gameId 
                        << " reason=" << reason << " code=" << code << std::endl;
            }
            delete gameIdPtr;
            conn.userdata(nullptr);
        }
    })
    .onmessage([&](crow::websocket::connection& conn, const std::string& data, bool /*is_binary*/) {
        std::cout << "WebSocket message: " << data << std::endl;
        
        auto json = crow::json::load(data);
        if (!json || !json.has("action")) {
            std::cout << "ERROR: Invalid WebSocket message format: " << data << std::endl;
            return;
        }

        std::string action = json["action"].s();

        
        if (action == "ping") {
            conn.send_text("{\"type\":\"pong\"}");
            return;
        }
        
        
        if (action == "create_and_join") {
            int boardSize = json.has("boardSize") ? json["boardSize"].i() : 3;
            int winCondition = std::min(boardSize, 5);
            std::string player1Type = json.has("player1Type") ? std::string(json["player1Type"].s()) : std::string("human");
            std::string player2Type = json.has("player2Type") ? std::string(json["player2Type"].s()) : std::string("bot");
            std::string playerId = json.has("playerId") ? std::string(json["playerId"].s()) : std::string("");
            std::string gameId = json.has("gameId") ? std::string(json["gameId"].s()) : generateUniqueId();
            
            
            std::shared_ptr<ServerGame> newGame;
            {
                std::lock_guard<std::mutex> lock(gamesMutex);
                newGame = std::make_shared<ServerGame>(boardSize, winCondition, player1Type, player2Type);
                
                
                if (dynamic_cast<HumanPlayer*>(newGame->player1.get())) {
                    newGame->player1Id = playerId;
                }
                
                games[gameId] = newGame;
            }
            
            std::cout << "Game created via WebSocket: " << gameId << " with board size " << boardSize << std::endl;
            
            
            conn.userdata(new std::string(gameId));
            
            
            newGame->connections.push_back(&conn);
            
            
            crow::json::wvalue msg;
            msg["type"] = "update";
            msg["board"] = newGame->board->getBoardState();
            msg["currentSign"] = newGame->currentSign;
            msg["gameId"] = gameId;
            conn.send_text(msg.dump());
            
            
            if (newGame->isBot(newGame->currentSign)) {
                handleBotMove(newGame, gameId);
            }
            
            return;
        }
        
        
        if (!json.has("gameId")) {
            std::cout << "ERROR: Missing gameId in message: " << data << std::endl;
            conn.send_text("{\"type\":\"error\",\"message\":\"Missing gameId\"}");
            return;
        }
        
        std::string gameId = json["gameId"].s();
        std::shared_ptr<ServerGame> game;
        
        {
            std::lock_guard<std::mutex> lock(gamesMutex);
            auto it = games.find(gameId);
            if (it == games.end()) {
                std::cout << "ERROR: Game not found: " << gameId << std::endl;
                conn.send_text("{\"type\":\"error\",\"message\":\"Game not found\"}");
                return;
            }
            game = it->second;
        }

        
        if (action == "join") {
            
            if (!conn.userdata()) {
                conn.userdata(new std::string(gameId));
            }
            
            
            if (std::find(game->connections.begin(), game->connections.end(), &conn) == game->connections.end()) {
                game->connections.push_back(&conn);
            }

            
            crow::json::wvalue msg;
            msg["type"] = "update";
            msg["board"] = game->board->getBoardState();
            msg["currentSign"] = game->currentSign;
            conn.send_text(msg.dump());
            
            std::cout << "Player joined game " << gameId << " via WebSocket" << std::endl;

            
            if (game->isBot(game->currentSign)) {
                handleBotMove(game, gameId);
            }
        } 
        
        else if (action == "make_move") {
            if (!json.has("position") || !json.has("sign")) {
                std::cout << "ERROR: Missing position or sign in make_move: " << data << std::endl;
                return;
            }

            int position = json["position"].i();
            std::string sign = json["sign"].s();
            std::string playerId = json.has("playerId") ? std::string(json["playerId"].s()) : std::string("");

            
            bool validPlayer = false;
            if ((sign == "X" && dynamic_cast<HumanPlayer*>(game->player1.get()) && 
                 (playerId == game->player1Id || game->player1Id.empty())) ||
                (sign == "O" && dynamic_cast<HumanPlayer*>(game->player2.get()) && 
                 (playerId == game->player2Id || game->player2Id.empty()))) {
                validPlayer = true;
                
                
                if (sign == "X" && game->player1Id.empty()) {
                    game->player1Id = playerId;
                } else if (sign == "O" && game->player2Id.empty()) {
                    game->player2Id = playerId;
                }
            }

            
            if (!validPlayer || game->currentSign != sign) {
                std::cout << "ERROR: Invalid move attempt by " << sign << " player " << playerId << std::endl;
                conn.send_text("{\"type\":\"error\",\"message\":\"Not your turn\"}");
                return;
            }

            
            if (game->makeMove(position, sign)) {
                std::cout << "Player " << sign << " made move at position " << position << std::endl;
                
                
                crow::json::wvalue moveMsg;
                if (game->checkWin(sign)) {
                    moveMsg["type"] = "win";
                    moveMsg["sign"] = sign;
                    broadcast(gameId, moveMsg);
                } else if (game->isFull()) {
                    moveMsg["type"] = "tie";
                    broadcast(gameId, moveMsg);
                } else {
                    
                    game->currentSign = (sign == "X") ? "O" : "X";
                    moveMsg["type"] = "update";
                    moveMsg["board"] = game->board->getBoardState();
                    moveMsg["currentSign"] = game->currentSign;
                    broadcast(gameId, moveMsg);

                    
                    if (game->isBot(game->currentSign)) {
                        handleBotMove(game, gameId);
                    }
                }
            } else {
                std::cout << "ERROR: Invalid move position " << position << " by " << sign << std::endl;
                conn.send_text("{\"type\":\"error\",\"message\":\"Invalid move position\"}");
            }
        }
    });

    
    CROW_ROUTE(app, "/")
    ([]{
        crow::json::wvalue res;
        res["status"] = "ok";
        res["message"] = "Tic Tac Toe Server is running";
        return res;
    });

    
    CROW_ROUTE(app, "/games")
    ([]{
        std::lock_guard<std::mutex> lock(gamesMutex);
        crow::json::wvalue res;
        res["active_games"] = static_cast<int>(games.size());
        return res;
    });

    
    CROW_ROUTE(app, "/cleanup")
    .methods("POST"_method)
    ([]{
        std::lock_guard<std::mutex> lock(gamesMutex);
        return crow::response(200);
    });

    std::cout << "INFO: Server started on port 3000" << std::endl;
    app.port(3000)
       .multithreaded()
       .run();
}