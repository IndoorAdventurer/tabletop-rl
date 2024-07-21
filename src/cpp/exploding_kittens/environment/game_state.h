#ifndef EK_GAME_STATE_H
#define EK_GAME_STATE_H

#include "game_defs.h"
#include "cards.h"

#include <cstdint>


namespace exploding_kittens {

/**
 * @brief All information that captures the current state of the game is stored
 * in this struct. It is composed of:
 * - A Cards object, specifying which cards are where;
 * - All other data, such as who's turn it is, etc.
 */
struct GameState {
    Cards cards;
    
    // Primary state info (initialized at reset):
    State state;            // A discrete description of current state.
    uint8_t current_player; // Player who's turn it is.
    uint8_t turns_left;     // Related to the attack card: number of cards to
                            // draw (or skips to play :-p).
    
    // Additional state info (used by some non-default states):
    uint8_t secondary_player;   // Player to nope, or to give favor.
    // TODO everything needed for nopes. 

    GameState() = default;
    
    // Disable copy and move semantics. We just want one.
    GameState(const GameState &) = delete;
    GameState &operator=(const GameState &) = delete;
    GameState(GameState &&) = delete;
    GameState &operator=(GameState &&) = delete;

    /**
     * @brief Reset own state to that of a new game.
     * 
     * @param num_players The number of players for the new game.
     */
    void reset(size_t num_players);

    /**
     * @brief Convenience function: for a given player (index), returns who would
     * logically come next.
     */
    uint8_t next_player(uint8_t player) const;

    /**
     * @brief applies the state changes needed after a turn is taken (i.e. card
     * drawn, or skip played).
     */
    void register_turn();
};

inline void GameState::reset(size_t num_players) {
    cards.reset(num_players);
    state = State::Default;
    current_player = 0;     // Player 0 always starts.
    turns_left = 1;         // Only attack gives >1
}

inline uint8_t GameState::next_player(uint8_t player) const {
    return ++player % cards.hands.size();
}

inline void GameState::register_turn() {
    if (--turns_left == 0) {
        turns_left = 1;
        current_player = next_player(current_player);
    }
}

} // namespace exploding_kittens

#endif // #ifndef EK_GAME_STATE_H