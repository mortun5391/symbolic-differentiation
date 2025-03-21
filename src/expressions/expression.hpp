#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP



#include <complex>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

enum class OpPrecedence {
    AddSub = 0,
    Mult = 1,
    Div = 2,
    Pow = 3
};
template <typename T> class Parser;

template <typename T> class ExpressionImpl {
  public:
	virtual ~ExpressionImpl() = default;

	virtual std::shared_ptr<ExpressionImpl<T>> diff(const std::string &by
	) const = 0;
	virtual std::shared_ptr<ExpressionImpl<T>> with_context(const std::unordered_map<std::string, T> &context) const = 0;

	virtual T eval(void) const = 0;
	virtual std::string to_string(void) const = 0;
};

template <typename T> class Expression {
  public:
	Expression(T number);
	Expression(const std::string &variable);

	~Expression() = default;
	Expression(const Expression &other);

	Expression<T> sin(void) const;
	Expression<T> cos(void) const;
	Expression<T> ln(void) const;
	Expression<T> exp(void) const;

	Expression<T> diff(const std::string &by) const;
	Expression<T> with_context(const std::unordered_map<std::string, T> &context
	) const;
	T eval(void) const;
	T eval_with(const std::unordered_map<std::string, T> &context) const;
	std::string to_string(void) const;
	static Expression<T> from_string(const std::string& expression_str, bool ignore_case);

	Expression<T> &operator=(const Expression<T> &other);
	Expression<T> &operator=(Expression<T> &&other);

	Expression<T> operator+(const Expression<T> &other) const;
	Expression<T> &operator+=(const Expression<T> &other);

	Expression<T> operator-(const Expression<T> &other) const;
	Expression<T> &operator-=(const Expression<T> &other);

	Expression<T> operator*(const Expression<T> &other) const;
	Expression<T> &operator*=(const Expression<T> &other);

	Expression<T> operator/(const Expression<T> &other) const;
	Expression<T> &operator/=(const Expression<T> &other);

	Expression<T> operator^(const Expression<T> &other) const;
	Expression<T> &operator^=(const Expression<T> &other);

  private:
	Expression(std::shared_ptr<ExpressionImpl<T>> impl_);
	std::shared_ptr<ExpressionImpl<T>> impl;
	friend class Parser<T>;
};

template <typename T> class Value : public ExpressionImpl<T> {
  private:
	T value;

  public:
	explicit Value(T number);

	virtual std::shared_ptr<ExpressionImpl<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<ExpressionImpl<T>>
	with_context(const std::unordered_map<std::string, T> &context
	) const override;
	virtual T eval(void) const override;
	virtual std::string to_string(void) const override;
};

template <typename T> class Variable : public ExpressionImpl<T> {
  private:
	std::string name;

  public:
	Variable(const std::string var);

	virtual std::shared_ptr<ExpressionImpl<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<ExpressionImpl<T>>
	with_context(const std::unordered_map<std::string, T> &context
	) const override;
	virtual T eval(void) const override;
	virtual std::string to_string(void) const override;
};

template <typename T> class SinFunc : public ExpressionImpl<T> {
  private:
	std::shared_ptr<ExpressionImpl<T>> argument;

  public:
	explicit SinFunc(std::shared_ptr<ExpressionImpl<T>> argument_);

	virtual std::shared_ptr<ExpressionImpl<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<ExpressionImpl<T>>
	with_context(const std::unordered_map<std::string, T> &context
	) const override;
	virtual T eval(void) const override;
	virtual std::string to_string(void) const override;
};

template <typename T> class CosFunc : public ExpressionImpl<T> {
  private:
	std::shared_ptr<ExpressionImpl<T>> argument;

  public:
	explicit CosFunc(std::shared_ptr<ExpressionImpl<T>> argument_);

	virtual std::shared_ptr<ExpressionImpl<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<ExpressionImpl<T>>
	with_context(const std::unordered_map<std::string, T> &context
	) const override;
	virtual T eval(void) const override;
	virtual std::string to_string(void) const override;
};

template <typename T> class LnFunc : public ExpressionImpl<T> {
  private:
	std::shared_ptr<ExpressionImpl<T>> argument;

  public:
	explicit LnFunc(std::shared_ptr<ExpressionImpl<T>> argument_);

	virtual std::shared_ptr<ExpressionImpl<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<ExpressionImpl<T>>
	with_context(const std::unordered_map<std::string, T> &context
	) const override;
	virtual T eval(void) const override;
	virtual std::string to_string(void) const override;
};

template <typename T> class ExpFunc : public ExpressionImpl<T> {
  private:
	std::shared_ptr<ExpressionImpl<T>> argument;

  public:
	explicit ExpFunc(std::shared_ptr<ExpressionImpl<T>> argument_);

	virtual std::shared_ptr<ExpressionImpl<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<ExpressionImpl<T>>
	with_context(const std::unordered_map<std::string, T> &context
	) const override;
	virtual T eval(void) const override;
	virtual std::string to_string(void) const override;
};

template <typename T> class OperationAdd : public ExpressionImpl<T> {
  private:
	std::shared_ptr<ExpressionImpl<T>> left;
	std::shared_ptr<ExpressionImpl<T>> right;

  public:
	OperationAdd(
		const std::shared_ptr<ExpressionImpl<T>> &_left,
		const std::shared_ptr<ExpressionImpl<T>> &_right
	);

	virtual std::shared_ptr<ExpressionImpl<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<ExpressionImpl<T>>
	with_context(const std::unordered_map<std::string, T> &context
	) const override;
	virtual T eval(void) const override;
	virtual std::string to_string(void) const override;
};

template <typename T> class OperationMult : public ExpressionImpl<T> {
  private:
	std::shared_ptr<ExpressionImpl<T>> left;
	std::shared_ptr<ExpressionImpl<T>> right;

  public:
	OperationMult(
		const std::shared_ptr<ExpressionImpl<T>> &_left,
		const std::shared_ptr<ExpressionImpl<T>> &_right
	);

	virtual std::shared_ptr<ExpressionImpl<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<ExpressionImpl<T>>
	with_context(const std::unordered_map<std::string, T> &context
	) const override;
	virtual T eval(void) const override;
	virtual std::string to_string(void) const override;
};

template <typename T> class OperationSub : public ExpressionImpl<T> {
  private:
	std::shared_ptr<ExpressionImpl<T>> left;
	std::shared_ptr<ExpressionImpl<T>> right;

  public:
	OperationSub(
		const std::shared_ptr<ExpressionImpl<T>> &_left,
		const std::shared_ptr<ExpressionImpl<T>> &_right
	);

	virtual std::shared_ptr<ExpressionImpl<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<ExpressionImpl<T>>
	with_context(const std::unordered_map<std::string, T> &context
	) const override;
	virtual T eval(void) const override;
	virtual std::string to_string(void) const override;
};

template <typename T> class OperationDiv : public ExpressionImpl<T> {
  private:
	std::shared_ptr<ExpressionImpl<T>> left;
	std::shared_ptr<ExpressionImpl<T>> right;

  public:
	OperationDiv(
		const std::shared_ptr<ExpressionImpl<T>> &_left,
		const std::shared_ptr<ExpressionImpl<T>> &_right
	);

	virtual std::shared_ptr<ExpressionImpl<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<ExpressionImpl<T>>
	with_context(const std::unordered_map<std::string, T> &context
	) const override;
	virtual T eval(void) const override;
	virtual std::string to_string(void) const override;
};

template <typename T> class OperationPow : public ExpressionImpl<T> {
  private:
	std::shared_ptr<ExpressionImpl<T>> left;
	std::shared_ptr<ExpressionImpl<T>> right;

  public:
	OperationPow(
		const std::shared_ptr<ExpressionImpl<T>> &_left,
		const std::shared_ptr<ExpressionImpl<T>> &_right
	);

	virtual std::shared_ptr<ExpressionImpl<T>> diff(const std::string &by
	) const override;
	virtual std::shared_ptr<ExpressionImpl<T>>
	with_context(const std::unordered_map<std::string, T> &context
	) const override;
	virtual T eval(void) const override;
	virtual std::string to_string(void) const override;
};
#endif