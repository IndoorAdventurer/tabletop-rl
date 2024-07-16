#include "utils.h"

#include <ctime>

namespace tabletop_general
{

std::mt19937_64 randnum_gen(std::time(0));

} // namespace tabletop_general