#pragma once
#include <functional>
#include <cmath>
#include <vector>


namespace AD {

class Expression {
 public:
    virtual ~Expression() {}
    virtual void evaluate() = 0;
    virtual void backwards(double resid) = 0;
    virtual void forwards(Parameter* param) = 0;
    // virtual std::function<double> lambda() = 0;
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
    Parameter(double value) : {
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


std::vector<double> differentiate(std::function<Expression*()> f, const std::vector<double>& x) {
    // idk what actually goes here
    return std::vector<double>();
}

};  // namespace AD


