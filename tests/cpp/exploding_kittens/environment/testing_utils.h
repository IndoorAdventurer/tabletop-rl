// Some extra functions for often needed during testing.
#ifndef TESTING_UTILS_H
#define TESTING_UTILS_H

#include <gtest/gtest.h>
#include "exploding_kittens/environment/cards.h"

#include <array>

namespace exploding_kittens {

// Sum over all the rows: get total count for each type of card.
std::array<size_t, UNIQUE_CARDS> row_sums(Cards &cards);

// Sum over a column: a player hand or deck or discard pile
size_t col_sum(const CardCollection &col);

// Get a counts representation from an ordered vector:
std::array<size_t, UNIQUE_CARDS> to_counts(CardStack &col);

// Test the integrity of cards according to some rules.
testing::AssertionResult cards_integrity_check(Cards &cards);

} // namespace exploding_kittens

#endif // TESTING_UTILS_H