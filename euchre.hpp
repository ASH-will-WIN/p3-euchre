#ifndef EUCHRE_HPP
#define EUCHRE_HPP

#include "Player.hpp"
#include "Card.hpp"
#include "Pack.hpp"

class Game
{
public:
    Game(bool shuffle, int pts_to_win, std::vector<Player *> players, std::istream &pack_input);
    void play();

private:
    Pack pack;
    std::vector<Player *> players;
    int pts_to_win;
    int dealer = 0;
    int team1_score = 0;
    int team2_score = 0;
    bool noshuffle;

    void shuffle();
    Card deal();
    int make_trump(Suit &trump_suit, Card &upcard);
    int play_trick(int start, Suit &trump_suit);
    void play_hand(Suit &trump_suit, Card &upcard);
};

#endif
