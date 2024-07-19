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


} // namespace exploding_kittens