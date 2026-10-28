#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name)
{
    Player *alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_make_trump_round1_yes)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(ACE, HEARTS));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(TEN, CLUBS));
    p->add_card(Card(QUEEN, DIAMONDS));

    Suit trump = SPADES;
    Card upcard(NINE, HEARTS);

    bool result = p->make_trump(upcard, false, 1, trump);

    ASSERT_EQUAL(result, true);
    ASSERT_EQUAL(trump, HEARTS);

    delete p;
}

TEST(test_make_trump_round1_no)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(ACE, HEARTS));
    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(QUEEN, CLUBS));
    p->add_card(Card(KING, DIAMONDS));

    Suit trump = SPADES;
    Card upcard(NINE, HEARTS);

    bool result = p->make_trump(upcard, false, 1, trump);

    ASSERT_EQUAL(result, false);
    ASSERT_EQUAL(trump, SPADES);

    delete p;
}

TEST(test_make_trump_round2_yes)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(ACE, DIAMONDS));
    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(QUEEN, CLUBS));
    p->add_card(Card(KING, SPADES));

    Suit trump = SPADES;
    Card upcard(NINE, HEARTS);

    bool result = p->make_trump(upcard, false, 2, trump);

    ASSERT_EQUAL(result, true);
    ASSERT_EQUAL(trump, DIAMONDS);

    delete p;
}

TEST(test_make_trump_dealer)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(QUEEN, CLUBS));
    p->add_card(Card(ACE, SPADES));

    Suit trump = SPADES;
    Card upcard(NINE, HEARTS);

    bool result = p->make_trump(upcard, true, 2, trump);

    ASSERT_EQUAL(result, true);
    ASSERT_EQUAL(trump, DIAMONDS);

    delete p;
}

TEST(test_lead_card)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(ACE, SPADES));
    p->add_card(Card(KING, SPADES));
    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(JACK, HEARTS));
    p->add_card(Card(TEN, CLUBS));

    Card led = p->lead_card(HEARTS);

    ASSERT_EQUAL(led, Card(ACE, SPADES));

    delete p;
}

TEST(test_lead_card_all_trump)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(ACE, HEARTS));

    Card led = p->lead_card(HEARTS);

    ASSERT_EQUAL(led, Card(ACE, HEARTS));

    delete p;
}

TEST(test_play_card_follow_suit)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(ACE, SPADES));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(TEN, CLUBS));
    p->add_card(Card(QUEEN, DIAMONDS));

    Card led_card(QUEEN, SPADES);
    Card played = p->play_card(led_card, HEARTS);

    ASSERT_EQUAL(played, Card(ACE, SPADES));

    Card played1 = p->play_card(led_card, HEARTS);
    ASSERT_EQUAL(played1, Card(NINE, SPADES));

    Card played_again = p->play_card(led_card, HEARTS);
    ASSERT_EQUAL(played_again, Card(TEN, CLUBS));

    delete p;
}

TEST(test_lead_card_right_bower)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(JACK, HEARTS));

    Card led = p->lead_card(HEARTS);

    ASSERT_EQUAL(led, Card(JACK, HEARTS));

    delete p;
}

TEST(test_play_card_no_follow_suit)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(ACE, CLUBS));
    p->add_card(Card(KING, DIAMONDS));
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(NINE, HEARTS));

    Card led_card(QUEEN, SPADES);
    Card played = p->play_card(led_card, HEARTS);

    ASSERT_EQUAL(played, Card(KING, DIAMONDS));

    delete p;
}

TEST(test_play_card_left_bower_led)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(TEN, CLUBS));
    p->add_card(Card(ACE, HEARTS));
    p->add_card(Card(KING, DIAMONDS));
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(NINE, HEARTS));

    Card led_card(JACK, CLUBS);
    Suit trump = SPADES;
    Card played = p->play_card(led_card, trump);

    ASSERT_EQUAL(played, Card(NINE, HEARTS));

    delete p;
}

TEST(test_add_and_discard_discards_lowest)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(JACK, HEARTS));
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(ACE, HEARTS));

    Card upcard(NINE, HEARTS);
    p->add_and_discard(upcard);

    Card led_card(QUEEN, SPADES);
    Card played = p->play_card(led_card, SPADES);

    ASSERT_EQUAL(played, Card(TEN, HEARTS));

    delete p;
}

TEST(test_lead_card_removes_selected_card)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(ACE, SPADES));
    p->add_card(Card(KING, CLUBS));
    p->add_card(Card(TEN, DIAMONDS));
    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(JACK, HEARTS));

    Card first = p->lead_card(HEARTS);
    ASSERT_EQUAL(first, Card(ACE, SPADES));
    Card second = p->lead_card(HEARTS);
    ASSERT_EQUAL(second, Card(KING, CLUBS));

    delete p;
}

TEST(test_make_trump_round1_left_bower)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(JACK, HEARTS));
    p->add_card(Card(JACK, DIAMONDS));
    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(TEN, CLUBS));
    p->add_card(Card(QUEEN, SPADES));

    Suit trump = SPADES;
    Card upcard(NINE, HEARTS);

    bool result = p->make_trump(upcard, false, 1, trump);

    ASSERT_EQUAL(result, true);
    ASSERT_EQUAL(trump, HEARTS);

    delete p;
}

TEST(test_make_trump_round2_no)
{
    Player *p = Player_factory("Alice", "Simple");

    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(QUEEN, CLUBS));
    p->add_card(Card(KING, CLUBS));
    p->add_card(Card(ACE, SPADES));

    Suit trump = SPADES;
    Card upcard(NINE, HEARTS);

    bool result = p->make_trump(upcard, false, 2, trump);

    ASSERT_EQUAL(result, false);
    ASSERT_EQUAL(trump, SPADES);

    delete p;
}

TEST_MAIN()