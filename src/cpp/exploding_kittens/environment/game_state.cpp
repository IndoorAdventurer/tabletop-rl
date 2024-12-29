#include "game_state.h"

namespace exploding_kittens {

void GameState::reset(size_t num_players) {
    cards.reset(num_players);
    state = State::Default;
    primary_player = 0;     // Player 0 always starts.
    turns_left = 1;         // 1 turn p.p. by default. (Attack gives >1)
}

uint8_t GameState::next_player(uint8_t player) const
{
    do
        player = (player + 1) % num_players();
    while (not is_alive(player));
    return player;
}

void GameState::register_turn() {
    if (--turns_left == 0) {
        turns_left = 1;
        primary_player = next_player(primary_player);
    }
}

// FNV-1a hash from:
//   https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
uint32_t GameState::hash() {
    constexpr uint32_t fnv_prime = 0x01000193u;
    constexpr uint32_t fnv_offset = 0x811c9dc5u;
    uint32_t hash = fnv_offset;

    // Hashing the minimum card data to capture whole state:
    for (CardIdx i : cards.deck.d_ordered) {
        hash ^= to_uint(i);
        hash *= fnv_prime;
    }
    for (CardIdx i : cards.discard_pile.d_ordered) {
        hash ^= to_uint(i);
        hash *= fnv_prime;
    }
    for (size_t player = 0; player != num_players(); ++player) {
        uint8_t *counts = cards.hands[player].counts();
        for (size_t i = 0; i != UNIQUE_CARDS; ++i) {
            hash ^= counts[i];
            hash *= fnv_prime;
        }
    }

    // Hashing all the other stuff:
    uint8_t nums[] = {
        static_cast<uint8_t>(state), primary_player, turns_left,
        static_cast<uint8_t>(is_noped),
        static_cast<uint8_t>(staged_action.type),
        staged_action.arg1, staged_action.arg2
    };
    for (uint8_t i : nums) {
        hash ^= i;
        hash *= fnv_prime;
    }
    for (uint8_t i : secondary_players) {
        hash ^= i;
        hash *= fnv_prime;
    }

    return hash;
}

} // namespace exploding_kittens