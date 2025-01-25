#include <numeric>

#include "util.hh"

namespace ext
{

namespace utl
{

rational_number
rational_number::operator+ (const rational_number &other) const
{
    rational_number rnum
        = { (this->n * other.d) + (other.n * this->d), this->d * other.d };
    int64_t gcd = std::gcd (rnum.n, rnum.d);
    rnum.n      = rnum.n / gcd;
    rnum.d      = rnum.d / gcd;
    return rnum;
}

rational_number
rational_number::operator- (const rational_number &other) const
{
    rational_number other_new = { other.n * -1, other.d };
    return *this + other_new;
}

rational_number
rational_number::operator* (const rational_number &other) const
{
    rational_number rnum = { this->n * other.n, this->d * other.d };
    int64_t         gcd  = std::gcd (rnum.n, rnum.d);
    rnum.n               = rnum.n / gcd;
    rnum.d               = rnum.d / gcd;
    return rnum;
}

rational_number
rational_number::operator/ (const rational_number &other) const
{
    rational_number other_new = { other.d, other.n };
    return *this * other_new;
}

bool
rational_number::operator< (const rational_number &other) const
{
    return (this->n * other.d) < (other.n * this->d);
}

bool
rational_number::operator> (const rational_number &other) const
{
    return (this->n * other.d) > (other.n * this->d);
}

long double
rational_number::value () const
{
    return (this->n / (double)this->d);
}

} // namespace utl

} // namespace ext
