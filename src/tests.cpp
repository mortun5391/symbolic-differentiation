#include <gtest/gtest.h>
#include "expressions/expression.hpp" 
#include "parser/lexer.hpp"
#include "parser/parser.hpp"


template<typename T>
void check_tokens(Lexer<T>& lexer, const std::vector<Token>& expected_tokens) {
    for (const auto& expected_token : expected_tokens) {
        Token token = lexer.next_token();
        EXPECT_EQ(token.type, expected_token.type);
        EXPECT_EQ(token.value, expected_token.value);
    }
}

// Тесты для класса Value
TEST(ValueTest, Eval) {
    Value<long double> val(10.5L);
    EXPECT_DOUBLE_EQ(val.eval(), 10.5L);
}

TEST(ValueTest, ToString) {
    Value<long double> val(10.5L);
    EXPECT_EQ(val.to_string(), "10.5");
}

// Тесты для класса Variable
TEST(VariableTest, EvalWithoutContext) {
    Variable<long double> var("x");
    EXPECT_THROW(var.eval(), std::runtime_error);
}

TEST(VariableTest, ToString) {
    Variable<long double> var("x");
    EXPECT_EQ(var.to_string(), "x");
}

// Тесты для класса OperationAdd
TEST(OperationAddTest, Eval) {
    auto left = std::make_shared<Value<long double>>(10.5L);
    auto right = std::make_shared<Value<long double>>(20.3L);
    OperationAdd<long double> add(left, right);
    EXPECT_DOUBLE_EQ(add.eval(), 30.8L);
}

// Тесты для класса OperationMult
TEST(OperationMultTest, Eval) {
    auto left = std::make_shared<Value<long double>>(10.0L);
    auto right = std::make_shared<Value<long double>>(20.0L);
    OperationMult<long double> mult(left, right);
    EXPECT_DOUBLE_EQ(mult.eval(), 200.0L);
}

TEST(OperationSubTest, Eval) {
    auto left = std::make_shared<Value<long double>>(10.5L);
    auto right = std::make_shared<Value<long double>>(5.2L);
    OperationSub<long double> sub(left, right);
    EXPECT_DOUBLE_EQ(sub.eval(), 5.3L); // 10.5 - 5.2 = 5.3
}

TEST(OperationSubTest, ToString) {
    auto left = std::make_shared<Value<long double>>(10.5L);
    auto right = std::make_shared<Value<long double>>(5.2L);
    OperationSub<long double> sub(left, right);
    EXPECT_EQ(sub.to_string(), "(10.5 - 5.2)");
}

TEST(OperationDivTest, Eval) {
    auto left = std::make_shared<Value<long double>>(10.0L);
    auto right = std::make_shared<Value<long double>>(2.0L);
    OperationDiv<long double> div(left, right);
    EXPECT_DOUBLE_EQ(div.eval(), 5.0L); // 10 / 2 = 5
}

TEST(OperationDivTest, EvalDivisionByZero) {
    auto left = std::make_shared<Value<long double>>(10.0L);
    auto right = std::make_shared<Value<long double>>(0.0L);
    OperationDiv<long double> div(left, right);
    EXPECT_THROW(div.eval(), std::runtime_error); // Деление на ноль вызывает исключение
}

TEST(OperationDivTest, ToString) {
    auto left = std::make_shared<Value<long double>>(10.0L);
    auto right = std::make_shared<Value<long double>>(2.0L);
    OperationDiv<long double> div(left, right);
    EXPECT_EQ(div.to_string(), "(10 / 2)");
}

TEST(OperationPowTest, Eval) {
    auto left = std::make_shared<Value<long double>>(2.0L);
    auto right = std::make_shared<Value<long double>>(3.0L);
    OperationPow<long double> pow(left, right);
    EXPECT_DOUBLE_EQ(pow.eval(), 8.0L); // 2^3 = 8
}

TEST(OperationPowTest, ToString) {
    auto left = std::make_shared<Value<long double>>(2.0L);
    auto right = std::make_shared<Value<long double>>(3.0L);
    OperationPow<long double> pow(left, right);
    EXPECT_EQ(pow.to_string(), "(2) ^ (3)");
}

// Тесты для класса SinFunc
TEST(SinFuncTest, Eval) {
    auto arg = std::make_shared<Value<long double>>(0.0L);
    SinFunc<long double> sinFunc(arg);
    EXPECT_DOUBLE_EQ(sinFunc.eval(), 0.0L);
}

TEST(CosFuncTest, Eval) {
    auto arg = std::make_shared<Value<long double>>(0.0L);
    CosFunc<long double> cosFunc(arg);
    EXPECT_DOUBLE_EQ(cosFunc.eval(), 1.0L); // cos(0) = 1
}

TEST(CosFuncTest, ToString) {
    auto arg = std::make_shared<Value<long double>>(0.0L);
    CosFunc<long double> cosFunc(arg);
    EXPECT_EQ(cosFunc.to_string(), "cos(0)");
}

TEST(LnFuncTest, Eval) {
    auto arg = std::make_shared<Value<long double>>(1.0L);
    LnFunc<long double> lnFunc(arg);
    EXPECT_DOUBLE_EQ(lnFunc.eval(), 0.0L); // ln(1) = 0
}

TEST(LnFuncTest, EvalNegative) {
    auto arg = std::make_shared<Value<long double>>(-1.0L);
    LnFunc<long double> lnFunc(arg);
    EXPECT_THROW(lnFunc.eval(), std::runtime_error); // Логарифм отрицательного числа вызывает исключение
}

TEST(LnFuncTest, ToString) {
    auto arg = std::make_shared<Value<long double>>(1.0L);
    LnFunc<long double> lnFunc(arg);
    EXPECT_EQ(lnFunc.to_string(), "ln(1)");
}

TEST(ExpFuncTest, Eval) {
    auto arg = std::make_shared<Value<long double>>(0.0L);
    ExpFunc<long double> expFunc(arg);
    EXPECT_DOUBLE_EQ(expFunc.eval(), 1.0L); // e^0 = 1
}

TEST(ExpFuncTest, ToString) {
    auto arg = std::make_shared<Value<long double>>(1.0L);
    ExpFunc<long double> expFunc(arg);
    EXPECT_EQ(expFunc.to_string(), "exp(1)");
}

TEST(DifferentiationTest, Variable) {
    auto var = std::make_shared<Variable<long double>>("x");
    auto diff = var->diff("x");
    EXPECT_DOUBLE_EQ(diff->eval(), 1.0L); // d/dx(x) = 1
}

TEST(DifferentiationTest, Constant) {
    auto val = std::make_shared<Value<long double>>(10.0L);
    auto diff = val->diff("x");
    EXPECT_DOUBLE_EQ(diff->eval(), 0.0L); // d/dx(10) = 0
}

TEST(DifferentiationTest, SinFunc) {
    auto arg = std::make_shared<Variable<long double>>("x");
    auto sinFunc = std::make_shared<SinFunc<long double>>(arg);
    auto diff = sinFunc->diff("x");
    EXPECT_EQ(diff->to_string(), "(cos(x) * 1)"); // d/dx(sin(x)) = cos(x)
}

// Тест для дифференцирования pow
TEST(DiffOperations, PowDiff) {
    Expression<long double> x("x");
    Expression<long double> y("y");

    // Проверка дифференцирования x^y по x
    auto pow_expr = x ^ y;
    auto diff_expr = pow_expr.diff("x");
    EXPECT_EQ(diff_expr.to_string(), "((x) ^ (y) * ((0 * ln(x)) + ((y * 1) / x)))");

    // Проверка дифференцирования x^y по y
    auto diff_expr_y = pow_expr.diff("y");
    EXPECT_EQ(diff_expr_y.to_string(), "((x) ^ (y) * ((1 * ln(x)) + ((y * 0) / x)))");
}

// Тест для дифференцирования ln
TEST(DiffOperations, LnDiff) {
    Expression<long double> x("x");

    // Проверка дифференцирования ln(x) по x
    auto ln_expr = x.ln();
    auto diff_expr = ln_expr.diff("x");
    EXPECT_EQ(diff_expr.to_string(), "((1 / x) * 1)");
}

// Тест для дифференцирования exp
TEST(DiffOperations, ExpDiff) {
    Expression<long double> x("x");

    // Проверка дифференцирования exp(x) по x
    auto exp_expr = x.exp();
    auto diff_expr = exp_expr.diff("x");
    EXPECT_EQ(diff_expr.to_string(), "(exp(x) * 1)");
}

// Тест для дифференцирования /
TEST(DiffOperations, DivDiff) {
    Expression<long double> x("x");
    Expression<long double> y("y");

    // Проверка дифференцирования x / y по x
    auto div_expr = x / y;
    auto diff_expr = div_expr.diff("x");
    EXPECT_EQ(diff_expr.to_string(), "(((1 * y) - (x * 0)) / (y * y))");
}

TEST(ComplexOperations, ComplexTest) {
    using namespace std::complex_literals;
    Expression<std::complex<long double>> expr(std::complex<long double>(1L,2L));

    // Проверка вычисления значения
    EXPECT_EQ(expr.to_string(), "(1 + 2i)");

    // Проверка дифференцирования
    auto diff_expr = expr.diff("x");
    EXPECT_EQ(diff_expr.to_string(), "0");

    // Проверка функций
    auto sin_expr = expr.sin();
    EXPECT_EQ(sin_expr.to_string(), "sin((1 + 2i))");
}



// Тест для чисел
TEST(LexerTest, HandlesNumbers) {
    Lexer<long double> lexer("42 3.14");
    std::vector<Token> expected_tokens = {
        {RealNumber, "42"},
        {RealNumber, "3.14"},
        {EOL, "EOL"}
    };
    check_tokens(lexer, expected_tokens);
}

// Тест для идентификаторов
TEST(LexerTest, HandlesIdentifiers) {
    Lexer<long double> lexer("x y_z");
    std::vector<Token> expected_tokens = {
        {Identifier, "x"},
        {Identifier, "y_z"},
        {EOL, "EOL"}
    };
    check_tokens(lexer, expected_tokens);
}

// Тест для операторов
TEST(LexerTest, HandlesOperators) {
    Lexer<long double> lexer("+-*/^");
    std::vector<Token> expected_tokens = {
        {Operator, "+"},
        {Operator, "-"},
        {Operator, "*"},
        {Operator, "/"},
        {Operator, "^"},
        {EOL, "EOL"}
    };
    check_tokens(lexer, expected_tokens);
}

// Тест для скобок
TEST(LexerTest, HandlesParentheses) {
    Lexer<long double> lexer("()");
    std::vector<Token> expected_tokens = {
        {LeftParen, "("},
        {RightParen, ")"},
        {EOL, "EOL"}
    };
    check_tokens(lexer, expected_tokens);
}

// Тест для функций
TEST(LexerTest, HandlesFunctions) {
    Lexer<long double> lexer("sin(cos(ln(exp(");
    std::vector<Token> expected_tokens = {
        {Function, "sin("},
        {Function, "cos("},
        {Function, "ln("},
        {Function, "exp("},
        {EOL, "EOL"}
    };
    check_tokens(lexer, expected_tokens);
}

// Тест для мнимой единицы
TEST(LexerTest, HandlesImaginaryUnit) {
    Lexer<std::complex<long double>> lexer("i");
    std::vector<Token> expected_tokens = {
        {ImaginaryUnit, "i"},
        {EOL, "EOL"}
    };
    check_tokens(lexer, expected_tokens);
}

// Тест для неявного умножения
TEST(LexerTest, HandlesImplicitMultiplication) {
    Lexer<long double> lexer("2x(3+4)");
    std::vector<Token> expected_tokens = {
        {RealNumber, "2"},
        {Operator, "*"},
        {Identifier, "x"},
        {Operator, "*"},
        {LeftParen, "("},
        {RealNumber, "3"},
        {Operator, "+"},
        {RealNumber, "4"},
        {RightParen, ")"},
        {EOL, "EOL"}
    };
    check_tokens(lexer, expected_tokens);
}

// Тест для пробелов и конца строки
TEST(LexerTest, HandlesSpacesAndEOL) {
    Lexer<long double> lexer(" 1 + 2 ");
    std::vector<Token> expected_tokens = {
        {RealNumber, "1"},
        {Operator, "+"},
        {RealNumber, "2"},
        {EOL, "EOL"}
    };
    check_tokens(lexer, expected_tokens);
}

// Тест для неожиданных символов
TEST(LexerTest, ThrowsOnUnexpectedSymbol) {
    Lexer<long double> lexer("1 + @");
    EXPECT_THROW({
        while (lexer.next_token().type != EOL) {}
    }, std::runtime_error);
}



// Тесты для разбора простых выражений
TEST(ParserTest, ParseSimpleExpression) {
    Parser<long double> parser("2 + 3");
    auto expr = parser.parse();
    EXPECT_NEAR(expr.eval(), 5.0, 1e-9); // Проверяем, что 2 + 3 = 5
}

TEST(ParserTest, ParseComplexExpression) {
    Parser<long double> parser("2 + 3 * 4");
    auto expr = parser.parse();
    EXPECT_NEAR(expr.eval(), 14.0, 1e-9); // Проверяем, что 2 + 3 * 4 = 14
}

TEST(ParserTest, ParseParentheses) {
    Parser<long double> parser("(2 + 3) * 4");
    auto expr = parser.parse();
    EXPECT_NEAR(expr.eval(), 20.0, 1e-9); // Проверяем, что (2 + 3) * 4 = 20
}

// Тесты для функций
TEST(ParserTest, ParseSinFunction) {
    Parser<long double> parser("sin(0)");
    auto expr = parser.parse();
    EXPECT_NEAR(expr.eval(), 0.0, 1e-9); // Проверяем, что sin(0) = 0
}

TEST(ParserTest, ParseCosFunction) {
    Parser<long double> parser("cos(0)");
    auto expr = parser.parse();
    EXPECT_NEAR(expr.eval(), 1.0, 1e-9); // Проверяем, что cos(0) = 1
}

// Тесты для ошибок
TEST(ParserTest, InvalidExpression) {
    Parser<long double> parser("2 + ");
    EXPECT_THROW(parser.parse(), std::runtime_error); // Ожидаем ошибку
}



// Тесты для комплексных чисел
TEST(ParserTest, ParseComplexNumber) {
    Parser<std::complex<long double>> parser("(2 + 3i)");
    auto expr = parser.parse();
    auto result = expr.eval();
    EXPECT_NEAR(result.real(), 2.0, 1e-9); // Проверяем вещественную часть
    EXPECT_NEAR(result.imag(), 3.0, 1e-9); // Проверяем мнимую часть
}

TEST(ParserTest, ParseComplexExpressionWithOperations) {
    Parser<std::complex<long double>> parser("(2 + 3i) * (4 - 5i)");
    auto expr = parser.parse();
    auto result = expr.eval();
    EXPECT_NEAR(result.real(), 23.0, 1e-9); // Проверяем вещественную часть
    EXPECT_NEAR(result.imag(), 2.0, 1e-9);  // Проверяем мнимую часть
}

// Тесты для переменных
TEST(ParserTest, ParseVariable) {
    Parser<long double> parser("x");
    auto expr = parser.parse();
    EXPECT_THROW(expr.eval(), std::runtime_error); // Переменная без значения вызовет ошибку
}

// Тесты для приоритетов операций
TEST(ParserTest, OperatorPrecedence) {
    Parser<long double> parser("2 + 3 * 4 ^ 2");
    auto expr = parser.parse();
    EXPECT_NEAR(expr.eval(), 50.0, 1e-9); // Проверяем, что 2 + 3 * 4^2 = 50
}

// Тесты для мнимой единицы
TEST(ParserTest, ParseImaginaryUnit) {
    Parser<std::complex<long double>> parser("i");
    auto expr = parser.parse();
    auto result = expr.eval();
    EXPECT_NEAR(result.real(), 0.0, 1e-9); // Проверяем вещественную часть
    EXPECT_NEAR(result.imag(), 1.0, 1e-9); // Проверяем мнимую часть
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

