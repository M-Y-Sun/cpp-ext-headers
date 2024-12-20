#include <algorithm>
#include <functional>
#include <limits>
#include <vector>

#include "array.hh"

namespace ext
{

namespace arr
{

/** @return the longest increasing subsequence in the array in O(NlogN) */
template <typename T>
std::vector<T>
lis (const std::vector<T> &arr, const sqtype_e &sqtype)
{
    bool    is_incr_d = sqtype == INCR || sqtype == NDCR;
    const T maxv      = std::numeric_limits<T> ().max ();
    const T minv      = std::numeric_limits<T> ().min ();

    const size_t SZ  = arr.size ();
    const T      DFT = is_incr_d ? maxv : minv;

    // d[l] is the smallest element at which an
    // increasing subsequence of length l ends.

    std::vector<T>   d (SZ + 1, DFT);
    std::vector<int> idxs (SZ + 1, -1);
    std::vector<int> prev (SZ + 1, -1);
    d[0] = is_incr_d ? minv : maxv;

    for (size_t i = 0; i < SZ; ++i) {
        size_t                                     l;
        std::function<bool (const T &, const T &)> cmp;

        switch (sqtype) {
            case INCR:
                l = std::upper_bound (d.begin (), d.end (), arr[i])
                    - d.begin ();
                cmp = [] (const T &lhs, const T &rhs) { return lhs < rhs; };
                break;
            case DECR:
                l = std::upper_bound (d.begin (), d.end (), arr[i],
                                      std::greater<T> ())
                    - d.begin ();
                cmp = [] (const T &lhs, const T &rhs) { return lhs > rhs; };
                break;
            case NDCR:
                l = std::lower_bound (d.begin (), d.end (), arr[i])
                    - d.begin ();
                cmp = [] (const T &lhs, const T &rhs) { return lhs <= rhs; };
                break;
            case NICR:
                l = std::lower_bound (d.begin (), d.end (), arr[i],
                                      std::greater<T> ())
                    - d.begin ();
                cmp = [] (const T &lhs, const T &rhs) { return lhs >= rhs; };
                break;
        }

        if (cmp (d[l - 1], arr[i])) {
            d[l]    = arr[i];
            idxs[l] = i;
            prev[i] = idxs[l - 1];
        }
    }

    int pos = -1;
    for (size_t l = 0; l <= SZ; ++l) {
        if (d[l] != DFT)
            pos = idxs[l];
    }

    std::vector<T> &subseq = d; // use the memory allocated by d
    subseq.clear ();

    while (pos != -1) {
        subseq.push_back (arr[pos]);
        pos = prev[pos];
    }

    std::reverse (subseq.begin (), subseq.end ());

    return subseq;
}

/** @return the number of distinct LIS in O(N^2)*/
template <typename T>
T
cnt_lis (const std::vector<T> &arr)
{
    const size_t SZ = arr.size ();

    // d[i] is the length of the longest increasing subsequence that ends in
    // the element at index i;
    std::vector<T> d (SZ, 1); // initialize to length 1 to begin
    std::vector<T> cnt (SZ, 1);

    for (size_t i = 0; i < SZ; ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (arr[j] < arr[i]) {
                T newlen = d[j] + 1;

                if (d[i] == newlen) {
                    cnt[i] = cnt[i] + cnt[j];
                } else if (d[i] < newlen) {
                    d[i]   = newlen;
                    cnt[i] = cnt[j];
                }
            }
        }
    }

    T maxlen = d[0];
    T ans    = cnt[0];
    for (size_t i = 1; i < SZ; ++i) {
        if (maxlen == d[i]) {
            ans = ans + cnt[i];
        } else if (maxlen < d[i]) {
            maxlen = d[i];
            ans    = cnt[i];
        }
    }

    return ans;
}

template std::vector<int8_t>  lis (const std::vector<int8_t> &arr,
                                   const sqtype_e            &sqtype);
template std::vector<int16_t> lis (const std::vector<int16_t> &arr,
                                   const sqtype_e             &sqtype);
template std::vector<int32_t> lis (const std::vector<int32_t> &arr,
                                   const sqtype_e             &sqtype);
template std::vector<int64_t> lis (const std::vector<int64_t> &arr,
                                   const sqtype_e             &sqtype);

template int8_t  cnt_lis (const std::vector<int8_t> &arr);
template int16_t cnt_lis (const std::vector<int16_t> &arr);
template int32_t cnt_lis (const std::vector<int32_t> &arr);
template int64_t cnt_lis (const std::vector<int64_t> &arr);

} // namespace arr

} // namespace ext
