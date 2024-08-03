#include "card_hand.h"

#include <stdexcept>

namespace exploding_kittens {

// ---CardHand:-----
CardIdx CardHand::take_from(CardStack &stack) {
    CardIdx i = stack.pop();
    base_insert(i);
    return i;
}

CardIdx CardHand::take_from(CardHand &other, CardIdx i) {
    if (other.base_remove(i)) {
        base_insert(i);
        return i;
    }
    return CardIdx::Error;
}

CardIdx CardHand::take_from(CardHand &other) {
    CardIdx i = other.random_card();
    if (i != CardIdx::Error)
        return take_from(other, i);
    return i;
}

void CardHand::place_at(CardStack &stack, CardIdx i) {
    if (not base_remove(i))
        throw std::range_error("Specified card not in own hand.");
    stack.push(i);
}

void CardHand::place_at(CardStack &stack, CardIdx i, size_t depth) {
    if (not base_remove(i))
        throw std::range_error("Specified card not in own hand.");
    stack.insert(i, depth);
}

void CardHand::give_to(CardHand &other, CardIdx i) {
    if (other.take_from(*this, i) == CardIdx::Error)
        throw std::range_error("Specified card not in own hand.");
}

} // namespace exploding_kittens