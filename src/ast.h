#ifndef KAL_AST_H
#define KAL_AST_H

#include <vector> 
#include <string>

namespace helper {
// Cloning make_unique here until it's standard in C++14.
// Using a namespace to avoid conflicting with MSVC's std::make_unique (which
// ADL can sometimes find in unqualified calls).
template <class T, class... Args>
    typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type
    make_unique(Args &&... args) { return std::unique_ptr<T>(new T(std::forward<Args>(args)...)); }
}


namespace {

class ExprAST;
class PrototypeAST;
class FunctionAST;
using Expr_Type = std::unique_ptr<ExprAST>;
using Proto_Type = std::unique_ptr<PrototypeAST>;
using Func_Type = std::unique_ptr<FunctionAST>;

/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
    virtual ~ExprAST() {}
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double Val) : Val(Val) {}
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
    std::string Name;

public:
    VariableExprAST(const std::string &Name) : Name(Name) {}
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
    char Op;
    Expr_Type LHS, RHS;

public:
    BinaryExprAST(char Op, Expr_Type LHS, Expr_Type RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<Expr_Type> Args;

public:
    CallExprAST(const std::string &Callee, std::vector<Expr_Type> Args)
        : Callee(Callee), Args(std::move(Args)) {}
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &Name, std::vector<std::string> Args)
        : Name(Name), Args(std::move(Args)) {}
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    Proto_Type Proto;
    Expr_Type Body;

public:
    FunctionAST(Proto_Type Proto, Expr_Type Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}
};
}

#endif
