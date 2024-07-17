#include <gtest/gtest.h>
#include "testing_utils.h"

#include "exploding_kittens/environment/cards.h"
#include <iostream>

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
    }
}


} // namespace exploding_kittens