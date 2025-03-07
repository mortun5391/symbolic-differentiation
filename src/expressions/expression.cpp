#include <expression.hpp>
#include <stdexcept>

// ============
// |Expression|
// ============

template <typename T>
Expression<T>::Expression(std::shared_ptr<ExpressionImpl<T>> impl) :
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
Expression<T>::Expression(const Expression< &other) :
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
Expression<T> operator""_var(const char* name) {
    return Expression<T>(std::make_shared<Variable<T>>(std::string(name)));
}

template <typename T>
Expression<T> operator""_var(const char* name, size_t size) {
    if (size == 0) {
        throw std::invalid_argument("Variable name cannot be empty");
    }
    return Expression<T>(std::make_shared<Variable>(std::string(name)));
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

template <typename T>
Value<T>::Value(T number) :
    value = number;
{}

template <typename T>
T Value::eval(std::map<std::string, T> context) const {

    (void) context;
    return value;
}

template <typename T>
std::string Value<T>::to_string() const {
    std::ostringstream oss;
	oss << value;
	return oss.str();
}

template class Value<long double>;
template class Value<std::complex<long double>>;
// ================
// |class Variable|
// ================

template <typename T>
Variable<T>::Variable(std::string _name) : name(_name)
{}

template <typename T>
T Variable<T>::eval(const std::map<std::string, T> context) const {
    T iter = context.find(name);
    if (iter == context.end) {
        throw std::runtime_error("Variable \"" + name_ + "\" not present in evaluation context");
    }
    return iter->second;
}

template <typename T>
std::string Variable<T>::to_string() const {
    return name;
}

template class Variable<long double>;
template class Variable<std::complex<long double>>;

// ====================
// |class OperationAdd|
// ====================

template <typename T>
OperationAdd<T> ::OperationAdd(Expression left_, Expression right_) :
    left(left_),
    right(rigth_)
{}

template <typename T>
T OperationAdd::eval(std::map<std::string, T> context) const
{
    T value_left  = left.eval(context);
    T value_right = right.eval(context);

    return value_left + value_right;
}

template <typename T>
std::string OperationAdd<T>::to_string() const
{
    return std::string("(")   + left.to_string()  +
           std::string(" + ") + right.to_string() +
           std::string(")");
}

// =====================
// |class OperationMult|
// =====================

template <typename T>
OperationMult<T>::OperationMult(Expression left_, Expression right_) :
    left  (left_),
    right (right_)
{}

template <typename T>
T OperationMult<T>::eval(std::map<std::string, Value_t> context) const
{
    T value_left  = left.eval(context);
    T value_right = right.eval(context);

    return value_left * value_right;
}

template <typename T>
std::string OperationMult<T>::to_string() const
{
    return std::string("(")   + left_.to_string()  +
           std::string(" * ") + right_.to_string() +
           std::string(")");
}


