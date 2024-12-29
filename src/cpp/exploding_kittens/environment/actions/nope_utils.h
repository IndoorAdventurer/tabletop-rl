#ifndef EK_NOPE_UTILS_H
#define EK_NOPE_UTILS_H

#include "../action_type.h"


namespace exploding_kittens {

/**
 * @brief Base class for any action that can be noped. Classes that derive from
 * it should implement the enforce_action method instead of the do_take_action
 * method.
 */
class NopeableBase: public ActionType {

    public:
        using ActionType::ActionType;
    
    protected:
        // drops the cards and moves to nope state:
        void do_take_action(Action const &a) override;

        /**
         * @brief This method gets executed if the action survived the nope
         * state. (i.e. zero or an even number of nopes).
         * 
         * @warning The do_take_action method plays all cards in a.cards. Do not
         * do this a second time, of course..
         */
        virtual void enforce_action(Action const &a) = 0;

        friend void nopers_to_secondaries(GameState &g);
        friend void exit_nope_state(GameState &g);
};

/**
 * @brief Sets g.secondary_players to a randomly shuffled list of players that
 * are allowed to nope.
 */
void nopers_to_secondaries(GameState &g);

/**
 * @brief executes the pending action if g.is_noped is false. Then returns to
 * default state.
 */
void exit_nope_state(GameState &g);

} // namespace exploding_kittens

#endif // EK_NOPE_UTILS_H