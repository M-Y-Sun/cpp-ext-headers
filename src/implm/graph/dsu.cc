#include <algorithm>

#include "graph.hh"

namespace ext
{

namespace dsu
{

template <typename T>
disjoint_set<T>::disjoint_set (size_t sz) : parents_ (sz), sizes_ (sz, 1)
{
    for (size_t i = 0; i < sz; ++i)
        parents_[i] = i;
}

/**
 * Recursively finds the representative node of x's component, visiting the
 * immediate parent every iteration.
 *
 * Optimized by path compression to reduce tree height by raising the parent on
 * each iteration of find.
 *
 * @return The representative node in x's component.
 * */
template <typename T>
T
disjoint_set<T>::find (T x)
{
    return parents_[x] == x ? x : (parents_[x] = find (parents_[x]));
}

/**
 * Merges two components optimized with union by size to work in O(log N).
 * @return If the merge changed connectivity.
 */
template <typename T>
bool
disjoint_set<T>::unite (T x, T y)
{
    x = find (x);
    y = find (y);

    // already in the same set
    if (x == y)
        return false;

    // union by size: put smaller trees under bigger trees
    // x is bigger
    if (sizes_[x] < sizes_[y])
        std::swap (x, y);

    sizes_[x] += sizes_[y];
    parents_[y] = x;

    return true;
}

/** @return If x and y are in the same connected component */
template <typename T>
bool
disjoint_set<T>::connected (T x, T y)
{
    return find (x) == find (y);
}

template class disjoint_set<int8_t>;
template class disjoint_set<int16_t>;
template class disjoint_set<int32_t>;
template class disjoint_set<int64_t>;
template class disjoint_set<uint8_t>;
template class disjoint_set<uint16_t>;
template class disjoint_set<uint32_t>;
template class disjoint_set<uint64_t>;

template class disjoint_set<float>;
template class disjoint_set<double>;
template class disjoint_set<long double>;

} // namespace dsu

} // namespace ext
