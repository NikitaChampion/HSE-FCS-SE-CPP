#include <string>
#include <vector>
#include <utility>
#include <type_traits>

class BigInt {
public:
    // Constructors:
    BigInt();
    BigInt(const BigInt&);
    BigInt(const std::string&);
    BigInt(int);
    BigInt(unsigned int);
    BigInt(int64_t);
    BigInt(uint64_t);

    // Assignment operators:
    BigInt& operator=(const BigInt&);
    BigInt& operator=(int64_t);
    BigInt& operator=(uint64_t);

    // Unary arithmetic operators:
    BigInt operator+() const;
    BigInt operator-() const;

    // Arithmetic-assignment operators:
    BigInt& operator+=(const BigInt&);
    BigInt& operator-=(const BigInt&);
    BigInt& operator*=(const BigInt&);
    BigInt& operator/=(const BigInt&);
    BigInt& operator%=(const BigInt&);
    BigInt& operator+=(const std::string&);
    BigInt& operator-=(const std::string&);
    BigInt& operator*=(const std::string&);
    BigInt& operator/=(const std::string&);
    BigInt& operator%=(const std::string&);
    BigInt& operator+=(int64_t);
    BigInt& operator-=(int64_t);
    BigInt& operator*=(int64_t);
    BigInt& operator/=(int64_t);
    BigInt& operator%=(int64_t);

    // Binary arithmetic operators:
    BigInt operator+(const BigInt&) const;
    BigInt operator-(const BigInt&) const;
    BigInt operator*(const BigInt&) const;
    BigInt operator/(const BigInt&) const;
    BigInt operator%(const BigInt&) const;
    BigInt operator+(const std::string&) const;
    BigInt operator-(const std::string&) const;
    BigInt operator*(const std::string&) const;
    BigInt operator/(const std::string&) const;
    BigInt operator%(const std::string&) const;
    BigInt operator+(int64_t) const;
    BigInt operator-(int64_t) const;
    BigInt operator*(int64_t) const;
    BigInt operator/(int64_t) const;
    BigInt operator%(int64_t) const;

    // Increment and decrement operators:
    BigInt& operator++();    // pre-increment
    BigInt& operator--();    // pre-decrement
    BigInt operator++(int);  // post-increment
    BigInt operator--(int);  // post-decrement

    // Relational operators:
    bool operator<(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>=(const BigInt&) const;
    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator<(int64_t) const;
    bool operator>(int64_t) const;
    bool operator<=(int64_t) const;
    bool operator>=(int64_t) const;
    bool operator==(int64_t) const;
    bool operator!=(int64_t) const;

    friend bool operator<(int64_t, const BigInt&);
    friend bool operator>(int64_t, const BigInt&);
    friend bool operator<=(int64_t, const BigInt&);
    friend bool operator>=(int64_t, const BigInt&);
    friend bool operator==(int64_t, const BigInt&);
    friend bool operator!=(int64_t, const BigInt&);

    // I/O stream operators:
    friend std::istream& operator>>(std::istream&, BigInt&);
    friend std::ostream& operator<<(std::ostream&, const BigInt&);

    // Math functions:
    BigInt abs() const;                                                     // NOLINT
    friend std::pair<BigInt, BigInt> divmod(const BigInt&, const BigInt&);  // NOLINT

    // Conversion functions:
    static std::string to_string(const BigInt&);  // NOLINT
    static int to_int(const BigInt&);             // NOLINT
    static int64_t to_int64_t(const BigInt&);     // NOLINT
    static uint64_t to_uint64_t(const BigInt&);   // NOLINT

private:
    static const int kBase = 1000000000;
    static const int kBaseDigits = 9;

    int sign_;
    std::vector<int> digits_;

    // Utility functions:
    void read(const std::string&);
    bool isValidNumber(const std::string&);
    void convert(const std::string&);
    void trim();
    static std::vector<int> convertBase(const std::vector<int>&, int, int);
    static std::vector<int64_t> karatsubaMultiply(const std::vector<int64_t>&,
                                                  const std::vector<int64_t>&);
};
