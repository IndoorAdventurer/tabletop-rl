#include "card_collection.h"


namespace exploding_kittens {

CardCollection::CardCollection(uint8_t *view)
:
    d_data(view, UNIQUE_CARDS)
{}


void CardStack::ordered_from_data() {
    
    // Inserting:
    d_ordered.clear();
    for (uint8_t i = 0; i != UNIQUE_CARDS; ++i) {
        d_ordered.insert(d_ordered.end(), has(i), from_uint(i));
    }
}

void CardStack::shuffle() {
    // TODO
}

}