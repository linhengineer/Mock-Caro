#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Player.h"

#define FILE_PATH "Player.ini"

Player::Player() 
    : name{"Guest"}, win{0}, lost{0}, draw{0}, win_rate{0} {
}

Player::~Player() {
    std::ofstream out_file {FILE_PATH, std::ios::out | std::ios::app};
    if (!out_file) {
        std::cerr << "Problem opening file" << std::endl;
    }
    if (win + lost + draw == 0) win_rate = 0;
    else win_rate = (static_cast<float> (win)/(static_cast<float>(win + lost + draw)))*100;
    out_file << std::setw(20) << std::left << name
             << std::setw(10) << win
             << std::setw(10) << lost
             << std::setw(10) << draw
             << std::setw(10) << std::setprecision(2) << std::fixed << win_rate
             << std::endl;
    out_file.close();
    std::cout << name << " has been saved successfully" << std::endl;
}