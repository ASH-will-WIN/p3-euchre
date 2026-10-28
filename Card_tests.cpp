#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;

TEST(test_card_ctor)
{
    Card c(ACE, HEARTS);
    Card d(KING, SPADES);
    Card e(QUEEN, DIAMONDS);
    Card f;
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
    ASSERT_EQUAL(KING, d.get_rank());
    ASSERT_EQUAL(SPADES, d.get_suit());
    ASSERT_EQUAL(QUEEN, e.get_rank());
    ASSERT_EQUAL(DIAMONDS, e.get_suit());
    ASSERT_EQUAL(f.get_rank(), TWO);
    ASSERT_EQUAL(f.get_suit(), SPADES);
}

TEST(test_get_rank)
{
    Card c(ACE, HEARTS);
    Card d(KING, SPADES);
    Card e(QUEEN, DIAMONDS);
    Card f;
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(KING, d.get_rank());
    ASSERT_EQUAL(QUEEN, e.get_rank());
    ASSERT_EQUAL(f.get_rank(), TWO);
}

TEST(test_get_suit)
{
    Card c(ACE, HEARTS);
    Card d(KING, SPADES);
    Card e(QUEEN, DIAMONDS);
    Card f;
    ASSERT_EQUAL(HEARTS, c.get_suit());
    ASSERT_EQUAL(SPADES, d.get_suit());
    ASSERT_EQUAL(DIAMONDS, e.get_suit());
    ASSERT_EQUAL(f.get_suit(), SPADES);
}

TEST(test_get_suit_trump)
{
    Card left(JACK, DIAMONDS);
    Card regular(NINE, DIAMONDS);
    Card right(JACK, HEARTS);

    ASSERT_EQUAL(left.get_suit(HEARTS), HEARTS);
    ASSERT_EQUAL(regular.get_suit(HEARTS), DIAMONDS);
    ASSERT_EQUAL(right.get_suit(HEARTS), HEARTS);
}

TEST(is_face_or_ace)
{
    Card c(ACE, HEARTS);
    Card d(KING, SPADES);
    Card e(QUEEN, DIAMONDS);
    Card f(JACK, CLUBS);
    Card g(TWO, SPADES);
    ASSERT_EQUAL(c.is_face_or_ace(), true);
    ASSERT_EQUAL(d.is_face_or_ace(), true);
    ASSERT_EQUAL(e.is_face_or_ace(), true);
    ASSERT_EQUAL(f.is_face_or_ace(), true);
    ASSERT_EQUAL(g.is_face_or_ace(), false);
}

TEST(is_right_bower)
{
    Suit trump1 = HEARTS;
    Suit trump2 = SPADES;
    Card c(JACK, HEARTS);
    Card d(JACK, SPADES);
    Card e(JACK, DIAMONDS);
    Card f(JACK, CLUBS);
    ASSERT_EQUAL(c.is_right_bower(trump1), true);
    ASSERT_EQUAL(d.is_right_bower(trump2), true);
    ASSERT_EQUAL(e.is_right_bower(trump1), false);
    ASSERT_EQUAL(f.is_right_bower(trump2), false);
    ASSERT_EQUAL(Card(ACE, HEARTS).is_right_bower(HEARTS), false);
    ASSERT_EQUAL(Card(ACE, DIAMONDS).is_left_bower(HEARTS), false);
}
// Add more test cases here

TEST(is_left_bower)
{
    Suit trump1 = HEARTS;
    Suit trump2 = SPADES;
    Card c(JACK, HEARTS);
    Card d(JACK, SPADES);
    Card e(JACK, DIAMONDS);
    Card f(JACK, CLUBS);
    ASSERT_EQUAL(c.is_left_bower(trump1), false);
    ASSERT_EQUAL(d.is_left_bower(trump2), false);
    ASSERT_EQUAL(e.is_left_bower(trump1), true);
    ASSERT_EQUAL(f.is_left_bower(trump2), true);
    ASSERT_EQUAL(Card(ACE, HEARTS).is_right_bower(HEARTS), false);
    ASSERT_EQUAL(Card(ACE, DIAMONDS).is_left_bower(HEARTS), false);
}

TEST(is_trump)
{
    Card right(JACK, HEARTS);
    Card left(JACK, DIAMONDS);
    Card regular_trump(NINE, HEARTS);
    Card non_trump(ACE, SPADES);

    ASSERT_EQUAL(right.is_trump(HEARTS), true);
    ASSERT_EQUAL(left.is_trump(HEARTS), true);
    ASSERT_EQUAL(regular_trump.is_trump(HEARTS), true);
    ASSERT_EQUAL(non_trump.is_trump(HEARTS), false);
}

TEST(card_equal)
{
    Card c(ACE, HEARTS);
    Card d(ACE, HEARTS);
    Card e(KING, SPADES);
    Card f(KING, CLUBS);
    ASSERT_EQUAL(c == d, true);
    ASSERT_EQUAL(c == e, false);
    ASSERT_EQUAL(e == f, false);
}

TEST(card_not_equal)
{
    Card c(ACE, HEARTS);
    Card d(KING, HEARTS);
    Card e(ACE, SPADES);
    ASSERT_EQUAL(c != d, true);
    ASSERT_EQUAL(c != e, true);
    ASSERT_EQUAL(c != c, false);
}

TEST(card_less_than)
{
    Card c(ACE, HEARTS);
    Card d(KING, SPADES);
    Card e(QUEEN, DIAMONDS);
    Card f(TWO, CLUBS);
    ASSERT_EQUAL(c < d, false);
    ASSERT_EQUAL(d < c, true);
    ASSERT_EQUAL(e < f, false);
    ASSERT_EQUAL(f < e, true);
}

TEST(card_less_equal_greater_equal)
{
    Card c(TWO, SPADES);
    Card d(TEN, SPADES);
    Card e(ACE, SPADES);
    ASSERT_EQUAL(c <= d, true);
    ASSERT_EQUAL(d <= c, false);
    ASSERT_EQUAL(e >= d, true);
    ASSERT_EQUAL(c >= d, false);
    ASSERT_EQUAL(c <= c, true);
    ASSERT_EQUAL(c >= c, true);
    ASSERT_EQUAL(e > d, true);
    ASSERT_EQUAL(d > e, false);
    ASSERT_EQUAL(c > c, false);
}

TEST(test_card_less)
{
    Card ace_spades(ACE, SPADES);
    Card nine_hearts(NINE, HEARTS);
    Card jack_hearts(JACK, HEARTS);
    ASSERT_EQUAL(Card_less(ace_spades, nine_hearts, HEARTS), true);
    ASSERT_EQUAL(Card_less(nine_hearts, ace_spades, HEARTS), false);
    ASSERT_EQUAL(Card_less(ace_spades, jack_hearts, HEARTS), true);
    ASSERT_EQUAL(Card_less(jack_hearts, ace_spades, HEARTS), false);
}

TEST(test_card_less_left_bower)
{
    Card left(JACK, DIAMONDS);
    Card ace_spades(ACE, SPADES);
    Card right(JACK, HEARTS);
    ASSERT_EQUAL(Card_less(ace_spades, left, HEARTS), true);
    ASSERT_EQUAL(Card_less(left, ace_spades, HEARTS), false);
    ASSERT_EQUAL(Card_less(left, right, HEARTS), true);
}

TEST(test_card_less_trump)
{
    Card n(NINE, HEARTS);
    Card k(KING, HEARTS);
    Card a(ACE, SPADES);
    ASSERT_EQUAL(Card_less(n, k, HEARTS), true);
    ASSERT_EQUAL(Card_less(k, n, HEARTS), false);
    ASSERT_EQUAL(Card_less(a, n, HEARTS), true);
}

TEST(test_suit_next)
{
    ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
    ASSERT_EQUAL(Suit_next(DIAMONDS), HEARTS);
    ASSERT_EQUAL(Suit_next(SPADES), CLUBS);
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
}

TEST(test_card_less_same_rank)
{
    Card s(ACE, SPADES);
    Card h(ACE, HEARTS);
    ASSERT_EQUAL(Card_less(s, h, CLUBS), true);
    ASSERT_EQUAL(Card_less(h, s, CLUBS), false);
    ASSERT_EQUAL(Card_less(s, s, CLUBS), false);
}

TEST(test_card_less_led_suit)
{
    Card ace_clubs(ACE, CLUBS);
    Card nine_spades(NINE, SPADES);
    Card led_card(NINE, SPADES);

    ASSERT_EQUAL(Card_less(ace_clubs, nine_spades, led_card, HEARTS), true);
    ASSERT_EQUAL(Card_less(nine_spades, ace_clubs, led_card, HEARTS), false);
}

TEST(test_card_less_left_bower_led)
{
    Card ace_clubs(ACE, CLUBS);
    Card nine_hearts(NINE, HEARTS);
    Card led_card(JACK, DIAMONDS);

    ASSERT_EQUAL(Card_less(ace_clubs, nine_hearts, led_card, HEARTS), true);
    ASSERT_EQUAL(Card_less(nine_hearts, ace_clubs, led_card, HEARTS), false);
}

TEST(test_card_output)
{
    Card c(ACE, HEARTS);
    ostringstream os;

    os << c;

    ASSERT_EQUAL(os.str(), "Ace of Hearts");
}

TEST(test_card_input)
{
    istringstream is("Queen of Diamonds");
    Card c;

    is >> c;

    ASSERT_EQUAL(c, Card(QUEEN, DIAMONDS));
}
TEST_MAIN()
