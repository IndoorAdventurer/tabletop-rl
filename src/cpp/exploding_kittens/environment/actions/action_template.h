#ifndef TODO_H
#define TODO_H

#include "action_defs.h"


namespace exploding_kittens {

struct ToDo : public Action<ActionType::Default> {
    static bool is_valid(const GameState &gs, uint8_t arg1, uint8_t arg2);
    static void play_action(GameState &gs, uint8_t arg1, uint8_t arg2);
};

// I don't think these can actually be inlined: used with function pointers.
inline bool ToDo::is_valid(
                    const GameState &gs, uint8_t arg1, uint8_t arg2) {
    // TODO
    return true;
}

inline void ToDo::play_action(
                    GameState &gs, uint8_t arg1, uint8_t arg2) {
    // TODO
}

} // namespace exploding_kittens

#endif // TODO_H