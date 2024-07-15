#include <gtest/gtest.h>
#include "exploding_kittens/environment/cards.h"
#include <iostream>

namespace ek = exploding_kittens;

TEST(CardTests, InitChecks) {
    ek::Cards cards;
    cards.reset(5);
    ASSERT_EQ(cards.hands[0].has(ek::CardIdx::Defuse), 1)
        << "Must have one defuse for now, as I did not implement dealing yet.";
}