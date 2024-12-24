#include "play_nope.h"
#include "nope_utils.h"

#include <cassert>

namespace exploding_kittens {

void PlayNope::append_legal_actions(std::vector<Action> &vec) const {
    // WARNING: not checking if current secondary player has a nope because
    // we wouldn't get here if not. (See nope_utils.cpp:nopers_to_secondaries).
    // Only asserting if this holds at debug time:
    assert(gs.secondary_hand().has(CardIdx::Nope) &&
        "Player got in the secondary_players list without Nope card.");

    if (gs.state == State::Nope) {
        // Braces guarantee zero-init for std::array:
        std::array<uint8_t, UNIQUE_CARDS> cards = {};
        cards[to_uint(CardIdx::Nope)] = 1U;
        vec.emplace_back(
            ActionEnum::Play_Nope, cards, 0U, 0U);
    }
}

void PlayNope::do_take_action(Action const &a) {
    // Manually placing the nope on the discard pile. Not even checking if it
    // actually is specified in a.cards :-p
    gs.secondary_hand().place_at(gs.cards.discard_pile, CardIdx::Nope);

    // Inverting the noped flag and giving everyone a chance to nope again:
    gs.is_noped = !gs.is_noped;
    nopers_to_secondaries(gs);

    // Exit the nope state if there is nobody left that can nope:
    if (gs.secondary_players.size() == 0)
        exit_nope_state(gs);
}

} // namespace exploding_kittens