#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "../expressions/expression.hpp"

template<typename T = long double>
class Parser {
public:
    explicit Parser(const std::string& expression_str, bool ignore_case = false);
    Expression<T> parse();

private:
    Lexer<T> lexer;
    Token cur_token;

    Token advance();
    Token consume(TokenType expected_type);
    std::shared_ptr<ExpressionImpl<T>> create_function(
		const std::string &func_name,
		std::shared_ptr<ExpressionImpl<T>> argument
	);

    OpPrecedence get_precedence_by_name(std::string& name);

    std::shared_ptr<ExpressionImpl<T>> get_op_by_name(std::string& name, 
        std::shared_ptr<ExpressionImpl<T>> left, std::shared_ptr<ExpressionImpl<T>> right);


    std::shared_ptr<ExpressionImpl<T>> parse_real_number();
    std::shared_ptr<ExpressionImpl<T>> parse_imaginary_unit();
    std::shared_ptr<ExpressionImpl<T>> parse_identifier();
    std::shared_ptr<ExpressionImpl<T>> parse_parentheses_expr();
    std::shared_ptr<ExpressionImpl<T>> parse_function();
    std::shared_ptr<ExpressionImpl<T>> parse_primary();
    std::shared_ptr<ExpressionImpl<T>> parse_op_right(OpPrecedence expr_precedence, std::shared_ptr<ExpressionImpl<T>> lhs);
    std::shared_ptr<ExpressionImpl<T>> parse_expression();
};

#endif 