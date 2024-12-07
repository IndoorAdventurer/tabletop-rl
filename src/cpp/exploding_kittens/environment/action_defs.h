#ifndef EK_ACTION_DEFS_H
#define EK_ACTION_DEFS_H

#include "game_state.h"


namespace exploding_kittens {

/**
 * @brief Enum defining all possible actions in the game. Not all of them are
 * atomic: some need to target another player, for example.
 */
enum class ActionEnum : uint8_t {
    Default = 0U,               // Draw card or don't play Nope
    Play_Defuse,                // Args: position to place kitten

    Play_Nope,
    Play_Skip,
    Play_Attack,
    Play_Shuffle,
    Play_See_Future,
    Play_Favor,                 // Args: player to get favor from
    Give_Favor,                 // Args: card to give

    Play_Two_Card_Combo,        // Args: player to take card from
    Play_Three_Card_Combo       // Args: player AND card
    // FIXME: problem I need to solve: how to let agent pick the specific
    // cards that make up the combo?
};

/**
 * @brief A struct that defines a specific action. It contains all the needed
 * information to describe the corresponding state change.
 */
struct Action {
    /** @brief The general type for this specific action. Used to link it back
     * to corresponding ActionType.
     */
    ActionEnum type;

    /** @brief Cards that need to get played for this action. */
    std::array<uint8_t, UNIQUE_CARDS> cards;
    
    /** @brief Argument 1: can specify position in deck or opponent index. */
    uint8_t arg1;

    /** @brief Argument 2: target card (for 3 card combo) */
    uint8_t arg2;
};

} // namespace exploding_kittens

#endif // EK_ACTION_DEFS_H