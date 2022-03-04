#include <iostream>

#include "Container.h"
#include "ContainerGrid.h"
#include "parser.h"

int main(){
    /*
    Container test = Container();
    test.name = "CONTAINER";
    test.weight = 0;

    // 3 x 2 grid
    ContainerGrid testGrid = ContainerGrid(3, 2);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            Container t = Container();
            t.name = "C";
            t.weight = i + j;
            testGrid.addContainer(i, j, t);
        }
    }

    testGrid.print();
    */
    ContainerGrid* testGrid = new ContainerGrid(8, 12);
    Parser parser = Parser(testGrid);
    std::string file = "..//Manifests//ShipCase1.txt";
    parser.parse(file);

    // testGrid->print();
    parser.createManifest("..//Outputs//manifest.txt");

    return 0;
}