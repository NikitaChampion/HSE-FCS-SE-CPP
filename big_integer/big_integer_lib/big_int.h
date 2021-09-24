#include <string>
#include <vector>

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
    BigInt& operator=(const std::string&);
    BigInt& operator=(int64_t);
    BigInt& operator=(uint64_t);

    // Unary arithmetic operators:
    BigInt operator+() const;  // unary +
    BigInt operator-() const;  // unary -

    // Binary arithmetic operators:
    BigInt operator+(const BigInt&) const;
    BigInt operator-(const BigInt&) const;
    BigInt operator+(const std::string&) const;
    BigInt operator-(const std::string&) const;
    BigInt operator+(int64_t) const;
    BigInt operator-(int64_t) const;

    // Arithmetic-assignment operators:
    BigInt& operator+=(const BigInt&);
    BigInt& operator-=(const BigInt&);
    BigInt& operator+=(const std::string&);
    BigInt& operator-=(const std::string&);
    BigInt& operator+=(int64_t);
    BigInt& operator-=(int64_t);

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
    bool operator<(const std::string&) const;
    bool operator>(const std::string&) const;
    bool operator<=(const std::string&) const;
    bool operator>=(const std::string&) const;
    bool operator==(const std::string&) const;
    bool operator!=(const std::string&) const;
    bool operator<(int64_t) const;
    bool operator>(int64_t) const;
    bool operator<=(int64_t) const;
    bool operator>=(int64_t) const;
    bool operator==(int64_t) const;
    bool operator!=(int64_t) const;

    // I/O stream operators:
    friend std::istream& operator>>(std::istream&, BigInt&);
    friend std::ostream& operator<<(std::ostream&, const BigInt&);

    // Conversion functions:
    std::string to_string() const;  // NOLINT
    int to_int() const;             // NOLINT
    int64_t to_int64_t() const;     // NOLINT
    uint64_t to_uint64_t() const;   // NOLINT

private:
    static const int kBase = 1000000000;
    static const int kBaseDigits = 9;

    int sign_;
    std::vector<int> digits_;

    void Read(const std::string&);
    bool IsValidNumber(const std::string&);
    void Convert(const std::string&);
    void Trim();
    BigInt Abs() const;
};
