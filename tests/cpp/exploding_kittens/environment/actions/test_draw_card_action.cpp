#include <gtest/gtest.h>
#include "../testing_utils.h"

#include "exploding_kittens/environment/game_state.h"
#include "exploding_kittens/environment/actions/draw_card.h"

#include <cstdint>

namespace exploding_kittens {

TEST(DrawCardActionTest, SimpleIsValidTest) {
    GameState g;
    DrawCard dc(g);
    g.reset(2);

    EXPECT_EQ(get_legal_actions(dc).size(), 1)
        << "Draw card action always valid in starting state.";
    
    g.state = State::Nope;

    EXPECT_EQ(get_legal_actions(dc).size(), 0)
        << "Draw card action never valid in nope state.";
    
    g.state = State::Favor;

    EXPECT_EQ(get_legal_actions(dc).size(), 0)
        << "Draw card action never valid in any other state.";
    
    g.state = State::Defuse;

    EXPECT_EQ(get_legal_actions(dc).size(), 0)
        << "Draw card action never valid in any other state.";
}

TEST(DrawCardActionTest, SimpleFromDefaultTest) {
    GameState g;
    DrawCard dc(g);
    g.reset(2);

    auto av = get_legal_actions(dc);
    ASSERT_EQ(av.size(), 1)
        << "Draw card action always valid in starting state.";
    
    ASSERT_EQ(av[0].type, ActionEnum::Draw)
        << "Draw should be the corresponding enum type.";
    
    // In this case I actually don't care about the integrity of the Action
    // object, because it won't get checked anyway.
    
    CardIdx i = g.cards.deck.get_top_n(1)[0];
    uint8_t deck_has = g.cards.deck.has(i);
    uint8_t hand_has = g.primary_hand().has(i);

    dc.take_action(av[0]);

    EXPECT_EQ(g.cards.deck.has(i), deck_has - 1) << "Top card was taken";
    EXPECT_EQ(g.cards.hands[0].has(i), hand_has + 1) << "Top card was taken";

    if (i == CardIdx::Exploding_Kitten) {
        EXPECT_EQ(g.state, State::Defuse)
            << "Get to defuse state if kitten drawn.";
    } else {
        EXPECT_EQ(g.state, State::Default);
    }
}

TEST(DrawCardActionTest, TestTillDefuse) {
    GameState g;
    DrawCard dc(g);
    g.reset(2);

    uint8_t player_idx = 0;
    while (true) {
        auto av = get_legal_actions(dc);
        ASSERT_EQ(av.size(), 1);
        EXPECT_EQ(g.primary_player, player_idx);
        EXPECT_EQ(g.state, State::Default);
        
        dc.take_action(av[0]);

        if (g.cards.hands[player_idx].has(CardIdx::Exploding_Kitten))
            break;
        
        player_idx = (player_idx + 1) % 2;
    }

    EXPECT_EQ(g.primary_player, player_idx);
    EXPECT_EQ(g.state, State::Defuse);
}

TEST(DrawCardActionTest, TestTillDeath) {
    GameState g;
    DrawCard dc(g);
    g.reset(3);

    while (true) {
        while (g.primary_hand().has(CardIdx::Defuse))
            g.primary_hand().place_at(g.cards.discard_pile, CardIdx::Defuse);
        
        ASSERT_EQ(g.state, State::Default) << "Can only be in default state.";
        auto av = get_legal_actions(dc);
        ASSERT_EQ(av.size(), 1);
        dc.take_action(av[0]);

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