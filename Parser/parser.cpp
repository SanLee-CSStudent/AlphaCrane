#include "parser.h"

void Parser::parse(std::string filename){
    std::ifstream fin(filename);

    if(fin.fail()){
        return;
    }

    std::string line;
    while(std::getline(fin, line)){
        
        // std::cout << line << std::endl;
        std::string index = line.substr(1, 5);
        std::string weightLine = line.substr(10, 5);
        std::string name = line.substr(18, line.length() - 1);

        int column = atoi(index.substr(0, 2).data());
        int row = atoi(index.substr(3, 2).data());

        // printing column and row index
        // std::cout << column << " " << row << std::endl;

        int weight = atoi(weightLine.data());
        // printing weight
        // std::cout << weight << std::endl;

        Container newContainer = this->containerFactory->create(weight, name);
        this->grid->addContainer(8 - column, row - 1, newContainer);
    }

    fin.close();
}