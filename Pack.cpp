#include "Pack.hpp"
#include <array>
#include <string>
#include <iostream>

using namespace std;

Pack::Pack()
{
    int ct = 0;
    for (int i = SPADES; i <= DIAMONDS; i++)
    {
        for (int j = NINE; j <= ACE; j++)
        {
            cards[ct] = Card(static_cast<Rank>(j), static_cast<Suit>(i));
            ct++;
        }
    }

    next = 0;
}

Pack::Pack(std::istream &pack_input)
{
    for (int i = 0; i < 24; i++)
    {
        pack_input >> cards[i];
    }

    next = 0;
}

Card Pack::deal_one()
{
    int old = next;
    next++;
    return cards[old];
}

void Pack::reset()
{
    next = 0;
}

void Pack::shuffle()
{
    for (int i = 0; i < 7; i++)
    {
        array<Card, 12> first_half = array<Card, 12>{cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6], cards[7], cards[8], cards[9], cards[10], cards[11]};
        array<Card, 12> second_half = array<Card, 12>{cards[12], cards[13], cards[14], cards[15], cards[16], cards[17], cards[18], cards[19], cards[20], cards[21], cards[22], cards[23]};
        array<Card, 24> shuffled;
        for (int j = 0; j < 24; j++)
        {
            if (j % 2 == 0)
            {
                shuffled[j] = second_half[j / 2];
            }
            else
            {
                shuffled[j] = first_half[j / 2];
            }
        }
        cards = shuffled;
    }
    next = 0;
}

bool Pack::empty() const
{
    return next >= 24;
}