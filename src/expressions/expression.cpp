#include <expression.hpp>
#include "../parser/parser.hpp"
#include <stdexcept>
#include <complex>

// ============
// |Expression|
// ============

template <typename T>
Expression<T>::Expression(std::shared_ptr<ExpressionImpl<T>> impl_) :
    impl(impl_)
{}

template <typename T>
Expression<T>::Expression(T number) :
    impl(std::make_shared<Value<T>>(number))
{}

template <typename T>
Expression<T>::Expression(const std::string &variable) :
    impl(std::make_shared<Variable<T>>(variable))
{}

template <typename T>
Expression<T>::Expression(const Expression<T> &other) :
    impl(other.impl)
{}

template <typename T>
Expression<T> &Expression<T>::operator=(Expression &&other) 
{
	if (this != &other) {
		impl = std::move(other.impl);
	}
	return *this;
}

template <typename T>
Expression<T> &Expression<T>::operator=(const Expression &other) 
{
	if (this != &other) {
		impl = other.impl;
	}
	return *this;
}


template <typename T>
Expression<T> Expression<T>::operator+(const Expression &other) const
{
    return Expression<T>(std::make_shared<OperationAdd<T>>(impl, other.impl));
}

template <typename T>
Expression<T> &Expression<T>::operator+=(const Expression &other) 
{
    *this = *this + other;
    return *this;
}

template <typename T>
Expression<T> Expression<T>::operator-(const Expression &other) const
{
    return Expression<T>(std::make_shared<OperationSub<T>>(impl, other.impl));
}

template <typename T>
Expression<T> &Expression<T>::operator-=(const Expression &other) 
{
    *this = *this - other;
    return *this;
}

template <typename T>
Expression<T> Expression<T>::operator*(const Expression &other) const 
{
    return Expression<T>(std::make_shared<OperationMult<T>>(impl, other.impl));
}

template <typename T>
Expression<T> &Expression<T>::operator*=(const Expression &other) 
{
    *this = *this * other;
    return *this;
}

template <typename T>
Expression<T> Expression<T>::operator/(const Expression &other) const 
{
    return Expression<T>(std::make_shared<OperationDiv<T>>(impl, other.impl));
}

template <typename T>
Expression<T> &Expression<T>::operator/=(const Expression &other) 
{
    *this = *this / other;
    return *this;
}

template <typename T>
Expression<T> Expression<T>::operator^(const Expression &other) const
{
    return Expression<T>(std::make_shared<OperationPow<T>>(impl, other.impl));
}

template <typename T>
Expression<T> &Expression<T>::operator^=(const Expression &other) 
{
    *this = *this ^ other;
    return *this;
}

template <typename T> Expression<T> Expression<T>::sin(void) const 
{
	return Expression<T>(std::make_shared<SinFunc<T>>(impl));
};

template <typename T> Expression<T> Expression<T>::cos(void) const 
{
	return Expression<T>(std::make_shared<CosFunc<T>>(impl));
};

template <typename T> Expression<T> Expression<T>::ln(void) const {
	return Expression<T>(std::make_shared<LnFunc<T>>(impl));
};

template <typename T> Expression<T> Expression<T>::exp(void) const 
{
	return Expression<T>(std::make_shared<ExpFunc<T>>(impl));
};

template <typename T>
Expression<T> Expression<T>::diff(const std::string &by) const 
{
	return Expression<T>(impl->diff(by));
}

template <typename T>
Expression<T> Expression<T>::with_context(const std::unordered_map<std::string, T> &context) const 
{
	return Expression<T>(impl->with_context(context));
}

template <typename T>
T Expression<T>::eval(void) const 
{
    return impl->eval();
}

template <typename T>
T Expression<T>::eval_with(const std::unordered_map<std::string, T> &context) const 
{
	return impl->with_context(context)->eval();
}

template <typename T>
std::string Expression<T>::to_string() const 
{
    return impl->to_string();
}

template<typename T>
Expression<T> Expression<T>::from_string(const std::string& expression_str, bool ignore_case) {
    return Parser<T>(expression_str, ignore_case).parse();
}

template class Expression<long double>;
template class Expression<std::complex<long double>>;

// =============
// |class Value|
// =============

template <typename T>
Value<T>::Value(T number) :
    value(number)
{}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> Value<T>::diff(const std::string &by) const 
{
	return std::make_shared<Value<T>>(Value<T>(0));
}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> Value<T>::with_context( const std::unordered_map<std::string, T> &context) const 
{
	return std::make_shared<Value<T>>(Value<T>(value));
};

template <typename T>
T Value<T>::eval() const 
{
    return value;
}

template <typename T>
std::string Value<T>::to_string(void) const 
{
    std::ostringstream oss;
	oss << value;
	return oss.str();
}

template <>
std::string Value<std::complex<long double>>::to_string(void) const 
{
	std::ostringstream oss;
	bool with_both_parts = false;
	if (value.imag() == 0 && value.real() == 0)
		return "0";
	else if (value.imag() != 0 && value.real() != 0) {
		with_both_parts = true;
		oss << "(";
	}
	if (value.real() != 0) oss << value.real();

	if (with_both_parts && value.imag() >= 0)
		oss << " + ";
	else if (with_both_parts)
		oss << " - ";

	if (value.imag() != 0) {
		if (std::abs(value.imag()) != 1) oss << std::abs(value.imag());
		oss << "i";
	}

	if (with_both_parts) oss << ")";
	return oss.str();
}

template class Value<long double>;
template class Value<std::complex<long double>>;
// ================
// |class Variable|
// ================

template <typename T>
Variable<T>::Variable(std::string var) : name(var)
{}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> Variable<T>::diff(const std::string &by) const 
{
	if (by == name) {
		return std::make_shared<Value<T>>(Value<T>(1));
	}
	return std::make_shared<Value<T>>(Value<T>(0));
}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> Variable<T>::with_context(const std::unordered_map<std::string, T> &context) const 
{
	if (context.find(name) == context.end())
		return std::make_shared<Variable<T>>(Variable<T>(name));
	return std::make_shared<Value<T>>(Value<T>(context.at(name)));
};

template <typename T>
T Variable<T>::eval() const 
{
    throw std::runtime_error("Varriable " + name +  " cannot be resolved without context");
}

template <typename T>
std::string Variable<T>::to_string() const 
{
    return name;
}

template class Variable<long double>;
template class Variable<std::complex<long double>>;

// ====================
// |class OperationAdd|
// ====================

template <typename T>
OperationAdd<T>::OperationAdd(
	const std::shared_ptr<ExpressionImpl<T>> &left_,
	const std::shared_ptr<ExpressionImpl<T>> &right_
):
    left(left_),
    right(right_)
{}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> OperationAdd<T>::diff(const std::string &by) const 
{
	return std::make_shared<OperationAdd<T>>(left->diff(by), right->diff(by));
}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> OperationAdd<T>::with_context(const std::unordered_map<std::string, T> &context) const 
{
	return std::make_shared<OperationAdd<T>>(left->with_context(context), right->with_context(context));
}

template <typename T>
T OperationAdd<T>::eval() const
{
    return left->eval() + right->eval();
}

template <typename T>
std::string OperationAdd<T>::to_string() const
{
    return std::string("(")   + left->to_string()  +
           std::string(" + ") + right->to_string() +
           std::string(")");
}

template class OperationAdd<long double>;
template class OperationAdd<std::complex<long double>>;
// =====================
// |class OperationMult|
// =====================


template <typename T>
OperationMult<T>::OperationMult(
	const std::shared_ptr<ExpressionImpl<T>> &left_,
	const std::shared_ptr<ExpressionImpl<T>> &right_
):
    left(left_),
    right(right_)
{}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> OperationMult<T>::diff(const std::string &by) const 
{
	return std::make_shared<OperationAdd<T>>(
		std::make_shared<OperationMult<T>>(left->diff(by), right),
		std::make_shared<OperationMult<T>>(left, right->diff(by))
	);
}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> OperationMult<T>::with_context(const std::unordered_map<std::string, T> &context) const
{
	return std::make_shared<OperationMult<T>>(
		left->with_context(context), right->with_context(context)
	);
};

template <typename T>
T OperationMult<T>::eval() const
{
    return left->eval() * right->eval();
}

template <typename T>
std::string OperationMult<T>::to_string() const
{
    return std::string("(")   + left->to_string()  +
           std::string(" * ") + right->to_string() +
           std::string(")");
}

template class OperationMult<long double>;
template class OperationMult<std::complex<long double>>;


// =====================
// |class OperationSub|
// =====================

template <typename T>
OperationSub<T>::OperationSub(
	const std::shared_ptr<ExpressionImpl<T>> &left_,
	const std::shared_ptr<ExpressionImpl<T>> &right_
):
    left(left_),
    right(right_)
{}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> OperationSub<T>::diff(const std::string &by) const 
{
	return std::make_shared<OperationSub<T>>(left->diff(by), right->diff(by));
}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> OperationSub<T>::with_context(const std::unordered_map<std::string, T> &context) const 
{
	return std::make_shared<OperationSub<T>>(left->with_context(context), right->with_context(context));
}

template <typename T>
T OperationSub<T>::eval() const
{
    return left->eval() - right->eval();
}

template <typename T>
std::string OperationSub<T>::to_string() const
{
    return std::string("(")   + left->to_string()  +
           std::string(" - ") + right->to_string() +
           std::string(")");
}

template class OperationSub<long double>;
template class OperationSub<std::complex<long double>>;
// =====================
// |class OperationDiv|
// =====================


template <typename T>
OperationDiv<T>::OperationDiv(
	const std::shared_ptr<ExpressionImpl<T>> &left_,
	const std::shared_ptr<ExpressionImpl<T>> &right_
):
    left(left_),
    right(right_)
{}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> OperationDiv<T>::diff(const std::string &by) const 
{
	return std::make_shared<OperationDiv<T>>(
		std::make_shared<OperationSub<T>>(
            std::make_shared<OperationMult<T>>(left->diff(by), right), std::make_shared<OperationMult<T>>(left, right->diff(by))),
		std::make_shared<OperationMult<T>>(right, right)
	);
}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> OperationDiv<T>::with_context(const std::unordered_map<std::string, T> &context) const
{
	return std::make_shared<OperationDiv<T>>(
		left->with_context(context), right->with_context(context)
	);
};

template <typename T>
T OperationDiv<T>::eval() const
{
    T r_value = right->eval();
    if (r_value == 0.L){
        throw std::runtime_error("Division by zero -> OperationDiv::eval");
    }
    return left->eval() / r_value;
}

template <typename T>
std::string OperationDiv<T>::to_string() const
{
    return std::string("(")   + left->to_string()  +
           std::string(" / ") + right->to_string() +
           std::string(")");
}

template class OperationDiv<long double>;
template class OperationDiv<std::complex<long double>>;



// =====================
// |class OperationPow|
// =====================


template <typename T>
OperationPow<T>::OperationPow(
	const std::shared_ptr<ExpressionImpl<T>> &left_,
	const std::shared_ptr<ExpressionImpl<T>> &right_
):
    left(left_),
    right(right_)
{}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> OperationPow<T>::diff(const std::string &by) const 
{
	// left^right * (right' * ln(left) + (right * left') / left)

    auto exp1 = std::make_shared<OperationMult<T>>(right->diff(by), std::make_shared<LnFunc<T>>(left));

    auto exp2 = std::make_shared<OperationDiv<T>>(std::make_shared<OperationMult<T>>(right, left->diff(by)), left);

    return std::make_shared<OperationMult<T>>(std::make_shared<OperationPow<T>>(left,right), 
                                              std::make_shared<OperationAdd<T>>(exp1, exp2)
    );
}

template <typename T>
std::shared_ptr<ExpressionImpl<T>> OperationPow<T>::with_context(const std::unordered_map<std::string, T> &context) const
{
	return std::make_shared<OperationPow<T>>(
		left->with_context(context), right->with_context(context)
	);
};

template <typename T>
T OperationPow<T>::eval() const
{   
    return std::pow(left->eval(), right->eval());
}

template <typename T>
std::string OperationPow<T>::to_string() const
{
    return std::string("(")   + left->to_string()  +
           std::string(") ^ (") + right->to_string() +
           std::string(")");
}

template class OperationPow<long double>;
template class OperationPow<std::complex<long double>>;

// =====================
//   |class SinFunc|
// =====================

template <typename T>
SinFunc<T>::SinFunc(std::shared_ptr<ExpressionImpl<T>> argument_) :
    argument(argument_)
{};

template <typename T>
std::shared_ptr<ExpressionImpl<T>> SinFunc<T>::diff(const std::string &by) const 
{
	return std::make_shared<OperationMult<T>>(
        std::make_shared<CosFunc<T>>(argument), argument->diff(by));
};

template <typename T>
std::shared_ptr<ExpressionImpl<T>> SinFunc<T>::with_context(const std::unordered_map<std::string, T> &context) const 
{
	return std::make_shared<SinFunc<T>>(argument->with_context(context));
};

template <typename T> T SinFunc<T>::eval() const {
	return std::sin(argument->eval());
};

template <typename T> std::string SinFunc<T>::to_string(void) const 
{
	return "sin(" + argument->to_string() + ")";
};

template class SinFunc<long double>;
template class SinFunc<std::complex<long double>>;


// =====================
//   |class CosFunc|
// =====================

template <typename T>
CosFunc<T>::CosFunc(std::shared_ptr<ExpressionImpl<T>> argument_) :
    argument(argument_)
{};

template <typename T>
std::shared_ptr<ExpressionImpl<T>> CosFunc<T>::diff(const std::string &by) const 
{
	return std::make_shared<OperationMult<T>>(
        std::make_shared<OperationMult<T>>(
            std::make_shared<SinFunc<T>>(argument), std::make_shared<Value<T>>(-1.0L)),
      argument->diff(by));
};

template <typename T>
std::shared_ptr<ExpressionImpl<T>> CosFunc<T>::with_context(const std::unordered_map<std::string, T> &context) const
{
	return std::make_shared<CosFunc<T>>(argument->with_context(context));
};

template <typename T> T CosFunc<T>::eval() const 
{
	return std::cos(argument->eval());
};

template <typename T> std::string CosFunc<T>::to_string(void) const 
{
	return "cos(" + argument->to_string() + ")";
};

template class CosFunc<long double>;
template class CosFunc<std::complex<long double>>;


// =====================
//   |class LnFunc|
// =====================

template <typename T>
LnFunc<T>::LnFunc(std::shared_ptr<ExpressionImpl<T>> argument_) :
    argument(argument_)
{};

template <typename T>
std::shared_ptr<ExpressionImpl<T>> LnFunc<T>::diff(const std::string &by) const 
{
	return std::make_shared<OperationMult<T>>(
        std::make_shared<OperationDiv<T>>(std::make_shared<Value<T>>(1.0L), argument), 
        argument->diff(by));
};

template <typename T>
std::shared_ptr<ExpressionImpl<T>> LnFunc<T>::with_context(const std::unordered_map<std::string, T> &context) const 
{
	return std::make_shared<LnFunc<T>>(argument->with_context(context));
};

template <typename T> T LnFunc<T>::eval() const
 {
    T arg_val = argument->eval();
	if (arg_val <= 0.0L)
		throw std::runtime_error("Argument cannot be negative in LnFunc::eval");
	return std::log(arg_val);
};

template <>
std::complex<long double> LnFunc<std::complex<long double>>::eval() const 
{
	throw std::runtime_error(
		"Logarithm of complex numbers is not supported in this implementation"
	);
}

template <typename T> std::string LnFunc<T>::to_string(void) const 
{
	return "ln(" + argument->to_string() + ")";
};

template class LnFunc<long double>;
template class LnFunc<std::complex<long double>>;


// =====================
//   |class ExpFunc|
// =====================

template <typename T>
ExpFunc<T>::ExpFunc(std::shared_ptr<ExpressionImpl<T>> argument_) :
    argument(argument_)
{};

template <typename T>
std::shared_ptr<ExpressionImpl<T>> ExpFunc<T>::diff(const std::string &by) const 
{
	return std::make_shared<OperationMult<T>>(std::make_shared<ExpFunc<T>>(argument), argument->diff(by));
};


template <typename T>
std::shared_ptr<ExpressionImpl<T>> ExpFunc<T>::with_context(const std::unordered_map<std::string, T> &context) const 
{
	return std::make_shared<ExpFunc<T>>(argument->with_context(context));
};

template <typename T> T ExpFunc<T>::eval() const 
{
    return std::exp(argument->eval());

};

template <typename T> std::string ExpFunc<T>::to_string(void) const 
{
	return "exp(" + argument->to_string() + ")";
};

template class ExpFunc<long double>;
template class ExpFunc<std::complex<long double>>;