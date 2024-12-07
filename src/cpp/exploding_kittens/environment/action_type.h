#ifndef EK_ACTION_TYPE_H
#define EK_ACTION_TYPE_H

#include "action_defs.h"
#include "game_state.h"

#include <vector>


namespace exploding_kittens {

/**
 * @brief An abstract base class for action types. Every action type (like
 * defuse, attack, shuffle, etc.) needs a corresponding derivative of this class.
 * These derivatives will implement logic for determining if an action is legal
 * (e.g. do you have the right cards for it), as well as for altering the game
 * state accordingly.
 */
struct ActionType {

    const ActionEnum type;
    
    // Disable copy and move semantics:
    ActionType(const ActionType &) = delete;
    ActionType &operator=(const ActionType &) = delete;
    ActionType(ActionType &&) = delete;
    ActionType &operator=(ActionType &&) = delete;
    
    /**
     * @brief Appends actions that are legal to take from the current game state.
     */
    virtual void append_legal_actions(std::vector<Action> &vec) const = 0;

    /**
     * @brief Takes an action as input and alters the game state accordingly.
     * @warning Action should be of the correct type. This doesn't get checked.
     */
    void take_action(Action const &a);

    protected:
        GameState &gs;

        ActionType(ActionEnum type, GameState &gs);

        // The core logic of take_action. Should be implemented by each derived
        // class.
        virtual void do_take_action(Action const &a) = 0;
};

inline ActionType::ActionType(ActionEnum type, GameState &gs)
:
    type(type),
    gs(gs)
{}

inline void ActionType::take_action(Action const &a) {
    do_take_action(a);
}

} // namespace exploding_kittens

#endif // EK_ACTION_TYPE_H