#include "Balance.h"
#include "Container.h"
#include "ContainerGrid.h"
#include "Node.h"
#include "parser.h"
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
using namespace std;

namespace Loader {
vector<string> unload(const vector<Container> &unload) {
  ContainerGrid *testGrid = new ContainerGrid(8, 12);
  ContainerGrid *goalGrid = new ContainerGrid(8, 12);
  ContainerGrid *bufferGrid = new ContainerGrid(4, 24);

  for (int col = 0; col < 4;
       col++) // use addContainer to fill bufferGrid with UNUSED space
  {
    for (int row = 0; row < 24; row++) {
      Container empty = {0, "UNUSED", {col, row}};
      bufferGrid->addContainer(col, row, empty);
    }
  }

  Parser parser = Parser(testGrid);
  string file = "C:\\Users\\amidd\\source\\repos\\Balance Algorithm "
                "CS179\\Balance Algorithm CS179\\ShipCase5.txt";
  parser.parse(file);

  Node *startNode =
      new Node(nullptr, parser.getParseGrid(), goalGrid,
               bufferGrid); // initialize blank goal state and buffer
  startNode->setMoveContainers(vector<Container>(),
                               unload); // for testing loading/unloading

  Balance *balance = new Balance(startNode);
  Node *solution = balance->aStarSearch(2); // for testing movement
  stack<Node *> trace;
  while (solution->getParent() != nullptr) // fill trace stack
  {
    trace.push(solution);
    solution = solution->getParent();
  }
  trace.push(solution);
  cout << endl << endl << endl << "Now printing trace: " << endl << endl;
  while (trace.size() != 0) {
    Node *printNode = trace.top();
    cout << endl;
    printNode->getData()->print();
    cout << endl;
    trace.pop();
  }

  return 0;
}
} // namespace Loader
