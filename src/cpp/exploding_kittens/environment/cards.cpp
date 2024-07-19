#include "cards.h"

#include <algorithm>
#include <stdexcept>

namespace exploding_kittens {


void Cards::init_new_game(size_t num_players) {
    if (num_players < MIN_PLAYERS or num_players > MAX_PLAYERS)
        throw std::invalid_argument("num_players out of legal range.");
    
    // Re-initializing deck and discard pile's d_card_counts. Discard pile
    // should be empty at init, but using it here to deal from:
    initArray<CardInfoField::init_deck>(num_players, deck.counts());
    initArray<CardInfoField::init_rand>(num_players, discard_pile.counts());
    
    // Re-initializing the player hands:
    hands = std::span<CardHand>{
        d_hands_internal.begin(), d_hands_internal.begin() + num_players};
    for (CardHand &hand : hands) {
        initArray<CardInfoField::init_hand>(num_players, hand.counts());
    }

    // Deal cards from discard pile to player hands:
    discard_pile.ordered_from_data();
    discard_pile.shuffle();
    for (CardHand &hand : hands) {
        for (size_t i = 0; i != CARDS_2_DEAL; ++i) {
            CardIdx card = discard_pile.pop();
            hand.base_insert(card);
        }
    }

    // Move the rest from discard pile to deck:
    for (size_t i = 0; i != UNIQUE_CARDS; ++i)
        deck.d_card_counts[i] += discard_pile.d_card_counts[i];
    std::fill(discard_pile.d_card_counts.begin(), discard_pile.d_card_counts.end(), 0);
    discard_pile.ordered_from_data();
    deck.ordered_from_data();
    deck.shuffle();
}

} // namespace exploding_kittens