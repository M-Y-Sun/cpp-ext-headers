// https://usaco.guide/CPH.pdf#page=99
// based on problem cses 1649: https://cses.fi/problemset/task/1649/

#include <climits>
#include <cstdio>
#include <functional>
#include <vector>

/** A data structure that can answer point update and range maximum queries. */
template <typename T> class segment_tree
{
private:
    const T DEFAULT_;

    // tree[1] is the top node, tree[2] and tree[3] are its children, and so on
    // values from tree[len] to tree[2 * len - 1] is the original array
    // that is:
    // the parent of tree[i] is tree[i / 2]
    // and its children are tree[2i] and tree[2i + 1]

    std::vector<T> tree_; // length of this is 2 * len
    size_t         len_;  // simulated "length" of the array

    std::function<T (const T &, const T &)> queryfunc;

public:
    segment_tree (size_t len, T dft,
                  std::function<T (const T &, const T &)> const &queryfunc)
        : DEFAULT_ (dft), tree_ (len << 1, dft), len_ (len),
          queryfunc (queryfunc)
    {
    }

    /** Sets the value at id to val */
    void
    set (int idx, T val)
    {
        idx += len_;
        tree_[idx] = val;

        // start at the bottom of the tree and update nodes until root
        for (; idx > 1; idx >>= 1)
            tree_[idx >> 1] = queryfunc (tree_[idx], tree_[idx ^ 1]);
    }

    /** @return A query of the range [start, end) */
    T
    query (size_t start, size_t end)
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
};

/*
int
main ()
{
    int n, q;
    scanf ("%d%d", &n, &q);

    segment_tree<int64_t> segtree (
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
*/
