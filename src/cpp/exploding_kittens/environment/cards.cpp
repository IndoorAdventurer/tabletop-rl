#include "cards.h"

#include <algorithm>

namespace exploding_kittens {


Cards::Cards()
:
    d_data(),
    deck          (d_data.data() + COLS * DECK_IDX),
    discard_pile  (d_data.data() + COLS * DISCARD_PILE_IDX),
    hands()
{
    // Ensuring we never need to reallocate (for performance).
    hands.reserve(MAX_PLAYERS);
}

void Cards::reset(size_t num_players) {
    // Re-initializing deck and discard pile's d_data:
    initArray<CardInfoField::init_deck>(num_players, deck.data());
    deck.ordered_from_data();
    // Putting everything to still divide in discard pile since it is not used
    initArray<CardInfoField::init_rand>(num_players, discard_pile.data());
    discard_pile.ordered_from_data();
    
    // Re-initializing the player hands:
    hands.clear();
    for (size_t p = 0; p != num_players; ++p) {
        uint8_t *player_data = d_data.data() + COLS * (FIRST_PLAYER_IDX + p);
        hands.emplace_back(player_data);
        initArray<CardInfoField::init_hand>(num_players, player_data);
    }

    // Deal cards:
    discard_pile.shuffle();       // All remaining cards are on discard pile
    
    deck.shuffle();               // To mix in the exploding kittens better
}

}