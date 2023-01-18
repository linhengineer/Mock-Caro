#ifndef _PLAYER_H_
#define _PLAYER_H_
class Player {
public:
    std::string name;
    int win;
    int lost;
    int draw;
    float win_rate;
    Player();
    ~Player();
};
#endif // _PLAYER_H_