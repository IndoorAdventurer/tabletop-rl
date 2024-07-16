// Definitions related to the Exploding Kittens game, with the exception of
// definitions related to cards. For that, see "card_defs.h".

#ifndef EK_GAME_DEFS_H
#define EK_GAME_DEFS_H

#include <cstddef>

namespace exploding_kittens {

constexpr size_t MIN_PLAYERS = 2;   // Minimum allowed number of players.
constexpr size_t MAX_PLAYERS = 5;   // Maximum allowed number of players.
constexpr size_t CARDS_2_DEAL = 7;  // Init cards per player. Excludes defuse.

// ---GAME-STATES:------------------------------------------------------------
// TODO

// ---ACTIONS:----------------------------------------------------------------
// TODO

} // namespace exploding_kittens

#endif // EK_GAME_DEFS_H