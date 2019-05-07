#ifndef EXPRINTER_FUNCTIONS_HPP
#define EXPRINTER_FUNCTIONS_HPP
#include <memory>
#include <string>
#include <map>
#include <vector>

class Function;

class Functions {
public:
    void addFunction(std::string funcName, std::shared_ptr<Function>);
    bool isDefined(std::string funcName);
    std::shared_ptr<Function> getFunction(std::string funcName);

private:
    std::map<std::string, std::shared_ptr<Function> > funcTab;
};

#endif //EXPRINTER_FUNCTIONS_HPP
