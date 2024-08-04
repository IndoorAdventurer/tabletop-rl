#ifndef EK_ACTION_DEFS_H
#define EK_ACTION_DEFS_H

#include "../game_state.h"


namespace exploding_kittens {

/**
 * @brief Enum defining all possible actions in the game. Not all of them are
 * atomic: some need to target another player, for example.
 */
enum class ActionType {
    Default,                    // Draw card or don't play Nope
    Play_Defuse,                // Args: position to place kitten

    Play_Nope,
    Play_Skip,
    Play_Attack,
    Play_Shuffle,
    Play_See_Future,
    Play_Favor,                 // Args: player to get favor from
    Give_Favor,                 // Args: card to give

    Play_Two_Card_Combo,        // Args: player to take card from
    Play_Three_Card_Combo       // Args: player and card
    // FIXME: problem I need to solve: how to let agent pick the specific
    // cards that make up the combo?
};

enum class ActionArg {
    Void,           // No argument
    
    // Possible argument types for Arg1:
    Position,       // Position where to place kitten after defuse
    Player,         // Index of another player
    
    // Possible argument types for Arg2:
    Card            // CardIdx argument
};

/**
 * @brief Abstract base class for actions. All possible actions in the game
 * should derive from it, and redefine the specified static member functions.
 * 
 * @tparam A The corresponding action enum type.
 * @tparam Arg1 The type for the first argument (Void, Position or Player)
 * @tparam Arg2 The type for the second argument (Void or Card)
 */
template <
    ActionType A,
    ActionArg Arg1 = ActionArg::Void, ActionArg Arg2 = ActionArg::Void>
struct Action {
    static constexpr ActionType Type = A;

    /**
     * @brief Check if an action is legal in the current state
     * 
     * @param gs The current game state.
     * @param arg1 First action argument (void, position or player)
     * @param arg2 Second action argument (void or card)
     * @return true if action can be taken, false if not.
     */
    static bool is_valid(const GameState &gs, uint8_t arg1, uint8_t arg2);

    /**
     * @brief Make the action take place: change the game state accordingly
     * 
     * @param gs The game state to apply changes to
     * @param arg1 First action argument (void, position or player)
     * @param arg2 Second action argument (void or card)
     */
    static void play_action(GameState &gs, uint8_t arg1, uint8_t arg2);
};

} // namespace exploding_kittens

#endif // EK_ACTION_DEFS_H