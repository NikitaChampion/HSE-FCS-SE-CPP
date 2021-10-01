#include <cassert>
#include <limits>
#include "big_integer_lib/big_int.h"
#include <gtest/gtest.h>

TEST(Constructor, Test1) {
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

    ASSERT_EQ(BigInt::to_string(a), "1000000000000000000000");
    ASSERT_EQ(BigInt::to_string(b), "123456789101112131415");
    ASSERT_EQ(BigInt::to_string(c), "-123456789101112131415");
    ASSERT_EQ(BigInt::to_string(d), "-1000000000000000000000");
    ASSERT_EQ(e, 0);

    ASSERT_EQ(i_max, std::numeric_limits<int>::max());
    ASSERT_EQ(i_min, std::numeric_limits<int>::min());

    ASSERT_EQ(i64_max, std::numeric_limits<int64_t>::max());
    ASSERT_EQ(i64_min, std::numeric_limits<int64_t>::min());

    ASSERT_EQ(BigInt::to_string(ui64_max), "18446744073709551615");
    ASSERT_EQ(ui64_min, 0);
}

TEST(Addition, Test2) {
    BigInt x = 0;
    int64_t answer = 0;
    for (int i = 0; i < 1'000'000; ++i) {
        x += i;
        answer += i;
        ASSERT_EQ(x, answer);
    }

    BigInt left("123456789876543212345678987654321");
    BigInt right("185472482954376984235728912432574952364745901482584172538415819427175865915691");
    ASSERT_EQ(BigInt::to_string(left + right),
              "185472482954376984235728912432574952364745901606040962414959031772854853570012");

    BigInt big_to("999999999999999999999999999999999999999999999999");
    ASSERT_EQ(BigInt::to_string(++big_to), "1000000000000000000000000000000000000000000000000");
    ASSERT_EQ(BigInt::to_string(big_to++), "1000000000000000000000000000000000000000000000000");
    ASSERT_EQ(BigInt::to_string(big_to), "1000000000000000000000000000000000000000000000001");
}

TEST(Subtraction, Test3) {
    BigInt x = 0;
    int64_t answer = 0;
    for (int i = 0; i < 1'000'000; ++i) {
        x -= i;
        answer -= i;
        ASSERT_EQ(x, answer);
    }

    BigInt left("123456789876543212345678987654321");
    BigInt right("185472482954376984235728912432574952364745901482584172538415819427175865915691");
    ASSERT_EQ(BigInt::to_string(left - right),
              "-185472482954376984235728912432574952364745901359127382661872607081496878261370");
    ASSERT_EQ(BigInt::to_string(right - left),
              "185472482954376984235728912432574952364745901359127382661872607081496878261370");

    BigInt big_to("1000000000000000000000000000000000000000000000001");
    ASSERT_EQ(BigInt::to_string(--big_to), "1000000000000000000000000000000000000000000000000");
    ASSERT_EQ(BigInt::to_string(big_to--), "1000000000000000000000000000000000000000000000000");
    ASSERT_EQ(BigInt::to_string(big_to), "999999999999999999999999999999999999999999999999");
}

TEST(Less, Test4) {
    BigInt a("1000000000000000000000");
    BigInt b("123456789101112131415");
    BigInt c("-123456789101112131415");
    BigInt d("-1000000000000000000000");

    ASSERT_FALSE(a < a);
    ASSERT_FALSE(b < b);
    ASSERT_FALSE(c < c);
    ASSERT_FALSE(d < d);

    ASSERT_LE(a, a);
    ASSERT_LE(b, b);
    ASSERT_LE(c, c);
    ASSERT_LE(d, d);

    ASSERT_LT(b, a);
    ASSERT_LE(b, a);

    ASSERT_LT(c, a);
    ASSERT_LE(c, a);
    ASSERT_LT(c, b);
    ASSERT_LE(c, b);

    ASSERT_LT(d, a);
    ASSERT_LE(d, a);
    ASSERT_LT(d, b);
    ASSERT_LE(d, b);

    ASSERT_LT(d, c);
    ASSERT_LE(d, c);

    ASSERT_LT(c, 0);
    ASSERT_LE(c, 0);
    ASSERT_LT(d, 0);
    ASSERT_LE(d, 0);

    ASSERT_LT(BigInt(std::numeric_limits<int>::min()), 0);
    ASSERT_LE(BigInt(std::numeric_limits<int>::min()), 0);
    ASSERT_LE(BigInt(std::numeric_limits<int>::min()), std::numeric_limits<int>::max());

    ASSERT_LT(BigInt(std::numeric_limits<int64_t>::min()), 0);
    ASSERT_LE(BigInt(std::numeric_limits<int64_t>::min()), 0);
    ASSERT_LE(BigInt(std::numeric_limits<int64_t>::min()), std::numeric_limits<int64_t>::min());

    ASSERT_LE(BigInt(std::numeric_limits<uint64_t>::min()), 0);
    ASSERT_LE(BigInt(std::numeric_limits<uint64_t>::min()), std::numeric_limits<uint64_t>::min());

    ASSERT_LE(BigInt(0), 0);
}

TEST(Greater, Test5) {
    BigInt a("1234104719517412534");
    BigInt b("2413521243981745");
    BigInt c("-123456789101112131415");
    BigInt d("-1000000000000000000000");

    ASSERT_FALSE(a > a);
    ASSERT_FALSE(b > b);
    ASSERT_FALSE(c > c);
    ASSERT_FALSE(d > d);

    ASSERT_GE(a, a);
    ASSERT_GE(b, b);
    ASSERT_GE(c, c);
    ASSERT_GE(d, d);

    ASSERT_GT(0, c);
    ASSERT_GE(0, c);
    ASSERT_GT(0, d);
    ASSERT_GE(0, d);

    ASSERT_GT(BigInt(std::numeric_limits<int>::max()), 0);
    ASSERT_GE(BigInt(std::numeric_limits<int>::max()), 0);
    ASSERT_GE(BigInt(std::numeric_limits<int>::max()), std::numeric_limits<int>::max());

    ASSERT_GT(BigInt(std::numeric_limits<int64_t>::max()), 0);
    ASSERT_GE(BigInt(std::numeric_limits<int64_t>::max()), 0);
    ASSERT_GE(BigInt(std::numeric_limits<int64_t>::max()), std::numeric_limits<int64_t>::max());

    ASSERT_GT(BigInt(std::numeric_limits<uint64_t>::max()), 0);
    ASSERT_GE(BigInt(std::numeric_limits<uint64_t>::max()), 0);
    ASSERT_GE(BigInt(std::numeric_limits<uint64_t>::max()), std::numeric_limits<uint64_t>::max());

    ASSERT_GE(BigInt(0), 0);
}

TEST(Equal, Test6) {
    BigInt a("1234104719517412534");
    BigInt b("2413521243981745");
    BigInt c("-14541454215");
    BigInt d("-1000000000000000000000000000000000");

    ASSERT_FALSE(a != a);
    ASSERT_FALSE(b != b);
    ASSERT_FALSE(c != c);
    ASSERT_FALSE(d != d);

    ASSERT_EQ(a, a);
    ASSERT_EQ(b, b);
    ASSERT_EQ(c, c);
    ASSERT_EQ(d, d);

    ASSERT_EQ(BigInt(0), 0);
}
