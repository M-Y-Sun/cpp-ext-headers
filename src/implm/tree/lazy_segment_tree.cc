// https://usaco.guide/plat/RURQ?lang=cpp
// based on problem cses 1735: https://cses.fi/problemset/task/1735/

#include <algorithm>
#include <vector>

#include "../../include/tree.hh"

namespace ext
{

namespace lz_segtr
{

template <typename T>
T
tree<T>::next_p2_ (T x)
{
    if (x == 1)
        return 1;

    if (sizeof (T) <= 32)
        return 1 << (sizeof (T) - __builtin_clzl (x - 1));
    else
        return 1 << (sizeof (T) - __builtin_clzll (x - 1));
}

template <typename T>
inline void
tree<T>::pushup_ (size_t pos)
{
    tree_[pos] = combine_ (tree_[pos << 1], tree_[(pos << 1) + 1]);
}

template <typename T>
void
tree<T>::apply_ (size_t pos, size_t len, const query_t<T> &q)
{
    if (q.type == ADD) {
        if (lazy_[pos].type != SET)
            lazy_[pos] = query_t<T>{ ADD, lazy_[pos].val + q.val };
        else
            lazy_[pos] = query_t<T>{ SET, lazy_[pos].val + q.val };

        // SUM: += q.val * len
        // MIN: += q.val
        // MAX: += q.val
        tree_[pos] += upd_upstream_ (q.val, len);
    } else if (q.type == SET) {
        // SUM: = q.val * len
        // MIN: = q.val
        // MAX: = q.val
        tree_[pos] = upd_upstream_ (q.val, len);
        lazy_[pos] = q;
    }
}

template <typename T>
void
tree<T>::pushdown_ (size_t pos, size_t l, size_t mid, size_t r)
{
    T lc = pos << 1;

    if (lazy_[pos].type != NONE && l != r) {
        apply_ (lc, mid - l + 1, lazy_[pos]);
        apply_ (lc + 1, r - mid, lazy_[pos]);
    }

    lazy_[pos] = query_t<T>{ NONE, 0 };
}

template <typename T>
void
tree<T>::build_ (size_t pos, size_t lb, size_t rb, const std::vector<T> &arr)
{
    if (lb == rb) {
        tree_[pos] = arr[lb];
        return;
    }

    size_t mid = lb + ((rb - lb) >> 1);
    T      lc  = pos << 1;

    build_ (lc, lb, mid, arr);
    build_ (lc + 1, mid + 1, rb, arr);

    pushup_ (pos);
}

template <typename T>
tree<T>::tree (size_t len, T dft, std::vector<T> arr, treeop_e type)
    : DEFAULT_ (dft), tree_ (next_p2_ (len) << 1, dft),
      lazy_ (next_p2_ (len), query_t<T> ()), len_ (len)
{
    switch (type) {
        case SUM:
            combine_ = [] (const T &x, const T &y) { return x + y; };
            upd_upstream_
                = [] (const T &val, const T &len) { return val * len; };
            break;
        case MIN:
            combine_ = [] (const T &x, const T &y) { return std::min (x, y); };
            upd_upstream_ = [] (const T &val, const T &len) { return val; };
            break;
        case MAX:
            combine_ = [] (const T &x, const T &y) { return std::max (x, y); };
            upd_upstream_ = [] (const T &val, const T &len) { return val; };
            break;
    }

    build_ (1, 0, len - 1, arr);
}

template <typename T>
void
tree<T>::upd_ (size_t start, size_t end, const query_t<T> &q, size_t pos_,
               size_t l_, size_t r_)
{
    // if [lb, rb] and [start, end] do not intersect at all, do nothing
    if (start > r_ || end < l_)
        return;

    // if [lb, rb] is completely inside [start, end], set the current node
    // and return
    if (start <= l_ && r_ <= end) {
        // if the lower and upper bounds is completely inside [start, end]
        apply_ (pos_, r_ - l_ + 1, q);

        return;
    }

    // if [lb, rb] only partially intersects with [start, end], continue
    // searching until something happens

    size_t mid = l_ + ((r_ - l_) >> 1);
    pushdown_ (pos_, l_, mid, r_);

    // search the left and right children
    size_t lc = pos_ << 1;
    upd_ (start, end, q, lc, l_, mid);
    upd_ (start, end, q, lc + 1, mid + 1, r_);

    pushup_ (pos_);
}

template <typename T>
T
tree<T>::query_ (size_t start, size_t end, size_t pos_, size_t l_, size_t r_)
{
    // if [lb, rb] and [start, end] do not intersect at all, return dft
    if (start > r_ || end < l_)
        return DEFAULT_;

    // if [lb, rb] is completely inside [start, end], return the current
    // value
    if (start <= l_ && end >= r_)
        return tree_[pos_];

    // if it is partially inside, continue the search to the left and right
    // children
    size_t mid = l_ + ((r_ - l_) >> 1);
    pushdown_ (pos_, l_, mid, r_);

    size_t lc = pos_ << 1;

    // search the left and right half
    return combine_ (query_ (start, end, lc, l_, mid),
                     query_ (start, end, lc + 1, mid + 1, r_));
}

template <typename T>
inline void
tree<T>::upd (size_t start, size_t end, T val, qtype_e qtype)
{
    upd_ (start, end, query_t<T>{ qtype, val }, 1, 0, len_ - 1);
}

template <typename T>
inline T
tree<T>::query (size_t start, size_t end)
{
    return query_ (start, end, 1, 0, len_ - 1);
}

template class tree<int32_t>;
template class tree<int64_t>;
template class tree<uint32_t>;
template class tree<uint64_t>;

} // namespace lz_segtr

} // namespace ext
