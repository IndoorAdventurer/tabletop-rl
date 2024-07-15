#ifndef EK_CARDS_H
#define EK_CARDS_H

#include "card_defs.h"
#include "game_defs.h"
#include "card_collection.h"

#include <cstdint>
#include <array>
#include <vector>

namespace exploding_kittens {

/**
 * @brief The current state of the game is in large determined by which cards
 * are where. This class is responsible for that.
 */
struct Cards {

    static constexpr size_t DECK_IDX = 0;
    static constexpr size_t DISCARD_PILE_IDX = 1;
    static constexpr size_t FIRST_PLAYER_IDX = 2;
    static constexpr size_t MAX_ROWS = MAX_PLAYERS + FIRST_PLAYER_IDX;
    static constexpr size_t COLS = UNIQUE_CARDS;

    private:
        // A 2D array: d_data[location][card_type] = num_cards_of_type.
        std::array<uint8_t, MAX_ROWS * COLS> d_data;
        
    public:
        CardStack deck;
        CardStack discard_pile;
        std::vector<CardHand> hands;

        Cards();
        
        /**
         * @brief Reinitializes the game to the starting configuration.
         * 
         * @param num_players The number of players for the new game.
         */
        void reset(size_t num_players);

        /**
         * @brief Returns a pointer to the internal data of the object
         */
        uint8_t *data();
};

inline uint8_t *Cards::data() {
    return d_data.data();
}

}

#endif // EK_CARDS_H