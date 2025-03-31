#pragma once
#include <functional>
#include <cmath>
#include <vector>


namespace AD {
class Parameter;

class Expression {
 public:
    Expression() : value(0), derivative(0) {}
    virtual ~Expression();
    virtual void evaluate() = 0;
    virtual void backwards(double resid) = 0;
    virtual void forwards(Parameter* param) = 0;
    double value;
    double derivative;
};

class Constant : public Expression {
 public:
    Constant(double value) {
        this->value = value;
    }

    void evaluate() override {
        return;
    }
    void backwards(double resid) override {
        return;
    }

    void forwards(Parameter* param) override {
        this->derivative = 0;
    }
};

class Parameter : public Expression {
 public:
    Parameter(double value) {
        this->value = value;
    }

    void evaluate() override {
        return;
    }

    void backwards(double resid) override {
        this->derivative += resid;
        return;
    }

    void forwards(Parameter* param) override {
        if (param == this) {
            this->derivative = 1;
        } else {
            this->derivative = 0;
        }
    }
};

class Add : public Expression {
 public:
    Add(Expression* a, Expression* b) : a(a), b(b) {}

    void evaluate() override {
        a->evaluate();
        b->evaluate();
        this->value = a->value + b->value;
    }

    void backwards(double resid) override {
        a->backwards(resid);
        b->backwards(resid);
    }

    void forwards(Parameter* param) override {
        a->forwards(param), b->forwards(param);
        this->value = a->value + b->value;
        this->derivative = a->derivative + b->derivative;
    }

 private:
    Expression* a;
    Expression* b;
};

class Sub : public Expression {
 public:
    Sub(Expression* a, Expression* b) : a(a), b(b) {}

    void evaluate() override {
        a->evaluate();
        b->evaluate();
        this->value = a->value - b->value;
    }

    void backwards(double resid) override {
        a->backwards(resid);
        b->backwards(-resid);
    }

    void forwards(Parameter* param) override {
        a->forwards(param), b->forwards(param);
        this->value = a->value - b->value;
        this->derivative = a->derivative - b->derivative;
    }

 private:
    Expression* a;
    Expression* b;
};


class Mult : public Expression {
 public:
    Mult(Expression* a, Expression* b) : a(a), b(b) {}

    void evaluate() override {
        a->evaluate();
        b->evaluate();
        this->value = a->value * b->value;
    }

    void backwards(double resid) override {
        a->backwards(resid * b->value);
        b->backwards(resid * a->value);
    }

    void forwards(Parameter* param) override {
        a->forwards(param), b->forwards(param);
        this->value = a->value * b->value;
        this->derivative = a->derivative * b->value + a->value * b->derivative;
    }

 private:
    Expression* a;
    Expression* b;
};


class Div : public Expression {
 public:
    Div(Expression* a, Expression* b) : a(a), b(b) {}

    void evaluate() override {
        a->evaluate();
        b->evaluate();
        this->value = a->value / b->value;
    }

    void backwards(double resid) override {
        a->backwards(resid / b->value);
        b->backwards(-resid * a->value / (b->value * b->value));
    }

    void forwards(Parameter* param) override {
        a->forwards(param), b->forwards(param);
        this->value = a->value / b->value;
        this->derivative = (a->derivative * b->value - a->value * b->derivative) / (b->value * b->value);
    }

 private:
    Expression* a;
    Expression* b;
};

class Pow : public Expression {
 public:
    Pow(Expression* a, Expression* b) : a(a), b(b) {}

    void evaluate() override {
        a->evaluate();
        b->evaluate();
        this->value = std::pow(a->value, b->value);
    }

    void backwards(double resid) override {
        a->backwards(resid * b->value * std::pow(a->value, b->value - 1));
        b->backwards(resid * std::pow(a->value, b->value) * std::log(a->value));
    }

    void forwards(Parameter* param) override {
        a->forwards(param), b->forwards(param);

        this->value = std::pow(a->value, b->value);
        this->derivative = b->value * std::pow(a->value, b->value - 1) * a->derivative + std::pow(a->value, b->value) * std::log(a->value) * b->derivative;
    }

 private:
    Expression* a;
    Expression* b;
};

class Log : public Expression {
 public:
    Log(Expression* a) : a(a) {}

    void evaluate() override {
        a->evaluate();
        this->value = std::log(a->value);
    }

    void backwards(double resid) override {
        a->backwards(resid / a->value);
    }

    void forwards(Parameter* param) override {
        a->forwards(param);
        this->value = std::log(a->value);
        this->derivative = a->derivative / a->value;
    }

 private:
    Expression* a;
};

class Exp : public Expression {
 public:
    Exp(Expression* a) : a(a) {}

    void evaluate() override {
        a->evaluate();
        this->value = std::exp(a->value);
    }

    void backwards(double resid) override {
        a->backwards(resid * std::exp(a->value));
    }

    void forwards(Parameter* param) override {
        a->forwards(param);
        this->value = std::exp(a->value);
        this->derivative = a->derivative * std::exp(a->value);
    }
 private:
    Expression* a;
};

class Sin : public Expression {
 public:
    Sin(Expression* a) : a(a) {}

    void evaluate() override {
        a->evaluate();
        this->value = std::sin(a->value);
    }

    void backwards(double resid) override {
        a->backwards(resid * std::cos(a->value));
    }

    void forwards(Parameter* param) override {
        a->forwards(param);
        this->value = std::sin(a->value);
        this->derivative = a->derivative * std::cos(a->value);
    }

 private:
    Expression* a;
};


class Cos : public Expression {
 public:
    Cos(Expression* a) : a(a) {}

    void evaluate() override {
        a->evaluate();
        this->value = std::cos(a->value);
    }

    void backwards(double resid) override {
        a->backwards(-1 * resid * std::sin(a->value));
    }

    void forwards(Parameter* param) override {
        a->forwards(param);
        this->value = std::cos(a->value);
        this->derivative = -1 * a->derivative * std::sin(a->value);
    }

 private:
    Expression* a;
};

class Tan : public Expression {
 public:
    Tan(Expression* a) : a(a) {}

    void evaluate() override {
        a->evaluate();
        this->value = std::tan(a->value);
    }

    void backwards(double resid) override {
        a->backwards(resid / (std::cos(a->value) * std::cos(a->value)));
    }

    void forwards(Parameter* param) override {
        a->forwards(param);
        this->value = std::tan(a->value);
        this->derivative = a->derivative / (std::cos(a->value) * std::cos(a->value));
    }

 private:
    Expression* a;
};

inline Expression* operator+(Expression& a, Expression& b) {
    return new Add(&a, &b);
}

inline Expression* operator*(Expression& a, Expression& b) {
    return new Mult(&a, &b);
}

inline Expression* operator-(Expression& a, Expression& b) {
    return new Sub(&a, &b);
}

inline Expression* operator/(Expression& a, Expression& b) {
    return new Div(&a, &b);
}



};  // namespace AD


