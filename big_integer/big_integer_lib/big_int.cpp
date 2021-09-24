#include "big_int.h"
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <sstream>

/*
    Utility functions
*/

bool BigInt::IsValidNumber(const std::string& number) {
    for (char digit : number) {
        if (digit < '0' || digit > '9') {
            return false;
        }
    }
    return true;
}

void BigInt::Convert(const std::string& str) {
    for (int i = str.size() - 1; i >= 0; i -= kBaseDigits) {
        int current = 0;
        for (int j = std::max(0, i - kBaseDigits + 1); j <= i; ++j) {
            current = current * 10 + str[j] - '0';
        }
        digits_.push_back(current);
    }
}

void BigInt::Trim() {
    while (!digits_.empty() && !digits_.back()) {
        digits_.pop_back();
    }
    if (digits_.empty()) {
        sign_ = 1;
    }
}

void BigInt::Read(const std::string& str) {
    if (str.empty()) {
        sign_ = 1;
    } else if (str[0] == '+' || str[0] == '-') {
        std::string magnitude = str.substr(1);
        if (IsValidNumber(magnitude)) {
            sign_ = (str[0] == '+' ? 1 : -1);
            Convert(magnitude);
        } else {
            throw std::invalid_argument("Expected an integer, got \'" + str + "\'");
        }
    } else if (IsValidNumber(str)) {
        sign_ = 1;
        Convert(str);
    } else {
        throw std::invalid_argument("Expected an integer, got \'" + str + "\'");
    }
    Trim();
}

BigInt BigInt::abs() const {
    BigInt result = *this;
    result.sign_ = 1;
    return result;
}

/*
    Constructors
*/

BigInt::BigInt() : sign_(1) {
}

BigInt::BigInt(const BigInt& number) : sign_(number.sign_), digits_(number.digits_) {
}

BigInt::BigInt(const std::string& str) {
    Read(str);
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

BigInt& BigInt::operator+=(const std::string& value) {
    return *this = (*this + BigInt(value));
}

BigInt& BigInt::operator-=(const std::string& value) {
    return *this = (*this - BigInt(value));
}

BigInt& BigInt::operator+=(int64_t value) {
    return *this = (*this + BigInt(value));
}

BigInt& BigInt::operator-=(int64_t value) {
    return *this = (*this - BigInt(value));
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
        result.Trim();
        return result;
    }
    return -(number - *this);
}

BigInt BigInt::operator+(const std::string& number) const {
    return *this + BigInt(number);
}

BigInt operator+(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) + rhs;
}

BigInt BigInt::operator-(const std::string& num) const {
    return *this - BigInt(num);
}

BigInt operator-(const std::string& lhs, const BigInt& rhs) {
    return BigInt(lhs) - rhs;
}

BigInt BigInt::operator+(int64_t number) const {
    return *this + BigInt(number);
}

BigInt operator+(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) + rhs;
}

BigInt BigInt::operator-(int64_t number) const {
    return *this - BigInt(number);
}

BigInt operator-(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) - rhs;
}

/*
    Relational operators; All operators depend on the '<' operator.
*/

bool BigInt::operator<(const BigInt& number) const {
    if (sign_ != number.sign_) {
        return sign_ == -1;
    }
    if (digits_.size() != number.digits_.size()) {
        return digits_.size() * sign_ < number.digits_.size() * sign_;
    }
    for (int i = digits_.size() - 1; i >= 0; --i) {
        if (digits_[i] != number.digits_[i]) {
            return digits_[i] * sign_ < number.digits_[i] * sign_;
        }
    }
    return false;
}

bool BigInt::operator>(const BigInt& number) const {
    return number < *this;
}

bool BigInt::operator<=(const BigInt& number) const {
    return !(number < *this);
}

bool BigInt::operator>=(const BigInt& number) const {
    return !(*this < number);
}

bool BigInt::operator==(const BigInt& number) const {
    return !(*this < number) && !(number < *this);
}

bool BigInt::operator!=(const BigInt& number) const {
    return *this < number || number < *this;
}

bool BigInt::operator<(int64_t number) const {
    return *this < BigInt(number);
}

bool operator<(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) < rhs;
}

bool BigInt::operator>(int64_t number) const {
    return *this > BigInt(number);
}

bool operator>(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) > rhs;
}

bool BigInt::operator<=(int64_t number) const {
    return *this <= BigInt(number);
}

bool operator<=(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) <= rhs;
}

bool BigInt::operator>=(int64_t number) const {
    return *this >= BigInt(number);
}

bool operator>=(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) >= rhs;
}

bool BigInt::operator==(int64_t number) const {
    return *this == BigInt(number);
}

bool operator==(int64_t lhs, const BigInt& rhs) {
    return BigInt(lhs) == rhs;
}

bool BigInt::operator!=(int64_t number) const {
    return *this != BigInt(number);
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
    number.Read(input);
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
