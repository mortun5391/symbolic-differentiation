#include <gtest/gtest.h>
#include "expressions/expression.hpp" 

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
    EXPECT_EQ(expFunc.to_string(), "e^(1)");
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



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}