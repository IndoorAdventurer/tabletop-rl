#ifndef EK_CARD_COLLECTION_H
#define EK_CARD_COLLECTION_H

#include "../../utils.h"
#include "card_defs.h"

#include <cstdint>
#include <span>
#include <algorithm>


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

        /**
         * @brief Returns a random card (probabilities proportional to counts).
         * If empty, returns CardIdx::Error.
         */
        CardIdx random_card() const;

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

/**
 * @brief A player's hand in the Exploding Kittens game.
 */
class CardHand: public CardCollection {

    public:
        using CardCollection::CardCollection;

        /**
         * @brief Take a card from the top of a stack.
         * 
         * @param stack Stack (i.e. deck or discard pile) to take from.
         * @return The card that was taken.
         */
        CardIdx take_from(CardStack &stack);
        
        /**
         * @brief Take a specified card from another player
         * 
         * @param other The hand of the other player.
         * @param i The card to select.
         * @return i if succeeded, CardIdx::Error if not.
         */
        CardIdx take_from(CardHand &other, CardIdx i);
        
        /**
         * @brief Take a random card from another player.
         * 
         * @param other The hand of the other player.
         * @return CardIdx The card you ended up picking, or CardIdx::Error.
         */
        CardIdx take_from(CardHand &other);

        /**
         * @brief Place a card on top of a stack.
         * 
         * @param stack The stack (deck/discard pile) to place the card on.
         * @param i The card to place there
         */
        void place_at(CardStack &stack, CardIdx i);
        
        /**
         * @brief Place a card somewhere in the stack
         * 
         * @param stack The stack (deck/discard pile) to place the card.
         * @param i The card to place there.
         * @param depth The CardStack::insert's depth parameter.
         */
        void place_at(CardStack &stack, CardIdx i, size_t depth);

        /**
         * @brief Give a card to another player.
         * 
         * @param other Hand of other player.
         * @param i Card to give.
         */
        void give_to(CardHand &other, CardIdx i);
};

} // namespace exploding_kittens

#include "card_collection.ih"

#endif // EK_CARD_COLLECTION_H