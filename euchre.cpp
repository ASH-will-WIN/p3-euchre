#include "Player.hpp"
#include "Card.hpp"
#include "Pack.hpp"
#include <iostream>
#include <fstream>

using namespace std;

class Game
{
public:
    Game(bool shuffle, int pts_to_win, std::vector<Player *> players, std::istream &pack_input);
    void play();

private:
    Pack pack;
    vector<Player *> players;
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

Game::Game(bool noshuffle, int pts_to_win, vector<Player *> players, istream &pack_input)
{
    this->pack = Pack(pack_input);
    this->noshuffle = noshuffle;
    this->pts_to_win = pts_to_win;
    this->players = players;
}

void Game::shuffle()
{
    pack.shuffle();
}

Card Game::deal()
{
    std::vector<int> deal_order = {3, 2, 3, 2, 2, 3, 2, 3};
    for (size_t i = 0; i < deal_order.size(); ++i)
    {
        for (int j = 0; j < deal_order[i]; ++j)
        {
            players[(dealer + 1 + i) % players.size()]->add_card(pack.deal_one());
        }
    }

    return pack.deal_one();
}

int Game::make_trump(Suit &trump_suit, Card &upcard)
{
    for (int i = 0; i < players.size(); i++)
    {
        if (players[(i + dealer + 1) % players.size()]->make_trump(upcard, (i + dealer + 1) % players.size() == dealer, 1, trump_suit))
        {
            cout << players[(i + dealer + 1) % players.size()]->get_name() << " orders up " << trump_suit << endl;
            players[dealer]->add_and_discard(upcard);
            cout << endl;
            return (i + dealer + 1) % players.size();
        }
        cout << players[(i + dealer + 1) % players.size()]->get_name() << " passes" << endl;
    }

    for (int i = 0; i < players.size(); i++)
    {
        if (players[(i + dealer + 1) % players.size()]->make_trump(upcard, (i + dealer + 1) % players.size() == dealer, 2, trump_suit))
        {
            cout << players[(i + dealer + 1) % players.size()]->get_name() << " orders up " << trump_suit << endl
                 << endl;
            return (i + dealer + 1) % players.size();
        }

        cout << players[(i + dealer + 1) % players.size()]->get_name() << " passes" << endl;
    }

    return -1;
}

void Game::play_hand(Suit &trump_suit, Card &upcard)
{
    int maker = make_trump(trump_suit, upcard) % 2 + 1;
    int team1_hand_score = 0;
    int team2_hand_score = 0;

    int last_trick_winner = (dealer + 1) % players.size();

    for (int i = 0; i < 5; i++)
    {
        int winner = play_trick(last_trick_winner, trump_suit);
        last_trick_winner = winner;
        if (winner % 2 + 1 == 1)
        {
            team1_hand_score++;
        }
        else
        {
            team2_hand_score++;
        }
    }

    if (maker == 1)
    {
        if (team1_hand_score > 4)
        {
            team1_score += 2;
            cout << players[0]->get_name() << " and " << players[2]->get_name() << " win the hand" << endl;
            cout << "march!"
                 << endl;
        }
        else if (team1_hand_score >= 3)
        {
            team1_score += 1;
            cout << players[0]->get_name() << " and " << players[2]->get_name() << " win the hand" << endl;
        }
        else
        {
            team2_score += 2;
            cout << players[1]->get_name() << " and " << players[3]->get_name() << " win the hand" << endl;
            cout << "euchred!" << endl;
        }
    }
    else
    {
        if (team2_hand_score > 4)
        {
            team2_score += 2;
            cout << players[1]->get_name() << " and " << players[3]->get_name() << " win the hand" << endl;
            cout << "march!"
                 << endl;
        }
        else if (team2_hand_score >= 3)
        {
            team2_score += 1;
            cout << players[1]->get_name() << " and " << players[3]->get_name() << " win the hand" << endl;
        }
        else
        {
            team1_score += 2;
            cout << players[0]->get_name() << " and " << players[2]->get_name() << " win the hand" << endl;
            cout << "euchred!" << endl;
        }
    }

    cout << players[0]->get_name() << " and " << players[2]->get_name() << " have " << team1_score << " points" << endl;
    cout << players[1]->get_name() << " and " << players[3]->get_name() << " have " << team2_score << " points" << endl
         << endl;
}

int Game::play_trick(int start, Suit &trump_suit)
{
    int max_card_player;
    Card max_card;
    Card led_card;

    for (int i = 0; i < 4; i++)
    {
        if (i == 0)
        {
            Card played = players[(start + i) % players.size()]->lead_card(trump_suit);
            max_card = played;
            max_card_player = (start + i) % players.size();
            led_card = max_card;

            cout << played << " led by " << players[(start + i) % players.size()]->get_name() << endl;
        }
        else
        {
            Card played = players[(start + i) % players.size()]->play_card(led_card, trump_suit);
            if (Card_less(max_card, played, led_card, trump_suit))
            {
                max_card_player = (start + i) % players.size();
                max_card = played;
            }
            cout << played << " played by " << players[(start + i) % players.size()]->get_name() << endl;
        }
    }

    cout << players[max_card_player]->get_name() << " takes the trick" << endl
         << endl;
    return max_card_player;
}

void Game::play()
{
    int hand_count = 0;

    Suit trump_suit;
    Card upcard;

    while (team1_score < pts_to_win && team2_score < pts_to_win)
    {
        if (!noshuffle)
        {
            shuffle();
        }
        else
        {
            pack.reset();
        }

        upcard = deal();

        cout << "Hand " << hand_count << endl;
        cout << players[dealer]->get_name() << " deals" << endl;
        cout << upcard << " turned up" << endl;

        play_hand(trump_suit, upcard);
        hand_count++;
        dealer = (dealer + 1) % 4;
    }

    if (team1_score >= pts_to_win)
    {
        cout << players[0]->get_name() << " and " << players[2]->get_name() << " win!" << endl;
    }
    else
    {
        cout << players[1]->get_name() << " and " << players[3]->get_name() << " win!" << endl;
    }

    for (size_t i = 0; i < players.size(); ++i)
    {
        delete players[i];
    }
}

int main(int argc, char **argv)
{
    if (argc == 12 && stoi(argv[3]) >= 1 && stoi(argv[3]) <= 100 && (string(argv[2]) == "shuffle" || string(argv[2]) == "noshuffle") && (string(argv[5]) == "Simple" || string(argv[5]) == "Human") && (string(argv[7]) == "Simple" || string(argv[7]) == "Human") && (string(argv[9]) == "Simple" || string(argv[9]) == "Human") && (string(argv[11]) == "Simple" || string(argv[11]) == "Human"))
    {

        for (int i = 0; i < 12; i++)
        {
            cout << argv[i] << " ";
        }
        cout << endl;

        bool noshuffle = (string(argv[2]) == "noshuffle");
        int pts_to_win = stoi(argv[3]);

        ifstream pack_input(argv[1]);
        if (!pack_input)
        {
            cout << "Error opening " << argv[1] << endl;
            return -1;
        }
        vector<Player *> players;
        for (int i = 0; i < 8; i += 2)
        {
            players.push_back(Player_factory(string(argv[i + 4]), string(argv[i + 5])));
        }
        Game game(noshuffle, pts_to_win, players, pack_input);
        game.play();
    }
    else
    {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;

        return -1;
    }
}
