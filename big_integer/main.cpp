#include <iostream>
#include "big_integer_lib/big_int.h"
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <list>
#include <iterator>

const int kLeftAssoc = 0;
const int kRightAssoc = 1;

// Map the different operators: +, -, *, / etc
typedef std::map<std::string, std::pair<int, int>> OpMap;

const OpMap::value_type kAssocs[] = {
    OpMap::value_type("+", {0, kLeftAssoc}), OpMap::value_type("-", {0, kLeftAssoc}),
    OpMap::value_type("*", {5, kLeftAssoc}), OpMap::value_type("/", {5, kLeftAssoc}),
    OpMap::value_type("%", {5, kLeftAssoc})};

const OpMap kOpmap(kAssocs, kAssocs + sizeof(kAssocs) / sizeof(kAssocs[0]));

bool isParenthesis(const std::string& token) {
    return token == "(" || token == ")";
}

bool isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%";
}

bool isAssociative(const std::string& token, int type) {
    const std::pair<int, int> p = kOpmap.find(token)->second;
    return p.second == type;
}

int cmpPrecedence(const std::string& token1, const std::string& token2) {
    const std::pair<int, int> p1 = kOpmap.find(token1)->second;
    const std::pair<int, int> p2 = kOpmap.find(token2)->second;

    return p1.first - p2.first;
}

bool infixToRPN(std::queue<std::string>* input_tokens, int size,
                std::vector<std::string>* str_array) {
    bool success = true;

    std::list<std::string> out;
    std::stack<std::string> stack;

    for (int i = 0; i < size; ++i) {
        const std::string token = input_tokens->front();
        input_tokens->pop();

        if (isOperator(token)) {
            const std::string o1 = token;

            if (!stack.empty()) {
                std::string o2 = stack.top();

                while (isOperator(o2) &&
                       ((isAssociative(o1, kLeftAssoc) && cmpPrecedence(o1, o2) == 0) ||
                        cmpPrecedence(o1, o2) < 0)) {
                    stack.pop();
                    out.push_back(o2);

                    if (!stack.empty()) {
                        o2 = stack.top();
                    } else {
                        break;
                    }
                }
            }

            stack.push(o1);
        } else if (token == "(") {
            stack.push(token);
        } else if (token == ")") {
            std::string top_token = stack.top();

            while (top_token != "(") {
                out.push_back(top_token);
                stack.pop();
                if (stack.empty()) {
                    break;
                }
                top_token = stack.top();
            }

            if (!stack.empty()) {
                stack.pop();
            }

            if (top_token != "(") {
                return false;
            }
        } else {
            out.push_back(token);
        }
    }

    while (!stack.empty()) {
        const std::string stack_token = stack.top();

        if (isParenthesis(stack_token)) {
            return false;
        }
        out.push_back(stack_token);
        stack.pop();
    }

    str_array->assign(out.begin(), out.end());
    return success;
}

BigInt rpNtoBigInt(const std::vector<std::string>& tokens) {
    std::stack<std::string> st;

    for (const std::string& token : tokens) {
        if (!isOperator(token)) {
            st.push(token);
        } else {
            BigInt result;

            const std::string val2 = st.top();
            st.pop();
            const BigInt d2(val2);

            if (!st.empty()) {
                const std::string val1 = st.top();
                st.pop();
                const BigInt d1(val1);

                result = token == "+"   ? d1 + d2
                         : token == "-" ? d1 - d2
                         : token == "*" ? d1 * d2
                         : token == "/" ? d1 / d2
                                        : d1 % d2;
            } else {
                if (token == "-") {
                    result = -d2;
                } else {
                    result = d2;
                }
            }
            st.push(BigInt::to_string(result));
        }
    }

    return BigInt(st.top());
}

std::queue<std::string> getExpressionTokens(const std::string& expression) {
    std::queue<std::string> tokens;
    std::string str;
    for (char expression_char : expression) {
        const std::string token(1, expression_char);

        if (isOperator(token) || isParenthesis(token)) {
            if (!str.empty()) {
                tokens.push(str);
            }
            str.clear();
            tokens.push(token);
        } else {
            if (!token.empty() && token != " " && token != "\t") {
                str.append(token);
            } else {
                if (!str.empty()) {
                    tokens.push(str);
                    str.clear();
                }
            }
        }
    }
    if (!str.empty()) {
        tokens.push(str);
    }
    return tokens;
}

template <typename T, typename InputIterator>
void print(const std::string& message, const InputIterator& it_begin, const InputIterator& it_end,
           const std::string& delimiter) {
    std::cout << message << '\n';

    std::copy(it_begin, it_end, std::ostream_iterator<T>(std::cout, delimiter.c_str()));

    std::cout << '\n';
}

int main() {
    std::string s;
    std::getline(std::cin, s);

    print<char, std::string::iterator>("Input expression:", s.begin(), s.end(), "");

    std::queue<std::string> tokens = getExpressionTokens(s);
    std::vector<std::string> rpn;
    if (infixToRPN(&tokens, tokens.size(), &rpn)) {
        BigInt big_integer = rpNtoBigInt(rpn);
        print<std::string, std::vector<std::string>::const_iterator>("RPN tokens:", rpn.begin(),
                                                                     rpn.end(), " ");
        std::cout << "Result = " << big_integer << '\n';
    } else {
        std::cout << "Mis-match in parentheses" << '\n';
    }
    return 0;
}
