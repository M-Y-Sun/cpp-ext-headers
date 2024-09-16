// https://usaco.guide/CPH.pdf#page=99
// based on problem cses 1649: https://cses.fi/problemset/task/1649/

#include <climits>
#include <cstdio>
#include <functional>

#include "../include/tree.hh"

namespace ext
{

namespace segtr
{

template <typename T>
tree<T>::tree (size_t len, const T dft,
               std::function<T (const T &, const T &)> const &combine)
    : DEFAULT_ (dft), tree_ (len << 1, dft), len_ (len), combine_ (combine)
{
}

/** Sets the value at id to val */
template <typename T>
void
tree<T>::set (size_t idx, T val)
{
    idx += len_;
    tree_[idx] = val;

    // start at the bottom of the tree and update nodes until root
    for (; idx > 1; idx >>= 1)
        tree_[idx >> 1] = combine_ (tree_[idx], tree_[idx ^ 1]);
}

/** @return A query of the range [start, end) */
template <typename T>
T
tree<T>::query (size_t start, size_t end)
{
    T ans = DEFAULT_;
    for (start += len_, end += len_; start < end; start >>= 1, end >>= 1) {
        // for each level, process the values that do not belong to the
        // higher range then move the range one level higher

        // if it is left child => not part of the [start, end] range, it
        // should be processed
        if (start & 1)
            ans = combine_ (ans, tree_[start++]);

        // if it is left child => part of the [start, end] range, the right
        // child (previous index) should be processed
        if (end & 1)
            ans = combine_ (ans, tree_[--end]);
    }

    return ans;
}

template class tree<int32_t>;
template class tree<int64_t>;
template class tree<uint32_t>;
template class tree<uint64_t>;

} // namespace segtr

} // namespace ext
