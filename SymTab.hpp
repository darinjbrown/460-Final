//
// Created 2/5/19
//

#ifndef EXPRINTER_SYMTAB_HPP
#define EXPRINTER_SYMTAB_HPP

#include <string>
#include <map>
#include <vector>
#include "Token.hpp"
#include <memory>


class Statements ; // forward declaration for the FunctionBlockStatements in class Function

// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.




struct TypeDescriptor {
    // The base class for datatype descriptors.

    // This enum can expand to include additional types.
    enum types {INTEGER, BOOL, STRING, VECTOR};

    TypeDescriptor(types type): _type{type} {}

    types &type() { return _type; }

    virtual ~TypeDescriptor() {}

private:
    types _type;
};

struct NumberTypeDescriptor: public TypeDescriptor {
    // Number descriptor includes integer, double, and boolean datatypes.

    NumberTypeDescriptor(types descType):  TypeDescriptor(descType) {}

    union {
	    int intValue;
        //float floatValue;
	    //double doubleValue;
	    short boolValue;
    } value;
};

struct StringTypeDescriptor: public TypeDescriptor {
    // String descriptor includes string datatypes

    StringTypeDescriptor(types descType):  TypeDescriptor(descType) {}
    std::string stringValue;
};

struct VectorTypeDescriptor: public TypeDescriptor{
  VectorTypeDescriptor(types descType): TypeDescriptor(descType){}
  std::string _name;

  void append(TypeDescriptor newElement);
  void setElement(int index, TypeDescriptor newElement);
  void initializeArray(std::string name, std::vector<TypeDescriptor> initialValues);

 private:
    types _elementType;
    types _type;
    std::vector<TypeDescriptor>* _array;

};

class SymTab {
public:
    SymTab();

    void setValueFor(std::string vName, std::shared_ptr<TypeDescriptor>);

    bool isDefined(std::string vName);
    std::shared_ptr<TypeDescriptor> getValueFor(std::string vName);
    //std::shared_ptr<TypeDescriptor> getReturnValue() const {return _returnValue;}
    bool hasReturnValue() const         {return _returnValue != nullptr; }
    std::shared_ptr<TypeDescriptor> getReturnValue();
    void setReturnValue(std::shared_ptr<TypeDescriptor>);


    int currentScope() const {return _currentScope;}
    void openNewScope(std::vector<std::string>);
    void closeScope();

private:
    // returning functions place thier value in here
    std::shared_ptr<TypeDescriptor> _returnValue;
    // this is a stack of symbol tables
    // index 0 contains the gobal scope
    std::vector< std::map<std::string, std::shared_ptr<TypeDescriptor> > > scopeStack;
    int _currentScope;
};

void printValue(std::shared_ptr<TypeDescriptor>);

/*
class Function {
public:
    Function();
    Function(std::vector<std::string> params, Statements* blockStatements);
    Statements*& getStatements()             {return _blockStatements;}
    std::vector<std::string> getParams()    {return _paramList;}
private:
    // parameter list
    std::vector<std::string> _paramList;
    // code block
    Statements* _blockStatements;
};



class Functions {
public:
    void addFunction(std::string funcName, std::shared_ptr<Function>);
    bool isDefined(std::string funcName);
    std::shared_ptr<Function> getFunction(std::string funcName);


private:
    std::map<std::string, std::shared_ptr<Function> > funcTab;
};
*/

#endif //EXPRINTER_SYMTAB_HPP
