#include "game_state.h"

namespace exploding_kittens {

void GameState::reset(size_t num_players) {
    cards.reset(num_players);
    state = State::Default;
    current_player = 0;     // Player 0 always starts.
    turns_left = 1;         // 1 turn p.p. by default. (Attack gives >1)
}

uint8_t GameState::next_player(uint8_t player) const
{
    do
        player = (player + 1) % cards.hands.size();
    while (not is_alive(player));
    return player;
}

void GameState::register_turn() {
    if (--turns_left == 0) {
        turns_left = 1;
        current_player = next_player(current_player);
    }
}

} // namespace exploding_kittens