#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <ctime> 
#include "Board.h"

#define REPLAY_PATH "Replay.ini"

Board::Board() 
    : board {std::vector<std::vector<char>> (10 , std::vector<char> (10, ' '))} {
    std::cout << "A board game 10x10 has been created" << std::endl;
}

Board::Board(int row, int col) 
    : board {std::vector<std::vector<char>> (row , std::vector<char> (col, ' '))} {
    std::cout << "A board game " << row << "x" << col << " has been created" << std::endl;
}

Board::~Board() {

    std::cout << "Board game has been deleted" << std::endl;
}

void Board::display() const {
    std::cout << std::setw(3) << std::right << "\\|";
    for (std::size_t i = 0 ; i < board[0].size() ; i ++) {
        std::cout << std::setw(3) << std::left << i << "|";
    }
    std::cout << std::endl;
    int i = 0;
    for (const auto &vec : board) {
        std::cout << std::setw(2) << std::left << i << "|";
        i++;
        for (const auto &s : vec) {
            std::cout << std::setw(3) << std::left << s << "|";
        }
        std::cout << std::endl;
    }
}

void Board::update(int x, int y, char s) {
	while (x < 0 || x >= static_cast<int>(board.size()) || y < 0 || y >= static_cast<int>(board[0].size()) || board[x][y] != ' ') {
		std::cout << "Invalid Selection!\nPlease enter a valid selection: ";
		std::cin >> x >> y;
	}
    board[x][y] = s;
}

bool Board::check_win(int row, int col) const {
	int count;
	int row_temp, col_temp;
	int max_row = static_cast<int>(board.size());
	int max_col = static_cast<int>(board[0].size());

	// Check column
	count = 0; row_temp = row;
	while (row_temp < max_row && board[row_temp][col] == board[row][col]) {
		count++;
		row_temp++;
	}
	row_temp = row - 1;
	while (row_temp > -1 && board[row_temp][col] == board[row][col]) {
		count++;
		row_temp--;
	}
	if (count > 3) return true;

	// Check row
	count = 0; col_temp = col;
	while(col_temp < max_col && board[row][col_temp] == board[row][col]) {
		count++;
        col_temp++;
	}
	col_temp = col - 1;
	while(col_temp > -1 && board[row][col_temp] == board[row][col]) {
		count++;
		col_temp--;
	}
	if (count > 3) return true;

	// Check diagonal 1
	 count = 0; row_temp = row; col_temp = col;
	while (row_temp < max_row && col_temp < max_col && board[row_temp][col_temp] == board[row][col]) {
		count++;
		row_temp++;
		col_temp++;
	}
	row_temp = row - 1; col_temp = col - 1;
	while (row_temp > -1 && col_temp > -1 && board[row_temp][col_temp] == board[row][col]) {
		count++;
		row_temp--;
		col_temp--;
	}
	if (count > 3) return true;

	// Check diagonal 2
	count = 0; row_temp = row; col_temp = col; 
	while (row_temp < max_row && col_temp > -1 && board[row_temp][col_temp] == board[row][col]) {
		count++;
		row_temp++;
		col_temp--;
	}
	row_temp = row - 1; col_temp = col + 1;
	while (row_temp > -1 && col_temp < max_col && board[row_temp][col_temp] == board[row][col]) {
		count++;
		row_temp--;
		col_temp++;
	}
	if (count > 3) return true;

	// Nothing return false
	return false;
}

void Board::start_game(Player &p1, Player &p2) {
	this->display();
	for (int i = 0 ; i <= static_cast<int> (board.size() * board[0].size()) ; i++) {
        int x, y;
        if (i == static_cast<int> (board.size() * board[0].size())) {
            std::cout << p1.name << " and " << p2.name << " drew" << std::endl;
            p1.draw++;
            p2.draw++;
			save_game(p1, p2, true);
            break;
        }
        if (i % 2 == 0) {
            std::cout << "Player " << p1.name << " turn: ";
            std::cin >> x >> y; 
            this->update(x, y, 'O');
            this->display();
            if (this->check_win(x, y)) {
                std::cout << "Player " << p1.name << " win" << std::endl;
                p1.win++;
                p2.lost++;
				save_game(p1, p2, false);
                break;
            }
        }
        else {
            std::cout << "Player " << p2.name << " turn: ";
            std::cin >> x >> y; 
            this->update(x, y, 'X');
			this->display();
            if (this->check_win(x, y)) {
                std::cout << "Player " << p2.name << " win" << std::endl;
                p2.win++;
                p1.lost++;
				save_game(p2, p1, false);
                break;
            }
        }  
    }
}

void Board::save_game(Player &p1, Player &p2, bool draw) {
	std::ofstream out_file {REPLAY_PATH, std::ios::out | std::ios::app};
	auto time_now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(time_now);
    if (!out_file) {
        std::cerr << "Problem opening file" << std::endl;
    }
    out_file << std::ctime(&time);
	if (draw == true) {
		out_file << p1.name << "(O) and " << p2.name << "(X) drew" << std::endl;
	}
	else {
		out_file << p1.name << "(O) win " << p2.name << "(X)" << std::endl;
	}
	out_file << std::setw(3) << std::right << "\\|";
    for (std::size_t i = 0 ; i < board[0].size() ; i ++) {
        out_file << std::setw(3) << std::left << i << "|";
    }
    out_file << std::endl;
    int i = 0;
    for (const auto &vec : board) {
        out_file << std::setw(2) << std::left << i << "|";
        i++;
        for (const auto &s : vec) {
            out_file << std::setw(3) << std::left << s << "|";
        }
        out_file << std::endl;
    }
    out_file.close();
    out_file << "Game has been saved successfully" << std::endl;
}