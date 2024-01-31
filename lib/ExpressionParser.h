#include <deque>
#include <set>
#include <regex>
#include <cmath>
#include <string>
#include <utility>
#include <vector>
#include <variant>

struct Expression {
    Expression() = default;
    explicit Expression(std::string token) : token(std::move(token)) {}
    Expression(std::string token, const Expression& a) : token(std::move(token)), args{a} {}
    Expression(std::string token, const Expression& a, const Expression& b) : token(std::move(token)), args{a, b} {}

    std::string token;
    std::vector<Expression> args;
};

double Evaluate(const Expression& expression);

class ExpressionParser {
public:
    explicit ExpressionParser(std::vector<std::string>& input);
    Expression Parse();
private:
    std::string ParseToken();
    Expression ParseUnaryExpression();
    Expression ParseBinaryExpression(uint8_t min_priority);
    Expression final_expression;
    static uint8_t GetPriority(const std::string& token);
    std::deque<std::string> data;
    const std::set<std::string> tokens {"+", "-", "**", "*", "/", "mod", "abs", "sin", "cos", "AND", "OR", "NOT", "=", "!=", ">", "<", "(", ")"};
};
