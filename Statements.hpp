//
// Created 2/5/19
//

#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "ExprNode.hpp"
#include "SymTab.hpp"
#include "Functions.hpp"

// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.

// Each line read from the stream is converted into a "string" 

class Statement {
public:
    Statement();

    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab, Functions &funcTab) = 0;

};


// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements {
public:
    Statements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab, Functions &funcTab);

    void print();

private:
    std::vector<Statement *> _statements;
};

class Function {
public:
    Function();
    Function(std::vector<std::string> params, Statements* blockStatements);
    Statements*& getStatements()             {return _blockStatements;}
    std::vector<std::string> getParams()    {return _paramList;}

    // evaluate method?
    // must be called within the context 
    void evaluate(SymTab& symTab, Functions& funcTab);
private:
    // parameter list
    std::vector<std::string> _paramList;
    // code block
    Statements* _blockStatements;
};

/*

// these are statements executed in a subprogram.
// they keep track of if they are a return statement or not
class FunctionBlockStatement : public Statement{
public:
    FunctionBlockStatement();
    FunctionBlockStatement(bool, Statement*);
    void evaluate(SymTab &symTab, Functions &funcTab);
    void print();

    bool isReturnStatement() const  {return _isReturnStatement;}
private:
    bool _isReturnStatement;
    Statement* _statement;
};

// check if a statement is a return statement before evaluating it
// stop execution and "return" it's value if so
class FunctionBlockStatements {
public:
    FunctionBlockStatements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab, Functions &funcTab);

    void print();

private:
    std::vector<FunctionBlockStatement*> _funcBlockStatements;
};

*/


class FunctionReturnStatement : public Statement{
public:
    FunctionReturnStatement();
    FunctionReturnStatement(ExprNode*);

    void evaluate(SymTab &symTab, Functions &funcTab);
    void print();
private:
    ExprNode* _returnExpr;
};

// this will prepare the function object when evaluated
class FunctionDefinitionStatement : public Statement{
public:
    FunctionDefinitionStatement();
    FunctionDefinitionStatement(std::string, std::vector<std::string>, Statements*);
    void evaluate(SymTab &symTab, Functions &funcTab);
    void print();
private:
    // function name
    std::string _funcName;
    // function parameters
    std::vector<std::string> _paramNames;
    // function code block
    Statements * _blockStatements;
};




class FunctionCallStatement : public Statement{
public:
    FunctionCallStatement();
    FunctionCallStatement(CallFunctionNode*);
    void evaluate(SymTab &symTab, Functions &funcTab);
    void print();
private:
    CallFunctionNode* _callFuncNode;
};




// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression (ExprNode)

class AssignmentStatement : public Statement {
public:

    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr);
    AssignmentStatement(std::string lhsVar, ExprNode* arrayIndex, std::vector<ExprNode*> testList);


    std::string &lhsVariable();
    std::string &lhsVariable(int index);
    ExprNode *&rhsExpression();
    ExprNode *&arrayIndex();
    bool _isArray;
    bool _isArrayInit;
    ExprNode* _arrayIndex;
    std::vector<TypeDescriptor*> _testList;

    virtual void evaluate(SymTab &symTab, Functions &funcTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode * _rhsExpression;
};




class PrintStatement : public Statement {
public:
    PrintStatement();
    PrintStatement(std::vector<ExprNode *> rhsExpressions);


    ExprNode *&rhsExpression();
    std::vector<ExprNode *> &rhsExpressions();
    
    virtual void evaluate(SymTab &symTab, Functions &funcTab);
    virtual void print();

private:
    std::vector<ExprNode *> _rhsExpressions;
};



class IfStatement : public Statement {
public:
    IfStatement();
    IfStatement(std::vector<ExprNode*>, std::vector<Statements*>, Statements*);
    virtual void evaluate(SymTab &symTab, Functions &funcTab);
    virtual void print();
private:
    std::vector<ExprNode*> _if_tests_vector;
    std::vector<Statements*> _block_statements_vector;
    Statements* _else_statements;
};



class PyForStatement : public Statement {
public:
    PyForStatement();
    PyForStatement(AssignmentStatement*, Statements*, ExprNode*, ExprNode*, ExprNode*, AssignmentStatement*);
    Statements *&blockStatements()                  { return _blockStatements; }
    virtual void evaluate(SymTab &symTab, Functions &funcTab);
    virtual void print();
private:
    class Range {
        public:
            Range();
            //Range(int end);
            //Range(int start, int end);
            Range(int,int,ExprNode*);

            bool atEnd();
            void increment(SymTab &symTab, Functions &funcTab);
            int getCurrent();

        private:
            int _start;
            int _end;
            ExprNode * _step;
    };
    AssignmentStatement * _indexVariableInitialize;
    Statements * _blockStatements;
    ExprNode * _rangeStart;
    ExprNode * _rangeEnd;
    ExprNode * _rangeStep;
    AssignmentStatement * _stepIndexVar;
    Range _iterator;
};





#endif //EXPRINTER_STATEMENTS_HPP