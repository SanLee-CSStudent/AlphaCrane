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

    Container c1 = testGrid->getContainer(8- 1, 2 -1);
    // successfully move container
    testGrid->moveContainer(c1, 8- 1, 9 -1);
    // print error message: USED up destination
    testGrid->moveContainer(c1, 8- 1, 3 -1);

    Container c2 = testGrid->getContainer(8- 1, 1 -1);
    // print error message: NAN
    testGrid->moveContainer(c2, 8- 1, 8 -1);

    Container c3 = testGrid->getContainer(8- 1, 2 -1);
    // print error message: UNUSED
    testGrid->moveContainer(c3, 8- 1, 8 -1);

    // moved container does not maintain
    Container c4 = testGrid->getContainer(8- 1, 9 -1);
    testGrid->moveContainer(c4, 6- 1, 2- 1);
    testGrid->print();
    parser.createManifest("..//Outputs//manifest.txt");

    return 0;
}