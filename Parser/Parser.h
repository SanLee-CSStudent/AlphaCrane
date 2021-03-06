#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>
#include <iostream>

#include "ContainerGrid.h"
#include "Container.h"
#include "ContainerFactory.h"

class Parser {
public:
    Parser() {
        containerFactory = new ContainerFactory();
        grid = new ContainerGrid(8, 12);
    }

    Parser(ContainerGrid* grid) {
        containerFactory = new ContainerFactory();
        this->grid = grid;
    }

    void parse(std::string filename);
    void createManifest(std::string filename);

    ContainerGrid* getParseGrid();
    ~Parser() {
        delete containerFactory;
        delete grid;
    }

private:
    ContainerFactory* containerFactory;
    ContainerGrid* grid;
};

#endif
