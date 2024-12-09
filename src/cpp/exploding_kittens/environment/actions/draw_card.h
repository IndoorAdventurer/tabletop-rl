#ifndef EK_DRAW_CARD_H
#define EK_DRAW_CARD_H

#include "../action_type.h"


namespace exploding_kittens {

/**
 * @brief Draw a card from the deck. Unless you play a skip or attack, your
 * turn always ends with this action.
 */
class DrawCard: public ActionType {
    
    public:
        DrawCard(GameState &gs);

        void append_legal_actions(std::vector<Action> &vec) const override;

    protected:
        void do_take_action(Action const &a) override;
};

inline DrawCard::DrawCard(GameState &gs)
:
    ActionType(ActionEnum::Draw, gs)
{}

} // namespace exploding_kittens

#endif // EK_DRAW_CARD_H