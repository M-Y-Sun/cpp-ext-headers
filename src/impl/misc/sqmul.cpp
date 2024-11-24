#include <cstdint>
#include <cstdio>

#define BUFL 4096

static void
bsmul_ (const char *x, const size_t xl, const char *y, const size_t yl,
        char *ansbuf, size_t *ansl)
{
    if (xl < yl) {
        ansbuf[0] = 0;
        *ansl     = 0;
        return;
    }

    // https://github.com/dandclark/BigInt/blob/master/BigInt.c#L432

    size_t carry = 0;
    for (size_t i = 0; i < yl; ++i) {
        if (i) {
        }
    }
}

uint64_t
sqmul_ll (uint64_t base, uint64_t exp, uint64_t mod)
{
    base %= mod;

    uint64_t ans = 1;

    while (exp) {
        if (exp & 1)
            ans = (ans * base) % mod;

        base = (base * base) % mod;

        exp >>= 1;
    }

    return ans;
}

void
sqmul_bs (const char *base, const size_t blen, const char *exp,
          const size_t elen, const char *mod, const size_t mlen, char *ansbuf,
          size_t *ansbl)
{
    // reversed
    bool   bbuf[BUFL];
    bool   ebuf[BUFL];
    bool   mbuf[BUFL];
    size_t bbufl = 0;
    size_t ebufl = 0;
    size_t mbufl = 0;
    size_t blenc = blen;
    size_t elenc = elen;
    size_t mlenc = mlen;

    while (blen)
        bbuf[bbufl++] = base[--blenc] == 1;

    while (elen)
        ebuf[ebufl++] = base[--elenc] == 1;

    while (mlen)
        mbuf[mbufl++] = base[--mlenc] == 1;
}

int
main ()
{
    printf ("%llu\n", sqmul_ll (3, 45, 7));

    return 0;
}
