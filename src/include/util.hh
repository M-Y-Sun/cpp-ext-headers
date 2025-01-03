#pragma once
#ifndef UTIL_HH
#define UTIL_HH

#include <cstdint>
#include <utility>

namespace ext
{

template <typename T> T next_p2 (T x);

class rational_number
{
public:
    int64_t n;
    int64_t d;

    rational_number operator+ (const rational_number &other) const;
    rational_number operator- (const rational_number &other) const;
    rational_number operator* (const rational_number &other) const;
    rational_number operator/ (const rational_number &other) const;

    bool operator< (const rational_number &other) const;
    bool operator> (const rational_number &other) const;

    long double value () const;
};

template <typename T1, typename T2, typename T3> class tup
{
    std::pair<T1, std::pair<T2, T3> > c_;

public:
    tup ();
    tup (T1 first, T2 second, T3 third);

    T1 &first ();
    T2 &second ();
    T3 &third ();
};

} // namespace ext

#endif
