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
 * are where. This class is encapsulates that information, as well as who knows
 * which cards are where.
 */
class Cards {

    static constexpr size_t MAX_ROWS = MAX_PLAYERS + FIRST_PLAYER_IDX;
    static constexpr size_t COLS = UNIQUE_CARDS;

    // d_card_counts[COLS * location + card_type] = num_cards_of_type.
    std::array<uint8_t, MAX_ROWS * COLS> d_card_counts;
        
    public:
        CardStack deck;
        CardStack discard_pile;
        std::vector<CardHand> hands;

        Cards();

        // Disable copy and move semantics. We just want one.
        Cards(const Cards &) = delete;
        Cards &operator=(const Cards &) = delete;
        Cards(Cards &&) = delete;
        Cards &operator=(Cards &&) = delete;
        
        /**
         * @brief Reinitializes the game to the starting configuration.
         * 
         * @param num_players The number of players for the new game.
         */
        void reset(size_t num_players);
    
    private:
        /**
         * @brief Responsible for reinitializing d_card_counts in call to
         * public reset function.
         * 
         * @param num_players The number of players for the new game.
         */
        void reset_card_counts(size_t num_players);
};

inline void Cards::reset(size_t num_players) {
    reset_card_counts(num_players);
}

} // namespace exploding_kittens

#endif // EK_CARDS_H