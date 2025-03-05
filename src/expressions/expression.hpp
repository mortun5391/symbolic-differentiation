#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

template <typename T> class ExpressionImpl {
    public:
        // Запрет на создание экземпляров класса ExpressionImpl.
        ExpressionImpl() = default;
        virtual ~ExpressionImpl() = default;

        // Функция вычисления результата выражения.
        virtual T eval(std::map<std::string, T> context) const = 0;

        // Функция преобразования выражения в строку.
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

        // Конструирование выражений на основе других выражений.
        Expression<T>  operator+ (const Expression& that) const;
        Expression<T> &operator+=(const Expression& that);
        Expression<T>  operator* (const Expression& that) const;
        Expression<T> &operator*=(const Expression& that);

        // Операции с выражениями.
        T eval(std::map<std::string, T> context) const;
        std::string to_string() const;


    private:
        Expression(std::shared_ptr<ExpressionImpl> impl);
        std::shared_ptr<ExpressionImpl> impl_;
};

// Класс, представляющий число в рамках выражения.
template <typename T> class Value : public ExpressionImpl
{
public:
    // Создание числа на основе ... числа.
    Value(T number);

    virtual ~Value() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;

private:
    T value;
};

// Класс, представляющий переменную в рамках выражения.
template <typename T> class Variable : public ExpressionImpl
{
public:
    // Создание переменной на основе её имени.
    Variable(std::string name);

    virtual ~Variable() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;

private:
    std::string name_;
};

// Класс, представляющий выражение сложения двух выражений.
template <typename T> class OperationAdd : public ExpressionImpl
{
public:
    // Создание выражения для суммы на основе подвыражений.
    OperationAdd(Expression left, Expression right);

    virtual ~OperationAdd() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;

private:
    Expression left_;
    Expression right_;
};

// Класс, представляющий выражение умножения двух выражений.
template <typename T> class OperationMult : public ExpressionImpl
{
public:
    // Создание выражения для суммы на основе подвыражений.
    OperationMult(Expression left, Expression right);

    virtual ~OperationMult() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;

private:
    Expression left_;
    Expression right_;
};

#endif