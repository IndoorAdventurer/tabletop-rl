#ifndef EK_PLAY_NOPE_H
#define EK_PLAY_NOPE_H

#include "../action_type.h"


namespace exploding_kittens {

/**
 * @brief Block the action of the current player. In the actual game, a nope
 * card can basically be played at any time. In this implementation, all players
 * with a nope cards get their turn to nope in random order after each nopable
 * action.
 */
class PlayNope: public ActionType {

    public:
        PlayNope(GameState &gs);

        void append_legal_actions(std::vector<Action> &vec) const override;

    protected:
        // Negate the nope flag and again give everyone a chance to nope:
        void do_take_action(Action const &a) override;
};

inline PlayNope::PlayNope(GameState &gs)
:
    ActionType(ActionEnum::Play_Nope, gs)
{}

} // namespace exploding_kittens

#endif // EK_PLAY_NOPE_H