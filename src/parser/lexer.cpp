#include "lexer.hpp"
#include "../expressions/expression.hpp"
#include <vector>
#include <utility>
#include <ranges>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

template<typename T>
Lexer<T>::Lexer(const std::string& input_str, bool ignore_case) {
    input = input_str;
    if (!ignore_case) {
        std::ranges::transform(input, input.begin(), ::tolower);
    }
    it = input.begin();
}

template<typename T>
char Lexer<T>::peek() const {
    return it == input.end() ? '\0' : *it;
}

template<typename T>
void Lexer<T>::advance() {
    if (it != input.end()) {
        ++it;
    }
}

template<typename T>
std::optional<std::string> Lexer<T>::get_str_by_pattern(const std::regex& regexp) {
    std::smatch match_result;
    if (std::regex_search(it, input.cend(), match_result, regexp, std::regex_constants::match_continuous)) {
        it = match_result[0].second;
        return match_result[0].str();
    }
    return std::nullopt;
}

template<typename T>
Token Lexer<T>::get_token_from_input() {
    
    while (std::isspace(peek())) {
        advance();
    }

    if (peek() == '\0') {
        return Token(EOL, "EOL");
    }

    for (const auto& [type, regexp] : PATTERN_MAP) {
        if (auto str = get_str_by_pattern(regexp); str.has_value()) {
            if constexpr (std::is_same_v<T, std::complex<long double>>) {
                if (type == Identifier && str.value() == "i") {
                    return Token(ImaginaryUnit, "i");
                }
            }
            return Token(type, str.value());
        }
    }

    const char current_char = peek();
    advance();
    switch (current_char) {
        case '+': case '-': case '*': case '/': case '^':
            return Token(Operator, std::string(1, current_char));
        case '(':
            return Token(LeftParen, "(");
        case ')':
            return Token(RightParen, ")");
        default:
            throw std::runtime_error("Unexpected symbol: " + std::string(1, current_char));
    }
}

template<typename T>
Token Lexer<T>::next_token() {

    if (discarded_token.has_value()) {
        Token token = discarded_token.value();
        discarded_token.reset();
        return token;
    }

    
    Token token = get_token_from_input();
    if (token.type == EOL) {
        return token;
    }

    static const std::vector<std::pair<TokenType, TokenType>> implicit_multiplication_pairs = {
        {RealNumber, Identifier},
        {RealNumber, Function},
        {RealNumber, LeftParen},
        {RealNumber, ImaginaryUnit},
        {Identifier, LeftParen},
        {ImaginaryUnit, LeftParen},
        {RightParen, LeftParen},
        {RightParen, RealNumber},
        {RightParen, ImaginaryUnit},
        {RightParen, Identifier},
        {RightParen, Function}
    };

    if (std::ranges::find(implicit_multiplication_pairs, std::pair{prev_token_type, token.type}) != implicit_multiplication_pairs.end()) {
        discarded_token = token;
        prev_token_type = token.type;
        return Token(Operator, "*");
    }

    prev_token_type = token.type;
    return token;
}

template class Lexer<long double>;
template class Lexer<std::complex<long double>>;