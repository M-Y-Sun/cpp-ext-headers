#include "misc.hh"

namespace ext
{

template <typename T>
T
next_p2 (T x)
{
    if (x == 1)
        return 1;

    if (sizeof (T) <= 32)
        return 1 << (sizeof (T) - __builtin_clzl (x - 1));
    else
        return 1 << (sizeof (T) - __builtin_clzll (x - 1));
}

} // namespace ext
