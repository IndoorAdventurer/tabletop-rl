#include "testing_utils.h"

#include <algorithm>

namespace exploding_kittens {

std::array<size_t, UNIQUE_CARDS> row_sums(Cards &cards) {
    std::array<size_t, UNIQUE_CARDS> ret;
    std::fill(ret.begin(), ret.end(), 0);
    for (size_t i = 0; i != UNIQUE_CARDS; ++i) {
        ret[i] += cards.deck.has(i) + cards.discard_pile.has(i);
        for (const CardHand &hand : cards.hands) {
            ret[i] += hand.has(i);
        }
    }
    return ret;
}

size_t col_sum(const CardCollection &col) {
    size_t sum = 0;
    for (size_t i = 0; i != UNIQUE_CARDS; ++i) {
        sum += col.has(i);
    }
    return sum;
}

std::array<size_t, UNIQUE_CARDS> to_counts(CardStack &col) {
    auto ordered = col.get_top_n(1000);   // 1000 ensures we get all of them
    std::array<size_t, UNIQUE_CARDS> counts;
    std::fill(counts.begin(), counts.end(), 0);
    for (CardIdx i : ordered)
        ++counts[to_uint(i)];
    return counts;
}

testing::AssertionResult cards_integrity_check(Cards &cards) {
    size_t num_players = cards.hands.size();
    
    auto failure = testing::AssertionFailure();
    bool failed = false;

    // Row sum checks:
    auto arr = row_sums(cards);

    auto idx_check = [&](const char *card_name, CardIdx i, uint8_t expected) {
        if (arr[to_uint(i)] != expected) {
            if (failed) failure << " | ";
            failed = true;
            failure << "[" << card_name << "]: expected sum of "
                << static_cast<int>(expected) << ", but got "
                << arr[to_uint(i)] << ".";
        }
    };
    idx_check("Exploding_Kitten", CardIdx::Exploding_Kitten, num_players - 1);
    idx_check("Defuse", CardIdx::Defuse, num_players + (num_players == 5 ? 1 : 2));
    idx_check("Nope", CardIdx::Nope, 5);
    idx_check("Skip", CardIdx::Skip, 4);
    idx_check("Attack", CardIdx::Attack, 4);
    idx_check("Shuffle", CardIdx::Shuffle, 4);
    idx_check("See_Future", CardIdx::See_Future, 5);
    idx_check("Favor", CardIdx::Favor, 4);
    idx_check("Cat_1", CardIdx::Cat_1, 4);
    idx_check("Cat_2", CardIdx::Cat_2, 4);
    idx_check("Cat_3", CardIdx::Cat_3, 4);
    idx_check("Cat_4", CardIdx::Cat_4, 4);
    idx_check("Cat_5", CardIdx::Cat_5, 4);

    
    // Stacks correspondence checks:
    auto stack_check = [&](const char *stack_name, CardStack &stack) {
        auto computed_counts = to_counts(stack);
        auto actual_counts = stack.counts();
        for (size_t i = 0; i != computed_counts.size(); ++i) {
            if (computed_counts[i] != actual_counts[i]) {
                if (failed) failure << " | ";
                failed = true;
                failure << "[" << stack_name << "]: mismatch at idx " << i
                    << " (" << static_cast<int>(computed_counts[i]) << " where "
                    << static_cast<int>(actual_counts[i]) << " was expected).";
            }
        }
    };
    stack_check("Deck", cards.deck);
    stack_check("Discard Pile", cards.discard_pile);

    if (failed)
        return failure;
    return testing::AssertionSuccess();
}

std::array<uint8_t, UNIQUE_CARDS> copy_counts(CardCollection &col) {
    std::array<uint8_t, UNIQUE_CARDS> counts;
    for (size_t i = 0; i != UNIQUE_CARDS; ++i)
        counts[i] = col.has(i);
    return counts;
}

std::vector<Action> get_legal_actions(ActionType &at) {
    std::vector<Action> av;
    at.append_legal_actions(av);
    return av;
}

} // namespace exploding_kittens