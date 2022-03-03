#include "ContainerGrid.h"

ContainerGrid::ContainerGrid(){

}

void ContainerGrid::addContainer(int rowIndex, int columnIndex, Container c){
    if(this->checkIndexRange(rowIndex, columnIndex)){
        // throw an error message and return;
        return;
    }
    
    grid[columnIndex][rowIndex] = c;
}