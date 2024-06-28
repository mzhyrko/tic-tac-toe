# Tic Tac Toe Ultimate

## Description
Tic Tac Toe Ultimate is a versatile implementation of the classic Tic Tac Toe game. It currently supports simple 3x3 gameplay as well as customizable grid sizes, offering a more challenging experience.

### Features
- **Version 1: Terminal-Based Tic Tac Toe**
  - A simple 3x3 grid game for two players in the terminal.
  - Players input moves by typing the cell number.
  - Game checks for valid moves, displays the board, and announces the winner or tie.
- **Version 2: n x n Grid Tic Tac Toe**
  - Customizable grid size (e.g., 4x4, 5x5).
  - Adjustable win conditions based on grid size.

## Badges
![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Version](https://img.shields.io/badge/version-2.0-blue)

## Installation
### Requirements
- C++ compiler (e.g., g++, clang++)
- Standard Library

### Steps
1. Clone the repository:
   ```sh
   git clone https://gitlab.com/mikhail-zhirko/tic-tac-toe.git
   ```
2. Navigate to the project directory:
   ```sh
   cd tic-tac-toe
   ```
3. Compile the project:
   ```sh
   g++ -o tictactoe ttt.cpp
   ```
4. Run the executable:
   ```sh
   ./tictactoe
   ```

## Usage
### Starting the Game
1. Launch the game executable.
2. Follow the on-screen instructions to select the board size, game mode, and player types.
3. Players take turns inputting their moves, and the game updates the board after each move.
4. The game announces the winner or a tie when the game ends.

### Example
```
Board size (3 - 1024): 4
Game modes:
1 - Human vs Human
Select game mode: 1
Player X, enter your move: 5
```

## Support
For help and support, you can:
- Open an issue on the [GitHub repository](https://gitlab.com/mikhail-zhirko/tic-tac-toe).
- Contact the author at [youremail@example.com](mailto:mikhail.zhirko@example.com).

## Roadmap
- **Version 3**: AI Opponent with multiple difficulty levels.
- **Version 4**: GUI-Based Tic Tac Toe App.
- **Version 5**: Online Multiplayer.
- **Version 6**: Points System with player profiles and leaderboards.

## Contributing
Contributions are welcome! Please follow these steps to contribute:
1. Fork the repository.
2. Create a new branch for your feature or bugfix:
   ```sh
   git checkout -b feature-name
   ```
3. Make your changes and commit them:
   ```sh
   git commit -m "Description of your changes"
   ```
4. Push to your branch:
   ```sh
   git push origin feature-name
   ```
5. Create a pull request detailing your changes.

## Authors and Acknowledgments
- **Author**: Mikhail Zhyrko

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Project Status
This project is actively maintained. For any updates or to become a maintainer, please contact the author.