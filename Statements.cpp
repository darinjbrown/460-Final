//
// Created 2/5/19
//

#include <vector>
#include <iostream>
#include "Statements.hpp"

// Statement
Statement::Statement() {}


// Statements
Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }


void Statements::print() {
    for (auto s: _statements)
        s->print();
}

/*
void Statements::evaluate(SymTab &symTab, Functions &funcTab) {
    for (auto s: _statements){
        // check if s is a return statement. If so, we end execution of these statements
        FunctionReturnStatement* rs_ptr = dynamic_cast<FunctionReturnStatement*>(s);
        if(rs_ptr){
            s->evaluate(symTab, funcTab);
            break;
        }
        else{
            s->evaluate(symTab, funcTab);
        }
    }
}
*/

void Statements::evaluate(SymTab &symTab, Functions &funcTab) {
    for (auto s: _statements){
        if(symTab.hasReturnValue()){
            break;
        }
        else{
            s->evaluate(symTab, funcTab);
        }
    }
}


// Function class
Function::Function() : _paramList{{}}, _blockStatements{nullptr} {};
Function::Function(std::vector<std::string> params, Statements* blockStatements) : _paramList{params}, _blockStatements{blockStatements} {};

void Function::evaluate(SymTab& symTab, Functions& funcTab){

}


//
FunctionReturnStatement::FunctionReturnStatement() : _returnExpr{nullptr} {}
FunctionReturnStatement::FunctionReturnStatement(ExprNode* returnExpr) : _returnExpr{returnExpr} {}

void FunctionReturnStatement::evaluate(SymTab &symTab, Functions &funcTab){
    std::shared_ptr<TypeDescriptor> rhs = _returnExpr->evaluate(symTab, funcTab);
    symTab.setReturnValue(rhs); // places the return value into the symbol tables return value member
}
void FunctionReturnStatement::print(){
    std::cout << "return ";
    _returnExpr->print();
    std::cout << std::endl;
}


//function definition statement
FunctionDefinitionStatement::FunctionDefinitionStatement() : _funcName{""}, _paramNames{ {} }, _blockStatements{nullptr} {};
FunctionDefinitionStatement::FunctionDefinitionStatement(std::string funcName, std::vector<std::string> paramList, Statements* blockStatements) : _funcName{funcName}, _paramNames{paramList}, _blockStatements{blockStatements} {};

void FunctionDefinitionStatement::evaluate(SymTab &symTab, Functions &funcTab){
    if(funcTab.isDefined(_funcName)){
        std::cout << "Error: function '" << _funcName << "' already exists";
        exit(1);
    }
    else{
        std::shared_ptr<Function> func_ptr = std::make_shared<Function>(_paramNames, _blockStatements);
        funcTab.addFunction(_funcName, func_ptr);
    }
}


void FunctionDefinitionStatement::print(){
    std::cout << "def ";
    std::cout << _funcName << "(";
    for (auto s: _paramNames)
        std::cout << s << ", ";
    std::cout << "):\n";
    _blockStatements->print();
}



//function call statement
FunctionCallStatement::FunctionCallStatement() : _callFuncNode{nullptr} {};
FunctionCallStatement::FunctionCallStatement(CallFunctionNode* callFuncNode): _callFuncNode{callFuncNode} {};

void FunctionCallStatement::evaluate(SymTab &symTab, Functions &funcTab){
    _callFuncNode->evaluate(symTab, funcTab);
}
void FunctionCallStatement::print(){
    _callFuncNode->print();
}

// AssignmentStatement
AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr},
        _arrayIndex{nullptr}, _isArray{false}, _testList{nullptr}, _isArrayInit{false} {}


AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode* arrayIndex, std::vector<ExprNode*> testList):
    _lhsVariable{lhsVar}, _arrayIndex{arrayIndex} {}

// values have already been parsed and confirmed? (assumption)
void AssignmentStatement::evaluate(SymTab &symTab, Functions &funcTab) {
    if (_isArray == false){
        std::shared_ptr<TypeDescriptor> rhs = rhsExpression()->evaluate(symTab, funcTab);
        symTab.setValueFor( lhsVariable() , rhs);
    }
    //TODO: finish this after parsing of array assignment done
    else if(_isArrayInit == false){ // assignment of a test into an index of the array
        std::shared_ptr<TypeDescriptor> rhs = rhsExpression()->evaluate(symTab, funcTab);
        symTab.setValueFor(lhsVariable(arrayIndex()->evaluate(symTab, funcTab)), rhs)
    }
    //TODO: if (_isArrayInit) implement array initialization with a loop? (vname[test] = testList)
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

std::string &AssignmentStatement::lhsVariable(int index) {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

//ExprNode *&
void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
    std::cout << std::endl;
}


PrintStatement::PrintStatement() : _rhsExpressions{nullptr} {}
PrintStatement::PrintStatement(std::vector<ExprNode *> rhsExpressions) : _rhsExpressions{rhsExpressions} {}

std::vector<ExprNode *> &PrintStatement::rhsExpressions(){
    return _rhsExpressions;
}

void PrintStatement::print(){
    std::cout << "print ";
    for(int i = 0; i < _rhsExpressions.size(); i++){
        _rhsExpressions[i]->print();
        //std::cout << "size of rhsExpressions is: " << _rhsExpressions.size() << std::endl;
        if(_rhsExpressions.size() - i > 1){ // make sure there are additional statements or don't print a comma
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

void PrintStatement::evaluate(SymTab &symTab, Functions &funcTab) {

    for(int i = 0; i < rhsExpressions().size(); i++ ){
        std::shared_ptr<TypeDescriptor> rhs = rhsExpressions()[i]->evaluate(symTab, funcTab);

        if(rhs->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> td_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor >(rhs);
            if(td_ptr){
                auto TD_value = td_ptr->value.intValue;
                std::cout << TD_value << std::endl;
            }
        }
        else if(rhs->type() == TypeDescriptor::BOOL){
            std::shared_ptr<NumberTypeDescriptor> td_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor >(rhs);
            if(td_ptr){
                auto TD_value = td_ptr->value.boolValue;
                std::cout << TD_value << std::endl;
            }
        }
        else if(rhs->type() == TypeDescriptor::STRING){
            std::shared_ptr<StringTypeDescriptor> td_ptr = std::dynamic_pointer_cast<StringTypeDescriptor >(rhs);
            if(td_ptr){
                auto TD_value = td_ptr->stringValue;
                std::cout << TD_value << std::endl;
            }
        }
        else{
            std::cout << "Unknown value in PrintStatement::evaluate()\n";
            exit(1);
        }
    }
}


IfStatement::IfStatement(): _if_tests_vector{nullptr}, _block_statements_vector{nullptr}, _else_statements{nullptr} {}
IfStatement::IfStatement(std::vector<ExprNode*> ifArg1, std::vector< Statements* > ifArg2, Statements* ifArg3): _if_tests_vector{ifArg1}, _block_statements_vector{ifArg2}, _else_statements{ifArg3}{}
void IfStatement::print(){
    std::cout << "if ";
    _if_tests_vector[0]->print();
    std::cout << " :\n    ";
    _block_statements_vector[0]->print();
    for(int i = 1; i < _if_tests_vector.size(); i++){
        std::cout << "elif ";
        _if_tests_vector[i]->print();
        std::cout << " :\n    ";
        _block_statements_vector[i]->print();
    }
    if(_else_statements){
        std::cout << "else ";
        std::cout << " :\n    ";
        _else_statements->print();
    }
}

void IfStatement::evaluate(SymTab &symTab, Functions &funcTab){
    bool else_flag = 1;
    int number_of_conditionals = _if_tests_vector.size();
    std::shared_ptr<TypeDescriptor> if_conditional_result = nullptr;

    // loop while we have elements in the if_vector and until an if stateent evaluates to true
    for(int i = 0; i < number_of_conditionals && else_flag; i++){
        if_conditional_result = _if_tests_vector[i]->evaluate(symTab, funcTab);
        std::shared_ptr<NumberTypeDescriptor> if_cndl_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(if_conditional_result);
        if(if_cndl_ptr){
            if(if_cndl_ptr->type() == TypeDescriptor::INTEGER){
                auto result = if_cndl_ptr->value.intValue;
                if(result){
                    _block_statements_vector[i]->evaluate(symTab, funcTab);
                    else_flag = 0;
                }
            }
            else if(if_cndl_ptr->type() == TypeDescriptor::BOOL){
                auto result = if_cndl_ptr->value.boolValue;
                if(result){
                    _block_statements_vector[i]->evaluate(symTab, funcTab);
                    else_flag = 0;
                }
            }
        }
    }

    if(else_flag && _else_statements){
        _else_statements->evaluate(symTab, funcTab);
    }
}






PyForStatement::PyForStatement() : _indexVariableInitialize{nullptr}, _blockStatements{nullptr}, _rangeStart{nullptr}, _rangeEnd{nullptr}, _rangeStep{nullptr}, _stepIndexVar{nullptr} {}
PyForStatement::PyForStatement(AssignmentStatement* initIndex, Statements* stmts, ExprNode* start, ExprNode* end, ExprNode* step, AssignmentStatement* stepIndex) : _indexVariableInitialize{initIndex}, _blockStatements{stmts}, _rangeStart{start}, _rangeEnd{end}, _rangeStep{step}, _stepIndexVar{stepIndex}  {}



void PyForStatement::evaluate(SymTab &symTab, Functions &funcTab){
    //std::cout << "WE EVALUATIN\n";
    // for our iterator, we get an int for the start and end indicies
    // we use a expression node to evaluate our step value.
    std::shared_ptr<TypeDescriptor> start = _rangeStart->evaluate(symTab, funcTab);
    std::shared_ptr<TypeDescriptor> end = _rangeEnd->evaluate(symTab, funcTab);
    std::shared_ptr<NumberTypeDescriptor> start_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(start);
    std::shared_ptr<NumberTypeDescriptor> end_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(end);
    int startVal, endVal;
    if(start_ptr && end_ptr){
        if(start_ptr->type() == TypeDescriptor::INTEGER){
            startVal = start_ptr->value.intValue;
        }
        else{
            std::cout << "Unsupported value type for range(start)\n";
            exit(1);
        }
        if(end_ptr->type() == TypeDescriptor::INTEGER){
            endVal = end_ptr->value.intValue;
        }
        else{
            std::cout << "Unsupported value type for range(end)\n";
            exit(1);
        }
    }
    _indexVariableInitialize->evaluate(symTab, funcTab);
    _iterator = Range(startVal, endVal, _rangeStep);
    while(!_iterator.atEnd()){
        _blockStatements->evaluate(symTab, funcTab);
        _stepIndexVar->evaluate(symTab, funcTab);
        _iterator.increment(symTab, funcTab);
    }
}
// PROBLEM: if the right hand of the conditional changes in the loop it won't be updated



void PyForStatement::print(){
    std::cout << "For ";
    std::cout << _indexVariableInitialize->lhsVariable() << " in range( ";
    _rangeStart->print();
    std::cout << ", ";
    _rangeEnd->print();
    std::cout << ", ";
    _rangeStep->print();
    std::cout << ") ";
    std::cout << ": \n";
    std::cout << "    ";
    _blockStatements->print();
}


// range methods
PyForStatement::Range::Range() : _start{0}, _end{1}, _step{nullptr} {}
PyForStatement::Range::Range(int start, int end, ExprNode* step) : _start{start}, _end{end}, _step{step} {}

bool PyForStatement::Range::atEnd() {
    return _start >= _end;
}

//PROBLEM: multiplication by the index variable causes problems
void PyForStatement::Range::increment(SymTab &symTab, Functions &funcTab) {
    std::shared_ptr<TypeDescriptor> result = _step->evaluate(symTab, funcTab);
    //increase the value of 'start' by the value of 'step'
    if (result->type() == TypeDescriptor::INTEGER) {
        std::shared_ptr<NumberTypeDescriptor> num_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(result);
        if (num_ptr) {
            _start += num_ptr->value.intValue;
            std::cout << "ITERATOR'S INDEX IS NOW AT " << _start << std::endl;
        }
    }
}



int PyForStatement::Range::getCurrent () {
    return _start;
}



