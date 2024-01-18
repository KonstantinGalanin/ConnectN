#include <vector>
#include <iostream>
#include <unordered_map>

enum pieceColor {
    EMPTY,
    YELLOW,
    RED
};

class Grid {
private:
    int rows;
    int columns;
    std::vector<std::vector<pieceColor>> grid;
public:

    Grid(int rows, int columns) : rows(rows), columns(columns) {
        initGrid();
    }

    void initGrid() {
        grid.resize(rows, std::vector<pieceColor> (columns, pieceColor::EMPTY));
        for(int i = 0; i < rows; ++i) {
            for(int j = 0; j < columns; ++j) {
                grid[i][j] = pieceColor::EMPTY;
            }
        }
    }

    const std::vector<std::vector<pieceColor>>* getGrid() {
        return &grid;
    }

    int getColumns() {
        return columns;
    }

    int placePiece(int col, pieceColor color) {
        if(col < 0 || col >= columns) {
            throw("Wrong place");
        }
        int line;
        for(line = rows - 1; line >= 0; --line) {
            if(grid[line][col] == pieceColor::EMPTY) {
                grid[line][col] = color;
                break;
            }
        }
        return line;
    }

    bool checkWin(pieceColor color, int N, int r, int c) {
        //horizontal
        int count = 0;
        for(int j = 0; j < columns; ++j) {
            if(grid[r][j] == color) {
                ++count;
            }
            else {
                count = 0;
            }
            if(count == N) {
                return true;
            }
        }

        //vertical
        count = 0;
        for(int i = 0; i < rows; ++i) {
            if(grid[i][c] == color) {
                ++count;
            }
            else {
                count = 0;
            }
            if(count == N) {
                return true;
            }
        }

        //diagonal
        count = 0;
        for(int i = 0; i < rows; ++i) {
            int j = r + c - i;
            if(j >=0 && j < columns && grid[i][j] == color) {
                ++count;
            }
            else {
                count = 0;
            }
            if(count == N) {
                return true;
            }
        }

        //anti-diagonal
        count = 0;
        for(int i = 0; i < rows; ++i) {
            int j = r - c + i;
            if(j >=0 && j < columns && grid[i][j] == color) {
                ++count;
            }
            else {
                count = 0;
            }
            if(count == N) {
                return true;
            }
        }

        return false;
    }
};

class Player {
private:
    std::string name;
    pieceColor color;
public:
    Player(std::string name, pieceColor color) : name(name), color(color){}

    std::string getName() {
        return name;
    }
    
    pieceColor getColor() {
        return color;
    }
};

class Game {
private:
    int connectN;
    int targetScore;
    std::vector<Player*> players;
    Grid* grid;
    std::unordered_map<Player*, int> wins;
    std::unordered_map<pieceColor, char> values;
public:
    Game(Grid* grid, int connectN, int targetScore) : grid(grid), connectN(connectN), targetScore(targetScore){
        Player* player1 = new Player("player1", pieceColor::RED);
        Player* player2 = new Player("player2", pieceColor::YELLOW);
        players = {player1, player2};
        for(auto& player : players) {
            wins[player] = 0;
        }
        setValues();
    }

    void setValues() {
        values[pieceColor::EMPTY] = '0';
        values[pieceColor::YELLOW] = 'Y';
        values[pieceColor::RED] = 'R';
    }

    void printBoard() {
        const std::vector<std::vector<pieceColor>>* board = grid->getGrid();
        std::cout << "Board:" << std::endl;
        for(auto& line : *board) {
            for(auto& val : line) {
                std::cout << values[val] << " ";
            }
            std::cout << std::endl;
        }
    }

    std::vector<int> makeMove(Player* player) {
        printBoard();
        std::cout << player->getName() << "'s turn" << std::endl;
        int makeCol;
        std::cout << "print column from 0 to " << grid->getColumns() - 1 << " to add piece: ";
        std::cin >> makeCol;
        int makeRow = grid->placePiece(makeCol, player->getColor());
        return {makeRow, makeCol};
    }

    Player* playRound() {
        while(true) {
            std::vector<int> coord;
            for(auto& player : players) {
                coord = makeMove(player);
                int r = coord[0];
                int c = coord[1];
                if(grid->checkWin(player->getColor(), connectN, r, c)) {
                    ++wins[player];
                    return player;
                }
            }
        }
    }

    void play() {
        int maxScore = 0;
        Player* winner;
        while(maxScore < targetScore) {
            winner = playRound();
            std::cout << "Won player: " << winner->getName() << std::endl;
            maxScore = std::max(maxScore, wins[winner]);
            grid->initGrid();
        }
        std::cout << "Final winner is " << winner->getName() << std::endl;
    }

    ~Game() {
        for(auto& player : players) {
            delete player;
        }
    }
};

int main() {
    Grid* grid = new Grid(6, 7);
    Game* game = new Game(grid, 4, 3);

    game->play();

    delete game;
    delete grid;

    return 0;
}