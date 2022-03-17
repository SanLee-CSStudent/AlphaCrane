#ifndef CONTAINERFACTORY_H
#define CONTAINERFACTORY_H

#include "Container.h"
#include <string>

class ContainerFactory {
public:
  ContainerFactory() {}
  Container create(int weight, std::string name) {
    Container c = Container();
    c.weight = weight;
    c.name = name;

    return c;
  }
};

#endif
