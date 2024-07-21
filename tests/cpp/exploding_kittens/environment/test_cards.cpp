#include <gtest/gtest.h>
#include "testing_utils.h"

#include "exploding_kittens/environment/cards.h"

#include <random>

namespace exploding_kittens {


TEST(CardsTests, InitChecks) {
    Cards cards;
    for (size_t num_players = MIN_PLAYERS; num_players <= MAX_PLAYERS; ++num_players) {
        cards.reset(num_players);

        EXPECT_TRUE(cards_integrity_check(cards));

        EXPECT_EQ(cards.hands.size(), num_players)
            << "Should actually have the specified number of players.";
        
        // Col sum checks:
        for (const CardHand &hand : cards.hands) {
            EXPECT_EQ(col_sum(hand), 8) << "Players must have 8 cards at start";
        }
        EXPECT_EQ(col_sum(cards.discard_pile), 0)
            << "Discard pile must be empty at start";
        EXPECT_EQ(col_sum(cards.deck), 
            (                   // Compute total cards in game
                num_players - 1 // Exploding kittens
                + num_players + (num_players == 5 ? 1 : 2)  // Defuses
                + 46            // The rest
            )
            - num_players * 8   // Subtract cards in hands
        ) << "Remainder of cards must be in the discard pile.";

        // Checking some stuff we know about player hands:
        for (const CardHand &hand : cards.hands) {
            EXPECT_EQ(hand.has(CardIdx::Exploding_Kitten), 0)
                << "Exploding Kittens can't be in a player's hand at init.";
            EXPECT_GE(hand.has(CardIdx::Defuse), 1)
                << "Each player has at least 1 defuse.";
        }

        // Check if it throws when we give an invalid number of players:
        EXPECT_THROW(cards.reset(1), std::invalid_argument)
            << "reset() should throw when specifying too few players.";
        EXPECT_THROW(cards.reset(6), std::invalid_argument)
            << "reset() should throw when given too many players.";
    }
}

TEST(CardsTests, StackPopAndTop) {
    Cards cards;
    cards.reset(5);

    size_t total = col_sum(cards.deck);
    EXPECT_EQ(cards.deck.get_top_n(1000).size(), total)
        << "get_top_n should give all when specifying too large range.";
    
    for (size_t n = 0; n <= total; ++n)
        EXPECT_EQ(cards.deck.get_top_n(n).size(), n)
            << "get_top_n should give the right number of cards for legal ranges.";
    
    for (size_t n = 1; n <= total; ++n) {
        ASSERT_NO_THROW(cards.deck.pop())
            << "Should be able to pop all cards";
        ASSERT_EQ(cards.deck.get_top_n(1000).size(), total - n)
            << "After popping, ordered list should be one shorter.";
        ASSERT_EQ(col_sum(cards.deck), total - n);
    }

    ASSERT_THROW(cards.deck.pop(), std::out_of_range)
        << "pop() must throw when deck is empty.";    

}

TEST(CardsTests, StackPush) {
    Cards cards;
    auto test_push = [&](CardIdx i) {
        uint8_t before = cards.deck.has(i);
        cards.deck.push(i);
        uint8_t after = cards.deck.has(i);
        EXPECT_EQ(after, before + 1) << "One card `i` should have been added.";
        EXPECT_EQ(cards.deck.get_top_n(1)[0], i) << "New card should be on top.";
    };

    std::uniform_int_distribution<uint8_t> dist(0, UNIQUE_CARDS - 1);
    for (size_t num_players = MIN_PLAYERS; num_players <= MAX_PLAYERS; ++num_players) {
        cards.reset(num_players);
        
        // Doing 3 pushes each time:
        test_push(static_cast<CardIdx>(dist(tabletop_general::randnum_gen)));
        test_push(static_cast<CardIdx>(dist(tabletop_general::randnum_gen)));
        test_push(static_cast<CardIdx>(dist(tabletop_general::randnum_gen)));
    }
}

TEST(CardsTests, StackInsert) {
    Cards cards;
    auto test_insert = [&](CardIdx i, size_t depth) {
        uint8_t before = cards.deck.has(i);
        cards.deck.insert(i, depth);
        uint8_t after = cards.deck.has(i);
        EXPECT_EQ(after, before + 1) << "One card `i` should have been added.";
        EXPECT_EQ(cards.deck.get_top_n(depth+1)[0], i)
            << "Card " << static_cast<int>(i) << " should be inserted at depth "
            << depth << ". Note that if depth > size, it gets inserted at bottom.";
    };
    std::uniform_int_distribution<uint8_t> dist(0, UNIQUE_CARDS - 1);
    for (size_t num_players = MIN_PLAYERS; num_players <= MAX_PLAYERS; ++num_players) {
        for (size_t depth = 0; depth != 40; ++depth) {
            cards.reset(num_players);
            test_insert(static_cast<CardIdx>(dist(tabletop_general::randnum_gen)), depth);
        }
    }
}

TEST(CardsTests, HandTakeFromStack) {
    Cards cards;
    auto test_take = [&](size_t hand_idx) {
        auto hand_prev = copy_counts(cards.hands[hand_idx]);
        auto deck_prev = copy_counts(cards.deck);
        CardIdx card = cards.hands[hand_idx].take_from(cards.deck);
        for (uint8_t i = 0; i != UNIQUE_CARDS; ++i) {
            if (i == static_cast<uint8_t>(card)) {
                EXPECT_EQ(cards.hands[hand_idx].has(i), hand_prev[i] + 1)
                    << "Should now have one more of the card we got.";
                EXPECT_EQ(cards.deck.has(i), deck_prev[i] - 1)
                    << "Deck should have one less, of course :-)";
            }
            else {
                EXPECT_EQ(cards.hands[hand_idx].has(i), hand_prev[i])
                    << "All other cards should be the same for hand.";
                EXPECT_EQ(cards.deck.has(i), deck_prev[i])
                    << "All other cards should be the same for deck.";
            }
        }
        ASSERT_TRUE(cards_integrity_check(cards));
    };

    for (size_t num_players = MIN_PLAYERS; num_players <= MAX_PLAYERS; ++num_players) {
        cards.reset(num_players);
        for (size_t hand_idx = 0; hand_idx != num_players; ++hand_idx) {
            test_take(hand_idx);
        }
    }

    EXPECT_THROW(cards.hands[0].take_from(cards.discard_pile), std::out_of_range)
        << "Trying to draw from empty stack should raise exception.";
}

TEST(CardsTests, HandTakeFromOther) {
    Cards cards;
    auto test_take = [&](size_t hand_idx, size_t other_idx) {
        auto hand_prev = copy_counts(cards.hands[hand_idx]);
        auto other_prev = copy_counts(cards.hands[other_idx]);
        CardIdx card = cards.hands[hand_idx].take_from(cards.hands[other_idx]);
        for (uint8_t i = 0; i != UNIQUE_CARDS; ++i) {
            if (i == static_cast<uint8_t>(card) and hand_idx != other_idx) {
                EXPECT_EQ(cards.hands[hand_idx].has(i), hand_prev[i] + 1)
                    << "Should now have one more of the card we got.";
                EXPECT_EQ(cards.hands[other_idx].has(i), other_prev[i] - 1)
                    << "Other hand should have one less now.";
            }
            else {
                EXPECT_EQ(cards.hands[hand_idx].has(i), hand_prev[i])
                    << "All other cards should be the same for hand.";
                EXPECT_EQ(cards.hands[other_idx].has(i), other_prev[i])
                    << "All other cards should be the same for other hand.";
            }
        }
        ASSERT_TRUE(cards_integrity_check(cards));
    };

    // Always next one:
    for (size_t num_players = MIN_PLAYERS; num_players <= MAX_PLAYERS; ++num_players) {
        cards.reset(num_players);
        for (size_t hand_idx = 0; hand_idx != num_players; ++hand_idx) {
            test_take(hand_idx, (hand_idx + 1) % num_players);
        }
    }

    // Always pick from player zero:
    for (size_t num_players = MIN_PLAYERS; num_players <= MAX_PLAYERS; ++num_players) {
        cards.reset(num_players);
        for (size_t hand_idx = 0; hand_idx != num_players; ++hand_idx) {
            test_take(hand_idx, 0);
        }
    }

    // Pick from player zero till empty:
    cards.reset(2);
    for (size_t i = 0; i != (CARDS_2_DEAL + 1); ++i) {
        test_take(1, 0);
    }
    EXPECT_EQ(cards.hands[1].take_from(cards.hands[0]), CardIdx::Error)
        << "Taking more cards than player has should return error card.";
    
    test_take(1, 0); // should still pass because for card == i never true, so
    // now it just checks that everything stays the same.
}

TEST(CardsTests, HandTakeKnownFromOther) {
    Cards cards;
    cards.reset(5);
    for (size_t i = 0; i != 5; ++i) {
        EXPECT_EQ(cards.hands[0].take_from(cards.hands[i], CardIdx::Defuse),
            CardIdx::Defuse) << "Should be able to take defuse from all, also self.";
    }
    EXPECT_GE(cards.hands[0].has(CardIdx::Defuse), 5)
        << "Hand 0 should now have at least 5 defuses.";
    ASSERT_TRUE(cards_integrity_check(cards));
    
    EXPECT_EQ(cards.hands[0].take_from(cards.hands[1], CardIdx::Exploding_Kitten),
            CardIdx::Error)
            << "Trying to pick card that is not there should give error card.";
    EXPECT_EQ(cards.hands[0].has(CardIdx::Exploding_Kitten), 0)
        << "Still no exploding kitten in hand ;-)";
    EXPECT_EQ(cards.hands[1].has(CardIdx::Exploding_Kitten), 0)
        << "Still no exploding kitten in hand ;-)";
}

TEST(CardsTests, HandPlaceAtTop) {
    Cards cards;
    cards.reset(2);

    EXPECT_THROW(cards.hands[0].place_at(
        cards.discard_pile, CardIdx::Exploding_Kitten), std::range_error)
        << "Placing a card that is not there should throw.";
    ASSERT_TRUE(cards_integrity_check(cards));
    int defuses = cards.hands[0].has(CardIdx::Defuse);
    cards.hands[0].place_at(cards.discard_pile, CardIdx::Defuse);
    EXPECT_EQ(cards.hands[0].has(CardIdx::Defuse), defuses - 1)
        << "Should have one defuse less now.";
    EXPECT_EQ(cards.discard_pile.has(CardIdx::Defuse), 1)
        << "Discard pile should have exactly one defuse.";
    EXPECT_EQ(cards.discard_pile.get_top_n(1)[0], CardIdx::Defuse)
        << "Defuse should be the top card (the only card also)";
    ASSERT_TRUE(cards_integrity_check(cards));
}

TEST(CardsTests, HandPlaceInside) {
    Cards cards;
    
    tabletop_general::randnum_gen.seed(1996);
    cards.reset(2);
    EXPECT_THROW(cards.hands[0].place_at(
        cards.deck, CardIdx::Exploding_Kitten, 5), std::range_error)
        << "Placing a card that is not there should throw.";
    ASSERT_TRUE(cards_integrity_check(cards));
    
    int defuses = cards.hands[0].has(CardIdx::Defuse);
    cards.hands[0].place_at(cards.deck, CardIdx::Defuse, 0);
    EXPECT_EQ(cards.hands[0].has(CardIdx::Defuse), defuses - 1)
        << "Should have one defuse less now.";
    EXPECT_EQ(cards.deck.get_top_n(1)[0], CardIdx::Defuse)
        << "Defuse should be the top card (the only card also)";
    ASSERT_TRUE(cards_integrity_check(cards));
    auto insert_hand = copy_counts(cards.hands[0]);
    auto insert_order = cards.deck.get_top_n(1000);
    std::vector<CardIdx> insert_order_copy{insert_order.begin(), 
        insert_order.end()};

    tabletop_general::randnum_gen.seed(1996);
    cards.reset(2);
    cards.hands[0].place_at(cards.deck, CardIdx::Defuse);
    auto push_hand = copy_counts(cards.hands[0]);
    auto push_order = cards.deck.get_top_n(1000);
    std::vector<CardIdx> push_order_copy{push_order.begin(), 
        push_order.end()};
    EXPECT_TRUE(insert_hand == push_hand)
        << "Inserting on top should be identical to pushing.";
    EXPECT_TRUE(insert_order_copy == push_order_copy)
        << "Inserting on top should be identical to pushing.";
    
    for (size_t depth = 0; depth != 50; ++depth) {
        cards.reset(2);
        cards.hands[0].place_at(cards.deck, CardIdx::Defuse, depth);
        EXPECT_EQ(cards.deck.get_top_n(depth + 1)[0], CardIdx::Defuse)
            << "Specified card should be at given location. If depth is larger "
            << "than stack size, should place at bottom (get_top_n accounts "
            << "for this.";
        ASSERT_TRUE(cards_integrity_check(cards));
    }
}

TEST(CardsTests, HandGiveTo) {
    Cards cards;
    cards.reset(2);

    ASSERT_THROW(cards.hands[0].give_to(
        cards.hands[1], CardIdx::Exploding_Kitten), std::range_error)
        << "Giving a card that is not there should throw.";
    ASSERT_THROW(cards.hands[0].give_to(
        cards.hands[0], CardIdx::Exploding_Kitten), std::range_error)
        << "Also throw when trying to give it to yourself.";
    
    ASSERT_TRUE(cards_integrity_check(cards));

    int defuses_0 = cards.hands[0].has(CardIdx::Defuse);
    int defuses_1 = cards.hands[1].has(CardIdx::Defuse);

    cards.hands[0].give_to(cards.hands[0], CardIdx::Defuse);
    EXPECT_EQ(cards.hands[0].has(CardIdx::Defuse), defuses_0)
        << "Giving something to yourself should not change anything.";
    
    ASSERT_TRUE(cards_integrity_check(cards));

    cards.hands[0].give_to(cards.hands[1], CardIdx::Defuse);
    EXPECT_EQ(cards.hands[0].has(CardIdx::Defuse), defuses_0 - 1)
        << "Should have one defuse less now because gave it to other.";
    EXPECT_EQ(cards.hands[1].has(CardIdx::Defuse), defuses_1 + 1)
            << "Other should have received the defuse.";
    
    ASSERT_TRUE(cards_integrity_check(cards));
}

} // namespace exploding_kittens