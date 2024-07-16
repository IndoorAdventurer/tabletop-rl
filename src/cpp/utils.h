// Some simple general functionality that can be used globally withing the
// project, such as a globally defined random number generator.

#ifndef TABLTETOP_UTILS_H
#define TABLTETOP_UTILS_H

#include <random>

namespace tabletop_general
{

/**
 * @brief Only use this pseudo random number generator as a source of randomness
 * to ensure deterministic results when seed gets fixed.
 */
extern ::std::mt19937_64 randnum_gen;

} // namespace tabletop_general


#endif // TABLTETOP_UTILS_H