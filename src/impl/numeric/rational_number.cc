#include <numeric>

#include "numeric.hh"

ext::rational_number
ext::rational_number::operator+ (const rational_number &other) const
{
    rational_number rnum
        = { (this->n * other.d) + (other.n * this->d), this->d * other.d };
    int64_t gcd = std::gcd (rnum.n, rnum.d);
    rnum.n      = rnum.n / gcd;
    rnum.d      = rnum.d / gcd;
    return rnum;
}

ext::rational_number
ext::rational_number::operator- (const rational_number &other) const
{
    rational_number other_new = { other.n * -1, other.d };
    return *this + other_new;
}

ext::rational_number
ext::rational_number::operator* (const rational_number &other) const
{
    rational_number rnum = { this->n * other.n, this->d * other.d };
    int64_t         gcd  = std::gcd (rnum.n, rnum.d);
    rnum.n               = rnum.n / gcd;
    rnum.d               = rnum.d / gcd;
    return rnum;
}

ext::rational_number
ext::rational_number::operator/ (const rational_number &other) const
{
    rational_number other_new = { other.d, other.n };
    return *this * other_new;
}

bool
ext::rational_number::operator< (const rational_number &other) const
{
    return (this->n * other.d) < (other.n * this->d);
}

bool
ext::rational_number::operator> (const rational_number &other) const
{
    return (this->n * other.d) > (other.n * this->d);
}

long double
ext::rational_number::value () const
{
    return (this->n / (double)this->d);
}
