#include "ContainerGrid.h"

ContainerGrid::ContainerGrid() {
  row = 0;
  column = 0;
  grid = nullptr;
}

ContainerGrid::ContainerGrid(int column, int row) : row(row), column(column) {
  grid = new Container *[this->column];
  for (int i = 0; i < this->column; i++) {
    grid[i] = new Container[this->row];
  }
}

<<<<<<< HEAD
void ContainerGrid::addContainer(int columnIndex, int rowIndex, Container c) {
  if (!this->checkIndexRange(rowIndex, columnIndex)) {
    // throw an error message and return;
    return;
  }
=======
void ContainerGrid::addContainer(int columnIndex, int rowIndex, Container c){
    if(!this->checkIndexRange(rowIndex, columnIndex)){
        // throw an error message and return;
        std::cout << "Row or column index out of range" << std::endl;
        return;
    }
>>>>>>> main

  grid[columnIndex][rowIndex] = c;
}

<<<<<<< HEAD
void ContainerGrid::print() {
  // just for the debugging purposes
  for (int i = 0; i < column; i++) {
    for (int j = 0; j < row; j++) {
      std::cout << i << j << grid[i][j].name << " " << grid[i][j].weight
                << " | ";
=======
// move container c to destination column and row indices
// think of column and row to be [8 - actual column number in manifest][actual row number in manifest - 1]
void ContainerGrid::moveContainer(Container c, int columnIndex, int rowIndex){
    if(!this->checkIndexRange(rowIndex, columnIndex)){
        // throw an error message and return;
        std::cout << "Row or column index out of range" << std::endl;
        return;
    }
    // when start or destination is NAN
    if(!this->checkStatus(columnIndex, rowIndex) || !this->checkStatus(c.position.column, c.position.row)){
        std::cout << "Origin or Destination is unavailable" << std::endl;
        return;
    }
    // if destination is already used up
    if(!isEmpty(columnIndex, rowIndex)){
        std::cout << "Destination is already being used" << std::endl;
        return;
    }
    // if origin is UNUSED
    if(isEmpty(c.position.column, c.position.row)){
        std::cout << "Origin is empty" << std::endl;
        return;
    }

    Container* destination = &grid[columnIndex][rowIndex];

    destination->name = c.name;
    destination->weight = c.weight;

    // make starting position UNUSED
    emptyContainer(c.position.column, c.position.row);
}

void ContainerGrid::print(){
    // just for the debugging purposes
    for(int i = 0; i < column; i++){
        for(int j = 0; j < row; j++){
            Container temp = grid[i][j];
            std::cout << temp.name << " " << temp.weight << " | ";
            // std::cout << temp.name << " " << temp.weight <<  " [" << temp.position.column << "," << temp.position.row << "]" << " | ";
        }
        std::cout << std::endl;
>>>>>>> main
    }
    std::cout << std::endl;
  }
}
