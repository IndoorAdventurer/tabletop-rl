// Definitions related to cards in the Exploding Kittens game.

#ifndef EK_CARD_DEFS_H
#define EK_CARD_DEFS_H

#include <cstdint>
#include <cstddef>

namespace exploding_kittens {

/**
 * @brief Gives an index to each unique card in the game.
 */
enum class CardIdx : uint8_t {
    Exploding_Kitten = 0U,
    Defuse,
    
    Nope,
    Skip,
    Attack,
    Shuffle,
    See_Future,
    Favor,

    Cat_1,
    Cat_2,
    Cat_3,
    Cat_4,
    Cat_5,

    Total,              // Number of elements. Always keep after last card!
    Error               // Error card: give on failure.
};

/**
 * @brief Convenience function to avoid static_casts everywhere
 */
constexpr uint8_t to_uint(CardIdx i) {
    return static_cast<uint8_t>(i);
}

/**
 * @brief Convenience function to avoid static_casts everywhere
 */
constexpr CardIdx from_uint(uint8_t i) {
    return static_cast<CardIdx>(i);
}

// Number of unique cards in the Exploding Kittens game.
constexpr size_t UNIQUE_CARDS = to_uint(CardIdx::Total);

/**
 * Struct defining constants related to a specific card.
 */
template <CardIdx c>
struct CardInfo {
    // Specialization should be given for every card, else compilation error.
    static_assert(c != c, "'CardInfo' struct not defined for given card.");
    enum {
        init_deck = -1,     // Put this many in deck at start of game
        init_hand = -1,     // Give each player this many at start of game
        init_rand = -1      // Randomly divide among deck and player hands
    };
};

// ---Now the card-specific definitions/specializations---:

template <> struct CardInfo<CardIdx::Exploding_Kitten> {
    static inline int init_deck(size_t num_players) {return num_players - 1; };
    enum { init_hand = 0, init_rand = 0 }; };

template <> struct CardInfo<CardIdx::Defuse> {
    enum { init_deck = 0, init_hand = 1 };
    static inline int init_rand(size_t num_players) {
        return num_players == 5 ? 1 : 2; } };

template <> struct CardInfo<CardIdx::Nope> {
    enum { init_deck = 0, init_hand = 0, init_rand = 5 }; };
template <> struct CardInfo<CardIdx::Skip> {
    enum { init_deck = 0, init_hand = 0, init_rand = 4 }; };
template <> struct CardInfo<CardIdx::Attack> {
    enum { init_deck = 0, init_hand = 0, init_rand = 4 }; };
template <> struct CardInfo<CardIdx::Shuffle> {
    enum { init_deck = 0, init_hand = 0, init_rand = 4 }; };
template <> struct CardInfo<CardIdx::See_Future> {
    enum { init_deck = 0, init_hand = 0, init_rand = 5 }; };
template <> struct CardInfo<CardIdx::Favor> {
    enum { init_deck = 0, init_hand = 0, init_rand = 4 }; };
template <> struct CardInfo<CardIdx::Cat_1> {
    enum { init_deck = 0, init_hand = 0, init_rand = 4 }; };
template <> struct CardInfo<CardIdx::Cat_2> {
    enum { init_deck = 0, init_hand = 0, init_rand = 4 }; };
template <> struct CardInfo<CardIdx::Cat_3> {
    enum { init_deck = 0, init_hand = 0, init_rand = 4 }; };
template <> struct CardInfo<CardIdx::Cat_4> {
    enum { init_deck = 0, init_hand = 0, init_rand = 4 }; };
template <> struct CardInfo<CardIdx::Cat_5> {
    enum { init_deck = 0, init_hand = 0, init_rand = 4 }; };

/**
 * @brief Convenience enum so we can index into the above struct later on.
 */
enum class CardInfoField {
    init_deck, init_hand, init_rand
};

/**
 * @brief Initializes the array in a call-by-pointer manner.
 * 
 * @tparam F CardInfo field to select. Should correspond to the array you init.
 * @param num_players The number of players. Game initialization depends on it.
 * @param arr The array to be initialized ***IN PLACE***! MUST BE THE RIGHT
 * LENGTH!
 */
template <CardInfoField F>
void initArray(size_t num_players, uint8_t *arr);

} // namespace exploding_kittens

// Hiding the implementation of initArray in an internal header file:
#include "card_defs.ih"

#endif // EK_CARD_DEFS_H