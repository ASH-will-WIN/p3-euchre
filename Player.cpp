#include "Player.hpp"
#include <string>
#include <vector>
#include <cassert>
#include "Card.hpp"
#include <algorithm>

using namespace std;

class SimplePlayer : public Player
{
public:
    SimplePlayer(string playerName) : name(playerName) {}

    const string &get_name() const
    {
        return name;
    }

    void add_card(const Card &c)
    {
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);
    }

    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, Suit &order_up_suit) const
    {
        assert(round == 1 || round == 2);
        if (round == 1)
        {
            int trump_count = 0;
            for (int i = 0; i < hand.size(); i++)
            {
                if (hand[i].is_face_or_ace() && hand[i].is_trump(upcard.get_suit()))
                {
                    trump_count++;
                }
            }

            if (trump_count >= 2)
            {
                order_up_suit = upcard.get_suit();
                return true;
            }
            return false;
        }
        else if (round == 2)
        {
            for (int i = 0; i < hand.size(); i++)
            {
                if (hand[i].is_face_or_ace() &&
                    hand[i].is_trump(Suit_next(upcard.get_suit())))
                {
                    order_up_suit = Suit_next(upcard.get_suit());
                    return true;
                }
            }

            if (is_dealer)
            {
                order_up_suit = Suit_next(upcard.get_suit());
                return true;
            }
        }
        return false;
    }

    void add_and_discard(const Card &upcard)
    {
        assert(hand.size() >= 1);
        hand.push_back(upcard);
        int worst_index = 0;
        for (int i = 0; i < hand.size(); i++)
        {
            if (Card_less(hand[i], hand[worst_index], upcard.get_suit()))
            {
                worst_index = i;
            }
        }
        hand.erase(hand.begin() + worst_index);
    }

    Card lead_card(Suit trump)
    {
        assert(hand.size() >= 1);
        bool all_trump = true;
        int highest_trump = -1;
        int highest_non_trump = -1;

        for (int i = 0; i < hand.size(); i++)
        {
            if (!hand[i].is_trump(trump))
            {
                all_trump = false;
            }

            if (hand[i].is_trump(trump) &&
                (highest_trump == -1 ||
                 Card_less(hand[highest_trump], hand[i], trump)))
            {
                highest_trump = i;
            }
            if (!hand[i].is_trump(trump) &&
                (highest_non_trump == -1 ||
                 Card_less(hand[highest_non_trump], hand[i], trump)))
            {
                highest_non_trump = i;
            }
        }

        if (all_trump)
        {
            Card final = hand[highest_trump];
            hand.erase(hand.begin() + highest_trump);
            return final;
        }
        else
        {
            Card final = hand[highest_non_trump];
            hand.erase(hand.begin() + highest_non_trump);
            return final;
        }
    }

    Card play_card(const Card &led_card, Suit trump)
    {
        assert(hand.size() >= 1);
        bool follow_suit = false;
        int highest_suit = -1;
        int lowest_index = -1;

        for (int i = 0; i < hand.size(); i++)
        {
            if (hand[i].get_suit(trump) == led_card.get_suit(trump))
            {
                follow_suit = true;

                if (highest_suit == -1 ||
                    Card_less(hand[highest_suit], hand[i], trump))
                {
                    highest_suit = i;
                }
            }
            else if (lowest_index == -1 ||
                     Card_less(hand[i], hand[lowest_index], trump))
            {
                lowest_index = i;
            }
        }

        if (follow_suit)
        {
            Card final = hand[highest_suit];
            hand.erase(hand.begin() + highest_suit);
            return final;
        }
        else
        {
            Card final = hand[lowest_index];
            hand.erase(hand.begin() + lowest_index);
            return final;
        }
    }

private:
    vector<Card> hand;
    string name;
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(string playerName) : name(playerName) {}

    const string &get_name() const
    {
        return name;
    }

    void add_card(const Card &c)
    {
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);

        sort(hand.begin(), hand.end());
    }

    void print_hand() const
    {
        for (size_t i = 0; i < hand.size(); ++i)
        {
            cout << "Human player " << name << "'s hand: "
                 << "[" << i << "] " << hand[i] << "\n";
        }
    }

    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, Suit &order_up_suit) const
    {
        print_hand();
        cout << "Human player " << name
             << ", please enter a suit, or \"pass\":\n";
        string input;
        cin >> input;
        if (input == "pass")
        {
            return false;
        }
        else
        {
            order_up_suit = string_to_suit(input);
            return true;
        }
    }

    void add_and_discard(const Card &upcard)
    {
        assert(hand.size() >= 1);

        print_hand();
        cout << "Discard upcard: [-1]\n";
        cout << "Human player " << name
             << ", please select a card to discard:\n";
        int discard_card;
        cin >> discard_card;
        if (discard_card != -1)
        {
            hand.erase(hand.begin() + discard_card);
            add_card(upcard);
        }
    }

    Card lead_card(Suit trump)
    {
        assert(hand.size() >= 1);
        print_hand();
        cout << "Human player " << name
             << ", please select a card:\n";
        int selected;
        cin >> selected;
        Card lead = hand[selected];
        hand.erase(hand.begin() + selected);
        return lead;
    }

    Card play_card(const Card &led_card, Suit trump)
    {
        assert(hand.size() >= 1);
        print_hand();
        cout << "Human player " << name
             << ", please select a card:\n";
        int selected;
        cin >> selected;
        Card play = hand[selected];
        hand.erase(hand.begin() + selected);
        return play;
    }

private:
    string name;
    vector<Card> hand;
};

// EFFECTS: Returns a pointer to a player with the given name and strategy
// To create an object that won't go out of scope when the function returns,
// use "return new Simple(name)" or "return new Human(name)"
// Don't forget to call "delete" on each Player* after the game is over
Player *Player_factory(const std::string &name, const std::string &strategy)
{
    if (strategy == "Simple")
    {
        return new SimplePlayer(name);
    }
    else if (strategy == "Human")
    {
        return new HumanPlayer(name);
    }
    else
    {
        assert(false);
        return nullptr;
    }
}

// EFFECTS: Prints player's name to os
std::ostream &operator<<(std::ostream &os, const Player &p)
{
    os << p.get_name();
    return os;
}
