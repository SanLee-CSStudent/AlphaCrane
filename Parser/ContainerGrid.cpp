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

void ContainerGrid::addContainer(int columnIndex, int rowIndex, Container c) {
  if (!this->checkIndexRange(rowIndex, columnIndex)) {
    // throw an error message and return;
    return;
  }

  grid[columnIndex][rowIndex] = c;
}

void ContainerGrid::print() {
  // just for the debugging purposes
  for (int i = 0; i < column; i++) {
    for (int j = 0; j < row; j++) {
      std::cout << i << j << grid[i][j].name << " " << grid[i][j].weight
                << " | ";
    }
    std::cout << std::endl;
  }
}
