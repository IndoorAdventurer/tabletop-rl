#include "skip_nope.h"
#include "nope_utils.h"

#include <cassert>

namespace exploding_kittens {

void SkipNope::append_legal_actions(std::vector<Action> &vec) const {
    // Only players with a nope card should get the option to refuse noping.
    assert(gs.secondary_hand().has(CardIdx::Nope) &&
        "Player got in the secondary_players list without Nope card..");
    
    if (gs.state == State::Nope)
        vec.emplace_back(   // Braces guarantee zero-init for std::array:
            ActionEnum::Skip_Nope, std::array<uint8_t, UNIQUE_CARDS>{}, 0U, 0U);
}

void SkipNope::do_take_action(Action const &a) {
    // Remove yourself from the secondary player list. IMPORTANT: always
    // assuming your own index is at the end of the list.
    gs.secondary_players.pop_back();

    // Exit the nope state if nobody left that can nope:
    if (gs.secondary_players.size() == 0)
        exit_nope_state(gs);
}

} // exploding_kittens