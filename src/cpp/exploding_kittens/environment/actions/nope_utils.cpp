#include "nope_utils.h"

#include "../../../utils.h"

#include <algorithm>
#include <cassert>

namespace exploding_kittens {

void NopeableBase::do_take_action(Action const &a) {
    // Remove the cards from hand:
    for (size_t cIdx = 0; cIdx != a.cards.size(); ++cIdx) {
        for (uint8_t cards = a.cards[cIdx]; cards != 0; --cards)
            gs.current_hand().place_at(gs.cards.discard_pile, from_uint(cIdx));
    }
    
    // Set state accordingly:
    gs.state = State::Nope;
    nopers_to_secondaries(gs);
    gs.is_noped = false;
    gs.staged_action = a;
    gs.action_type = this;

    // No players that can nope: exit nope state directly:
    if (gs.secondary_players.size() == 0)
        exit_nope_state(gs);
}

void nopers_to_secondaries(GameState &g) {
    // Find players that are alive and have a nope:
    g.secondary_players.clear();
    for (uint8_t player = 0; player != g.num_players(); ++player) {
        if (g.is_alive(player) and g.cards.hands[player].has(CardIdx::Nope))
            g.secondary_players.push_back(player);
    }

    // Shuffle found players (for fairness -- else giving early index advantage)
    std::shuffle(g.secondary_players.begin(), g.secondary_players.end(),
        tabletop_general::randnum_gen);
}

void exit_nope_state(GameState &g) {
    g.state = State::Default;   // Might get overwritten by enforce_action.
    if (g.is_noped)
        return;
    
    assert(g.action_type && "g.action_type can't be a null pointer.");
    // TODO: maybe some extra debug asserts: does g.action_type point to the
    // object corresponding to g.staged_action.type?
    
    // Execute action if it wasn't noped
    g.action_type->enforce_action(g.staged_action);
}

} // namespace exploding_kittens