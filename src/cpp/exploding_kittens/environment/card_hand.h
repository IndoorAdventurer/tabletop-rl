#ifndef EK_CARD_HAND_H
#define EK_CARD_HAND_H

#include "card_collection.h"
#include "card_stack.h"

#include <cstdint>

namespace exploding_kittens {

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
         * @throws std::range_error if i not in own hand.
         */
        void place_at(CardStack &stack, CardIdx i);
        
        /**
         * @brief Place a card somewhere in the stack
         * 
         * @param stack The stack (deck/discard pile) to place the card.
         * @param i The card to place there.
         * @param depth The CardStack::insert's depth parameter.
         * @throws std::range_error if i not in own hand.
         */
        void place_at(CardStack &stack, CardIdx i, size_t depth);

        /**
         * @brief Give a card to another player.
         * 
         * @param other Hand of other player.
         * @param i Card to give.
         * @throws std::range_error if i not in own hand.
         */
        void give_to(CardHand &other, CardIdx i);
};

} // namespace exploding_kittens

#endif // EK_CARD_HAND_H