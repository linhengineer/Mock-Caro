#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Player.h"
#include "Board.h"

#define FILE_PATH "Player.ini"
#define REPLAY_PATH "Replay.ini"

void new_game();
void add_player(Player &p);
void new_player(Player &p);
void old_player(Player &p);
bool name_exist(std::string n);
void player_management();
void display_all_player();
void display_header();
void edit_player();
void delete_player();
void find_player();
void find_same_level_player();
float get_win_rate(std::string n);
void replay();
void vec_to_ini(std::vector<std::string> *vec);
void find_same_win_rate(float win_rate_opp, std::string name_player, std::string name_opp, float err);

int main() {
    while (1) {
        std::cout << "1. New game\n2. Replay\n3. Player Management\n4. Quit" << std::endl;
        std::string i {};
        std::cin >> i;
        while (i != "1" && i != "2" && i != "3" && i != "4") {
            std::cout << "Invalid Selection!\nPlease enter a valid selection: ";
            std::cin >> i;
        }
        if (i == "1") {
            std::cout << "New Game" << std::endl;
            new_game();
        }
        else if (i == "2") {
            std::cout << "Replay" << std::endl;
            replay();
        }
        else if (i == "3") {
            std::cout << "Player Management" << std::endl;
            player_management();
        }
        else if (i == "4") {
            std::cout << "Quit\n";
            break;
        }       
    }
    std::cout << "Program ended" << std::endl;
    return 0;
}

void new_game() {
    Player p1, p2;
    std::cout << "Player 1 (O):" << std::endl; add_player(p1);
    std::cout << "Player 2 (X):" << std::endl; add_player(p2);
    std::cout << "Enter size (row and column) of boad game: ";
    int row, col;
    std::cin >> row >> col;
    Board *b = new Board(row, col);
    b->start_game(p1, p2);
    delete b;
    std::cout << "Finish game" << std::endl;
}

void add_player(Player &p) {
    std::cout << "1. Add new player\n2. Old player" << std::endl;
    std::string i {};
    std::cin >> i;
    while (i != "1" && i != "2") {
        std::cout << "Invalid Selection!\nPlease enter a valid selection: ";
        std::cin >> i;
    }
    if (i == "1") {
        new_player(p);
    }
    else if (i == "2") {
        old_player(p);
    }
}

bool name_exist (const std::string n) {
    std::ifstream my_file {FILE_PATH, std::ios::in};
    bool exist = false;
    if (!my_file.is_open()) {
        std::cerr << "Problem opening file" << std::endl;
    }
    std::string word_read {};
    while (my_file >> word_read) {
        if (word_read == n) {
            exist = true;
            break;
        }
        my_file >> word_read;
        my_file >> word_read;
        my_file >> word_read;
        my_file >> word_read;
    }
    my_file.close();
    return exist;
}

void new_player(Player &p) {
    std::cout << "Add new player\n";
    std::cout << "Enter new player name: ";
    std::string n {};
    std::cin >> n;
    while (name_exist(n)) {
        std::cout << "That name is already taken. Try a different one: " ;
        std::cin >> n;
    }
    p.name = n;
}

void old_player(Player &p) {
    std::cout << "Enter old player name: ";
    std::string n {};
    std::cin >> n;
    while (!name_exist(n)) {
        std::cout << "That name is not already taken. Try a different one: ";
        std::cin >> n;
    }
    std::ifstream my_file {FILE_PATH, std::ios::in};
    if (!my_file.is_open()) {
        std::cerr << "Problem opening file" << std::endl;
    }
    std::string word_read {};
    std::vector<std::string> *vec = new std::vector<std::string>;
    while (my_file >> word_read)
    {
        if (word_read == n) {
            p.name = n;
            my_file >> p.win;
            my_file >> p.lost;
            my_file >> p.draw;
            my_file >> p.win_rate;
        }
        else vec->push_back(word_read);
    }
    my_file.close();
    std::remove(FILE_PATH);
    vec_to_ini(vec);
    delete vec;
}

void player_management() {
    while (1) {
        std::cout << "1. Display all player\n2. Add new player\n3. Edit player\n4. Delete player\n5. Find player\n6. Find same level player\n7. Back" << std::endl;
        std::string i {};
        std::cin >> i;
        while (i != "1" && i != "2" && i != "3" && i != "4" && i != "5" && i != "6" && i != "7") {
            std::cout << "Invalid Selection!\nPlease enter a valid selection: ";
            std::cin >> i;
        }
        if (i == "1") {
            display_all_player();
        }
        else if (i == "2") {
            Player p;
            new_player(p);
            std::cout << "Successfully Added" << std::endl;
        }
        else if (i == "3") {
            edit_player();
        }
        else if (i == "4") {
            delete_player();
        }
        else if (i == "5") {
            find_player();
        }
        else if (i == "6") {
            find_same_level_player();
        }
        else if (i == "7") {
            break;
        }
    }
}

void display_all_player() {
    std::ifstream my_file {FILE_PATH, std::ios::in};
    if (!my_file.is_open()) {
        std::cerr << "Problem opening file" << std::endl;
    }
    display_header();
    char c {};
    while (my_file.get(c)) {
        std::cout << c;
    }
    my_file.close();
}

void display_header() {
    std::cout << std::setw(20) << std::left << "Name"
              << std::setw(10)  << "Win"
              << std::setw(10)  << "Lost"
              << std::setw(10)  << "Draw"
              << std::setw(10)  << "Win Rate %"
              << std::endl;
}

void edit_player() {
    Player p;
    old_player(p);
    std::cout << "Enter new name: ";
    std::string n {};
    std::cin >> n;
    while (name_exist(n)) {
        std::cout << "That name is already taken. Try a different one: " ;
        std::cin >> n;
    }
    p.name = n;
    std::cout << "Enter win, lost, draw numbers: ";
    std::cin >> p.win >> p.lost >> p.draw;
    std::cout << "Edit Successfully" << std::endl;
}

void delete_player() {
    std::cout << "Enter player name: ";
    std::string n {};
    std::cin >> n;
    while (!name_exist(n)) {
        std::cout << "That name is not already taken. Try a different one: ";
        std::cin >> n;
    }
    std::ifstream my_file {FILE_PATH, std::ios::in};
    if (!my_file.is_open()) {
        std::cerr << "Problem opening file" << std::endl;
    }
    std::string word_read {};
    std::vector<std::string> *vec = new std::vector<std::string>;
    while (my_file >> word_read)
    {
        if (word_read == n) {
            my_file >> word_read;
            my_file >> word_read;
            my_file >> word_read;
            my_file >> word_read;
        }
        else vec->push_back(word_read);
    }
    my_file.close();
    std::remove(FILE_PATH);
    vec_to_ini(vec);
    delete vec;
    std::cout << "Delete Successfully" << std::endl;
}

void find_player() {
    std::cout << "Enter player name: ";
    std::string n {};
    std::cin >> n;
    while (!name_exist(n)) {
        std::cout << "That name is not already taken. Try a different one: ";
        std::cin >> n;
    }
    std::ifstream my_file {FILE_PATH, std::ios::in};
    if (!my_file.is_open()) {
        std::cerr << "Problem opening file" << std::endl;
    }
    display_header();
    std::string word_read {};
    while (my_file >> word_read)
    {
        if (word_read == n) {
            int win, lost, draw;
            float win_rate;
            my_file >> win; my_file >> lost; my_file >> draw; my_file >> win_rate;
            std::cout << std::setw(20)  << std::left << n
                      << std::setw(10)  << win
                      << std::setw(10)  << lost
                      << std::setw(10)  << draw
                      << std::setw(10)  << std::setprecision(2) << std::fixed << win_rate
                      << std::endl;
        } 
    }
    my_file.close();
}

void find_same_level_player() {
    std::cout << "Enter player name: ";
    std::string n {};
    std::cin >> n;
    while (!name_exist(n)) {
        std::cout << "That name is not already taken. Try a different one: ";
        std::cin >> n;
    }
    std::ifstream my_file {FILE_PATH, std::ios::in};
    if (!my_file.is_open()) {
        std::cerr << "Problem opening file" << std::endl;
    }
    std::string word_read {};
    float win_rate, win_rate_sub;
    win_rate = get_win_rate(n);
    bool is_first = true;
    std::string opp_name {};
    while (my_file >> word_read)
    {
        if (word_read == n) {     // name
            my_file >> word_read; // win
            my_file >> word_read; // lost
            my_file >> word_read; // draw
            my_file >> word_read; // win_rate
        }
        else if (is_first) {
            int win, lost, draw;
            float win_rate_temp;
            my_file >> win; my_file >> lost; my_file >> draw; my_file >> win_rate_temp;
            win_rate_sub = abs(win_rate - win_rate_temp);
            opp_name = word_read;
            is_first = false;
        }
        else {
            int win, lost, draw;
            float win_rate_temp;
            my_file >> win; my_file >> lost; my_file >> draw; my_file >> win_rate_temp;
            if  (abs(win_rate - win_rate_temp) <= win_rate_sub) {
                win_rate_sub = abs(win_rate - win_rate_temp);
                opp_name = word_read;
            }
        }
    }
    my_file.close();
    std::cout << "Player same level: " << opp_name;
    find_same_win_rate(get_win_rate(opp_name), opp_name, n, 2);
}

float get_win_rate(std::string n) {
    std::ifstream my_file {FILE_PATH, std::ios::in};
    if (!my_file.is_open()) {
        std::cerr << "Problem opening file" << std::endl;
    }
    std::string word_read {};
    float win_rate;
    while (my_file >> word_read)
    {
        if (word_read == n) {     // name
            my_file >> word_read; // win
            my_file >> word_read; // lost
            my_file >> word_read; // draw
            my_file >> win_rate;
        } 
    }
    my_file.close();
    return win_rate;
}

void vec_to_ini(std::vector<std::string> *vec) {
    std::ofstream my_new_file {FILE_PATH, std::ios::out | std::ios::app};
    if (!my_new_file.is_open()) {
        std::cerr << "Problem opening file" << std::endl;
    }
    for (std::size_t i = 0 ; i < vec->size()/5; i++) {
        my_new_file << std::setw(20) << std::left << vec->at(5*i)
                    << std::setw(10)  << vec->at(5*i+1)
                    << std::setw(10)  << vec->at(5*i+2)
                    << std::setw(10)  << vec->at(5*i+3)
                    << std::setw(10)  << vec->at(5*i+4)
                    << std::endl;
    }
    my_new_file.close();
}

void replay() {
    std::ifstream in_file {REPLAY_PATH, std::ios::in};
    if (!in_file) {
        std::cerr << "Problem opening file" << std::endl;
    }
    char c {};
	while (in_file.get(c)) {
        std::cout << c;
    }
    in_file.close();
}

void find_same_win_rate(float win_rate_opp, std::string name_opp, std::string name_player, float err) {
    std::ifstream my_file {FILE_PATH, std::ios::in};
    if (!my_file.is_open()) {
        std::cerr << "Problem opening file" << std::endl;
    }
    std::string word_read {};
    while (my_file >> word_read)
    {
        std::string name_opp_temp = word_read;
        my_file >> word_read;
        my_file >> word_read;
        my_file >> word_read;
        float win_rate_temp;
        my_file >> win_rate_temp;
        if (abs(win_rate_opp - win_rate_temp) < err &&  name_opp_temp != name_player && name_opp_temp != name_opp) {
            std::cout << ", " << name_opp_temp;
        }
    }
    std::cout << "." << std::endl;
    my_file.close();
}