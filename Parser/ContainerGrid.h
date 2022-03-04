#ifndef CONTAINERGRID_H
#define CONTAINERGRID_H

#include "Container.h"
#include <iostream>

class ContainerGrid{
    public:
        // empty constructor
        ContainerGrid();
        ContainerGrid(int, int);

        // debugging function
        void print();

        // getter function
        int getRow(){ return row; }
        int getColumn(){ return column; }
        // bottom-left = [column - 1][0]
        // upper-right = [0][row - 1]
        Container getContainer(int rowIndex, int columnIndex){ return grid[rowIndex][columnIndex]; }

        // mutator function
        void addContainer(int, int, Container);

        ~ContainerGrid(){
            for(int i = 0; i < column; i++){
                delete[] grid[i];
            }
        }

    private:
        int row;
        int column;
        Container** grid;

        bool checkIndexRange(int rowIndex, int columnIndex){
            if(rowIndex < 0 || rowIndex >= this->row){ return false; }
            if(columnIndex < 0 || columnIndex >= this->column){ return false; }
            return true;
        }

};

#endif