#include "play_defuse.h"

namespace exploding_kittens {

void PlayDefuse::append_legal_actions(std::vector<Action> &vec) const {
    if (gs.state != State::Defuse)
        return;
    
    // I am assuming it has a defuse and it has an exploding kitten: it is the
    // responsibility of DrawCard to ensure this is the case.

    Action act{ // Braces guarantee zero init for std::array
        ActionEnum::Play_Defuse, std::array<uint8_t, UNIQUE_CARDS>{}, 0U, 0U};
    // act.cards doesn't get used, but I still want it to be correct and nice :-)
    act.cards[to_uint(CardIdx::Exploding_Kitten)] = 1U;
    act.cards[to_uint(CardIdx::Defuse)] = 1U;

    // Appending all options (i.e. legal positions to place kitten):
    uint8_t max_depth = gs.cards.deck.size();
    for (uint8_t d = 0U; d <= max_depth; ++d) {
        act.arg1 = d;
        vec.push_back(act);
    }
}

void PlayDefuse::do_take_action(Action const &a) {
    // Move cards around:
    size_t depth = a.arg1;
    gs.current_hand().place_at(gs.cards.discard_pile, CardIdx::Defuse);
    gs.current_hand().place_at(gs.cards.deck, CardIdx::Exploding_Kitten, depth);
    gs.state = State::Default;

    // I still need to register the turn:
    gs.register_turn();
}

} // namespace exploding_kittens