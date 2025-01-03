#pragma once
#ifndef UTIL_HH
#define UTIL_HH

#include <cstdint>

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

} // namespace ext

#endif
