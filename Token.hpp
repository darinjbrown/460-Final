//
// Created by Ali A. Kooshesh on 1/30/19.
//

// whenever a new number type is added, it needs to be added into the check of 'isNumeric'



#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
#include <string>
#include <iostream>

class Token {

public:
    Token();

    bool &eof()  { return _eof; }
    bool &eol()  { return _eol; }

    bool eof() const { return _eof; }
    bool eol() const  { return _eol; }

    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }

    void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }

    void stringSymbol(std::string s) { _stringSymbol = s; }
    std::string stringSymbol() { return _stringSymbol; }

    bool isStringSymbol() const     { return _stringSymbol.length() > 0;}

    bool isOpenBrackets() const     { return _symbol == '{'; }
    bool isClosedBrackets() const   { return _symbol == '}'; }
    bool isSemiColon() const        { return _symbol == ';'; }
    bool isComma() const            { return _symbol == ','; }
    bool isColon() const            { return _symbol == ':'; }

    bool isAssignmentOperator() const     { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
    bool isIntegerDivisionOperator() const       { return _stringSymbol == "//"; }
    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator();
    }
    bool isLessThanCompairson() const { return _stringSymbol == "<"; }
    bool isGreaterThanCompairson() const { return _stringSymbol == ">"; }
    bool isLessThanOrEqualCompairson() const { return _stringSymbol == "<="; }
    bool isGreaterThanOrEqualCompairson() const { return _stringSymbol == ">="; }
    bool isEqualCompairson() const { return _stringSymbol == "=="; }
    bool isNotEqualCompairson() const { return _stringSymbol == "!="; }
    bool isCompairsonOperator() const {
        return  isLessThanCompairson() ||
                isLessThanOrEqualCompairson() ||
                isGreaterThanCompairson() ||
                isGreaterThanOrEqualCompairson() ||
                isEqualCompairson() ||
                isNotEqualCompairson();
    }
    bool isArray() const     { return _isArray; }
    bool isBooleanOr() const { return _name == "or"; }
    bool isBooleanAnd() const { return _name == "and"; }
    bool isBooleanNot() const { return _name == "not"; }
    bool isBoolean() const {
        return isBooleanOr() ||
        isBooleanAnd() ||
        isBooleanNot();
    }
    
    bool isName() const                             { return _name.length() > 0; }
    std::string getName() const                     { return _name; }
    void setName(std::string n) { _name = n; }

    bool &isFuncName()             { return _isFuncName;}
    bool isFuncName()  const       { return _isFuncName;}

    
    bool isPrintKeyword() const                       { return _name == "print"; }

    bool isForLoopKeyword() const                     { return _name == "for"; }
    bool isIfStatementKeyword() const                 { return _name == "if"; }
    bool isFuncDefKeyword() const                     { return _name == "def"; }
    bool isReturnStatement() const                    { return _name == "return";}


    bool isCompoundStatement() const {
        return isForLoopKeyword()
          || isIfStatementKeyword()
          || isFuncDefKeyword();
    }

    bool isSimpleStatement() const {
        return !isCompoundStatement();
    }



    bool &isInteger()                           { return _isinteger; }
    bool isInteger() const                      { return _isinteger; }
    int getInteger() const                      { return _integer; }
    void setInteger(int n) {
        _integer = n;
        isInteger() = true;
    }
//    void setNegSign(){
//        bool _negSign;
//    };
    /*
    bool &isFloat()             { return _isFloat;}
    bool isFloat()  const       { return _isFloat;}
    float getFloat() const      {return _float;}
    void setFloat(float f) {
        _float = f;
        isFloat() = true;
    }
    bool &isDouble()             { return _isDouble;}
    bool isDouble()  const       { return _isDouble;}
    float getDouble() const      {return _double;}
    void setDouble(double d) {
        _double = d;
        isDouble() = true;
    }
    */
    
    bool getBoolean() const      { return _boolean;}
    void setBoolean(bool b) {
        _boolean = b;
        _isBoolean = true;
    }


    bool isNumeric() {return isInteger() || isBoolean() ; }


    bool &isString()    {return _isString; }
    bool isString() const    {return _isString; }
    std::string getString() const   {return _string; }
    void setString(std::string s) {
        _string = s;
        _isString = true;
    }

    bool isIndent() const   { return _isIndent; }
    bool &isIndent()        { return _isIndent; }
    bool isDedent() const   { return _isDedent; }
    bool &isDedent()        { return _isDedent; }

    //dedent and indent tokens store the number of thier indent in the tokens _integer field
    void setIndent(int n){
        _integer = n;
        _isIndent = true;
    }
    void setDedent(int n){
        _integer = n;
        _isDedent = true;
    }

    void print() const;

private:
    
    bool _eof, _eol;
    bool _isIndent, _isDedent;

    bool _isinteger;
    //bool _isFloat;
    //bool _isDouble;
    bool _isBoolean;

    bool _isArray;
    bool _isString;

    bool _isFuncName;

    std::string _name;
    char _symbol;
    std::string _stringSymbol; // holds ">=" , "<=" , "!=" , "==", "<", ">", "//"


    int _integer;
    //float _float;
    //double _double;
    short _boolean;
    std::string _string;
};

#endif //EXPRINTER_TOKEN_HPP
