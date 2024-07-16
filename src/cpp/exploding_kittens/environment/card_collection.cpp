#include "card_collection.h"


namespace exploding_kittens
{

CardCollection::CardCollection(uint8_t *view)
    : d_card_counts(view, UNIQUE_CARDS)
{
}

void CardStack::ordered_from_data()
{
    d_ordered.clear();
    for (uint8_t i = 0; i != UNIQUE_CARDS; ++i)
    {
        d_ordered.insert(d_ordered.end(), has(i), from_uint(i));
    }
}

} // namespace exploding_kittens