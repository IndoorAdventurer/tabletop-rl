#ifndef CARD_DEFS_H
#define CARD_DEFS_H

#include <cstdint>
#include <cstddef>

namespace exploding_kittens {

/**
 * Enum to give each card type in the Exploding Kittens game a unique index.
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

    Total               // Number of elements. Always keep as last!
};

// Number of unique cards in the Exploding Kittens game.
constexpr size_t unique_cards = static_cast<size_t>(CardIdx::Total);

/**
 * Struct defining constants related to a specific card.
 */
template <CardIdx c>
struct CardInfo {
    // Specialization should be given for every card, else compilation error.
    static_assert(false, "'CardInfo' struct not defined for given card.");
    enum {
        init_deck = -1,     // Put this many in deck at start of game
        init_hand = -1,     // Give each player this many at start of game
        init_rand = -1      // Randomly divide among deck and player hands
    }
};

// ---Now the card-specific definitions/specializations---:

template <> struct CardInfo<CardIdx::Exploding_Kitten> {
    static inline int init_deck(int num_players) {return num_players - 1;};
    enum { init_hand = 0, init_rand = 0 }; };

template <> struct CardInfo<CardIdx::Defuse> {
    enum { init_deck = 0, init_hand = 1 }; };
    static inline int init_rand(int num_players) {
        return num_players == 5 ? 1 : 2;
    };

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

}

#endif // CARD_DEFS_H