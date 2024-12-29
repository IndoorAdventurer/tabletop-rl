#include "draw_card.h"

namespace exploding_kittens {

void DrawCard::append_legal_actions(std::vector<Action> &vec) const {
    if (gs.state == State::Default)
        vec.emplace_back(   // Braces guarantee zero-init for std::array:
            ActionEnum::Draw, std::array<uint8_t, UNIQUE_CARDS>{}, 0U, 0U);
}

void DrawCard::do_take_action(Action const &a) {
    CardIdx i = gs.primary_hand().take_from(gs.cards.deck);
    if (i != CardIdx::Exploding_Kitten) {   // Normal card drawn.
        gs.register_turn();
        return;
    }

    if (gs.primary_hand().has(CardIdx::Defuse)) {
        gs.state = State::Defuse;           // Exploding Kitten drawn, but we
        return;                             // can defuse. No turn registered!
    }

    // If we get here, player is dead.

    if (gs.cards.deck.has(CardIdx::Exploding_Kitten) == 0)
        gs.state = State::Game_Over;        // No kittens left == game over.

    else {                  // Game did not end. Moving to next player.
        gs.turns_left = 1;  // Dying player eats remaining attacks.
        gs.register_turn();
    }
}

} // namespace exploding_kittens