#ifndef EK_CARDS_H
#define EK_CARDS_H

#include "card_defs.h"
#include "game_defs.h"
#include "card_collection.h"

#include <cstdint>
#include <array>
#include <span>

namespace exploding_kittens {

/**
 * @brief The current state of the game is in large determined by which cards
 * are where. This class encapsulates that information.
 */
struct Cards {
         
    CardStack deck;
    CardStack discard_pile;
    std::span<CardHand> hands;

    /**
     * @brief Construct a new Cards object. Note that Cards::reset should be
     * called before obtaining a valid state of the object.
     */
    Cards() = default;

    // Disable copy and move semantics. We just want one.
    Cards(const Cards &) = delete;
    Cards &operator=(const Cards &) = delete;
    Cards(Cards &&) = delete;
    Cards &operator=(Cards &&) = delete;
    
    /**
     * @brief Reset own state to that of a new game.
     * 
     * @param num_players The number of players for the new game.
     */
    void reset(size_t num_players);
    
    private:
        /**
         * @brief Following the setup rules of the game to get a starting
         * state.
         * 
         * @param num_players The number of players for the new game.
         * @todo I took the contents of this method out of the reset method
         * because I thought I wanted more from Cards. I don't anymore, so
         * might change it back later.
         */
        void init_new_game(size_t num_players);

        // internally creating all hands. The hands object is just a subrange
        // of it.
        std::array<CardHand, MAX_PLAYERS> d_hands_internal;
};

inline void Cards::reset(size_t num_players) {
    init_new_game(num_players);
}

} // namespace exploding_kittens

#endif // EK_CARDS_H