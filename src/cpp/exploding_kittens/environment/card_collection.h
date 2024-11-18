#ifndef EK_CARD_COLLECTION_H
#define EK_CARD_COLLECTION_H

#include "../../utils.h"
#include "card_defs.h"

#include <cstdint>
#include <array>


namespace exploding_kittens {

/**
 * @brief Base class for the card stacks (deck and discard pile), and player
 * hands.
 */
class CardCollection {

    // For each card, specifies how many there are in this collection.
    std::array<uint8_t, UNIQUE_CARDS> d_card_counts;

    public:
        
        /**
         * @brief Default constructor. Beware! does not achieve a valid initial
         * state. For that, see Cards::init_new_game.
         */
        CardCollection() = default;

        /**
         * @brief Returns the number of cards this collection has of type i.
         * 
         * @param i The unique card index (casted CardIdx value)
         * @return The number of cards of type i in collection.
         */
        uint8_t has(uint8_t i) const;
        uint8_t has(CardIdx i) const;
        
        /**
         * @return The data inside the d_card_counts span (i.e. span::data())
         * @todo Maybe change to something other than a pointer later on, like a
         * span
         */
        uint8_t *counts();
    
    protected:
        /**
         * @brief Insert card i into the collection
         */
        void base_insert(CardIdx i);

        /**
         * @brief Remove card i from the collection
         * 
         * @return true if succeeded, false if card was not present.
         */
        bool base_remove(CardIdx i);

        /**
         * @brief Returns a random card (probabilities proportional to counts).
         * If empty, returns CardIdx::Error.
         */
        CardIdx random_card() const;

        friend class Cards;
};

inline uint8_t CardCollection::has(uint8_t i) const {
    return d_card_counts[i];
}

inline uint8_t CardCollection::has(CardIdx i) const {
    return d_card_counts[to_uint(i)];
}

inline uint8_t *CardCollection::counts() {
    return d_card_counts.data();
}

inline void CardCollection::base_insert(CardIdx i) {
    ++d_card_counts[to_uint(i)];
}

} // namespace exploding_kittens

#endif // EK_CARD_COLLECTION_H