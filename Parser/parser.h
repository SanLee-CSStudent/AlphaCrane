#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>
#include <iostream>

#include "ContainerGrid.h"
#include "Container.h"
#include "ContainerFactory.h"

class Parser{
    public:
        Parser(){
            // if needed create new constructor for passed ContainerGrid/ContainerFactory object
            containerFactory = new ContainerFactory(); 
            grid = new ContainerGrid(8, 12);
        }
        void parse(std::string filename);

    private:
        ContainerFactory* containerFactory;
        ContainerGrid* grid;
};

#endif