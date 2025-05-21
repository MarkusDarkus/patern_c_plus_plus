#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <stdexcept>

// Базовый интерфейс для математических выражений
class MathExpr {
public:
    virtual ~MathExpr() = default;
    virtual void display(std::ostream& out) const = 0;
    virtual int compute(const std::map<std::string, int>& variables) const = 0;
};
using ExprHandle = std::shared_ptr<MathExpr>;

//---------------------------------
// Простые элементы выражения
//---------------------------------
class Number : public MathExpr {
    int num;
public:
    explicit Number(int n) : num(n) {}
    void display(std::ostream& out) const override { out << num; }
    int compute(const std::map<std::string, int>&) const override { return num; }
};

class NamedVariable : public MathExpr {
    std::string varName;
public:
    explicit NamedVariable(std::string name) : varName(std::move(name)) {}
    void display(std::ostream& out) const override { out << varName; }
    int compute(const std::map<std::string, int>& vars) const override {
        auto it = vars.find(varName);
        if (it == vars.end()) {
            throw std::runtime_error("Undefined variable: " + varName);
        }
        return it->second;
    }
};

//---------------------------------
// Составные выражения
//---------------------------------
class BinaryOperation : public MathExpr {
protected:
    ExprHandle lhs, rhs;
    std::string oper;
public:
    BinaryOperation(ExprHandle left, ExprHandle right, std::string op)
        : lhs(std::move(left)), rhs(std::move(right)), oper(std::move(op)) {}
    
    void display(std::ostream& out) const override {
        out << "(";
        lhs->display(out);
        out << " " << oper << " ";
        rhs->display(out);
        out << ")";
    }
};

class Sum : public BinaryOperation {
public:
    Sum(ExprHandle l, ExprHandle r) : BinaryOperation(l, r, "+") {}
    int compute(const std::map<std::string, int>& vars) const override {
        return lhs->compute(vars) + rhs->compute(vars);
    }
};

class Difference : public BinaryOperation {
public:
    Difference(ExprHandle l, ExprHandle r) : BinaryOperation(l, r, "-") {}
    int compute(const std::map<std::string, int>& vars) const override {
        return lhs->compute(vars) - rhs->compute(vars);
    }
};

class Product : public BinaryOperation {
public:
    Product(ExprHandle l, ExprHandle r) : BinaryOperation(l, r, "*") {}
    int compute(const std::map<std::string, int>& vars) const override {
        return lhs->compute(vars) * rhs->compute(vars);
    }
};

class Division : public BinaryOperation {
public:
    Division(ExprHandle l, ExprHandle r) : BinaryOperation(l, r, "/") {}
    int compute(const std::map<std::string, int>& vars) const override {
        return lhs->compute(vars) / rhs->compute(vars);
    }
};

//---------------------------------
// Фабрика выражений (одиночка)
//---------------------------------
class ExprFactory {
    std::map<int, std::weak_ptr<Number>> numberCache;
    std::map<std::string, std::weak_ptr<NamedVariable>> variableCache;

    ExprFactory() = default;
    
    template<typename Key, typename WeakPtr>
    void cleanupCache(std::map<Key, WeakPtr>& cache) {
        for (auto it = cache.begin(); it != cache.end(); ) {
            if (it->second.expired()) {
                it = cache.erase(it);
            } else {
                ++it;
            }
        }
    }

public:
    static ExprFactory& getInstance() {
        static ExprFactory instance;
        return instance;
    }

    ExprHandle createNumber(int value) {
        cleanupCache(numberCache);
        auto& cached = numberCache[value];
        if (auto shared = cached.lock()) {
            return shared;
        }
        auto newNumber = std::make_shared<Number>(value);
        cached = newNumber;
        return newNumber;
    }

    ExprHandle createVariable(const std::string& name) {
        cleanupCache(variableCache);
        auto& cached = variableCache[name];
        if (auto shared = cached.lock()) {
            return shared;
        }
        auto newVar = std::make_shared<NamedVariable>(name);
        cached = newVar;
        return newVar;
    }
};

//---------------------------------

int main() {
    auto& factory = ExprFactory::getInstance();

    {
        // Вычисляем выражение: (2 + x) * 5, где x = 3
        auto sumExpr = std::make_shared<Sum>(
            factory.createNumber(2),
            factory.createVariable("x")
        );
        auto finalExpr = std::make_shared<Product>(
            sumExpr,
            factory.createNumber(5)
        );

        std::map<std::string, int> context{{"x", 3}};
        finalExpr->display(std::cout);
        std::cout << " = " << finalExpr->compute(context) << '\n';
    }

    auto anotherExpr = std::make_shared<Sum>(
        factory.createNumber(3),
        factory.createVariable("y")
    );
}