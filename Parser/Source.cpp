#include "Balance.h"
#include "Container.h"
#include "ContainerGrid.h"
#include "Node.h"
#include "Parser.h"
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using namespace std;

ostream &operator<<(ostream &out, Point point) {
  out << (int)point.column << "," << (int)point.row;
  return out;
}
ostream &operator<<(ostream &out, pair<Point, Point> from_to) {
  out << from_to.first << "->" << from_to.second << endl;
  return out;
}

struct MoveInfo {
  string name;
  pair<Point, Point> from_to;
};
MoveInfo GetMoveInfo(Node *from, Node *to) {
  ContainerGrid *from_grid = from->getData();
  ContainerGrid *to_grid = to->getData();
  MoveInfo mI;
  bool found;
  for (char r = 0; r < from_grid->getRow(); r++) {
    for (char c = 0; c < from_grid->getColumn(); c++) {
      const Container from = from_grid->getContainer(c, r);
      const Container to = to_grid->getContainer(c, r);
      if (from.name != to.name) {
        if (found) {
          if (mI.name == from.name) {
            mI.from_to.first = from.position;
          } else {
            mI.from_to.second = from.position;
          }
        }

        mI.name = (from.name == "UNUSED") ? to.name : from.name;
        if (from.name == "UNUSED") {
          mI.from_to.second = to.position;
        } else {
          mI.from_to.first = to.position;
        }
        found = true;
      }
    }
  }
  return mI;
}
int main() {
  ContainerGrid *testGrid = new ContainerGrid(8, 12);
  ContainerGrid *goalGrid = new ContainerGrid(8, 12);
  ContainerGrid *bufferGrid = new ContainerGrid(4, 24);

  for (char col = 0; col < 4;
       col++) // use addContainer to fill bufferGrid with UNUSED space
  {
    for (char row = 0; row < 24; row++) {
      Container empty = {0, "UNUSED", {col, row}};
      bufferGrid->addContainer(col, row, empty);
    }
  }

  Parser parser = Parser(testGrid);
  string file = "/home/thuanvu/project/AlphaCrane/ShipCases/"
                "ShipCase2.txt";
  parser.parse(file);

  Node *startNode =
      new Node(nullptr, parser.getParseGrid(), goalGrid,
               bufferGrid); // initialize blank goal state and buffer
  cout << "We are asked to balance the following ship: " << endl;
  startNode->getData()->print();
  cout << endl << endl;
  startNode->calcBalanceCost();
  cout << endl << "startNode size is: " << sizeof(*startNode) << endl;
  cout << "It has a starting cost of: " << startNode->getCost()
       << " with an gCost of: " << startNode->getGCost()
       << " and an hCost of: " << startNode->getHCost() << endl;
  Balance *balance = new Balance(startNode);
  Node *solution = balance->aStarSearch();
  stack<Node *> trace;
  while (solution->getParent() != nullptr) // fill trace stack
  {
    trace.push(solution);
    solution = solution->getParent();
  }

  trace.push(solution);
  cout << endl << endl << endl << "Now printing trace: " << endl << endl;
  while (trace.size() > 1) {
    Node *currentNode = trace.top();
    trace.pop();
    Node *nextNode = trace.top();
    currentNode->getData()->print();
    if (nextNode == nullptr)
      break;
    cout << nextNode << " " << currentNode << endl;
    MoveInfo mi = GetMoveInfo(currentNode, nextNode);
    cout << mi.name << " " << mi.from_to << std::endl;
  }

  return 0;
}
