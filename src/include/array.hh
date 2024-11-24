#pragma once
#ifndef ARRAY_HH
#define ARRAY_HH

#include <vector>

namespace ext
{

namespace arr
{

enum sqtype_e { INCR, DECR, NDCR, NICR };

template <typename T>
std::vector<T> lis (const std::vector<T> &arr, const sqtype_e &sqtype);

template <typename T> T cnt_lis (const std::vector<T> &arr);

} // namespace arr

} // namespace ext

#endif
