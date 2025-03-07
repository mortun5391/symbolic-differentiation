#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP
#include <string>
#include <map>
#include <memory>



template <typename T> class ExpressionImpl {
    public:
        
        ExpressionImpl() = default;
        virtual ~ExpressionImpl() = default;

        virtual T eval(std::map<std::string, T> context) const = 0;

        virtual std::string to_string() const = 0;
};

template <typename T> class Expression {
    public:
        Expression(T number);
	    Expression(const std::string &variable);

        ~Expression() = default;
	    Expression(const Expression &other);

        friend Expression operator""_val(T val);
        friend Expression operator""_var(const char* variable);
        friend Expression operator""_var(const char* variable, size_t size);

       
        Expression<T>  operator+ (const Expression& that) const;
        Expression<T> &operator+=(const Expression& that);
        Expression<T>  operator* (const Expression& that) const;
        Expression<T> &operator*=(const Expression& that);

        
        T eval(std::map<std::string, T> context) const;
        std::string to_string() const;


    private:
        Expression(std::shared_ptr<ExpressionImpl<T>> impl);
        std::shared_ptr<ExpressionImpl<T>> impl_;
};


template <typename T> class Value : public ExpressionImpl {
    public:
        
        Value(T number);

        virtual ~Value() override = default;

        
        virtual T eval(std::map<std::string, T> context) const override;
        virtual std::string to_string() const override;

    private:
        T value;
};


template <typename T> class Variable : public ExpressionImpl
{
public:
   
    Variable(std::string name);

    virtual ~Variable() override = default;

   
    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;

private:
    std::string name_;
};


template <typename T> class OperationAdd : public ExpressionImpl
{
public:

    OperationAdd(Expression left, Expression right);

    virtual ~OperationAdd() override = default;

    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;

private:
    Expression left;
    Expression right;
};


template <typename T> class OperationMult : public ExpressionImpl
{
public:

    OperationMult(Expression left, Expression right);

    virtual ~OperationMult() override = default;
    
    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;

private:
    Expression left_;
    Expression right_;
};

#endif