//
// Created 2/5/19
//

#ifndef EXPRINTER_ExprNode_HPP
#define EXPRINTER_ExprNode_HPP

#include "Token.hpp"
#include "SymTab.hpp"
#include "Functions.hpp"
#include "Statements.hpp"
#include <memory>

//Classes in this file define the internal representation of arithmetic expressions.

class Statements ;



// An ExprNode serves as the base class (super class) for arithmetic expression.
// It forces the derived classes (subclasses) to implement two functions, print and
// evaluate.

// all expression nodes represent a variable, number, or string
// they evaulate to a type descriptor. Which returns a value from a value function.
// these values will be used by infex expression's node evaulate, which operators on the symbol table
class ExprNode {
public:
    ExprNode(Token token);
    Token token();
    virtual void print() = 0;
    virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab, Functions &funcTab) = 0;

private:
    Token _token;
};


// An InfixExprNode is useful to represent binary operators.
class InfixExprNode: public ExprNode {  // An expression tree node.

public:
    InfixExprNode(Token tk);

    ExprNode *&left();
    ExprNode *&right();
    virtual void print();
    virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab, Functions &funcTab);

private:
    ExprNode *_left, *_right;
};

class UnaryExprNode: public ExprNode {  // An expression tree node.
public:
    UnaryExprNode(Token tk);

    ExprNode *&left();
    virtual void print();
    virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab, Functions &funcTab);

private:
    ExprNode * _left;
};


// handles all the responsibilities of a calling object
// returns a type descriptor which is the return value of the called function
// gets function obect from Functions, uses symtab to open a new scope, computes the arguments, 
//  copies the arguments into the new scope using the parameter names found in Function
class CallFunctionNode: public ExprNode {
public:
    CallFunctionNode(Token token); // function name?

    // need a setter/getter to function args
    std::vector<ExprNode*> getFunctionArgs()                  { return _function_args; }
    void setFunctionArgs(std::vector<ExprNode*> func_args)    { _function_args = func_args; }
    virtual void print();
    virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab, Functions &funcTab);
private:
    std::vector<ExprNode*> _function_args;
};




// integer is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.

// returns a string representation of the operator when evaulated.
// otherwise holds an operator token (to be used by infix operator)
class OperatorNode: public ExprNode{
public:
    OperatorNode(Token token);
    virtual void print();
    virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab, Functions &funcTab);
};

// all leaf nodes?
class NumberNode: public ExprNode {
public:
    NumberNode(Token token); //instantiated with a token that is an integer
    virtual void print();
    virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab, Functions &funcTab);
};

class StringNode: public ExprNode {
public:
    StringNode(Token token); //instantiated with a token that is an integer
    virtual void print();
    virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab, Functions &funcTab);
};

class Variable: public ExprNode {
public:
    Variable(Token token);
    virtual void print();
    virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab, Functions &funcTab);
};

class Vector: public ExprNode{
public:
    Vector(Token token);
    virtual void print();
    virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab, Function &funcTab);
    std::vector<ExprNode*> _arrayElements;
};

#include "Statements.hpp"

#endif //EXPRINTER_ExprNode_HPP
