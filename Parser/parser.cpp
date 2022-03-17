#include "parser.h"

void Parser::parse(std::string filename) {
  std::ifstream fin(filename);

  if (fin.fail()) {
    return;
  }

  std::string line;
  while (std::getline(fin, line)) {

    // std::cout << line << std::endl;
    std::string index = line.substr(1, 5);
    std::string weightLine = line.substr(10, 5);
    std::string name = line.substr(18, line.length() - 1);

    int column = atoi(index.substr(0, 2).data());
    int row = atoi(index.substr(3, 2).data());

    // printing column and row index
    // std::cout << column << " " << row << std::endl;

<<<<<<< HEAD
    int weight = atoi(weightLine.data());
    // printing weight
    // std::cout << weight << std::endl;
=======
        int containerColumn = 8 - column;
        int containerRow = row - 1;
        Container newContainer = this->containerFactory->create(weight, name, containerColumn, containerRow);
        this->grid->addContainer(containerColumn, containerRow, newContainer);
    }
>>>>>>> main

    Container newContainer = this->containerFactory->create(weight, name);
    this->grid->addContainer(8 - column, row - 1, newContainer);
  }

  fin.close();
}

void Parser::createManifest(std::string filename) {
  std::ofstream fout(filename);

  if (fout.fail()) {
    return;
  }
  for (int i = grid->getColumn() - 1; i >= 0; i--) {
    for (int j = 0; j < grid->getRow(); j++) {
      std::string line;
      Container c = grid->getContainer(i, j);
      std::string columnIndex = std::to_string(grid->getColumn() - i);
      std::string column = "";
      if (i < 10) {
        column.append("0");
      }
      column.append(columnIndex);

      std::string rowIndex = std::to_string(j + 1);
      std::string row = "";
      if (j < 9) {
        row.append("0");
      }
      row.append(rowIndex);

      // add position of container
      line.append("[");
      line.append(column);
      line.append(",");
      line.append(row);
      line.append("], ");

      std::string weightAmount = std::to_string(c.weight);
      int weightLimit = 5 - weightAmount.length();

      std::string weight;
      for (int i = 0; i < weightLimit; i++) {
        weight.append("0");
      }
      weight.append(weightAmount);

      // add weight of container
      line.append("{");
      line.append(weight);
      line.append("}, ");

      line.append(c.name);
      line.append("\n");

      fout << line;
    }
  }
  fout.close();
}
