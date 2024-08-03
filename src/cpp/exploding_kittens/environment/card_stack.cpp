#include "card_stack.h"

#include <stdexcept>

namespace exploding_kittens {

CardIdx CardStack::pop() {
    if (d_ordered.empty())
        throw std::out_of_range("Tried to pop from empty card stack.");
    CardIdx ret = d_ordered.back();
    base_remove(ret);
    d_ordered.pop_back();
    return ret;
}

void CardStack::ordered_from_data()
{
    d_ordered.clear();
    for (uint8_t i = 0; i != UNIQUE_CARDS; ++i)
        d_ordered.insert(d_ordered.end(), has(i), from_uint(i));
}

void CardStack::insert(CardIdx i, size_t depth) {
    base_insert(i);
    auto position = depth > d_ordered.size() ? d_ordered.begin() :
        (d_ordered.end() - depth);
    d_ordered.insert(position, i);
}

std::span<CardIdx> CardStack::get_top_n(size_t n) {
    auto begin = n > d_ordered.size() ? d_ordered.begin() :
        (d_ordered.end() - n);
    return std::span<CardIdx>(begin, d_ordered.end());
}

} // namespace exploding_kittens