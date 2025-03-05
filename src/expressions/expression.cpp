#include <expression.hpp>
#include <stdexcept>

// ============
// |Expression|
// ============

template <typename T>
Expression<T>::Expression(std::shared_ptr<ExpressionImpl> impl) :
    impl_(impl)
{}

template <typename T>
Expression<T>::Expression(T number) :
    impl_(std::make_shared<Value<T>>(number))
{}

template <typename T>
Expression<T>::Expression(const std::string &variable) :
    impl_(std::make_shared<Variable<T>>(variable))
{}

template <typename T>
Expression<T>::Expression(const Expression &other) :
    impl_(other.impl_)
{}

template <typename T>
Expression<T> Expression<T>::operator+(const Expression &other) const{
    return Expression<T>(std::make_shared<OperationAdd<T>>(*this, other));
}

template <typename T>
Expression<T> Expression<T>::operator+=(const Expression &other) {
    *this = *this + other;
    return *this;
}

template <typename T>
Expression<T> Expression<T>::operator*(const Expression &other) const {
    return Expression<T>(std::make_shared<OperationMult<T>>(*this, other));
}

template <typename T>
Expression<T> Expression<T>::operator*=(const Expression &other) {
    *this = *this * other;
    return *this;
}

template <typename T>
Expression<T> operator""_val(T number) {
    return Expression<T>(std::make_shared<Value<T>>(number));
}

template <typename T>
Expression operator""_var(const char* name) {
    return Expression(std::make_shared<Variable<T>>(std::string(name)));
}

template <typename T>
Expression operator""_var(const char* name, size_t size) {
    if (size == 0) {
        throw std::invalid_argument("Variable name cannot be empty");
    }
    return Expression(std::make_shared<Variable>(std::string(name)));
}

template <typename T>
T Expression::eval(std::map<std::string, T> context) const {
    return impl_->eval(context);
}

template <typename T>
std::string Expression::to_string() const {
    return impl_->to_string();
}


// =============
// |class Value|
// =============

