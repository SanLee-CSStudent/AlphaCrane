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
        Container getContainer(int columnIndex, int rowIndex){ return grid[columnIndex][rowIndex]; }

        // mutator function
        void addContainer(int, int, Container);
        void moveContainer(Container, int, int);

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

        bool compare(Container c1, Container c2){
            if(c1.name == c2.name && 
                c1.weight == c2.weight){
                    return true;
            }

            return false;
        }

        bool checkStatus(int columnIndex, int rowIndex){
            if(getContainer(columnIndex, rowIndex).name == "NAN" ){
                // print error message as needed
                return false;
            }
            return true;
        }

        bool isEmpty(int columnIndex, int rowIndex){
            if(grid[columnIndex][rowIndex].name == "UNUSED"){
                return true;
            }

            return false;
        }

        void emptyContainer(int columnIndex, int rowIndex){
            if(isEmpty(columnIndex, rowIndex)){
                return;
            }

            Container* empty = &grid[columnIndex][rowIndex];
            empty->name = "UNUSED";
            empty->weight = 0;
        }
};

#endif