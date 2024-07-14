#ifndef EK_CARD_COLLECTION_H
#define EK_CARD_COLLECTION_H

#include "card_defs.h"

#include <cstdint>
#include <span>
#include <vector>


namespace exploding_kittens {

/**
 * @brief Base class for the card stacks (deck and discard pile), and player
 * hands.
 */
class CardCollection {

    // A span that provides a view into the d_data member of the Cards object.
    std::span<uint8_t, UNIQUE_CARDS> d_data;

    public:
        /**
         * @param view A view into an existing array. Must be UNIQUE_CARDS
         * in length!
         */
        CardCollection(uint8_t *view);

        /**
         * @brief Returns the number of cards this collection has of type i.
         * 
         * @param i The unique card index (casted CardIdx value)
         * @return The number of cards of type i in collection.
         */
        uint8_t has(uint8_t i) const;
        uint8_t has(CardIdx i) const;
        
        /**
         * @return The data inside the span object (i.e. -> span::data())
         */
        uint8_t *data();
    
    protected:
        /**
         * @brief Insert card i into the collection
         * 
         * @param i The card to be inserted
         */
        void base_insert(CardIdx i);

        /**
         * @brief Remove card i from the collection
         * 
         * @param i The card to be removed
         * @return true if succeeded, false if card was not present.
         */
        bool base_remove(CardIdx i);

};

inline uint8_t CardCollection::has(uint8_t i) const {
    return d_data[i];
}

inline uint8_t CardCollection::has(CardIdx i) const {
    return d_data[to_uint(i)];
}

inline uint8_t *CardCollection::data() {
    return d_data.data();
}

inline void CardCollection::base_insert(CardIdx i) {
    ++d_data[to_uint(i)];
}

inline bool CardCollection::base_remove(CardIdx i) {
    if (d_data[to_uint(i)] <= 0)
        return false;
    --d_data[to_uint(i)];
    return true;
}

/**
 * @brief An ordered CardCollection, such as the deck and discard pile.
 */
class CardStack: public CardCollection {
    
    // An ordered representation of d_data. They should always agree.
    std::vector<CardIdx> d_ordered;
    
    public:
        using CardCollection::CardCollection;
        
        /**
         * @brief Computes a valid state of d_ordered from its d_data.
         */
        void ordered_from_data();

        /**
         * @brief Shuffle order of cards
         */
        void shuffle();                             // TODO

        /**
         * @param i Card to be placed on top of the stack.
         */
        void push(CardIdx i);                       // TODO

        /**
         * @brief Remove card from top of stack.
         * 
         * @return The removed card.
         */
        CardIdx pop();                              // TODO

        /**
         * @brief Insert a card somewhere in the stack.
         * 
         * @param i The card to insert.
         * @param depth The location: 0 means place on top, 1 means place
         * under top card, etc. ***NOTE!*** if depth is larger than pile size,
         * card gets placed on the bottom.
         */
        void insert(CardIdx i, int depth);          // TODO
        
        const CardIdx *view_top_n(size_t n) const;  // TODO

};


/**
 * @brief A player's hand in the Exploding Kittens game. Has no inherent order.
 */
class CardHand: public CardCollection {

    public:
        using CardCollection::CardCollection;

        /**
         * @brief Insert a card into hand.
         * 
         * @param i Card to insert.
         */
        void insert(CardIdx i);

        /**
         * @brief Remove a card from hand.
         * 
         * @param i Card to remove
         * @return true if succeeded, false if card was not present.
         */
        bool remove(CardIdx i);

        /**
         * @brief Remove a random card from the hand.
         * 
         * @return The removed card 
         */
        CardIdx random_remove();
};

inline void CardHand::insert(CardIdx i) {
    return base_insert(i);
}

inline bool CardHand::remove(CardIdx i) {
    return base_remove(i);
}

}

#endif // EK_CARD_COLLECTION_H