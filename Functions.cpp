#include <iostream>
#include "Functions.hpp"

void Functions::addFunction(std::string funcName, std::shared_ptr<Function> funcPtr){
    funcTab[funcName] = funcPtr;
}

bool Functions::isDefined(std::string funcName){
    return funcTab.find(funcName) != funcTab.end();
}

std::shared_ptr<Function> Functions::getFunction(std::string funcName){
    std::shared_ptr<Function> ptr = funcTab.find(funcName)->second;
    return ptr;
}