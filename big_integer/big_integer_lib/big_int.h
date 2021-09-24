#include <string>
#include <vector>
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
    BigInt& operator+=(const std::string&);
    BigInt& operator-=(const std::string&);
    BigInt& operator+=(int64_t);
    BigInt& operator-=(int64_t);

    // Binary arithmetic operators:
    BigInt operator+(const BigInt&) const;
    BigInt operator-(const BigInt&) const;
    BigInt operator+(const std::string&) const;
    BigInt operator-(const std::string&) const;
    BigInt operator+(int64_t) const;
    BigInt operator-(int64_t) const;

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
    BigInt abs() const;  // NOLINT

    // Conversion functions:
    static std::string to_string(const BigInt& number);  // NOLINT
    static int to_int(const BigInt& number);             // NOLINT
    static int64_t to_int64_t(const BigInt& number);     // NOLINT
    static uint64_t to_uint64_t(const BigInt& number);   // NOLINT

private:
    static const int kBase = 1000000000;
    static const int kBaseDigits = 9;

    int sign_;
    std::vector<int> digits_;

    // Utility functions:
    void Read(const std::string&);
    bool IsValidNumber(const std::string&);
    void Convert(const std::string&);
    void Trim();
};
