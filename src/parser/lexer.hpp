#ifndef LEXER_HPP
#define LEXER_HPP

#include <complex>
#include <string>
#include <regex>
#include <optional>
#include <unordered_map>

enum TokenType {
    RealNumber,
    ImaginaryUnit,
    Identifier, // Variable
    Operator,
    LeftParen,
    RightParen,
    Function,
    EOL
};

const std::unordered_map<TokenType, std::regex> PATTERN_MAP = {
    {RealNumber, std::regex("(0|[1-9][0-9]*)(\\.[0-9]+)?")},
    {Identifier, std::regex("[a-zA-Z_]+")},
    {Function, std::regex("(sin|cos|ln|exp)\\(", std::regex_constants::icase)}
};

struct Token {
    TokenType type;
    std::string value;
};

template <typename T>
class Lexer {
    private:
        std::string input;
        std::string::const_iterator it;
        std::optional<Token> discarded_token = std::nullopt;
        TokenType prev_token_type = EOL;

        char peek(void) const;
        void advance(void);

        Token get_token_from_input(void);
        std::optional<std::string> get_str_by_pattern(const std::regex& regexp);

    public:
        Lexer(const std::string& str, bool ignore_case = false);
        Token next_token(void);
};

#endif 