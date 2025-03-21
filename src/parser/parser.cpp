#include "parser.hpp"
#include "lexer.hpp"
#include "../expressions/expression.hpp"

#include <string>
#include <stdexcept>
#include <format>



template<typename T>
Token Parser<T>::consume(TokenType expected_type) {
    if (cur_token.type != expected_type) {
        throw std::runtime_error(std::format("Unexpected token: \"{}\"", cur_token.value));
    }
    return advance();
}

template<typename T>
Token Parser<T>::advance() {
    cur_token = lexer.next_token();
    return cur_token;
}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> Parser<T>::create_function(
	const std::string &func_name, std::shared_ptr<ExpressionImpl<T>> argument
) {
	if (func_name == "sin") return std::make_shared<SinFunc<T>>(argument);
	if (func_name == "cos") return std::make_shared<CosFunc<T>>(argument);
	if (func_name == "ln") return std::make_shared<LnFunc<T>>(argument);
	if (func_name == "exp") return std::make_shared<ExpFunc<T>>(argument);
	throw std::runtime_error("Unknown function: " + func_name);
}

template <typename T>
OpPrecedence Parser<T>::get_precedence_by_name(std::string& name) {
    if (name == "+" || name == "-") {
        return OpPrecedence::AddSub;
    }
    if (name == "*") {
        return OpPrecedence::Mult;
    }
    if (name == "/") {
        return OpPrecedence::Div;
    }
    if (name == "^") {
        return OpPrecedence::Pow;
    }
    throw std::runtime_error(std::format("Unknown binary operator: \"{}\"", name));
}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> Parser<T>::get_op_by_name(std::string& name, 
    std::shared_ptr<ExpressionImpl<T>> left, std::shared_ptr<ExpressionImpl<T>> right) {
        if (name == "+") {
            return std::make_shared<OperationAdd<T>>(left, right);
        }
        if (name == "-") {
            return std::make_shared<OperationSub<T>>(left, right);
        }
        if (name == "*") {
            return std::make_shared<OperationMult<T>>(left, right);
        }
        if (name == "/") {
            return std::make_shared<OperationDiv<T>>(left, right);
        }
        if (name == "^") {
            return std::make_shared<OperationPow<T>>(left, right);
        }
        throw std::runtime_error(std::format("Unknown binary operator: \"{}\"", name));
    }

template<>
std::shared_ptr<ExpressionImpl<long double>> Parser<long double>::parse_real_number() {
    auto value = std::stold(cur_token.value); // Преобразуем строку в long double
    advance();
    return std::make_shared<Value<long double>>(value);
}

template<>
std::shared_ptr<ExpressionImpl<std::complex<long double>>> Parser<std::complex<long double>>::parse_real_number() {
    std::complex<long double> value(std::stold(cur_token.value), 0); // Вещественная часть, мнимая = 0
    advance();
    return std::make_shared<Value<std::complex<long double>>>(value);
}

template<>
std::shared_ptr<ExpressionImpl<long double>> Parser<long double>::parse_imaginary_unit() {
    throw std::runtime_error("Imaginary unit is not supported for RealNumber");
}

template<>
std::shared_ptr<ExpressionImpl<std::complex<long double>>> Parser<std::complex<long double>>::parse_imaginary_unit() {
    std::complex<long double> value(0, 1); // Мнимая единица
    advance();
    return std::make_shared<Value<std::complex<long double>>>(value);
}

template<typename T>
std::shared_ptr<ExpressionImpl<T>> Parser<T>::parse_identifier() {
    std::string name = cur_token.value;
    advance();
    return std::make_shared<Variable<T>>(name);
}

template<typename T>
std::shared_ptr<ExpressionImpl<T>> Parser<T>::parse_parentheses_expr() {
    consume(LeftParen);
    auto expr = parse_expression();
    consume(RightParen);
    return expr;
}

template<typename T>
std::shared_ptr<ExpressionImpl<T>> Parser<T>::parse_function() {
    std::string func_name = cur_token.value.substr(0, cur_token.value.length() - 1); // Убираем '('
    consume(Function);
    auto expr = parse_expression();
    consume(RightParen);
    return create_function(func_name, expr);
}


template<typename T>
std::shared_ptr<ExpressionImpl<T>> Parser<T>::parse_primary() {
    switch (cur_token.type) {
        case LeftParen:
            return parse_parentheses_expr();
        case Function:
            return parse_function();
        case RealNumber:
            return parse_real_number();
        case ImaginaryUnit:
            return parse_imaginary_unit();
        case Identifier:
            return parse_identifier();
        default:
            throw std::runtime_error(std::format("Unexpected token: \"{}\"", cur_token.value));
    }
}

template<typename T>
std::shared_ptr<ExpressionImpl<T>> Parser<T>::parse_op_right(OpPrecedence expr_precedence, std::shared_ptr<ExpressionImpl<T>> left) {
    while (cur_token.type != EOL && cur_token.type != RightParen) {
        if (cur_token.type != Operator) {
            throw std::runtime_error(std::format("Expected binary operator, got: \"{}\"", cur_token.value));
        }

        std::string op = cur_token.value;
        OpPrecedence op_precedence = get_precedence_by_name(op);

        if (op_precedence < expr_precedence) {
            break;
        }

        advance();
        auto right = parse_primary();

        // Обработка вложенных операторов с более высоким приоритетом
        if (cur_token.type == Operator) {
            std::string next_op = cur_token.value;
            OpPrecedence next_op_precedence = get_precedence_by_name(next_op);

            if (op_precedence < next_op_precedence) {
                right = parse_op_right(OpPrecedence(static_cast<int>(op_precedence) + 1), right);
            }
        }

        left = get_op_by_name(op, left, right);
    }
    return left;
}

template<typename T>
std::shared_ptr<ExpressionImpl<T>> Parser<T>::parse_expression() {
    auto left = parse_primary();
    return parse_op_right(OpPrecedence::AddSub, left);
}

template<typename T>
Parser<T>::Parser(const std::string& expression_str, bool ignore_case)
    : lexer(expression_str, ignore_case), cur_token(lexer.next_token()) {}

template<typename T>
Expression<T> Parser<T>::parse() {
    auto expr = parse_expression();
    if (cur_token.type != EOL) {
        throw std::runtime_error("Unexpected token at the end of expression");
    }
    return Expression<T>(expr);
}

template class Parser<long double>;
template class Parser<std::complex<long double>>;