#ifndef EK_DEFAULT_ACTION_H
#define EK_DEFAULT_ACTION_H

#include "action_defs.h"
#include "../game_state.h"


namespace exploding_kittens {

struct DefaultAction : public Action<ActionType::Default> {
    // Default only possible in default state or nope state. No cards needed.
    static bool is_valid(const GameState &gs, uint8_t arg1, uint8_t arg2);
    static void play_action(GameState &gs, uint8_t arg1, uint8_t arg2);
    
    protected:
        // play_default for default state and nope state, respectively:
        static void from_default(GameState &gs, uint8_t arg1, uint8_t arg2);
        static void from_nope(GameState &gs, uint8_t arg1, uint8_t arg2);
};

inline bool DefaultAction::is_valid(
                    const GameState &gs, uint8_t arg1, uint8_t arg2) {
    return gs.state == State::Default or gs.state == State::Nope;
}

inline void DefaultAction::play_action(
                    GameState &gs, uint8_t arg1, uint8_t arg2) {
    if (gs.state == State::Default)
        return from_default(gs, arg1, arg2);
    from_nope(gs, arg1, arg2);
}

inline void DefaultAction::from_nope(
                    GameState &gs, uint8_t arg1, uint8_t arg2) {
    // TODO
}

} // namespace exploding_kittens

#endif // EK_DEFAULT_ACTION_H