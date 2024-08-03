#ifndef EK_CARD_STACK_H
#define EK_CARD_STACK_H

#include "card_collection.h"

#include <cstdint>
#include <vector>
#include <span>
#include <algorithm>

namespace exploding_kittens {

/**
 * @brief An ordered CardCollection, such as the deck and discard pile.
 */
class CardStack: public CardCollection {
    
    // An ordered representation of d_card_counts. They should always agree.
    std::vector<CardIdx> d_ordered;
    
    public:
        using CardCollection::CardCollection;
        
        /**
         * @brief Computes a valid state of d_ordered from its d_card_counts.
         * Note that shuffle() has to be called afterwards still!
         */
        void ordered_from_data();

        /**
         * @brief Shuffle the cards randomly.
         */
        void shuffle();

        /**
         * @param i Card to be placed on top of the stack.
         */
        void push(CardIdx i);

        /**
         * @brief Remove card from top of stack.
         * 
         * @return The removed card.
         */
        CardIdx pop();

        /**
         * @brief Insert a card somewhere in the stack.
         * 
         * @param i The card to insert.
         * @param depth The location: 0 means place on top, 1 means place
         * under top card, etc. ***NOTE!*** if depth is larger than pile size,
         * card gets placed on the bottom.
         */
        void insert(CardIdx i, size_t depth);
        
        /**
         * @brief Get the n cards from the top of the stack.
         * 
         * @param n Number of cards to get from the top of the stack.
         * @return A span that provides a view into the vector.
         */
        std::span<CardIdx> get_top_n(size_t n);
};

inline void CardStack::shuffle()
{
    std::shuffle(d_ordered.begin(), d_ordered.end(),
        tabletop_general::randnum_gen);
}

inline void CardStack::push(CardIdx i) {
    base_insert(i);
    d_ordered.push_back(i);
}

} // namespace exploding_kittens

#endif // EK_CARD_STACK_H