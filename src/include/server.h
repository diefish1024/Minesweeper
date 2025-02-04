#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>

const int dx[] = {0, 0, 1, -1, 1, -1, 1, -1};  // The relative x coordinates of the 8 adjacent blocks
const int dy[] = {1, -1, 0, 0, 1, -1, -1, 1};  // The relative y coordinates of the 8 adjacent blocks

int rows;
int columns;
int total_mines;
int game_state;  // The state of the game, 0 for continuing, 1 for winning, -1 for losing. You MUST NOT modify its name.

class MineSweeperGame {
private:
    bool map[35][35];  // 1 - mine, 0 - no mine
    bool visited[35][35];  // 1 - visited, 0 - not visited
    bool marked[35][35];  // 1 - marked, 0 - not marked
    int mine_count[35][35];  // The number of mines around a block
    int marked_count;  // The number of blocks marked as mines
    int visit_count;  // The number of blocks visited

public:
    MineSweeperGame() {}
    MineSweeperGame(int r, int c) : visit_count(0), marked_count(0) {
        rows = r;
        columns = c;
        game_state = 0;
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                map[i][j] = false;
                visited[i][j] = false;
                marked[i][j] = false;
                mine_count[i][j] = 0;
            }
        }
    }

    void InitMap() {
        char ch;
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                std::cin >> ch;
                if(ch == 'X') {
                    map[i][j] = true;
                    total_mines++;
                    for(int k = 0; k < 8; k++) {
                        int x = i + dx[k];
                        int y = j + dy[k];
                        if(x >= 0 && x < rows && y >= 0 && y < columns) {
                            mine_count[x][y]++;
                        }
                    }
                }
            }
        }
    }

    void VisitBlock(int r, int c) {
        if(r < 0 || r >= rows || c < 0 || c >= columns || marked[r][c] || visited[r][c]) {
            return;
        }
        visited[r][c] = true;
        if(map[r][c]) {
            game_state = -1;
            return;
        }
        visit_count++;
        if(visit_count == rows * columns - total_mines) {
            game_state = 1;
            return;
        }
        if(!mine_count[r][c]) {
            for(int i = 0; i < 8; i++) {
                int x = r + dx[i];
                int y = c + dy[i];
                if(x >= 0 && x < rows && y >= 0 && y < columns && !visited[x][y]) {
                    VisitBlock(x, y);
                }
            }
        }
    }

    void MarkMine(int r, int c) {
        if(r < 0 || r >= rows || c < 0 || c >= columns || marked[r][c] || visited[r][c]) {
            return;
        }
        marked[r][c] = true;
        if(map[r][c]) {
            marked_count++;
        } else {
            game_state = -1;
        }
    }

    void AutoExplore(int r, int c) {
        if(r < 0 || r >= rows || c < 0 || c >= columns || !visited[r][c] || map[r][c]) {
            return;
        }
        int cnt = 0;
        for(int i = 0; i < 8; i++) {
            int x = r + dx[i];
            int y = c + dy[i];
            if(x >= 0 && x < rows && y >= 0 && y < columns && marked[x][y]) {
                cnt++;
            }
        }
        if(cnt == mine_count[r][c]) {
            for(int i = 0; i < 8; i++) {
                int x = r + dx[i];
                int y = c + dy[i];
                if(x >= 0 && x < rows && y >= 0 && y < columns && !visited[x][y] && !marked[x][y]) {
                    VisitBlock(x, y);
                }
            }
        }
    }

    void PrintMap() {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                if(visited[i][j] || marked[i][j]) {
                    if(marked[i][j]) {
                        std::cout << "@";
                    } else {
                        std::cout << mine_count[i][j];
                    }
                } else {
                    std::cout << "?";
                }
            }
            std::cout << std::endl;
        }
    }

    void PrintMap_win() {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                if(map[i][j]) {
                    std::cout << "@";
                } else {
                    std::cout << mine_count[i][j];
                }
            }
            std::cout << std::endl;
        }
    }

    void PrintMap_lose() {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                if(visited[i][j] || marked[i][j]) {
                    if((map[i][j] && visited[i][j]) || (!map[i][j] && marked[i][j])) {
                        std::cout << "X";
                    } else if(map[i][j]) {
                        std::cout << "@";
                    } else {
                        std::cout << mine_count[i][j];
                    }
                } else {
                    std::cout << "?";
                }
            }
            std::cout << std::endl;
        }
    }

    int getVisitCount() { return visit_count; }
    int getMarkedCount() { return game_state == 1 ? total_mines : marked_count; }

};

MineSweeperGame game;

/**
 * @brief The definition of function InitMap()
 *
 * @details This function is designed to read the initial map from stdin. For example, if there is a 3 * 3 map in which
 * mines are located at (0, 1) and (1, 2) (0-based), the stdin would be
 *     3 3
 *     .X.
 *     ...
 *     ..X
 * where X stands for a mine block and . stands for a normal block. After executing this function, your game map
 * would be initialized, with all the blocks unvisited.
 */
void InitMap() {
    std::cin >> rows >> columns;
    game = MineSweeperGame(rows, columns);
    game.InitMap();
}

/**
 * @brief The definition of function VisitBlock(int, int)
 *
 * @details This function is designed to visit a block in the game map. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call VisitBlock(0, 0), the return value would be 0 (game continues), and the game map would
 * be
 *     1??
 *     ???
 *     ???
 * If you call VisitBlock(0, 1) after that, the return value would be -1 (game ends and the players loses) , and the
 * game map would be
 *     1X?
 *     ???
 *     ???
 * If you call VisitBlock(0, 2), VisitBlock(2, 0), VisitBlock(1, 2) instead, the return value of the last operation
 * would be 1 (game ends and the player wins), and the game map would be
 *     1@1
 *     122
 *     01@
 *
 * @param r The row coordinate (0-based) of the block to be visited.
 * @param c The column coordinate (0-based) of the block to be visited.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 *
 * @note For invalid operation, you should not do anything.
 */
void VisitBlock(int r, int c) {
    game.VisitBlock(r, c);
}

/**
 * @brief The definition of function MarkMine(int, int)
 *
 * @details This function is designed to mark a mine in the game map.
 * If the block being marked is a mine, show it as "@".
 * If the block being marked isn't a mine, END THE GAME immediately. (NOTE: This is not the same rule as the real
 * game) And you don't need to
 *
 * For example, if we use the same map as before, and the current state is:
 *     1?1
 *     ???
 *     ???
 * If you call MarkMine(0, 1), you marked the right mine. Then the resulting game map is:
 *     1@1
 *     ???
 *     ???
 * If you call MarkMine(1, 0), you marked the wrong mine(There's no mine in grid (1, 0)).
 * The game_state would be -1 and game ends immediately. The game map would be:
 *     1?1
 *     X??
 *     ???
 * This is different from the Minesweeper you've played. You should beware of that.
 *
 * @param r The row coordinate (0-based) of the block to be marked.
 * @param c The column coordinate (0-based) of the block to be marked.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 *
 * @note For invalid operation, you should not do anything.
 */
void MarkMine(int r, int c) {
    game.MarkMine(r, c);
}

/**
 * @brief The definition of function AutoExplore(int, int)
 *
 * @details This function is designed to auto-visit adjacent blocks of a certain block.
 * See README.md for more information
 *
 * For example, if we use the same map as before, and the current map is:
 *     ?@?
 *     ?2?
 *     ??@
 * Then auto explore is available only for block (1, 1). If you call AutoExplore(1, 1), the resulting map will be:
 *     1@1
 *     122
 *     01@
 * And the game ends (and player wins).
 */
void AutoExplore(int r, int c) {
    game.AutoExplore(r, c);
}

/**
 * @brief The definition of function ExitGame()
 *
 * @details This function is designed to exit the game.
 * It outputs a line according to the result, and a line of two integers, visit_count and marked_mine_count,
 * representing the number of blocks visited and the number of marked mines taken respectively.
 *
 * @note If the player wins, we consider that ALL mines are correctly marked.
 */
void ExitGame() {
    std::cout << (game_state == 1 ? "YOU WIN!" : "GAME OVER!") << std::endl;
    std::cout << game.getVisitCount() << " " << game.getMarkedCount() << std::endl;
    exit(0);  // Exit the game immediately
}

/**
 * @brief The definition of function PrintMap()
 *
 * @details This function is designed to print the game map to stdout. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call PrintMap(), the stdout would be
 *    ???
 *    ???
 *    ???
 * If you call VisitBlock(2, 0) and PrintMap() after that, the stdout would be
 *    ???
 *    12?
 *    01?
 * If you call VisitBlock(0, 1) and PrintMap() after that, the stdout would be
 *    ?X?
 *    12?
 *    01?
 * If the player visits all blocks without mine and call PrintMap() after that, the stdout would be
 *    1@1
 *    122
 *    01@
 * (You may find the global variable game_state useful when implementing this function.)
 *
 * @note Use std::cout to print the game map, especially when you want to try the advanced task!!!
 */
void PrintMap() {
    if(game_state == 1) {
        game.PrintMap_win();
    } else if(game_state == -1) {
        game.PrintMap_lose();
    } else {
        game.PrintMap();
    }
}

#endif
