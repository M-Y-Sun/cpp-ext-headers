#pragma once
#ifndef UTIL_HH
#define UTIL_HH

#include <cstdint>
#include <utility>

namespace ext
{

namespace utl
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

struct point_t {
    int64_t x;
    int64_t y;

    bool operator< (const point_t &other);
};

struct vec_t {
    int64_t x;
    int64_t y;

    vec_t operator+ (const vec_t &other);

    vec_t operator- (const vec_t &other);

    vec_t operator* (const int64_t &scl);

    bool operator< (const vec_t &other);
};

bool intersects (point_t a, point_t b, point_t c, point_t d);

} // namespace utl

} // namespace ext

#endif
