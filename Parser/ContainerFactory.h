#ifndef CONTAINERFACTORY_H
#define CONTAINERFACTORY_H

#include <string>
#include "Container.h"

class ContainerFactory{
    public:
        ContainerFactory(){ }
        Container create(int weight, std::string name, int column, int row){
            Container c = {weight, name, {column, row}};

            return c;
        }
};

#endif