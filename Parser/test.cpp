#include <iostream>

#include "Container.h"

int main(){
    Container test = Container();
    test.name = "CONTAINER";
    test.weight = 0;

    std::cout << test.name << ", " << test.weight << std::endl;

    return 0;
}