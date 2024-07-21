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
        EXPECT_EQ(g.current_player, i) << "Expect sequence to work well here.";
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

} // namespace exploding_kittens