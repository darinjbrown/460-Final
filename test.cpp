#include <iostream>
#include <string>
#include <memory>
#include <vector>

int main(){

    std::vector<int> foo;
    foo.push_back(1);
    foo.push_back(2);

    std::cout << foo.size() << std::endl;


    return 0;
}