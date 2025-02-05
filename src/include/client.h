#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <cstring>

extern int rows;         // The count of rows of the game map.
extern int columns;      // The count of columns of the game map.
extern int total_mines;  // The count of mines of the game map.

// You MUST NOT use any other external variables except for rows, columns and total_mines.

const int c_dx[] = {0, 0, 1, -1, 1, -1, 1, -1};
const int c_dy[] = {1, -1, 0, 0, 1, -1, -1, 1};

/**
 * @brief The definition of function Execute(int, int, bool)
 *
 * @details This function is designed to take a step when player the client's (or player's) role, and the implementation
 * of it has been finished by TA. (I hope my comments in code would be easy to understand T_T) If you do not understand
 * the contents, please ask TA for help immediately!!!
 *
 * @param r The row coordinate (0-based) of the block to be visited.
 * @param c The column coordinate (0-based) of the block to be visited.
 * @param type The type of operation to a certain block.
 * If type == 0, we'll execute VisitBlock(row, column).
 * If type == 1, we'll execute MarkMine(row, column).
 * If type == 2, we'll execute AutoExplore(row, column).
 * You should not call this function with other type values.
 */
void Execute(int r, int c, int type);

int c_map[35][35];  // -1 - unknown, 0-8 - number of mines around, -2 - mine
int marked_count[35][35];
int unknown_count[35][35];
int marked[35][35];

// operatoration queue - (pos, type)
std::queue<std::pair<std::pair<int, int>, int>> op_queue;

/**
 * @brief The definition of function InitGame()
 *
 * @details This function is designed to initialize the game. It should be called at the beginning of the game, which
 * will read the scale of the game map and the first step taken by the server (see README).
 */
void InitGame() {
    // TODO (student): Initialize all your global variables!
    memset(c_map, 0, sizeof(c_map));
    memset(marked_count, 0, sizeof(marked_count));
    memset(unknown_count, 0, sizeof(unknown_count));
    op_queue = std::queue<std::pair<std::pair<int, int>, int>>();
    int first_row, first_column;
    std::cin >> first_row >> first_column;
    Execute(first_row, first_column, 0);
}

/**
 * @brief The definition of function ReadMap()
 *
 * @details This function is designed to read the game map from stdin when playing the client's (or player's) role.
 * Since the client (or player) can only get the limited information of the game map, so if there is a 3 * 3 map as
 * above and only the block (2, 0) has been visited, the stdin would be
 *     ???
 *     12?
 *     01?
 */
void ReadMap() {
    memset(marked_count, 0, sizeof(marked_count));
    memset(unknown_count, 0, sizeof(unknown_count));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            char c;
            std::cin >> c;
            if (c == '?') {
                // The block is unknown.
                c_map[i][j] = -1;
                for(int k = 0; k < 8; ++k) {
                    int x = i + c_dx[k];
                    int y = j + c_dy[k];
                    if(x >= 0 && x < rows && y >= 0 && y < columns) {
                        ++unknown_count[x][y];
                    }
                }
            } else if(c == '@'){
                // The block has been marked.
                c_map[i][j] = -2;
                for(int k = 0; k < 8; ++k) {
                    int x = i + c_dx[k];
                    int y = j + c_dy[k];
                    if(x >= 0 && x < rows && y >= 0 && y < columns) {
                        ++marked_count[x][y];
                    }
                }
            } else {
                // The block has been visited.
                c_map[i][j] = c - '0';
            }
        }
    }
}

// some point may be added multiple times, but it doesn't matter
void SimpleDectect() {
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < columns; ++j) {
            if(c_map[i][j] > 0 && marked_count[i][j] + unknown_count[i][j] == c_map[i][j]) {
                for(int k = 0; k < 8; ++k) {
                    int x = i + c_dx[k];
                    int y = j + c_dy[k];
                    if(x >= 0 && x < rows && y >= 0 && y < columns && !marked[x][y] && c_map[x][y] == -1) {
                        op_queue.push({{x, y}, 1});
                        marked[x][y] = true;
                    }
                }
            }
            if(c_map[i][j] > 0 && marked_count[i][j] == c_map[i][j] && unknown_count[i][j] > 0) {
                op_queue.push({{i, j}, 2});
            }
        }
    }
}

/**
 * @brief The definition of function Decide()
 *
 * @details This function is designed to decide the next step when playing the client's (or player's) role. Open up your
 * mind and make your decision here!
 */

/*
first: simple detect
second: randomly
*/

void Decide() {
    if(op_queue.empty()) {
        SimpleDectect();
    }
    if(!op_queue.empty()) {
        auto [pos, type] = op_queue.front();
        op_queue.pop();
        Execute(pos.first, pos.second, type);
    } else {
        // randomly choose a point (unknow) to visit
        int x, y;
        do {
            x = rand() % rows;
            y = rand() % columns;
        } while(c_map[x][y] != -1);
        Execute(x, y, 0);
    }
}

#endif