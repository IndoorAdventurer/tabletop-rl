#ifndef EK_GAME_STATE_H
#define EK_GAME_STATE_H

#include "game_defs.h"
#include "cards.h"
#include "action_defs.h"

#include <cstdint>
#include <vector>


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
    uint8_t primary_player; // Player who's turn it is.
    uint8_t turns_left;     // Related to the attack card: number of cards to
                            // draw (or skips to play :-p).
    
    // Secondary info (for specific cards like nope or favor):
    std::vector<uint8_t> secondary_players; // For noping and favor giving.
                                            // The next secondary is by
                                            // definition the one at .back().
    bool is_noped;                          // If true, action is noped
    Action staged_action;                   // To execute if not noped
    NopeableBase *action_type;              // Obj that can execute staged_action

    /**
     * @brief Constructor for GameState object
     * @note Does not result in a valid state. You need to call GameState::reset
     * first.
     */
    GameState() = default;
    
    // Disable copy and move semantics. We just want one for now.
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
     * @return the number of players in the current game.
     */
    uint8_t num_players() const;

    /**
     * @brief Convenience function: check if a player is alive or not.
     */
    bool is_alive(uint8_t player) const;

    /**
     * @brief shorthand for this->cards.hands[this->primary_player]
     */
    CardHand &primary_hand();

    /**
     * @brief shorthand for this->cards.hands[this->secondary_players.back()]
     */
    CardHand &secondary_hand();
    
    /**
     * @brief Convenience function: for a given player (index), returns who would
     * logically come next.
     */
    uint8_t next_player(uint8_t player) const;

    /**
     * @brief applies the state changes needed after a turn is taken, such as
     * moving to the next player, or decreasing turns_left.
     */
    void register_turn();

    /**
     * @return a hash for the current state.
     */
    uint32_t hash();
};

inline uint8_t GameState::num_players() const {
    return cards.hands.size();
}

inline bool GameState::is_alive(uint8_t player) const {
    // Expressing player is dead by the fact that they have an exploding kitten
    // and no defuse:
    return  cards.hands[player].has(CardIdx::Exploding_Kitten) == 0 or
            cards.hands[player].has(CardIdx::Defuse) != 0;
}

inline CardHand &GameState::primary_hand() {
    return cards.hands[primary_player];
}

inline CardHand &GameState::secondary_hand() {
    return cards.hands[secondary_players.back()];
}

} // namespace exploding_kittens

#endif // #ifndef EK_GAME_STATE_H