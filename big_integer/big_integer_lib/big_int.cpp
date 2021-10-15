#include "big_int.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

/*
    Utility functions
*/

bool BigInt::isValidNumber(const std::string& number) {
    for (char digit : number) {
        if (digit < '0' || digit > '9') {
            return false;
        }
    }
    return true;
}

void BigInt::convert(const std::string& str) {
    for (int i = str.size() - 1; i >= 0; i -= kBaseDigits) {
        int current = 0;
        for (int j = std::max(0, i - kBaseDigits + 1); j <= i; ++j) {
            current = current * 10 + str[j] - '0';
        }
        digits_.push_back(current);
    }
}

void BigInt::trim() {
    while (!digits_.empty() && !digits_.back()) {
        digits_.pop_back();
    }
    if (digits_.empty()) {
        sign_ = 1;
    }
}

void BigInt::read(const std::string& str) {
    if (str.empty()) {
        sign_ = 1;
    } else if (str[0] == '+' || str[0] == '-') {
        std::string magnitude = str.substr(1);
        if (isValidNumber(magnitude)) {
            sign_ = (str[0] == '+' ? 1 : -1);
            convert(magnitude);
        } else {
            throw std::invalid_argument("Expected an integer, got \'" + str + "\'");
        }
    } else if (isValidNumber(str)) {
        sign_ = 1;
        convert(str);
    } else {
        throw std::invalid_argument("Expected an integer, got \'" + str + "\'");
    }
    trim();
}

std::vector<int> BigInt::convertBase(const std::vector<int>& digits, int old_digits,
                                     int new_digits) {
    std::vector<int64_t> p(std::max(old_digits, new_digits) + 1);
    p[0] = 1;
    for (int i = 1; i < static_cast<int>(p.size()); ++i) {
        p[i] = p[i - 1] * 10;
    }
    std::vector<int> result;
    int64_t current = 0;
    int cur_digits = 0;
    for (int digit : digits) {
        current += digit * p[cur_digits];
        cur_digits += old_digits;
        while (cur_digits >= new_digits) {
            result.push_back(static_cast<int>(current % p[new_digits]));
            current /= p[new_digits];
            cur_digits -= new_digits;
        }
    }
    result.push_back(static_cast<int>(current));
    while (!result.empty() && !result.back()) {
        result.pop_back();
    }
    return result;
}

std::vector<int64_t> BigInt::karatsubaMultiply(const std::vector<int64_t>& a,
                                               const std::vector<int64_t>& b) {
    int n = a.size();
    std::vector<int64_t> res(n + n);
    if (n <= 32) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                res[i + j] += a[i] * b[j];
            }
        }
        return res;
    }

    int k = n >> 1;
    std::vector<int64_t> a1(a.begin(), a.begin() + k);
    std::vector<int64_t> a2(a.begin() + k, a.end());
    std::vector<int64_t> b1(b.begin(), b.begin() + k);
    std::vector<int64_t> b2(b.begin() + k, b.end());

    std::vector<int64_t> a1b1 = karatsubaMultiply(a1, b1);
    std::vector<int64_t> a2b2 = karatsubaMultiply(a2, b2);

    for (int i = 0; i < k; ++i) {
        a2[i] += a1[i];
    }
    for (int i = 0; i < k; ++i) {
        b2[i] += b1[i];
    }

    std::vector<int64_t> r = karatsubaMultiply(a2, b2);
    for (int i = 0; i < static_cast<int>(a1b1.size()); ++i) {
        r[i] -= a1b1[i];
    }
    for (int i = 0; i < static_cast<int>(a2b2.size()); ++i) {
        r[i] -= a2b2[i];
    }

    for (int i = 0; i < static_cast<int>(r.size()); ++i) {
        res[i + k] += r[i];
    }
    for (int i = 0; i < static_cast<int>(a1b1.size()); ++i) {
        res[i] += a1b1[i];
    }
    for (int i = 0; i < static_cast<int>(a2b2.size()); ++i) {
        res[i + n] += a2b2[i];
    }
    return res;
}

BigInt BigInt::abs() const {
    BigInt result = *this;
    result.sign_ = 1;
    return result;
}

std::pair<BigInt, BigInt> divmod(const BigInt& a1, const BigInt& b1) {
    if (a1.digits_.empty()) {
        return {0, 0};
    }
    int norm = a1.kBase / (b1.digits_.back() + 1);
    BigInt a = a1.abs() * norm;
    BigInt b = b1.abs() * norm;
    BigInt q, r = 0;
    q.digits_.resize(a.digits_.size());

    for (int i = static_cast<int>(a.digits_.size()) - 1; i >= 0; --i) {
        r *= a1.kBase;
        r += a.digits_[i];
        int s1 = r.digits_.size() <= b.digits_.size() ? 0 : r.digits_[b.digits_.size()];
        int s2 = r.digits_.size() <= b.digits_.size() - 1 ? 0 : r.digits_[b.digits_.size() - 1];
        int d = (static_cast<int64_t>(a1.kBase) * s1 + s2) / b.digits_.back();
        r -= b * d;
        while (r < 0) {
            r += b;
            --d;
        }
        q.digits_[i] = d;
    }

    q.sign_ = a1.sign_ * b1.sign_;
    r.sign_ = a1.sign_;
    q.trim();
    r.trim();
    return std::make_pair(q, r / norm);
}

/*
    Constructors
*/

BigInt::BigInt() : sign_(1) {
}

BigInt::BigInt(const BigInt& number) : sign_(number.sign_), digits_(number.digits_) {
}

BigInt::BigInt(const std::string& str) {
    read(str);
}

BigInt::BigInt(int number) {
    *this = static_cast<int64_t>(number);
}

BigInt::BigInt(unsigned int number) {
    *this = static_cast<uint64_t>(number);
}

BigInt::BigInt(int64_t number) {
    *this = number;
}

BigInt::BigInt(uint64_t number) {
    *this = number;
}

/*
    Assignment operators
*/

BigInt& BigInt::operator=(const BigInt& number) {
    sign_ = number.sign_;
    digits_ = number.digits_;
    return *this;
}

BigInt& BigInt::operator=(int64_t number) {
    sign_ = 1;
    digits_.clear();
    if (number < 0) {
        sign_ = -1;
        number = -number;
    }
    for (; number > 0; number /= kBase) {
        digits_.push_back(number % kBase);
    }
    return *this;
}

BigInt& BigInt::operator=(uint64_t number) {
    sign_ = 1;
    digits_.clear();
    for (; number > 0; number /= kBase) {
        digits_.push_back(number % kBase);
    }
    return *this;
}

/*
    Unary arithmetic operators
*/

BigInt BigInt::operator+() const {
    return *this;
}

BigInt BigInt::operator-() const {
    BigInt temp;
    if (!digits_.empty()) {
        temp.sign_ = -sign_;
    } else {
        temp.sign_ = 1;
    }
    temp.digits_ = digits_;
    return temp;
}

/*
    Arithmetic-assignment operators
*/

BigInt& BigInt::operator+=(const BigInt& value) {
    return *this = (*this + value);
}

BigInt& BigInt::operator-=(const BigInt& value) {
    return *this = (*this - value);
}

BigInt& BigInt::operator*=(const BigInt& value) {
    return *this = (*this * value);
}

BigInt& BigInt::operator/=(const BigInt& value) {
    return *this = (*this / value);
}

BigInt& BigInt::operator%=(const BigInt& value) {
    return *this = (*this % value);
}

BigInt& BigInt::operator+=(const std::string& value) {
    return *this = (*this + BigInt(value));
}

BigInt& BigInt::operator-=(const std::string& value) {
    return *this = (*this - BigInt(value));
}

BigInt& BigInt::operator*=(const std::string& value) {
    return *this = (*this * BigInt(value));
}

BigInt& BigInt::operator/=(const std::string& value) {
    return *this = (*this / BigInt(value));
}

BigInt& BigInt::operator%=(const std::string& value) {
    return *this = (*this % BigInt(value));
}

BigInt& BigInt::operator+=(int64_t value) {
    return *this = (*this + BigInt(value));
}

BigInt& BigInt::operator-=(int64_t value) {
    return *this = (*this - BigInt(value));
}

BigInt& BigInt::operator*=(int64_t value) {
    return *this = (*this * BigInt(value));
}

BigInt& BigInt::operator/=(int64_t value) {
    return *this = (*this / BigInt(value));
}

BigInt& BigInt::operator%=(int64_t value) {
    return *this = (*this % BigInt(value));
}

/*
    Binary arithmetic operators
*/

BigInt BigInt::operator+(const BigInt& number) const {
    if (sign_ != number.sign_) {
        return *this - (-number);
    }
    BigInt result = number;
    for (int i = 0, carry = 0;
         i < static_cast<int>(std::max(digits_.size(), number.digits_.size())) || carry; ++i) {
        if (i == static_cast<int>(result.digits_.size())) {
            result.digits_.push_back(0);
        }
        result.digits_[i] += carry + (i < static_cast<int>(digits_.size()) ? digits_[i] : 0);
        carry = result.digits_[i] >= kBase;
        if (carry) {
            result.digits_[i] -= kBase;
        }
    }
    return result;
}

BigInt BigInt::operator-(const BigInt& number) const {
    if (sign_ != number.sign_) {
        return *this + (-number);
    }
    if (abs() >= number.abs()) {
        BigInt result = *this;
        for (int i = 0, carry = 0; i < static_cast<int>(number.digits_.size()) || carry; ++i) {
            result.digits_[i] -=
                carry + (i < static_cast<int>(number.digits_.size()) ? number.digits_[i] : 0);
            carry = result.digits_[i] < 0;
            if (carry) {
                result.digits_[i] += kBase;
            }
        }
        result.trim();
        return result;
    }
    return -(number - *this);
}

BigInt BigInt::operator*(const BigInt& number) const {
    std::vector<int> a6 = convertBase(digits_, kBaseDigits, 6);
    std::vector<int> b6 = convertBase(number.digits_, kBaseDigits, 6);
    std::vector<int64_t> a(a6.begin(), a6.end());
    std::vector<int64_t> b(b6.begin(), b6.end());
    while (a.size() < b.size()) {
        a.push_back(0);
    }
    while (b.size() < a.size()) {
        b.push_back(0);
    }
    while (a.size() & (a.size() - 1)) {
        a.push_back(0);
        b.push_back(0);
    }
    std::vector<int64_t> multiply = karatsubaMultiply(a, b);
    BigInt result;
    result.sign_ = sign_ * number.sign_;
    int carry = 0;
    for (int64_t multiply_digit : multiply) {
        int64_t cur = multiply_digit + carry;
        result.digits_.push_back(static_cast<int>(cur % 1000000));
        carry = static_cast<int>(cur / 1000000);
    }
    result.digits_ = convertBase(result.digits_, 6, kBaseDigits);
    result.trim();
    return result;
}

BigInt BigInt::operator/(const BigInt& rhs) const {
    return divmod(*this, rhs).first;
}

BigInt BigInt::operator%(const BigInt& rhs) const {
    return divmod(*this, rhs).second;
}

BigInt BigInt::operator+(const std::string& rhs) const {
    return *this + BigInt(rhs);
}

BigInt operator+(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) + rhs;
}

BigInt BigInt::operator-(const std::string& rhs) const {
    return *this - BigInt(rhs);
}

BigInt operator-(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) - rhs;
}

BigInt BigInt::operator*(const std::string& rhs) const {
    return *this - BigInt(rhs);
}

BigInt operator*(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) * rhs;
}

BigInt BigInt::operator/(const std::string& rhs) const {
    return *this / BigInt(rhs);
}

BigInt operator/(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) / rhs;
}

BigInt BigInt::operator%(const std::string& rhs) const {
    return *this % BigInt(rhs);
}

BigInt operator%(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) % rhs;
}

BigInt BigInt::operator+(int64_t rhs) const {
    return *this + BigInt(rhs);
}

BigInt operator+(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) + rhs;
}

BigInt BigInt::operator-(int64_t rhs) const {
    return *this - BigInt(rhs);
}

BigInt operator-(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) - rhs;
}

BigInt BigInt::operator*(int64_t rhs) const {
    return *this * BigInt(rhs);
}

BigInt operator*(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) * rhs;
}

BigInt BigInt::operator/(int64_t rhs) const {
    return *this / BigInt(rhs);
}

BigInt operator/(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) / rhs;
}

BigInt BigInt::operator%(int64_t rhs) const {
    return *this % BigInt(rhs);
}

BigInt operator%(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) % rhs;
}

/*
    Relational operators; All operators depend on the '<' operator.
*/

bool BigInt::operator<(const BigInt& rhs) const {
    if (sign_ != rhs.sign_) {
        return sign_ == -1;
    }
    if (digits_.size() != rhs.digits_.size()) {
        return digits_.size() * sign_ < rhs.digits_.size() * sign_;
    }
    for (int i = digits_.size() - 1; i >= 0; --i) {
        if (digits_[i] != rhs.digits_[i]) {
            return digits_[i] * sign_ < rhs.digits_[i] * sign_;
        }
    }
    return false;
}

bool BigInt::operator>(const BigInt& rhs) const {
    return rhs < *this;
}

bool BigInt::operator<=(const BigInt& rhs) const {
    return !(rhs < *this);
}

bool BigInt::operator>=(const BigInt& rhs) const {
    return !(*this < rhs);
}

bool BigInt::operator==(const BigInt& rhs) const {
    return !(*this < rhs) && !(rhs < *this);
}

bool BigInt::operator!=(const BigInt& rhs) const {
    return *this < rhs || rhs < *this;
}

bool BigInt::operator<(int64_t rhs) const {
    return *this < BigInt(rhs);
}

bool operator<(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) < rhs;
}

bool BigInt::operator>(int64_t rhs) const {
    return *this > BigInt(rhs);
}

bool operator>(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) > rhs;
}

bool BigInt::operator<=(int64_t rhs) const {
    return *this <= BigInt(rhs);
}

bool operator<=(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) <= rhs;
}

bool BigInt::operator>=(int64_t rhs) const {
    return *this >= BigInt(rhs);
}

bool operator>=(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) >= rhs;
}

bool BigInt::operator==(int64_t rhs) const {
    return *this == BigInt(rhs);
}

bool operator==(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) == rhs;
}

bool BigInt::operator!=(int64_t rhs) const {
    return *this != BigInt(rhs);
}

bool operator!=(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) != rhs;
}

/*
    I/O stream operators
*/
std::istream& operator>>(std::istream& in, BigInt& number) {
    std::string input;
    in >> input;
    number.read(input);
    return in;
}

std::ostream& operator<<(std::ostream& out, const BigInt& number) {
    if (number.sign_ == -1) {
        out << '-';
    }
    out << (number.digits_.empty() ? 0 : number.digits_.back());
    for (int i = static_cast<int>(number.digits_.size()) - 2; i >= 0; --i) {
        out << std::setw(number.kBaseDigits) << std::setfill('0') << number.digits_[i];
    }
    return out;
}

/*
    Conversion functions for BigInt
*/

std::string BigInt::to_string(const BigInt& number) {
    std::stringstream ss;
    ss << number;
    std::string s;
    ss >> s;
    return s;
}

int BigInt::to_int(const BigInt& number) {
    int value = 0;
    for (int i = number.digits_.size() - 1; i >= 0; --i) {
        value = value * kBase + number.digits_[i];
    }
    return value * number.sign_;
}

int64_t BigInt::to_int64_t(const BigInt& number) {
    int64_t value = 0;
    for (int i = number.digits_.size() - 1; i >= 0; --i) {
        value = value * kBase + number.digits_[i];
    }
    return value * number.sign_;
}

uint64_t BigInt::to_uint64_t(const BigInt& number) {
    uint64_t value = 0;
    for (int i = number.digits_.size() - 1; i >= 0; --i) {
        value = value * kBase + number.digits_[i];
    }
    return value;
}

/*
    Increment and decrement operators
*/

BigInt& BigInt::operator++() {
    return *this += 1;
}

BigInt& BigInt::operator--() {
    return *this -= 1;
}

BigInt BigInt::operator++(int) {
    *this += 1;
    return *this - 1;
}

BigInt BigInt::operator--(int) {
    *this -= 1;
    return *this + 1;
}
