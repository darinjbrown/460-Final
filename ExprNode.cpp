//
// Created 2/5/19
//

#include <iostream>
#include "ExprNode.hpp"

// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}
ExprNode *&InfixExprNode::left() { return _left; }
ExprNode *&InfixExprNode::right() { return _right; }

UnaryExprNode::UnaryExprNode(Token tk) : ExprNode{tk}, _left{nullptr}{}
ExprNode *&UnaryExprNode::left() { return _left; }
void UnaryExprNode::print(){
    token().print();
    left()->print();
}

std::shared_ptr<TypeDescriptor> UnaryExprNode::evaluate(SymTab &symTab, Functions &funcTab){
    std::shared_ptr<TypeDescriptor> lValue = left()->evaluate(symTab, funcTab);

    std::cout << "UnaryExprNode::evaluating: ";
    token().print();
    printValue(lValue);

    if ( token().isBooleanNot() ){
        if(lValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            if(lVal_ptr){
                if(lVal_ptr->value.intValue == 0){   // value != 0, 'not' evaulates true
                    lVal_ptr->value.intValue = 1;
                    return lVal_ptr;
                }
                else{
                    lVal_ptr->value.intValue = 0;
                    return lVal_ptr;
                }
            }
        }
        if(lValue->type() == TypeDescriptor::BOOL){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            if(lVal_ptr){
                if(lVal_ptr->value.boolValue == 0){   // value != 0, 'not' evaulates true
                    lVal_ptr->value.intValue = 1;
                    return lVal_ptr;
                }
                else{
                    lVal_ptr->value.boolValue = 0;
                    return lVal_ptr;
                }
            }
        }
        else{
            std::cout << "InfixExprNode::evaluate, ERROR: Invalid unary operand.";
            exit(1);
        }
    }
}






// this return value is what we use to set our symbol's key, which is a TypeDescriptor.
// REPLACE ALL INT VALUES WITH TYPEDESCRIPTOR POINTERS

std::shared_ptr<TypeDescriptor> InfixExprNode::evaluate(SymTab &symTab, Functions &funcTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    // These are actually child pointers of type descriptor and must be downcast.

    // split these up to enable unary operations?
    std::shared_ptr<TypeDescriptor> lValue = left()->evaluate(symTab, funcTab);
    std::shared_ptr<TypeDescriptor> rValue = right()->evaluate(symTab, funcTab);

    std::cout << "InfixExprNode::evaluating: ";
    printValue(lValue);
    token().print();
    printValue(rValue);
    std::cout << std::endl;

    if( token().isBooleanAnd() ){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = rVal_ptr->value.intValue && lVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for boolean 'and'.\n";
            exit(2);
        }
    }
    else if( token().isBooleanOr() ){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = rVal_ptr->value.intValue || lVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for boolean 'and'.\n";
            exit(2);
        }
    }
    else if( token().isAdditionOperator() ){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue + rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else if(lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::STRING){
            std::shared_ptr<StringTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(lValue);
            std::shared_ptr<StringTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->stringValue + rVal_ptr->stringValue;
                std::shared_ptr<StringTypeDescriptor> new_ptr = std::make_shared<StringTypeDescriptor>(TypeDescriptor::STRING);
                new_ptr->stringValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for boolean.\n";
            exit(2);
        }
    }   
    else if(token().isSubtractionOperator()){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue - rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::BOOL){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue - rVal_ptr->value.boolValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else if(lValue->type() == TypeDescriptor::BOOL && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.boolValue - rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for subtract.\n";
            exit(2);
        }
    }
    else if(token().isMultiplicationOperator()){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue * rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for multiplication.\n";
            exit(2);
        }
    }
    else if(token().isDivisionOperator()){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue / rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for division.\n";
            exit(2);
        }
    }
    // doesn't properly do integer divison.
    else if(token().isIntegerDivisionOperator()){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue / rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for integer division.\n";
            exit(2);
        }
    }
    else if( token().isModuloOperator() ){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue % rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for modulus.\n";
            exit(2);
        }
    }
    // we assume the compairsons return an int with no problems.
    else if ( token().isEqualCompairson() ){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue == rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else if(lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::STRING){
            std::shared_ptr<StringTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(lValue);
            std::shared_ptr<StringTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->stringValue == rVal_ptr->stringValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for compairson.\n";
            exit(2);
        }
    }

    else if ( token().isNotEqualCompairson() ){
    
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue != rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else if(lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::STRING){
            std::shared_ptr<StringTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(lValue);
            std::shared_ptr<StringTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->stringValue.length() != rVal_ptr->stringValue.length();
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for compairson.\n";
            exit(2);
        }
    }
    else if ( token().isLessThanCompairson() ){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue < rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else if(lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::STRING){
            std::shared_ptr<StringTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(lValue);
            std::shared_ptr<StringTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->stringValue.length() < rVal_ptr->stringValue.length();
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for less than compairson.\n";
            exit(2);
        }
    }

    else if ( token().isGreaterThanCompairson() ){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue > rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else if(lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::STRING){
            std::shared_ptr<StringTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(lValue);
            std::shared_ptr<StringTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->stringValue.length() > rVal_ptr->stringValue.length();
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for greater than compairson.\n";
            exit(2);
        }
    }
    else if ( token().isLessThanOrEqualCompairson() ){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue <= rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else if(lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::STRING){
            std::shared_ptr<StringTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(lValue);
            std::shared_ptr<StringTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->stringValue.length() <= rVal_ptr->stringValue.length();
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for less than or equal compairson.\n";
            exit(2);
        }
    }
    else if ( token().isGreaterThanOrEqualCompairson() ){
        if(lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER){
            std::shared_ptr<NumberTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(lValue);
            std::shared_ptr<NumberTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->value.intValue >= rVal_ptr->value.intValue;
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else if(lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::STRING){
            std::shared_ptr<StringTypeDescriptor> lVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(lValue);
            std::shared_ptr<StringTypeDescriptor> rVal_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(rValue);
            if(lVal_ptr && rVal_ptr){
                auto result = lVal_ptr->stringValue.length() >= rVal_ptr->stringValue.length();
                std::shared_ptr<NumberTypeDescriptor> new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
                new_ptr->value.intValue = result;
                return new_ptr;
            }
        }
        else{
            std::cout << "Error: invalid operands for greater than or equal compairson.\n";
            exit(2);
        }
    }
    else {
        std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }
    
    //return new NumberTypeDescriptor(TypeDescriptor::INTEGER);
}

void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();

}

// OperatorNode stores a infix operation token.
// Created because parser::comp_op() needs to return an ExprNode which is used by
//      parser::compairson() to build an infix expression node.
OperatorNode::OperatorNode(Token token): ExprNode{token} {}

//stores the operator in a string. Not immediately used: the is<some operator>() token functions
//      are used to check what the token is, rather then direct compairsons
std::shared_ptr<TypeDescriptor> OperatorNode::evaluate(SymTab &symTab, Functions &funcTab){
    std::shared_ptr<StringTypeDescriptor> td_ptr = std::make_shared<StringTypeDescriptor>(TypeDescriptor::STRING);
    if(token().isStringSymbol()){
        td_ptr->stringValue = token().stringSymbol();
    }
    else{
        std::cout << "We are assigning the symbol to our Operator Node...\n";
        std::string s(1, token().symbol());
        td_ptr->stringValue = s;
    }
    return td_ptr;
}

void OperatorNode::print(){
    token().print();
}



// NumberNode stores a token that is (isNumeric)
// NumberNode's evaulate yields a type descriptor with some values type, and actual value.
// This comes from a stored token. 

NumberNode::NumberNode(Token token): ExprNode{token} {}

std::shared_ptr<TypeDescriptor> NumberNode::evaluate(SymTab &symTab, Functions &funcTab) {

    if(token().isInteger()){
        std::shared_ptr<NumberTypeDescriptor> td_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);
        td_ptr->value.intValue = token().getInteger();
        return td_ptr;
    }

}

void NumberNode::print() {
    token().print();
}


StringNode::StringNode(Token token): ExprNode{token} {}

std::shared_ptr<TypeDescriptor> StringNode::evaluate(SymTab &symTab, Functions &funcTab) {

    if(token().isString()){
       std::shared_ptr<StringTypeDescriptor> td_ptr = std::make_shared<StringTypeDescriptor>(TypeDescriptor::STRING);
       td_ptr->stringValue = token().getString();
       return td_ptr;
    }

}

void StringNode::print() {
    std::cout << "\"";
    token().print();
    std::cout << "\"";
}



// Variables return the variable value from the symbol table.
// A variable node will not hold a variable for an assignment statement? (assumption)

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

std::shared_ptr<TypeDescriptor> Variable::evaluate(SymTab &symTab, Functions &funcTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    std::shared_ptr<TypeDescriptor> td_ptr = symTab.getValueFor(token().getName());
    std::cout << "Variable::evaluate: returning ";
    printValue(td_ptr);
    std::cout << std::endl;
    return td_ptr;
}

//Vector node
Vector::Vector(Token token): ExprNode{token} {}

void Vector::print(){
    std::cout << token().getName();
    std::cout << " contains: [";
    for(auto a : _arrayElements){
        a->print();
        std::cout << ", ";
    }
    std::cout << "]\n";
}

std::shared_ptr<TypeDescriptor> Vector::evaluate(SymTab &symTab, Function &funcTab) {

    std::vector<std::shared_ptr<TypeDescriptor> > arrayName;
    std::shared_ptr<VectorTypeDescriptor> vec_ptr = std::make_shared<VectorTypeDescriptor>(TypeDescriptor::VECTOR);
    vec_ptr->_array = arrayName;
    return vec_ptr;
}

// token is the function name
// this needs to be a self-contained call, since it can appear outside of a call statement
CallFunctionNode::CallFunctionNode(Token token) : ExprNode{token} {}

void CallFunctionNode::print(){
    std::cout << token().getName();
    std::cout << "( ";
    for(auto a : _function_args){
        a->print();
        std::cout << ", ";
    }
    std::cout << ")\n";
}

// this executes the call and return cycle.
std::shared_ptr<TypeDescriptor> CallFunctionNode::evaluate(SymTab &symTab, Functions &funcTab){

    std::shared_ptr<Function> func_ptr = funcTab.getFunction( token().getName() );
    std::vector<std::string> params_vector = func_ptr->getParams();

    //calculate arguements, save them into the new map and open a new scope with that
  
    if(params_vector.size() != _function_args.size()){
        std::cout << "Error: incorrect number of arguements (expected " << params_vector.size() << "), got " <<  _function_args.size() << std::endl;
        exit(1);
    }
    else{
        for(int i = 0; i < _function_args.size(); i++){
            // this sets the value for the ith parameter with the ith value in the current scope
            // when we open a scope, we pass it a list of the names of variables to copy over from the current scope into the new one
            symTab.setValueFor( params_vector[i] , _function_args[i]->evaluate(symTab, funcTab) ); 
        }
    }

    //open scope with the names of the arguements
    //defines these arguements in the new scope
    symTab.openNewScope(params_vector);

    //execute function's statements
    //when a return statement is encountered, stop execution and save it in the symbTab::_returnValue
    auto stmts_ptr = func_ptr->getStatements();
    
    stmts_ptr->evaluate(symTab, funcTab);

    //close scope
    symTab.closeScope();

    return symTab.getReturnValue();

}