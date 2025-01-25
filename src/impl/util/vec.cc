#include "util.hh"

namespace ext
{

namespace utl
{

bool
point_t::operator< (const point_t &other)
{
    return this->x == other.x ? this->y < other.y : this->x < other.x;
}

vec_t
vec_t::operator+ (const vec_t &other)
{
    return vec_t{ this->x + other.x, this->y + other.y };
}

vec_t
vec_t::operator- (const vec_t &other)
{
    return vec_t{ this->x + other.x, this->y + other.y };
}

vec_t
vec_t::operator* (const int64_t &scl)
{
    return vec_t{ this->x * scl, this->y * scl };
}

bool
vec_t::operator< (const vec_t &other)
{
    return this->x == other.x ? this->y < other.y : this->x < other.x;
}

static bool
ccw_ (point_t a, point_t b, point_t c)
{
    return (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x);
}

bool
intersects (point_t a, point_t b, point_t c, point_t d)
{
    return ccw_ (a, c, d) != ccw_ (b, c, d)
           && ccw_ (a, b, c) != ccw_ (a, b, d);
}

} // namespace utl

} // namespace ext
