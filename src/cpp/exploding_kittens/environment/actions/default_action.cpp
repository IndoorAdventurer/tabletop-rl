#include "default_action.h"

namespace exploding_kittens {

void DefaultAction::from_default(
                    GameState &gs, uint8_t arg1, uint8_t arg2) {
    
    CardIdx i = gs.current_hand().take_from(gs.cards.deck);
    if (i != CardIdx::Exploding_Kitten) {   // Normal card drawn.
        gs.register_turn();
        return;
    }

    if (gs.current_hand().has(CardIdx::Defuse)) {
        gs.state = State::Defuse;           // Exploding Kitten drawn, but we
        return;                             // can defuse.
    }

    // If we get here, player is dead.
    
    if (gs.cards.deck.has(CardIdx::Exploding_Kitten) == 0)
        gs.state = State::Game_Over;        // No kittens left == game over.

    else {                  // Game did not end. Moving to next player.
    gs.turns_left = 1;      // Dying player eats remaining attacks.
    gs.register_turn();
    }

    // TODO: need some sort of on "player is dead" callback maybe..
}

} // namespace exploding_kittens