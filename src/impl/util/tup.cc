#include "util.hh"

namespace ext
{

namespace utl
{

template <typename T1, typename T2, typename T3>
tup<T1, T2, T3>::tup () = default;

template <typename T1, typename T2, typename T3>
tup<T1, T2, T3>::tup (T1 first, T2 second, T3 third)
{
    c_.first         = first;
    c_.second.first  = second;
    c_.second.second = third;
}

template <typename T1, typename T2, typename T3>
T1 &
tup<T1, T2, T3>::first ()
{
    return &c_.first;
}

template <typename T1, typename T2, typename T3>
T2 &
tup<T1, T2, T3>::second ()
{
    return c_.second.first;
}

template <typename T1, typename T2, typename T3>
T3 &
tup<T1, T2, T3>::third ()
{
    return c_.second.second;
}

} // namespace utl

} // namespace ext
