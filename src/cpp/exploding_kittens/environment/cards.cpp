#include "cards.h"

#include <algorithm>

namespace exploding_kittens {


void Cards::reset_card_counts(size_t num_players) {
    // Re-initializing deck and discard pile's d_card_counts:
    initArray<CardInfoField::init_deck>(num_players, deck.counts());
    // Putting everything to still divide in discard pile since it is not used
    initArray<CardInfoField::init_rand>(num_players, discard_pile.counts());
    
    // Re-initializing the player hands:
    hands = std::span<CardHand>{
        d_hands_internal.begin(), d_hands_internal.begin() + num_players};
    for (CardHand &hand : hands) {
        initArray<CardInfoField::init_hand>(num_players, hand.counts());
    }

    // Deal cards:
    discard_pile.ordered_from_data();
    discard_pile.shuffle();       // All remaining cards are on discard pile
    for (CardHand &hand : hands) {
        for (size_t i = 0; i != CARDS_2_DEAL; ++i) {
            CardIdx card = discard_pile.pop();
            hand.base_insert(card);
        }
    }

    // Move rest from discard pile to deck:
    for (size_t i = 0; i != UNIQUE_CARDS; ++i)
        deck.d_card_counts[i] += discard_pile.d_card_counts[i];
    std::fill(discard_pile.d_card_counts.begin(), discard_pile.d_card_counts.end(), 0);
    discard_pile.ordered_from_data();
    deck.ordered_from_data();
    deck.shuffle();
}

} // namespace exploding_kittens