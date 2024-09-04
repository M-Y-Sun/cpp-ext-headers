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
               std::function<T (const T &, const T &)> const &queryfunc)
    : DEFAULT_ (dft), tree_ (len << 1, dft), len_ (len), queryfunc (queryfunc)
{
}

/** Sets the value at id to val */
template <typename T>
void
tree<T>::set (int idx, T val)
{
    idx += len_;
    tree_[idx] = val;

    // start at the bottom of the tree and update nodes until root
    for (; idx > 1; idx >>= 1)
        tree_[idx >> 1] = queryfunc (tree_[idx], tree_[idx ^ 1]);
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
            ans = queryfunc (ans, tree_[start++]);

        // if it is left child => part of the [start, end] range, the right
        // child (previous index) should be processed
        if (end & 1)
            ans = queryfunc (ans, tree_[--end]);
    }

    return ans;
}

} // namespace segtr

} // namespace ext

int
main ()
{
    int n, q;
    scanf ("%d%d", &n, &q);

    ext::segtr::tree<int64_t> segtree (
        n, INT64_MAX,
        [] (const int64_t &x, const int64_t &y) { return std::min (x, y); });

    for (int i = 0; i < n; ++i) {
        int64_t x;
        scanf ("%lld", &x);
        segtree.set (i, x);
    }

    while (q--) {
        int     query_type, a;
        int64_t b;

        scanf ("%d%d%lld", &query_type, &a, &b);

        --a;

        if (query_type == 1)
            segtree.set (a, b);
        else
            printf ("%lld\n", segtree.query (a, b));
    }

    return 0;
}
