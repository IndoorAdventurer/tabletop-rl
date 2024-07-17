# Exploding Kittens
[Exploding Kittens](https://www.explodingkittens.com/) is a fun card game. It
can be described as a strategic version of Russian Roulette, but with kittens
and explosions. In this directory, the game is implemented as a multi-agent
reinforcement learning environment, which can be used as a challenge or
benchmark for people interested in AI.

## Leaderboard
None yet :-p

## How to use
TODO

## The Rules of the Game
The implementation given here, follows the rules found on on
[the official website](https://www.explodingkittens.com/pages/rules-kittens),
as of July 2024. They are also pasted below.

### HOW IT WORKS
In the deck of cards are some Exploding Kittens. You play the game by putting the
deck face down and taking turns drawing cards until someone draws an Exploding
Kitten. When that happens, that person explodes and they are out of the game.
All of the other cards will give you powerful tools to help you avoid exploding!
This process continues until there’s only 1 player left who wins the game.

### BASICALLY
* **if you explode, you lose.** (and you are full of incendiary loser sadsauce.)
* **if you don’t explode, YOU WIN.** (and you are full of greatness.
  good job, buddy.)
* **and all of the other cards** will lessen your chances of getting exploded by
  Exploding Kittens.

### SETUP
1) To start, remove all the Exploding Kittens (4) from the deck and set them aside.
2) Remove all of the Defuses (6) from the deck and deal 1 to each player.
   **NOTE**: Depending on how many people are playing, there will be some
   Defuses left over. Shuffle 2 of the extras back into the deck, and remove
   them from the game. (In the case of a 5 player game, you’ll only have 1 extra
   Defuse to shuffle back into the deck.)
> **DEFUSES:** Defuses are the most powerful card in the game. These are the only
> cards that can save you from exploding. If you draw an Exploding Kitten,
> instead of dying, you can play a Defuse and reinsert the Kitten back into the
> Draw Pile anywhere you’d like in secret. Try to get as many Defuses as you can.
3) Shuffle the deck and deal 7 cards face down to each player. Everyone now has
   a hand of 8 cards total. (7 cards + 1 Defuse). Look at your cards but keep
   them secret.
4) Insert enough Exploding Kittens back into the deck so that there is 1 fewer
   than the number of people playing. Remove any extra Exploding Kittens from
   the game.
> **FOR EXAMPLE:** For a 4 player game, insert 3 Kittens. For a 3 player game,
> insert 2 Kittens. This ensures that everyone eventually explodes except for 1
> person.

> **FASTER GAME VARIANT:** recommended for 2 & 3 players [NOT USED IN THIS
> IMPLEMENTATION!]
>
> Before inserting any Kittens into the deck, randomly remove about one-third of
> the deck from the game (you will be playing with approximately two-thirds of a
> deck, but you won’t know which cards have been removed). Then, shuffle the
> appropriate number of Exploding Kittens into the Draw Pile, and start the game.
5) Shuffle the deck, and put it face down in the middle of the table.
6) Pick a player to go first. (Some sample criteria: most excited to go first,
   most intimidating odor, shortest spleen, etc.)

### TAKING YOUR TURN
1) Gather all 8 of your cards into your hand and look at them. Do one of the
   following:
     * **PLAY:** Play a card from your hand by placing it FACE UP on top of the
       Discard Pile. Follow the instructions on the card. After you follow the
       instructions on the card, you can play another card. You can play as many
       cards as you’d like.
     * **PASS:** Play no (more) cards.
2) End your turn by drawing a card from the top of the Draw Pile into your hand
   and hoping it’s not an Exploding Kitten. Play continues clockwise around the
   table.
> **REMEMBER:** Play as many or as few cards as you’d like, then draw a card to
> end your turn.

### ENDING THE GAME
Eventually every player will explode except for one, who wins the game! You
won’t ever run out of cards in the Draw Pile because you inserted enough
Exploding Kittens to kill all but 1 player.

### THREE MORE THINGS
 * A good strategy generally is to save your cards early in the game while your 
   chance of exploding is low.
 * You can always count the cards left in the Draw Pile to figure out the odds
   of exploding.
 * There is no maximum or minimum hand size. If you run out of cards in your
   hand, there’s no special action to take. Keep playing. You’ll draw at least
   1 more card on your next turn.

### CARDS

#### EXPLODING KITTEN (4 Cards)
You must show this card immediately. Unless you have a Defuse, you’re dead.
When you die, put the kitten that killed you face up in front of you so that
everyone can see that you’re dead, and the rest of your cards face down in
front of you.

#### DEFUSE (6 Cards)
If you drew an Exploding Kitten, you can play this card instead of dying. Place
your Defuse in the Discard Pile. Then take the Exploding Kitten, and without
reordering or viewing the other cards, secretly put it back in the Draw Pile
anywhere you’d like. Your turn is over after playing this card.
> Want to hurt the player right after you? Put the Kitten right on top of the
> deck. If you’d like, hold the deck under the table so that no one else can see
> where you put it.

#### ATTACK (4 Cards)
End your turn without drawing a card, and immediately force the next player to
take 2 turns in a row. If the victim of an Attack plays this card on any of
their turns, the attacks “stack” and their turns are immediately transferred to
the next player, who must take the Attacker’s current and remaining untaken
turn(s) PLUS 2 additional turns.
> For Example: If the victim of an Attack Card plays another Attack Card, the
> next player must take 4 turns. However, if the victim completes 1 turn, and
> THEN plays an Attack Card on their second turn, the next player must take
> only 3 turns.

#### FAVOR (4 Cards)
Force any other player to give you 1 card from their hand. They choose which
card to give you.

#### NOPE (5 Cards)
Stop any action except for an Exploding Kitten or a Defuse. It’s as if the
card beneath a Nope never existed. You can also play a Nope on another Nope to
negate it and create a Yup, and so on. You can play a Nope at any time before an
action has begun, even if it’s not your turn. Any cards that have been Noped are
lost. Leave them in the Discard Pile. You can even play a Nope on a Special
Combo.

#### SHUFFLE (4 Cards)
Shuffle the Draw Pile until the next player tells you to stop. [When I play it,
players can shuffle as long as they want, but someone else can shuffle it more
after that. In this implementation shuffling is just a complete shuffle].
> Useful when you know there’s an Exploding Kitten coming.

#### SKIP (4 Cards)
Immediately end your turn without drawing a card.
> If you play a Skip Card as a defense to an Attack Card, it only ends 1 of
> the 2 turns. 2 Skip Cards would end both turns.

#### SEE FUTURE (5 Cards)
Privately view the top 3 cards from the Draw Pile and put them back in the same
order. Don’t show the cards to the other players.

#### CAT CARDS (5 different types; 4 of each)
These cards are powerless on their own, but if you collect any 2 matching Cat
Cards, you can play them as a Pair to steal a random card from any player.
They can also be used in Special Combos.

### SPECIAL COMBOS
(Read this after you've played your first game)

#### TWO OF A KIND
Playing matching Pairs of Cat Cards (where you get to steal a random card from
another player) no longer only applies to pairs of Cat Cards. It now applies to
ANY pair of cards with the same title (a pair of Shuffle Cards, a pair of Attack
Cards, etc). Ignore the instructions on the cards when you play them as a
Special Combo.

#### THREE OF A KIND
Exactly the same as Two of a Kind, but you get to name the card you want from
the other player. If they have it, you get to take it. If not, you get nothing.
Ignore the instructions on the cards when you play them as a Special Combo.