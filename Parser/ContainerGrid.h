#ifndef CONTAINERGRID_H
#define CONTAINERGRID_H

#include "Container.h"

class ContainerGrid{
    public:
        // empty constructor
        ContainerGrid ConatinerGrid();

        void print();
        int getRow(){ return row; }
        int getColumn(){ return column; }
        Container getContainer(int rowIndex, int columnIndex){ return grid[rowIndex][columnIndex]; }
        void addContainer(int, int, Container);

    private:
        int row = 12;
        int column = 8;
        Container grid[8][12];

        bool checkIndexRange(int rowIndex, int columnIndex){
            if(rowIndex < 0 || rowIndex > 11){ return false; }
            if(columnIndex < 0 || columnIndex > 7){ return false; }
            return true;
        }
};

#endif