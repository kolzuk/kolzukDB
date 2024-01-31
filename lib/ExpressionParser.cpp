#include "ExpressionParser.h"
#include <iostream>

double Evaluate(const Expression& expression) {
    if (expression.args.empty()) {
        return std::stod(expression.token);
    } else if (expression.args.size() == 1) {
        auto a = Evaluate(expression.args[0]);
        if (expression.token == "+") {
            return +a;
        } else if (expression.token == "-") {
            return -a;
        } else if (expression.token == "abs") {
            return abs(a);
        } else if (expression.token == "sin") {
            return sin(a);
        } else if (expression.token == "cos") {
            return cos(a);
        } else if (expression.token == "NOT") {
            if (a != 0) {
                return 0;
            } else {
                return 1;
            }
        }
        throw std::runtime_error("Unknown unary operation: '" + expression.token + "'");
    } else if (expression.args.size() == 2) {
        auto a = Evaluate(expression.args[0]);
        auto b = Evaluate(expression.args[1]);
        if (expression.token == "+") {
            return a + b;
        } else if (expression.token == "-") {
            return a - b;
        } else if (expression.token == "*") {
            return a * b;
        } else if (expression.token == "/") {
            return a / b;
        } else if (expression.token == "**") {
            return pow(a, b);
        } else if (expression.token == "mod") {
            return static_cast<int>(a) % static_cast<int>(b);
        } else if (expression.token == ">") {
            return a > b;
        } else if (expression.token == "<") {
            return a < b;
        } else if (expression.token == "=") {
            return a == b;
        } else if (expression.token == "!=") {
            return a != b;
        } else if (expression.token == "AND") {
            return a && b;
        } else if (expression.token == "OR") {
            return a || b;
        } else {
            throw std::logic_error("Unknown binary operation: '" + expression.token + "'");
        }
    }
}

ExpressionParser::ExpressionParser(std::vector<std::string>& input) {
    for (const auto& token : input) {
        data.push_back(token);
    }
}

std::string ExpressionParser::ParseToken() {
    if (data.empty()) {
        return "";
    }

    auto token = data.front();
    data.pop_front();

    if (std::isdigit(token.front())
    || tokens.contains(token)
    || token.front() == '"' && token.back() == '"') {
        return token;
    }

    throw std::logic_error("Bad token: " + token);
}

Expression ExpressionParser::Parse() {
    return ParseBinaryExpression(0);
}

Expression ExpressionParser::ParseUnaryExpression() {
    auto token = ParseToken();

    if (token == "(") {
        auto result = Parse();
        if (ParseToken() != ")") {
            throw std::logic_error("Expected: ')'");
        }
        return result;
    }

    if (std::isdigit(token.front()) || (token.front() == '"' && token.back() == '"')) {
        return Expression(token);
    }

    return Expression(token, ParseUnaryExpression());
}

Expression ExpressionParser::ParseBinaryExpression(uint8_t min_priority) {
    auto left_expr = ParseUnaryExpression();

    while (true) {
        auto operation = ParseToken();
        auto priority = GetPriority(operation);

        if (priority <= min_priority) {
            if (!operation.empty()) {
                data.push_front(operation);
            }
            return left_expr;
        }

        auto right_expr = ParseBinaryExpression(priority);
        left_expr = Expression(operation, left_expr, right_expr);
    }
}

uint8_t ExpressionParser::GetPriority(const std::string& token) {
    if (token == "NOT") {
        return 1;
    } else if (token == "OR") {
        return 2;
    } else if (token == "AND") {
        return 3;
    } else if (token == ">") {
        return 4;
    } else if (token == "<") {
        return 4;
    } else if (token == "=") {
        return 4;
    } else if (token == "!=") {
        return 4;
    }else if (token == "+") {
        return 5;
    } else if (token == "-") {
        return 5;
    } else if (token == "*") {
        return 6;
    } else if (token == "/") {
        return 6;
    } else if (token == "mod") {
        return 6;
    } else if (token == "**") {
        return 7;
    }
    return 0;
}
