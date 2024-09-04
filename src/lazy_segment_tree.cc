// https://usaco.guide/plat/RURQ?lang=cpp
// based on problem cses 1735: https://cses.fi/problemset/task/1735/

#include <climits>
#include <cstdio>
#include <functional>
#include <vector>

/** A data structure that can answer point update and range maximum queries. */
template <typename T> class lazy_segment_tree
{
private:
    struct node_t_ {
        T val;
        T lz_add;
        T lz_set;
    };

    const T DEFAULT_;

    // tree[1] is the top node, tree[2] and tree[3] are its children, and so on
    // values from tree[len] to tree[2 * len - 1] is the original array
    // that is:
    // the parent of tree[i] is tree[i / 2]
    // and its children are tree[2i] and tree[2i + 1]

    std::vector<node_t_> tree_; // length of this is 2 * len
    size_t               len_;  // simulated "length" of the array

    std::function<T (const T &, const T &)> queryfunc;

protected:
    T
    next_p2_ (T v)
    {
        --v;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;

        return v + 1;
    }

    inline void
    pushup_ (size_t pos)
    {
        const T lc     = pos << 1;
        tree_[pos].val = tree_[lc].val + tree_[lc + 1].val;
    }

    void
    pushdown_ (size_t pos, size_t lb, size_t mid, size_t rb)
    {
        const T lc = pos << 1;
        const T rc = lc + 1;

        if (tree_[pos].lz_set != 0) { // range set
            tree_[lc].lz_set = tree_[rc].lz_set = tree_[pos].lz_set;

            tree_[lc].val = (mid - lb + 1) * tree_[pos].lz_set;
            tree_[rc].val = (rb - mid) * tree_[pos].lz_set;

            tree_[lc].lz_add  = 0;
            tree_[rc].lz_add  = 0;
            tree_[pos].lz_set = 0;
        } else if (tree_[pos].lz_add != 0) { // range add
            if (tree_[lc].lz_set == 0) {
                tree_[lc].lz_add += tree_[pos].lz_add;
            } else {
                tree_[lc].lz_set += tree_[pos].lz_add;
                tree_[lc].lz_add = 0;
            }

            if (tree_[rc].lz_set == 0) {
                tree_[rc].lz_add += tree_[pos].lz_add;
            } else {
                tree_[rc].lz_set += tree_[pos].lz_add;
                tree_[rc].lz_add = 0;
            }

            tree_[lc].val += (mid - lb + 1) * tree_[pos].lz_add;
            tree_[rc].val += (rb - mid) * tree_[pos].lz_add;
            tree_[pos].lz_add = 0;
        }
    }

    void
    build_ (int pos, int lb, int rb, const std::vector<T> &arr)
    {
        // clear lazy fields
        tree_[pos].lz_add = 0;
        tree_[pos].lz_set = 0;

        // once we are at a leaf, set the value from the source array
        if (lb == rb) {
            tree_[pos].val = arr[lb];
            return;
        }

        T mid = lb + ((rb - lb) >> 1);
        T lc  = pos << 1;
        build_ (lc, lb, mid, arr);
        build_ (lc + 1, mid + 1, rb, arr);

        pushup_ (pos);
    }

public:
    lazy_segment_tree (
        size_t len, T dft, std::vector<T> arr,
        std::function<T (const T &, const T &)> const &queryfunc)
        : DEFAULT_ (dft), tree_ (next_p2_ (len) << 1, { dft, 0, 0 }),
          len_ (len), queryfunc (queryfunc)
    {
        build_ (1, 1, len, arr);
    }

    /** Adds a value to a range of elements. */
    void
    add (size_t start, size_t end, T val, size_t pos_, size_t lb_, size_t rb_)
    {
        // if [lb, rb] and [start, end] do not intersect at all, do nothing
        if (start > rb_ || end < lb_)
            return;

        // if [lb, rb] is completely inside [start, end], set the current node
        // and return
        if (start <= lb_ && rb_ <= end) {
            // if the lower and upper bounds is completely inside [start, end]
            tree_[pos_].val += (rb_ - lb_ + 1) * val;

            // add to lazy propagation if the lazy set value is 0
            if (tree_[pos_].lz_set == 0)
                tree_[pos_].lz_add += val;
            else
                tree_[pos_].lz_set += val;

            return;
        }

        // if [lb, rb] only partially intersects with [start, end], continue
        // searching until something happens

        size_t mid = lb_ + ((rb_ - lb_) >> 1);
        pushdown_ (pos_, lb_, mid, rb_);

        // search the left and right children
        size_t lc = pos_ << 1;
        add (start, end, val, lc, lb_, mid);
        add (start, end, val, lc + 1, mid + 1, rb_);

        pushup_ (pos_);
    }

    /** Sets the value to a range of elements to another value. */
    void
    set (size_t start, size_t end, T val, size_t pos_, size_t lb_, size_t rb_)
    {
        // if [lb, rb] and [start, end] do not intersect at all, do nothing
        if (start > rb_ || end < lb_)
            return;

        // if [lb, rb] is completely inside [start, end], set the current node
        // and return
        if (start <= lb_ && end >= rb_) {
            tree_[pos_].val    = (rb_ - lb_ + 1) * val;
            tree_[pos_].lz_add = 0;
            tree_[pos_].lz_set = val;
            return;
        }

        // if [lb, rb] only partially intersects with [start, end], continue
        // searching until something happens

        size_t mid = lb_ + ((rb_ - lb_) >> 1);
        pushdown_ (pos_, lb_, mid, rb_);

        // search the left and right children
        size_t lc = pos_ << 1;
        set (start, end, val, lc, lb_, mid);
        set (start, end, val, lc + 1, mid + 1, rb_);

        pushup_ (pos_);
    }

    /** @return Queries the range [start, end) recursively with O(log n) time
     * complexity.*/
    T
    query (size_t start, size_t end, size_t pos_, size_t lb_, size_t rb_)
    {
        // if [lb, rb] and [start, end] do not intersect at all, return dft
        if (start > rb_ || end < lb_)
            return DEFAULT_;

        // if [lb, rb] is completely inside [start, end], return the current
        // value
        if (start <= lb_ && end >= rb_)
            return tree_[pos_].val;

        // if it is partially inside, continue the search to the left and right
        // children
        size_t mid = lb_ + ((rb_ - lb_) >> 1);
        pushdown_ (pos_, lb_, mid, rb_);

        size_t lc = pos_ << 1;

        // search the left and right half
        return queryfunc (query (start, end, lc, lb_, mid),
                          query (start, end, lc + 1, mid + 1, rb_));
    }
};

int
main ()
{
    int n, q;
    scanf ("%d%d", &n, &q);

    // must be 1 indexed so lc works (so 0 * 2 = 0 doesnt happen)
    std::vector<int64_t> arr (n + 1, 0);

    for (int i = 1; i <= n; ++i)
        scanf ("%lld", &arr[i]);

    lazy_segment_tree<int64_t> segtree (
        n, 0, arr, [] (const int64_t &x, const int64_t &y) { return x + y; });

    while (q--) {
        int query_type;
        scanf ("%d", &query_type);

        int64_t a, b, x;
        switch (query_type) {
            case 1:
                scanf ("%lld%lld%lld", &a, &b, &x);
                segtree.add (a, b, x, 1, 1, n);
                break;
            case 2:
                scanf ("%lld%lld%lld", &a, &b, &x);
                segtree.set (a, b, x, 1, 1, n);
                break;
            case 3:
                scanf ("%lld%lld", &a, &b);
                printf ("%lld\n", segtree.query (a, b, 1, 1, n));
                break;
        }
    }

    return 0;
}
