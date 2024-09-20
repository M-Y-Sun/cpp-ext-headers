#pragma once

#include <vector>

namespace ext
{

namespace arr
{

enum sqtype_e { INCR, DECR, NDCR, NICR };

template <typename T>
std::vector<T> lis (const std::vector<T> &arr, const sqtype_e &sqtype);

} // namespace arr

} // namespace ext
