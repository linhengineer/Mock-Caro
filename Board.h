#ifndef _BOARD_H_
#define _BOARD_H_

#include <vector>
#include "Player.h"

class Board {
public:
    std::vector<std::vector<char>> board;
    Board();
    Board(int row, int col);
    ~Board();
    void display() const;
    void update(int x, int y, char s);
    bool check_win(int x, int y) const;
    void start_game(Player &p1, Player &p2);
    void save_game(Player &p1, Player &p2, bool draw);
};
#endif // _BOARD_H_