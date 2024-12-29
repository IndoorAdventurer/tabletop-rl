#include <gtest/gtest.h>
#include "testing_utils.h"

#include "exploding_kittens/environment/game_state.h"

namespace exploding_kittens {

TEST(CustomResetTest, DoesSetToZero) {
    GameState gs;
    custom_state_reset(gs, 3, [](Cards &c){});
    
    ASSERT_EQ(gs.cards.deck.get_top_n(1000).size(), 0)
        << "The deck should be empty";
    ASSERT_EQ(gs.cards.discard_pile.get_top_n(1000).size(), 0)
        << "The discard should be empty";
    
    ASSERT_EQ(col_sum(gs.cards.deck), 0)
        << "Counts repesentation should say same";
    ASSERT_EQ(col_sum(gs.cards.discard_pile), 0)
        << "Counts repesentation should say same";
    for (CardHand &hand : gs.cards.hands)
        ASSERT_EQ(col_sum(hand), 0) << "All hands should be empty";
}

TEST(CustomResetTest, DoesSetStuff) {
    GameState gs;
    custom_state_reset(gs, 3, [](Cards &c){
        c.hands[0].counts()[to_uint(CardIdx::Cat_3)] = 2U;
        c.hands[1].counts()[to_uint(CardIdx::Shuffle)] = 1U;
        c.hands[1].counts()[to_uint(CardIdx::Nope)] = 1U;
        c.hands[2].counts()[to_uint(CardIdx::Defuse)] = 1U;
        c.hands[2].counts()[to_uint(CardIdx::Attack)] = 1U;

        c.deck.counts()[to_uint(CardIdx::Exploding_Kitten)] = 2U;
        c.deck.counts()[to_uint(CardIdx::Defuse)] = 3U;
    });
    
    ASSERT_EQ(gs.cards.deck.get_top_n(1000).size(), 5)
        << "I inserted 5 cards total: 2 kittens, 3 defuses";
    ASSERT_EQ(gs.cards.discard_pile.get_top_n(1000).size(), 0)
        << "The discard should still be empty";
    
    ASSERT_EQ(col_sum(gs.cards.deck), 5)
        << "Counts repesentation should say same";
    ASSERT_EQ(col_sum(gs.cards.discard_pile), 0)
        << "Counts repesentation should say same";
    for (CardHand &hand : gs.cards.hands)
        ASSERT_EQ(col_sum(hand), 2) << "All hands should have 2 cards";

    ASSERT_EQ(gs.cards.hands[0].has(CardIdx::Cat_3), 2U)
        << "Hands should be set as specified";
    ASSERT_EQ(gs.cards.hands[1].has(CardIdx::Shuffle), 1U)
        << "Hands should be set as specified";
    ASSERT_EQ(gs.cards.hands[1].has(CardIdx::Nope), 1U)
        << "Hands should be set as specified";
    ASSERT_EQ(gs.cards.hands[2].has(CardIdx::Defuse), 1U)
        << "Hands should be set as specified";
    ASSERT_EQ(gs.cards.hands[2].has(CardIdx::Attack), 1U)
        << "Hands should be set as specified";

    ASSERT_EQ(gs.cards.deck.has(CardIdx::Exploding_Kitten), 2U)
        << "Deck should also be set as specified";
    ASSERT_EQ(gs.cards.deck.has(CardIdx::Defuse), 3U)
        << "Deck should also be set as specified";
}

} // exploding_kittens