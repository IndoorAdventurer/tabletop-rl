#include <gtest/gtest.h>
#include "../testing_utils.h"

#include "exploding_kittens/environment/game_state.h"
#include "exploding_kittens/environment/actions/nope_utils.h"
#include "exploding_kittens/environment/actions/play_nope.h"
#include "exploding_kittens/environment/actions/skip_nope.h"

namespace exploding_kittens {

class DummyNopable: public NopeableBase {
    public:
        size_t call_count = 0;
        
        DummyNopable(GameState &gs)
        :
            NopeableBase(ActionEnum::Play_Shuffle, gs)
        {}

        void append_legal_actions(std::vector<Action> &vec) const override {
            if (not gs.primary_hand().has(CardIdx::Shuffle))
                return;
            vec.emplace_back(ActionEnum::Play_Shuffle, std::array<uint8_t, UNIQUE_CARDS>{}, 0, 0);
            vec.back().cards.at(to_uint(CardIdx::Shuffle)) = 1;
        }

    protected:
        void enforce_action(Action const &a) override {
            ++call_count;
        }
};

TEST(NopingTest, BehaviorWithNoNopes) {
    GameState gs;
    DummyNopable dn(gs);
    custom_state_reset(gs, 2, [](Cards &c) {
        c.hands[0].counts()[to_uint(CardIdx::Shuffle)] = 1U;
    });
    auto actions = get_legal_actions(dn);
    ASSERT_EQ(actions.size(), 1) <<
        "We should have this action because we have a shuffle";

    dn.take_action(actions[0]);

    EXPECT_EQ(dn.call_count, 1)
        << "There are no nopes, so enforce_action should have been called.";
    EXPECT_EQ(gs.state, State::Default) << "Should be in default state still.";
    
    EXPECT_EQ(gs.cards.hands[0].has(CardIdx::Shuffle), 0)
        << "Player should have lost its shuffle card";
    EXPECT_EQ(gs.cards.discard_pile.has(CardIdx::Shuffle), 1)
        << "The shuffle should now be on the discard pile.";
}

TEST(NopingTest, SimpleEnterNopeTest) {
    GameState gs;
    DummyNopable dn(gs);
    PlayNope pn(gs);
    SkipNope sn(gs);
    custom_state_reset(gs, 2, [](Cards &c) {
        c.hands[0].counts()[to_uint(CardIdx::Shuffle)] = 1U;
        c.hands[1].counts()[to_uint(CardIdx::Nope)] = 2U;
    });
    auto actions = get_legal_actions(dn);
    ASSERT_EQ(actions.size(), 1) <<
        "We should have this action because we have a shuffle";

    dn.take_action(actions[0]);

    EXPECT_EQ(dn.call_count, 0)
        << "Someone can still nope, so enforce_action should not be called.";
    EXPECT_EQ(gs.state, State::Nope) << "We should have entered the nope state";
    
    EXPECT_EQ(gs.cards.hands[0].has(CardIdx::Shuffle), 0)
        << "Player should have lost its shuffle card";
    EXPECT_EQ(gs.cards.discard_pile.has(CardIdx::Shuffle), 1)
        << "The shuffle should now be on the discard pile.";
    
    ASSERT_EQ(gs.secondary_players.size(), 1)
        << "We had one player with a nope, so one secondary player";
    EXPECT_EQ(gs.secondary_players.back(), 1)
        << "Player 1 should be set as secondary player";
    
    actions = get_legal_actions(pn);
    sn.append_legal_actions(actions);
    EXPECT_EQ(actions.size(), 2)
        << "Secondary should have 2 options: nope or skip";
    EXPECT_EQ(actions[0].type, ActionEnum::Play_Nope);
    EXPECT_EQ(actions[1].type, ActionEnum::Skip_Nope);
}

TEST(NopingTest, SimpleSkipNopeTest) {
    GameState gs;
    DummyNopable dn(gs);
    SkipNope sn(gs);
    custom_state_reset(gs, 2, [](Cards &c) {
        c.hands[0].counts()[to_uint(CardIdx::Shuffle)] = 1U;
        c.hands[1].counts()[to_uint(CardIdx::Nope)] = 2U;
    });
    auto actions = get_legal_actions(dn);
    ASSERT_EQ(actions.size(), 1) <<
        "We should have this action because we have a shuffle";

    dn.take_action(actions[0]);
    
    ASSERT_EQ(gs.secondary_players.size(), 1)
        << "We had one player with a nope, so one secondary player";
    
    actions = get_legal_actions(sn);
    ASSERT_EQ(actions.size(), 1) << "Secondary can skip nope";

    sn.take_action(actions[0]);

    // Check if the action now happened:
    EXPECT_EQ(dn.call_count, 1)
        << "Noper skipped, so enforce_action should have been called.";
    EXPECT_EQ(gs.state, State::Default) << "Should be in default state again.";
    
    EXPECT_EQ(gs.cards.hands[0].has(CardIdx::Shuffle), 0)
        << "Player should have lost its shuffle card";
    EXPECT_EQ(gs.cards.discard_pile.has(CardIdx::Shuffle), 1)
        << "The shuffle should now be on the discard pile.";
    
    // Check if nope-related data is valid:
    EXPECT_EQ(gs.cards.hands[1].has(CardIdx::Nope), 2)
        << "Secondary should not have lost its nope";
    EXPECT_EQ(gs.secondary_players.size(), 0)
        << "Secondary players list should be empty again";
}

TEST(NopingTest, SimplePlayNopeTest) {
    GameState gs;
    DummyNopable dn(gs);
    PlayNope pn(gs);
    custom_state_reset(gs, 2, [](Cards &c) {
        c.hands[0].counts()[to_uint(CardIdx::Shuffle)] = 1U;
        c.hands[1].counts()[to_uint(CardIdx::Nope)] = 1U;
    });
    auto actions = get_legal_actions(dn);
    ASSERT_EQ(actions.size(), 1) <<
        "We should have this action because we have a shuffle";

    dn.take_action(actions[0]);
    
    ASSERT_EQ(gs.secondary_players.size(), 1)
        << "We had one player with a nope, so one secondary player";
    
    actions = get_legal_actions(pn);
    ASSERT_EQ(actions.size(), 1) << "Secondary can play nope";

    pn.take_action(actions[0]);

    // Check if the action did indeed not happen:
    EXPECT_EQ(dn.call_count, 0)
        << "Noper noped, so enforce_action should have been skipped.";
    EXPECT_EQ(gs.state, State::Default) << "Should be in default state again.";
    
    EXPECT_EQ(gs.cards.hands[0].has(CardIdx::Shuffle), 0)
        << "Player should have lost its shuffle card";
    EXPECT_EQ(gs.cards.discard_pile.has(CardIdx::Shuffle), 1)
        << "The shuffle should now be on the discard pile.";
    
    // Check if nope-related data is valid:
    EXPECT_EQ(gs.cards.hands[1].has(CardIdx::Nope), 0)
        << "Secondary should not have lost one nope";
    EXPECT_EQ(gs.cards.discard_pile.has(CardIdx::Nope), 1)
        << "This nope should be on the discard pile";
    EXPECT_EQ(gs.secondary_players.size(), 0)
        << "Secondary players list should be empty again";
}

TEST(NopingTest, SimpleDoubleNopeTest) {
    GameState gs;
    DummyNopable dn(gs);
    PlayNope pn(gs);
    custom_state_reset(gs, 2, [](Cards &c) {
        c.hands[0].counts()[to_uint(CardIdx::Shuffle)] = 1U;
        c.hands[0].counts()[to_uint(CardIdx::Nope)] = 1U;
        c.hands[1].counts()[to_uint(CardIdx::Nope)] = 1U;
    });
    auto actions = get_legal_actions(dn);
    ASSERT_EQ(actions.size(), 1) <<
        "We should have this action because we have a shuffle";

    dn.take_action(actions[0]);
    
    EXPECT_EQ(gs.cards.hands[0].has(CardIdx::Shuffle), 0)
        << "Player should have lost its shuffle card";
    EXPECT_EQ(gs.cards.discard_pile.has(CardIdx::Shuffle), 1)
        << "The shuffle should now be on the discard pile.";
    
    ASSERT_EQ(gs.secondary_players.size(), 2)
        << "2 players have a nope, so 2 players can nope.";
    
    actions = get_legal_actions(pn);
    ASSERT_EQ(actions.size(), 1) << "Secondary can play nope";

    pn.take_action(actions[0]);

    // Checks after first nope:
    EXPECT_EQ(dn.call_count, 0)
        << "Enforce_action should not yet have taken place.";
    EXPECT_EQ(gs.state, State::Nope) << "We should still be in the nope state";
    ASSERT_EQ(gs.secondary_players.size(), 1)
        << "One player left that can nope.";
    EXPECT_EQ(gs.cards.discard_pile.has(CardIdx::Nope), 1)
        << "This nope should be on the discard pile";
    
    actions = get_legal_actions(pn);
    ASSERT_EQ(actions.size(), 1) << "Secondary can play nope the nope";

    pn.take_action(actions[0]);

    // Checks after second nope:
    EXPECT_EQ(dn.call_count, 1)
        << "Nope blocked: action should have happened.";
    EXPECT_EQ(gs.state, State::Default) << "Should be in default state again.";
    
    // Check if nope-related data is valid:
    EXPECT_EQ(gs.cards.hands[0].has(CardIdx::Nope), 0)
        << "First player should have lost its nope";
    EXPECT_EQ(gs.cards.hands[1].has(CardIdx::Nope), 0)
        << "Second player should have lost its nope too";
    EXPECT_EQ(gs.secondary_players.size(), 0)
        << "Secondary players list should be empty again";
    EXPECT_EQ(gs.cards.discard_pile.has(CardIdx::Nope), 2)
        << "Both nopes should have been placed on the discard pile";
}

TEST(NopingTest, DeadPlayerHasNope) {
    GameState gs;
    DummyNopable dn(gs);
    custom_state_reset(gs, 3, [](Cards &c) {
        c.hands[0].counts()[to_uint(CardIdx::Shuffle)] = 1U;
        c.hands[1].counts()[to_uint(CardIdx::Nope)] = 1U;
        c.hands[1].counts()[to_uint(CardIdx::Exploding_Kitten)] = 1U;
        // Having an exploding kitten in your hand means you are dead.
    });
    auto actions = get_legal_actions(dn);
    ASSERT_EQ(actions.size(), 1) <<
        "We should have this action because we have a shuffle";

    dn.take_action(actions[0]);

    ASSERT_FALSE(gs.is_alive(1)) << "Make sure player 1 is dead.";
    EXPECT_EQ(dn.call_count, 1)
        << "Only noper is dead, so enforce_action should have been called.";
    EXPECT_EQ(gs.state, State::Default) << "Should be in default state still.";
    
    EXPECT_EQ(gs.cards.hands[0].has(CardIdx::Shuffle), 0)
        << "Player should have lost its shuffle card";
    EXPECT_EQ(gs.cards.discard_pile.has(CardIdx::Shuffle), 1)
        << "The shuffle should now be on the discard pile.";
}

TEST(NopingTest, SkipNopeSkipTest) {
    GameState gs;
    DummyNopable dn(gs);
    PlayNope pn(gs);
    SkipNope sn(gs);
    custom_state_reset(gs, 3, [](Cards &c) {
        c.hands[0].counts()[to_uint(CardIdx::Shuffle)] = 1U;
        c.hands[1].counts()[to_uint(CardIdx::Nope)] = 1U;
        c.hands[2].counts()[to_uint(CardIdx::Nope)] = 1U;
    });
    auto actions = get_legal_actions(dn);
    ASSERT_EQ(actions.size(), 1) <<
        "We should have this action because we have a shuffle";

    dn.take_action(actions[0]);

    ASSERT_EQ(gs.secondary_players.size(), 2) <<
        "2 players can skip or nope.";
    
    // The first player will skip
    uint8_t first_player = gs.secondary_players.back();
    actions = get_legal_actions(sn);
    ASSERT_EQ(actions.size(), 1) << "First player can skip";
    sn.take_action(actions[0]);

    ASSERT_EQ(gs.secondary_players.size(), 1) <<
        "1 player left that can skip or nope.";
    ASSERT_EQ(gs.state, State::Nope) << "We should still be in the nope state";

    // Second player will play action
    actions = get_legal_actions(pn);
    ASSERT_EQ(actions.size(), 1) << "Second player can nope still";
    pn.take_action(actions[0]);

    ASSERT_EQ(gs.secondary_players.size(), 1) <<
        "Player that skipped now has a new opportunity.";
    ASSERT_EQ(gs.state, State::Nope) << "We should still be in the nope state";
    EXPECT_EQ(gs.secondary_players.back(), first_player)
        << "This should be the same player as before";

    // First player skips again.
    actions = get_legal_actions(sn);
    ASSERT_EQ(actions.size(), 1) << "First player can skip";
    sn.take_action(actions[0]);

    EXPECT_EQ(dn.call_count, 0)
        << "The action was noped so enforce_action was not called.";
    EXPECT_EQ(gs.state, State::Default) << "Should be back to default state.";
    
    EXPECT_EQ(gs.cards.hands[0].has(CardIdx::Shuffle), 0)
        << "Player should have lost its shuffle card";
    EXPECT_EQ(gs.cards.discard_pile.has(CardIdx::Shuffle), 1)
        << "The shuffle should now be on the discard pile.";
    EXPECT_EQ(gs.cards.discard_pile.has(CardIdx::Nope), 1)
        << "Only the nope from the one player is on the pile.";
    EXPECT_EQ(gs.cards.hands[first_player].has(CardIdx::Nope), 1)
        << "First player skipped, so should still have its nope.";
}

} // exploding_kittens