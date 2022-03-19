#pragma once
#ifndef CONTAINERGRID_H
#define CONTAINERGRID_H

#include "Container.h"
#include <iostream>

//CHANGING ALL CHARS TO INTS TO TEST
class ContainerGrid {
public:
    // empty constructor
    ContainerGrid();
    ContainerGrid(char, char);

    // debugging function
    void print();

    // getter function
    char getRow() { return row; }
    char getColumn() { return column; }

    // bottom-left = [column - 1][0]
    // upper-right = [0][row - 1]
    Container getContainer(char columnIndex, char rowIndex) { return grid[columnIndex][rowIndex]; }

    // mutator function
    void addContainer(char, char, Container);
    void moveContainer(Container, char, char);

    void emptyContainer(int columnIndex, int rowIndex) {
        if (isEmpty(columnIndex, rowIndex)) {
            return;
        }

        Container* empty = &grid[columnIndex][rowIndex];
        empty->name = "UNUSED";
        empty->weight = 0;
    }

    ~ContainerGrid() {
        for (char i = 0; i < column; i++) {
            delete[] grid[i];
        }
    }

private:
    char row;
    char column;
    Container** grid;

    bool checkIndexRange(char rowIndex, char columnIndex) {
        if (rowIndex < 0 || rowIndex >= this->row) { return false; }
        if (columnIndex < 0 || columnIndex >= this->column) { return false; }
        return true;
    }

    bool compare(Container c1, Container c2) {
        if (c1.name == c2.name &&
            c1.weight == c2.weight) {
            return true;
        }

        return false;
    }

    bool checkStatus(char columnIndex, char rowIndex) {
        if (getContainer(columnIndex, rowIndex).name == "NAN") {
            // print error message as needed
            return false;
        }
        return true;
    }

    bool isEmpty(char columnIndex, char rowIndex) {
        if (grid[columnIndex][rowIndex].name == "UNUSED") {
            return true;
        }

        return false;
    }

    /*
        EMPTY CONTAINER USED TO BE HERE
        MOVING TO PUBLIC FUNCTIONS
    */
    
};

#endif