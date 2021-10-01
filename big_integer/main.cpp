#include <iostream>
#include "big_integer_lib/big_int.h"

int main() {
    std::cout << "Enter 2 valid numbers, may start with \"-\": \n";
    BigInt x, y;
    std::cin >> x >> y;
    std::cout << "x + y = " << x + y << '\n';
    std::cout << "x - y = " << x - y << '\n';
    std::cout << "x * y = " << x * y << '\n';
    std::cout << "x / y = " << x / y << '\n';
    std::cout << "x % y = " << x % y << '\n';
    std::cout << "x > y = " << (x > y) << '\n';
    std::cout << "x < y = " << (x < y) << '\n';
    std::cout << "x >= y = " << (x >= y) << '\n';
    std::cout << "x <= y = " << (x <= y) << '\n';
    std::cout << "x == y = " << (x == y) << '\n';
    std::cout << "x != y = " << (x != y) << '\n';
    std::cout << "abs(x) = " << x.abs() << '\n';
    std::cout << "+x = " << +x << '\n';
    std::cout << "-x = " << -x << '\n';
    std::cout << "x++ = " << x++ << '\n';
    std::cout << "++x = " << ++x << '\n';
    std::cout << "x-- = " << x-- << '\n';
    std::cout << "--x = " << --x << '\n';
    std::cout << "(x = y) = " << (x = y) << '\n';
    return 0;
}