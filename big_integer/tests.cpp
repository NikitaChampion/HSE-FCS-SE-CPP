#include <iostream>
#include <cassert>
#include <limits>
#include "big_integer_lib/big_int.h"

void testBIConstructors() {
    BigInt a("1000000000000000000000");
    BigInt b("123456789101112131415");
    BigInt c("-123456789101112131415");
    BigInt d("-1000000000000000000000");
    BigInt e(0);

    BigInt i_max(std::numeric_limits<int>::max());
    BigInt i_min(std::numeric_limits<int>::min());

    BigInt i64_max(std::numeric_limits<int64_t>::max());
    BigInt i64_min(std::numeric_limits<int64_t>::min());

    BigInt ui64_max(std::numeric_limits<uint64_t>::max());
    BigInt ui64_min(std::numeric_limits<uint64_t>::min());

    assert(BigInt::to_string(a) == "1000000000000000000000");
    assert(BigInt::to_string(b) == "123456789101112131415");
    assert(BigInt::to_string(c) == "-123456789101112131415");
    assert(BigInt::to_string(d) == "-1000000000000000000000");
    assert(e == 0);

    assert(i_max == std::numeric_limits<int>::max());
    assert(i_min == std::numeric_limits<int>::min());

    assert(i64_max == std::numeric_limits<int64_t>::max());
    assert(i64_min == std::numeric_limits<int64_t>::min());

    assert(BigInt::to_string(ui64_max) == "18446744073709551615");
    assert(ui64_min == 0);
}

void testBIAddition() {
    BigInt x = 0;
    int64_t answer = 0;
    for (int i = 0; i < 1'000'000; ++i) {
        x += i;
        answer += i;
        assert(x == answer);
    }

    BigInt left("123456789876543212345678987654321");
    BigInt right("185472482954376984235728912432574952364745901482584172538415819427175865915691");
    assert(BigInt::to_string(left + right) ==
           "185472482954376984235728912432574952364745901606040962414959031772854853570012");

    BigInt big_to("999999999999999999999999999999999999999999999999");
    assert(BigInt::to_string(++big_to) == "1000000000000000000000000000000000000000000000000");
    assert(BigInt::to_string(big_to++) == "1000000000000000000000000000000000000000000000000");
    assert(BigInt::to_string(big_to) == "1000000000000000000000000000000000000000000000001");
}

void testBISubtraction() {
    BigInt x = 0;
    int64_t answer = 0;
    for (int i = 0; i < 1'000'000; ++i) {
        x -= i;
        answer -= i;
        assert(x == answer);
    }

    BigInt left("123456789876543212345678987654321");
    BigInt right("185472482954376984235728912432574952364745901482584172538415819427175865915691");
    assert(BigInt::to_string(left - right) ==
           "-185472482954376984235728912432574952364745901359127382661872607081496878261370");
    assert(BigInt::to_string(right - left) ==
           "185472482954376984235728912432574952364745901359127382661872607081496878261370");

    BigInt big_to("1000000000000000000000000000000000000000000000001");
    assert(BigInt::to_string(--big_to) == "1000000000000000000000000000000000000000000000000");
    assert(BigInt::to_string(big_to--) == "1000000000000000000000000000000000000000000000000");
    assert(BigInt::to_string(big_to) == "999999999999999999999999999999999999999999999999");
}

void testBILess() {
    BigInt a("1000000000000000000000");
    BigInt b("123456789101112131415");
    BigInt c("-123456789101112131415");
    BigInt d("-1000000000000000000000");

    assert(!(a < a));
    assert(!(b < b));
    assert(!(c < c));
    assert(!(d < d));

    assert(a <= a);
    assert(b <= b);
    assert(c <= c);
    assert(d <= d);

    assert(b < a);
    assert(b <= a);

    assert(c < a);
    assert(c <= a);
    assert(c < b);
    assert(c <= b);

    assert(d < a);
    assert(d <= a);
    assert(d < b);
    assert(d <= b);

    assert(d < c);
    assert(d <= c);

    assert(c < 0);
    assert(c <= 0);
    assert(d < 0);
    assert(d <= 0);

    assert(BigInt(std::numeric_limits<int>::min()) < 0);
    assert(BigInt(std::numeric_limits<int>::min()) <= 0);
    assert(BigInt(std::numeric_limits<int>::min()) <= std::numeric_limits<int>::max());

    assert(BigInt(std::numeric_limits<int64_t>::min()) < 0);
    assert(BigInt(std::numeric_limits<int64_t>::min()) <= 0);
    assert(BigInt(std::numeric_limits<int64_t>::min()) <= std::numeric_limits<int64_t>::min());

    assert(BigInt(std::numeric_limits<uint64_t>::min()) <= 0);
    assert(BigInt(std::numeric_limits<uint64_t>::min()) <= std::numeric_limits<uint64_t>::min());

    assert(BigInt(0) <= 0);
}

void testBIMore() {
    BigInt a("1234104719517412534");
    BigInt b("2413521243981745");
    BigInt c("-123456789101112131415");
    BigInt d("-1000000000000000000000");

    assert(!(a > a));
    assert(!(b > b));
    assert(!(c > c));
    assert(!(d > d));

    assert(a >= a);
    assert(b >= b);
    assert(c >= c);
    assert(d >= d);

    assert(0 > c);
    assert(0 >= c);
    assert(0 > d);
    assert(0 >= d);

    assert(BigInt(std::numeric_limits<int>::max()) > 0);
    assert(BigInt(std::numeric_limits<int>::max()) >= 0);
    assert(BigInt(std::numeric_limits<int>::max()) >= std::numeric_limits<int>::max());

    assert(BigInt(std::numeric_limits<int64_t>::max()) > 0);
    assert(BigInt(std::numeric_limits<int64_t>::max()) >= 0);
    assert(BigInt(std::numeric_limits<int64_t>::max()) >= std::numeric_limits<int64_t>::max());

    assert(BigInt(std::numeric_limits<uint64_t>::max()) > 0);
    assert(BigInt(std::numeric_limits<uint64_t>::max()) >= 0);
    assert(BigInt(std::numeric_limits<uint64_t>::max()) >= std::numeric_limits<uint64_t>::max());

    assert(BigInt(0) >= 0);
}

void testBIEqual() {
    BigInt a("1234104719517412534");
    BigInt b("2413521243981745");
    BigInt c("-14541454215");
    BigInt d("-1000000000000000000000000000000000");

    assert(!(a != a));
    assert(!(b != b));
    assert(!(c != c));
    assert(!(d != d));

    assert(a == a);
    assert(b == b);
    assert(c == c);
    assert(d == d);

    assert(BigInt(0) == 0);
}

int main() {
    testBIConstructors();

    testBIAddition();
    testBISubtraction();

    testBILess();
    testBIMore();
    testBIEqual();

    return 0;
}
