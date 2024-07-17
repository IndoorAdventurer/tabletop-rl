#ifndef EK_CARDS_H
#define EK_CARDS_H

#include "card_defs.h"
#include "game_defs.h"
#include "card_collection.h"

#include <cstdint>
#include <array>
#include <vector>
#include <utility>

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
    
    // internally creating all hands. Later only giving a smaller span.
    std::array<CardHand, MAX_PLAYERS> d_hands_internal;
        
    public:
        CardStack deck;
        CardStack discard_pile;
        std::span<CardHand> hands;

        Cards();    // Default constructor.

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
         * @todo split the reset function up because I was still thinking about
         * adding data about who knows what. Might not put that in the core
         * Cards class anymore tho, so might just move contents to reset().
         */
        void reset_card_counts(size_t num_players);

        /**
         * @brief Hidden constructor that does the heavy lifting. Uses dummy
         * param to extract Seq (sequence of offsets to point to in memory).
         */
        template <size_t ...Seq>
        Cards(std::index_sequence<Seq...>);
};

inline Cards::Cards()
:
    Cards(std::make_index_sequence<MAX_PLAYERS>())
{}

inline void Cards::reset(size_t num_players) {
    reset_card_counts(num_players);
}

template <size_t ...Indices>
Cards::Cards::Cards(std::index_sequence<Indices...>)
:
    d_card_counts(),
    d_hands_internal{ CardHand(
        d_card_counts.data() + COLS * (FIRST_PLAYER_IDX + Indices))... },
    deck          (d_card_counts.data() + COLS * DECK_IDX),
    discard_pile  (d_card_counts.data() + COLS * DISCARD_PILE_IDX),
    hands()
{}

} // namespace exploding_kittens

#endif // EK_CARDS_H