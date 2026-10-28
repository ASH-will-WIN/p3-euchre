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
        array<Card, 12> first_half = array<Card, 12>{};
        for (int j = 0; j < 12; j++)
        {
            first_half[j] = cards[j];
        }
        array<Card, 12> second_half = array<Card, 12>{};
        for (int j = 0; j < 12; j++)
        {
            second_half[j] = cards[j + 12];
        }
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