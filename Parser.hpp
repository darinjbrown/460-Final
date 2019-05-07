
#ifndef __PARSER_HPP
#define __PARSER_HPP

#include <vector>
#include <iostream>
#include <map>

#include "Token.hpp"
#include "Tokenizer.hpp"
//#include "SymTab.hpp"
#include "ExprNode.hpp"
#include "Statements.hpp"
#include <memory>


class Parser {
public:
    Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}


    // {NEWLINE | statement}* EOF
    Statements* file_input();

    // simple_stmt | compound_stmt
    Statement* stmt();

    // simple_stmt | compound_stmt
    Statement* statement();

    // {print_statement | assignment_statement}+ NEWLINE
    Statement* simple_statement();

    // 'print' [ testlist ]
    PrintStatement* print_statement();

    // ID '=' test
    AssignmentStatement *assign_statement();

    Statement *compound_statement();
    
    //  call
    FunctionCallStatement *call_stmt();

    //return type ExprNode?
    // 'return' [ test ]
    FunctionReturnStatement* return_stmt();

    // test {',' test}*
    std::vector<ExprNode*> testList();

    // NEWLINE INDENT statement DEDENT
    Statements* suite();

    //--// NEWLINE INDENT ( stmt | return_stmt)+ DEDENT
    Statements* func_suite();

    IfStatement* if_statement();

    // 'for' ID 'in' testlist ':' suite
    PyForStatement* py_for_statement();

    // 'def' ID '(' [ parameter_list ] ')' ':' func_suite
    FunctionDefinitionStatement *func_def_statement();

    // ID '(' { testList } ')'
    CallFunctionNode *call();

    // ID {',' ID}*
    std::vector<std::string> parameter_list();


    ExprNode *relExpr();
    ExprNode *relTerm();
    
    // or_test
    ExprNode* test();

    // and_test {'or' and_test}*
    ExprNode* or_test();

    // not_test {'and' not_test}*
    ExprNode* and_test();

    // 'not' not_test | compairson
    ExprNode* not_test();

    // arith_expr {comp_op arith_expr}*
    ExprNode* compairson();

    // '<' | '>' | '==' | '>=' | '<=' | '<>' | '!='
    ExprNode* comp_op();

    // term {( '+'|'-' term )}*
    ExprNode* arith_expr();

    // factor { ('*'|'/'|'%'|'//') factor }*
    ExprNode* term();

    // {'-'} factor | atom
    ExprNode* factor();

    // ID | NUMBER | STRING+ | '(' test ')'
    ExprNode *atom();


private:
    Tokenizer &tokenizer;

    void die(std::string where, std::string message, Token &token);

};

#endif

