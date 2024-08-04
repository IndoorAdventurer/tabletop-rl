#include <gtest/gtest.h>

#include "exploding_kittens/environment/game_state.h"

#include <cstdint>

namespace exploding_kittens {

TEST(GameStateTests, RegisterTurnTest) {
    GameState g;
    g.reset(2);
    EXPECT_EQ(g.current_player, 0) << "Player 0 should be first to get turn.";
    g.register_turn();
    EXPECT_EQ(g.current_player, 1) << "After player 0 comes player 1 ofc.";
    g.register_turn();
    EXPECT_EQ(g.current_player, 0) << "And then we loop back around to 0";

    g.reset(5);
    EXPECT_EQ(g.current_player, 0) << "Player 0 should be first to get turn.";
    uint8_t to_expect[] = {1, 2, 3, 4, 0, 1, 2, 3, 4};
    for (uint8_t i : to_expect) {
        g.register_turn();
        EXPECT_EQ(g.current_player, i) << "Expect players getting turn in order.";
    }

    g.reset(2);
    g.turns_left = 3;

    g.register_turn();
    EXPECT_EQ(g.current_player, 0) << "Should still be 0 because 3 turns.";
    EXPECT_EQ(g.turns_left, 2) << "Two turns should be left";

    g.register_turn();
    EXPECT_EQ(g.current_player, 0) << "Should still be 0 because 3 turns.";
    EXPECT_EQ(g.turns_left, 1) << "One turns should be left";

    g.register_turn();
    EXPECT_EQ(g.current_player, 1) << "Now 3 turns taken, so next player is.";
    EXPECT_EQ(g.turns_left, 1) << "One turns should be left";
}

TEST(GameStateTests, RegisterTurnWithDeadPlayersTest) {
    GameState g;
    g.reset(5);

    while (g.is_alive(1))
        g.cards.hands[1].take_from(g.cards.deck);
    
    ASSERT_EQ(g.cards.hands[1].has(CardIdx::Exploding_Kitten), 1)
        << "Is alive checks if a player has exploding kitten in hand.";
    
    EXPECT_EQ(g.current_player, 0) << "Player 0 should be first to get turn.";
    uint8_t to_expect[] = {2, 3, 4, 0, 2, 3, 4, 0};
    for (uint8_t i : to_expect) {
        g.register_turn();
        EXPECT_EQ(g.current_player, i) << "Player 1 is dead and should be skipped.";
    }

    while (g.is_alive(2))
        g.cards.hands[2].take_from(g.cards.deck);
    
    EXPECT_EQ(g.current_player, 0) << "We left off at player 0";
    uint8_t to_expect2[] = {3, 4, 0, 3, 4, 0};
    for (uint8_t i : to_expect2) {
        g.register_turn();
        EXPECT_EQ(g.current_player, i) << "Player 2 now also skipped.";
    }

    while (g.is_alive(4))
        g.cards.hands[4].take_from(g.cards.deck);
    
    EXPECT_EQ(g.current_player, 0) << "We left off at player 0";
    uint8_t to_expect3[] = {3, 0, 3, 0};
    for (uint8_t i : to_expect3) {
        g.register_turn();
        EXPECT_EQ(g.current_player, i) << "Only 0 and 3 left.";
    }

    while (g.is_alive(0))
        g.cards.hands[0].take_from(g.cards.deck);
    
    for (uint8_t i = 0; i != 10; ++i) {
        g.register_turn();
        EXPECT_EQ(g.current_player, 3) << "Only 3 is left so should go here.";
    }

    EXPECT_EQ(g.cards.deck.has(CardIdx::Exploding_Kitten), 0)
        << "All exploding kittens should have been drawn.";
}

} // namespace exploding_kittens