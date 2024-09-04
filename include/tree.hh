#ifndef TREE_HH
#define TREE_HH

#include <climits>
#include <cstdio>
#include <functional>
#include <vector>

namespace ext
{

namespace segtr
{

/** A data structure that can answer point update and range maximum queries. */
template <typename T> class tree final
{
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
    tree (size_t len, const T dft,
          std::function<T (const T &, const T &)> const &queryfunc);

    /** Sets the value at id to val */
    void set (int idx, T val);

    /** @return A query of the range [start, end) */
    T query (size_t start, size_t end);
};

} // namespace segtr

namespace lz_segtr
{

template <typename T> struct node_t {
    T val;
    T lz_add;
    T lz_set;
};

template <typename T> class tree final
{
    const T DEFAULT_;

    // tree[1] is the top node, tree[2] and tree[3] are its children, and so on
    // values from tree[len] to tree[2 * len - 1] is the original array
    // that is:
    // the parent of tree[i] is tree[i / 2]
    // and its children are tree[2i] and tree[2i + 1]

    std::vector<node_t<T> > tree_; // length of this is 2 * len
    size_t                  len_;  // simulated "length" of the array

    std::function<T (const T &, const T &)> queryfunc;

    T next_p2_ (T v);

    inline void pushup_ (size_t pos);

    void pushdown_ (size_t pos, size_t lb, size_t mid, size_t rb);

    void build_ (int pos, int lb, int rb, const std::vector<T> &arr);

public:
    tree (size_t len, T dft, std::vector<T> arr,
          std::function<T (const T &, const T &)> const &queryfunc);

    /** Adds a value to a range of elements. */
    void add (size_t start, size_t end, T val, size_t pos_, size_t lb_,
              size_t rb_);

    /** Sets the value to a range of elements to another value. */
    void set (size_t start, size_t end, T val, size_t pos_, size_t lb_,
              size_t rb_);

    /** @return Queries the range [start, end) recursively with O(log n) time
     * complexity.*/
    T query (size_t start, size_t end, size_t pos_, size_t lb_, size_t rb_);
};

} // namespace lz_segtr

} // namespace ext

#endif
