#ifndef CONTAINER_H
#define CONTAINER_H

#include <string>
#include "Point.h"

struct Container {
    int weight;
    std::string name;
    Point position;
};

#endif