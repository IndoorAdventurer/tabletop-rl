#ifndef EK_PLAY_DEFUSE_H
#define EK_PLAY_DEFUSE_H

#include "../action_type.h"


namespace exploding_kittens {

/**
 * @brief If you have an exploding kitten in your hand, you will need to place
 * it back into the deck.
 */
class PlayDefuse: public ActionType {

    public:
        PlayDefuse(GameState &gs);

        void append_legal_actions(std::vector<Action> &vec) const override;

    protected:
        // Place back an exploding kitten at depth specified by arg1.
        void do_take_action(Action const &a) override;
};

inline PlayDefuse::PlayDefuse(GameState &gs)
:
    ActionType(ActionEnum::Play_Defuse, gs)
{}

} // namespace exploding_kittens

#endif // EK_PLAY_DEFUSE_H