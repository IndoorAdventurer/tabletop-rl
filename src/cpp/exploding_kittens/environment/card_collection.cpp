#include "card_collection.h"

#include "../../utils.h"

#include <numeric>
#include <random>
#include <stdexcept>

namespace exploding_kittens
{

CardIdx CardCollection::random_card() const {
    
    // Getting the total number of cards so we can give each an index:
    size_t total_cards = std::accumulate(
        d_card_counts.begin(), d_card_counts.end(), 0);
    if (total_cards == 0)
        return CardIdx::Error;

    // Getting a random index to pick:
    size_t rand_num = std::uniform_int_distribution(0UL, total_cards - 1)(
        tabletop_general::randnum_gen);
    
    // Seeing which card belongs to said index:
    uint8_t i = 0; uint8_t h = has(i);
    while (rand_num >= h) {
        rand_num -= h;
        h = has(++i);
    }
    return from_uint(i);
}

bool CardCollection::base_remove(CardIdx i) {
    if (d_card_counts[to_uint(i)] == 0)
        return false;
    --d_card_counts[to_uint(i)];
    return true;
}

} // namespace exploding_kittens