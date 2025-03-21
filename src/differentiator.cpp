#include "expressions/expression.hpp"

#include <iostream>
#include <regex>
#include <stdexcept>
#include <unordered_map>
#include <complex>

using VariableType = std::unordered_map<std::string, long double>;
using ComplexVariableType = std::unordered_map<std::string, std::complex<long double>>;

template <typename T, typename VarMap>
std::string run_task(
    Expression<T> expr, bool to_diff, bool to_eval,
    const std::string &diff_by, VarMap &values
) {
    std::stringstream oss;

    if (to_diff) {
        Expression<T> diff_expr = expr.diff(diff_by);
        oss << "Differentiated: " << diff_expr.to_string() << "\n";

        if (to_eval) {
            oss << "Evaluated derivative: " << diff_expr.eval_with(values) << "\n";
        }
    }

    if (to_eval && !to_diff) {
        oss << "Evaluated: " << expr.eval_with(values) << "\n";
    }

    return oss.str();
}

int main(int argc, char* argv[]) {
    std::string expression_string, diff_by;
    bool eval_expr = false, diff_expr = false, use_complex = false;
    VariableType variables;
    ComplexVariableType complex_variables;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--eval" || arg == "--diff") {
            if (++i >= argc)
                throw std::invalid_argument("No value specified for " + arg);
            expression_string = argv[i];
            diff_expr |= (arg == "--diff");
            eval_expr |= (arg == "--eval");
        } else if (arg == "--by") {
            if (++i >= argc)
                throw std::invalid_argument("No value specified for --by");
            diff_by = argv[i];
        } else if (arg == "--complex") {
            use_complex = true;
        } else if (arg.find("=") != std::string::npos) {
            auto pos = arg.find("=");
            std::string var_name = arg.substr(0, pos),
                        val_str = arg.substr(pos + 1);

            if (use_complex) {
                std::regex complex_regex(R"(([+-]?\d*\.?\d*)([+-]?\d*\.?\d*)i)");
                std::smatch match;
                if (std::regex_match(val_str, match, complex_regex)) {
                    long double real = match[1].str().empty() ? 0 : std::stold(match[1].str());
                    long double imag = match[2].str().empty() ? 0 : std::stold(match[2].str());
                    complex_variables[var_name] = std::complex<long double>(real, imag);
                } else {
                    throw std::invalid_argument("Invalid complex number format: " + val_str);
                }
            } else {
                
                variables[var_name] = std::stold(val_str);
            }
        } else {
            throw std::invalid_argument("Unknown argument: " + arg);
        }
    }

    if (use_complex) {
        auto expression = Expression<std::complex<long double>>::from_string(expression_string, true);
        std::cout << run_task(
            expression, diff_expr, eval_expr, diff_by, complex_variables
        ) << "\n";
    } else {
        auto expression = Expression<long double>::from_string(expression_string, true);
        std::cout << run_task(
            expression, diff_expr, eval_expr, diff_by, variables
        ) << "\n";
    }

    return 0;
}