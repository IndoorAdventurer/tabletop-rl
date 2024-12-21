#include <gtest/gtest.h>
#include "../testing_utils.h"

#include "exploding_kittens/environment/game_state.h"
#include "exploding_kittens/environment/actions/play_defuse.h"
#include "exploding_kittens/environment/actions/draw_card.h"
#include "utils.h"

#include <cstdint>
#include <random>

namespace exploding_kittens {

TEST(DefuseActionTest, SimpleIsValidTest) {
    GameState g;
    PlayDefuse pd(g);
    g.reset(2);

    EXPECT_EQ(get_legal_actions(pd).size(), 0)
        << "This action is illegal from the default state.";
    
    g.state = State::Nope;

    EXPECT_EQ(get_legal_actions(pd).size(), 0)
        << "This action is illegal from the nope state.";
    
    g.state = State::Favor;

    EXPECT_EQ(get_legal_actions(pd).size(), 0)
        << "This action is illegal from the nope state.";
    
    g.state = State::Defuse;
    // Not caring that current player doesn't actually have kitten in hand.

    EXPECT_EQ(get_legal_actions(pd).size(), g.cards.deck.size() + 1)
        << "For defuse, you can insert the kitten anywhere in the deck. "
        << "(one more position than number of cards, because both bottom and top).";
}

TEST(DefuseActionTest, ValidListInspectionTest) {
    GameState g;
    PlayDefuse pd(g);
    g.reset(2);

    g.state = State::Defuse;

    auto list = get_legal_actions(pd);

    ASSERT_EQ(list.size(), g.cards.deck.size() + 1)
        << "Testing size again to be sure :-)";
    
    ASSERT_GT(list.size(), 0) << "At least some stuff in the list, I hope :-p";

    size_t depth = 0;
    for (Action &a : list) {
        ASSERT_EQ(a.arg1, depth) << "Depths going deeper and deeper";
        ASSERT_EQ(a.arg2, 0) << "We don't use arg2, but should be set to 0 anyway";
        ASSERT_EQ(a.type, ActionEnum::Play_Defuse) << "Type should ofc be correct";
        ASSERT_EQ(a.cards[to_uint(CardIdx::Exploding_Kitten)], 1)
            << "We should loose 1 exploding kitten for this action";
        ASSERT_EQ(a.cards[to_uint(CardIdx::Defuse)], 1)
            << "We should loose 1 defuse for this action";
        for (uint8_t i = 0; i != UNIQUE_CARDS; ++i) {
            if (i == to_uint(CardIdx::Exploding_Kitten) || i == to_uint(CardIdx::Defuse))
                continue;
            ASSERT_EQ(a.cards[i], 0) << "All other cards should be set to 0, as"
                << "they are not relevant for this calculation.";
        }
        ++depth;
    }
}

TEST(DefuseActionTest, BasicRunAction) {
    GameState g;
    PlayDefuse pd(g);
    g.reset(2);
    g.state = State::Defuse;
    g.current_hand().counts()[to_uint(CardIdx::Exploding_Kitten)] += 1;

    Action ac = get_legal_actions(pd).at(0);

    uint8_t cur_player = g.current_player;
    uint8_t defuses = g.current_hand().has(CardIdx::Defuse);
    uint8_t kittens = g.current_hand().has(CardIdx::Exploding_Kitten);

    pd.take_action(ac);

    EXPECT_EQ(g.state, State::Default)
        << "We should have returned back to default state";
    EXPECT_EQ(g.current_player, g.next_player(cur_player))
        << "In this case we should have moved on to next player (no attacks)";
    EXPECT_EQ(g.cards.hands[cur_player].has(CardIdx::Defuse), defuses - 1)
        << "We should have lost 1 defuse from the action!";
    EXPECT_EQ(g.cards.hands[cur_player].has(CardIdx::Exploding_Kitten), kittens - 1)
        << "We should have lost 1 exploding kitten from the action!";
}

TEST(DefuseActionTest, TestAllInsertionPositions) {
    GameState g;
    PlayDefuse pd(g);
    g.reset(2);

    uint8_t max_depth = g.cards.deck.size() + 1;
    
    for (uint8_t i = 0; i != max_depth; ++i) {
        g.reset(2);
        g.state = State::Defuse;
        g.current_hand().counts()[to_uint(CardIdx::Exploding_Kitten)] += 1;

        auto list = get_legal_actions(pd);
        ASSERT_EQ(list.size(), max_depth)
            << "List length should always be the same as we are at the beginning"
            << " of this game.";
        
        EXPECT_EQ(g.current_player, 0)
            << "We always start as player 0.";
        
        pd.take_action(list[i]);

        EXPECT_EQ(g.cards.deck.get_top_n(i+1)[0], CardIdx::Exploding_Kitten)
            << "We should always find an exploding kitten where we inserted it.";
        
        EXPECT_EQ(g.state, State::Default)
            << "We should have returned back to default state";
        
        EXPECT_EQ(g.current_player, 1)
            << "In this case we should have moved on to next player (no attacks)";
    }
}

TEST(DefuseActionTest, SimpleGameTest) {

    GameState g;
    PlayDefuse pd(g);
    DrawCard dc(g);
    
    for (size_t num_players = 2; num_players <= 5; ++num_players) {
        g.reset(num_players);

        size_t max_steps =
            num_players - 1 +       // Number of fatal kittens
            2 * (                   // Max defuses (2 actions per defuse)
                CardInfo<CardIdx::Defuse>::init_hand * num_players +
                CardInfo<CardIdx::Defuse>::init_rand(num_players)) +
            g.cards.deck.size();    // Max num cards drawn
        std::vector<Action> actions;
        Action a;
        uint8_t player_idx;

        while (g.state != State::Game_Over) {
            switch (g.state)
            {
            case State::Default:
                player_idx = g.current_player;

                a = get_legal_actions(dc).back();
                dc.take_action(a);
                
                ASSERT_EQ(
                    // Can be defuse state or end of game state:
                    g.state != State::Default,
                    g.current_hand().has(CardIdx::Exploding_Kitten) == 1)
                    << "Exit default state iff exploding kitten in hand.";
                
                ASSERT_EQ(
                    g.current_player == g.next_player(player_idx),
                    g.state == State::Default)
                    << "Move to next state iff we did not draw a kitten.";
                    // i.e. we stay in the default state.

                break;
            case State::Defuse:
                player_idx = g.current_player;

                actions = get_legal_actions(pd);
                a = actions.at(
                    std::uniform_int_distribution<size_t>(
                        0, actions.size() - 1)(tabletop_general::randnum_gen));
                
                ASSERT_EQ(g.current_hand().has(CardIdx::Exploding_Kitten), 1)
                    << "We can only get here if we have a kitten, ofc.";
                
                pd.take_action(a);
                
                ASSERT_EQ(g.state, State::Default)
                    << "Always return to default after defuse.";
                
                ASSERT_EQ(g.current_player, g.next_player(player_idx))
                    << "Always move to next player after defuse.";

                break;
            default:
                FAIL() << "We should never see another type of state.";
                break;
            }

            if (--max_steps == 0) {
                FAIL() << "A game is not allowed to take this long."
                       << " (but if it does, might be I counted wrong :-p)";
            }

            ASSERT_LE(g.current_hand().has(CardIdx::Exploding_Kitten), 1)
                << "We can never have more than 1 kitten in our hand.";
        }
    }
}

}