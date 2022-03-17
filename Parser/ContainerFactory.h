#ifndef CONTAINERFACTORY_H
#define CONTAINERFACTORY_H

#include "Container.h"
#include <string>

<<<<<<< HEAD
class ContainerFactory {
public:
  ContainerFactory() {}
  Container create(int weight, std::string name) {
    Container c = Container();
    c.weight = weight;
    c.name = name;
=======
class ContainerFactory{
    public:
        ContainerFactory(){ }
        Container create(int weight, std::string name, int column, int row){
            Container c = {weight, name, {column, row}};
>>>>>>> main

    return c;
  }
};

#endif
