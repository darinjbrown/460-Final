//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include "Token.hpp"

Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isinteger{false},
    _isBoolean{false}, _isString{false}, _isFuncName{false}, _stringSymbol{""}, _isIndent(false), _isDedent(false) {}

void Token::print() const {
    if( eol() ) std::cout << "EOL\n" ;
    else if( eof() )                    std::cout << "EOF" ;
    else if( isOpenParen() )  std::cout << "(" ;
    else if( isCloseParen() )  std::cout << ")" ;
    else if( isOpenBrackets() ) std::cout << "{";
    else if( isClosedBrackets() ) std::cout << "}";
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isSemiColon() )  std::cout << ";" ;
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if( isIntegerDivisionOperator() ) std::cout << " // ";
    else if( isLessThanCompairson() ) std::cout <<  " < ";
    else if( isGreaterThanCompairson() ) std::cout << " > ";
    else if( isLessThanOrEqualCompairson() ) std::cout << " <= ";
    else if( isGreaterThanOrEqualCompairson() ) std::cout << " >= ";
    else if( isEqualCompairson() ) std::cout << " == ";
    else if( isNotEqualCompairson() ) std::cout << " != ";
    else if( isColon() ) std::cout << " : ";
    else if( isComma() ) std::cout << " , ";
    else if( isBooleanOr() ) std::cout << " or ";
    else if( isBooleanAnd() ) std::cout << " and ";
    else if( isBooleanNot() ) std::cout << " not ";
    else if( isName() )  std::cout << getName();
    else if( isInteger() ) std::cout << getInteger();
    //else if( isFloat() ) std::cout << getFloat();
    //else if( isDouble() ) std::cout << getDouble();
    else if( isBoolean() ) std::cout << getBoolean();
    else if( isString() ) std::cout << getString();
    else if( isIndent() ){
        std::cout << "INDENT Token: " << getInteger() << " spaces." ;
    }
    else if( isDedent() ){
        std::cout << "DEDENT Token: " << getInteger() << " spaces." ;
    }
    else std::cout << "Uninitialized token.\n";
}
