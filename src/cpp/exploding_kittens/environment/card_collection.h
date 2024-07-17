#ifndef EK_CARD_COLLECTION_H
#define EK_CARD_COLLECTION_H

#include "../../utils.h"
#include "card_defs.h"

#include <cstdint>
#include <span>
#include <vector>
#include <algorithm>


namespace exploding_kittens {

/**
 * @brief Base class for the card stacks (deck and discard pile), and player
 * hands.
 */
class CardCollection {

    // Span that provides view into d_card_counts member of the Cards object.
    std::span<uint8_t, UNIQUE_CARDS> d_card_counts;

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
         * @return The data inside the d_card_counts span (i.e. span::data())
         * @todo Maybe change to something other than a pointer later on.
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

        friend class Cards;
};


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
         * @brief Shuffle order of cards
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
        void insert(CardIdx i, int depth);          // TODO
        
        /**
         * @brief Get the n cards from the top of the stack.
         * 
         * @param n Number of cards to get from the top of the stack.
         * @return A span that provides a view into the vector.
         */
        std::span<CardIdx> get_top_n(size_t n);
};

/**
 * @brief A player's hand in the Exploding Kittens game. Has no inherent order.
 */
class CardHand: public CardCollection {

    public:
        using CardCollection::CardCollection;

        // /**
        //  * @brief Insert a card into hand.
        //  * 
        //  * @param i Card to insert.
        //  */
        // void insert(CardIdx i);

        // /**
        //  * @brief Remove a card from hand.
        //  * 
        //  * @param i Card to remove
        //  * @return true if succeeded, false if card was not present.
        //  */
        // bool remove(CardIdx i);

        /**
         * @brief Remove a random card from the hand.
         * 
         * @return The removed card 
         */
        CardIdx random_remove();
};

} // namespace exploding_kittens

#include "card_collection.ih"

#endif // EK_CARD_COLLECTION_H