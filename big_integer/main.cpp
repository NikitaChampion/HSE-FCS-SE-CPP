#include <iostream>
#include "big_integer_lib/big_int.h"

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    BigInt x, y;
    std::cin >> x >> y;
    std::cout << x - y;
    return 0;
}
