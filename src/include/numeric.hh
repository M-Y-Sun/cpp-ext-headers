#pragma once
#ifndef NUMERIC_HH
#define NUMERIC_HH

#include <cstdint>

namespace ext
{

template <typename int64_t> int64_t next_p2 (int64_t x);

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
