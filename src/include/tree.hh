#pragma once
#ifndef TREE_HH
#define TREE_HH

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

    std::function<T (const T &, const T &)> combine_;

public:
    tree (size_t len, const T dft,
          std::function<T (const T &, const T &)> const &combine);

    /** Sets the value at id to val */
    void set (size_t idx, T val);

    /** @return A query of the range [start, end) */
    T query (size_t start, size_t end);
};

} // namespace segtr

namespace lz_segtr
{

// TODO(M-Y-Sun): Test multiplication
enum treeop_e { SUM = 0, MIN = 1, MAX = 2 };

enum qtype_e { ADD = 0, SET = 1, NONE = 2 };

template <typename T> struct query_t {
    qtype_e type = NONE;
    T       val  = 0;
};

template <typename T> class tree final
{
    const T DEFAULT_;

    // tree[1] is the top node, tree[2] and tree[3] are its children, and so on
    // values from tree[len] to tree[2 * len - 1] is the original array
    // that is:
    // the parent of tree[i] is tree[i / 2]
    // and its children are tree[2i] and tree[2i + 1]

    std::vector<T>           tree_; // length of this is 2 * len
    std::vector<query_t<T> > lazy_;
    size_t                   len_; // simulated "length" of the array

    std::function<T (const T &, const T &)> combine_;
    std::function<T (const T &, const T &)> upd_upstream_;

    T next_p2_ (T v);

    inline void pushup_ (size_t pos);

    void pushdown_ (size_t pos, size_t lb, size_t mid, size_t rb);

    void build_ (size_t pos, size_t lb, size_t rb, const std::vector<T> &arr);

    void apply_ (size_t pos, size_t len, const query_t<T> &q);

    void upd_ (size_t start, size_t end, const query_t<T> &q, size_t pos_,
               size_t lb_, size_t rb_);

    T query_ (size_t start, size_t end, size_t pos_, size_t lb_, size_t rb_);

public:
    tree (size_t len, T dft, std::vector<T> arr, treeop_e type);

    /** Adds a value to a range of elements. */
    void upd (size_t start, size_t end, T val, qtype_e qtype);

    /** @return Queries the range [start, end) recursively with O(log n) time
     * complexity.*/
    T query (size_t start, size_t end);
};

} // namespace lz_segtr

} // namespace ext

#endif
