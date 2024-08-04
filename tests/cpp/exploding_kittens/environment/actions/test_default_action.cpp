#include <gtest/gtest.h>

#include "exploding_kittens/environment/game_state.h"
#include "exploding_kittens/environment/actions/default_action.h"

#include <cstdint>

namespace exploding_kittens {

TEST(DefaultActionTest, SimpleIsValidTest) {
    GameState g;
    g.reset(2);

    EXPECT_TRUE(DefaultAction::is_valid(g, 0, 0))
        << "Default action always valid in starting state.";
    
    g.state = State::Nope;

    EXPECT_TRUE(DefaultAction::is_valid(g, 0, 0))
        << "Default action always valid in nope state.";
    
    g.state = State::Favor;

    EXPECT_FALSE(DefaultAction::is_valid(g, 0, 0))
        << "Default action never valid in any other state.";
}

TEST(DefaultActionTest, SimpleFromDefaultTest) {
    GameState g;
    g.reset(2);

    ASSERT_TRUE(DefaultAction::is_valid(g, 0, 0))
        << "Default action always valid in starting state.";
    
    CardIdx i = g.cards.deck.get_top_n(1)[0];
    uint8_t deck_has = g.cards.deck.has(i);
    uint8_t hand_has = g.current_hand().has(i);

    DefaultAction::play_action(g, 0, 0);

    EXPECT_EQ(g.cards.deck.has(i), deck_has - 1) << "Top card was taken";
    EXPECT_EQ(g.cards.hands[0].has(i), hand_has + 1) << "Top card was taken";

    if (i == CardIdx::Exploding_Kitten) {
        EXPECT_EQ(g.state, State::Defuse)
            << "Get to defuse state if kitten drawn.";
    } else {
        EXPECT_EQ(g.state, State::Default);
    }
}

TEST(DefaultActionTest, TestTillDefuse) {
    GameState g;
    g.reset(2);

    uint8_t player_idx = 0;
    while (true) {
        ASSERT_TRUE(DefaultAction::is_valid(g, 0,0));
        EXPECT_EQ(g.current_player, player_idx);
        EXPECT_EQ(g.state, State::Default);
        
        DefaultAction::play_action(g, 0, 0);

        if (not g.is_alive(player_idx))
            break;
        
        player_idx = (player_idx + 1) % 2;
    }

    EXPECT_EQ(g.current_player, player_idx);
    EXPECT_EQ(g.state, State::Defuse);
}

TEST(DefaultActionTest, TestTillDeath) {
    GameState g;
    g.reset(3);

    while (true) {
        while (g.current_hand().has(CardIdx::Defuse))
            g.current_hand().place_at(g.cards.discard_pile, CardIdx::Defuse);
        
        ASSERT_EQ(g.state, State::Default) << "Can only be in default state.";
        ASSERT_TRUE(DefaultAction::is_valid(g, 0, 0));
        DefaultAction::play_action(g, 0, 0);

        if (g.state == State::Game_Over)
            break;
    }

    EXPECT_EQ(g.cards.deck.has(CardIdx::Exploding_Kitten), 0)
        << "All exploding Kittens should be drawn";
    EXPECT_LE(g.cards.hands[0].has(CardIdx::Exploding_Kitten), 1);
    EXPECT_LE(g.cards.hands[1].has(CardIdx::Exploding_Kitten), 1);
    EXPECT_LE(g.cards.hands[2].has(CardIdx::Exploding_Kitten), 1);
    EXPECT_EQ(
        g.cards.hands[0].has(CardIdx::Exploding_Kitten) +
        g.cards.hands[1].has(CardIdx::Exploding_Kitten) +
        g.cards.hands[2].has(CardIdx::Exploding_Kitten),
        2
    ) << "Two players have an exploding kitten in their hand, the other wins.";
}

} // namespace exploding_kittens