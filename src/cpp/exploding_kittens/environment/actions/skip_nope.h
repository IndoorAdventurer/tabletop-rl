#ifndef EK_SKIP_NOPE_H
#define EK_SKIP_NOPE_H

#include "../action_type.h"


namespace exploding_kittens {

/**
 * @brief When you are allowed to play a nope card, but you decide not to.
 */
class SkipNope: public ActionType {

    public:
        SkipNope(GameState &gs);

        void append_legal_actions(std::vector<Action> &vec) const override;

    protected:
        // Move on to the next player that can nope, or exit nope state:
        void do_take_action(Action const &a) override;
};

inline SkipNope::SkipNope(GameState &gs)
:
    ActionType(ActionEnum::Skip_Nope, gs)
{}

} // namespace exploding_kittens

#endif // EK_SKIP_NOPE_H