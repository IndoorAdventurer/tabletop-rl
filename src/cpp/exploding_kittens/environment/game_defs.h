// Definitions related to the Exploding Kittens game in general.

#ifndef EK_GAME_DEFS_H
#define EK_GAME_DEFS_H

#include <cstddef>

namespace exploding_kittens {

constexpr size_t MIN_PLAYERS = 2;   // Minimum allowed number of players.
constexpr size_t MAX_PLAYERS = 5;   // Maximum allowed number of players.
constexpr size_t CARDS_2_DEAL = 7;  // Init cards per player. Excludes defuse.

/**
 * @brief Enum to describe discrete game states.
 */
enum class State {
    Default,    // Starting state.
    Defuse,     // After drawing defusable exploding kitten. Place back in deck.
    Nope,       // After a nopable card was played. Give option to nope
    Favor,      // After favor card was played. Choose card to give.
    Game_Over,
};

} // namespace exploding_kittens

#endif // EK_GAME_DEFS_H