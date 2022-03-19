#include "Balance.h"
#include "Node.h"

#include <sstream>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

ostream &operator<<(ostream &out, Point point) {
  out << (int)point.column << "," << (int)point.row;
  return out;
}
ostream &operator<<(ostream &out, pair<Point, Point> from_to) {
  out << from_to.first << " -> " << from_to.second << endl;
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

vector<string> Unload(const vector<Container> &unload,
                      ContainerGrid *parserGrid) {
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

  Node *startNode =
      new Node(nullptr, parserGrid, goalGrid,
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
  vector<string> vec_string;
  stringstream ss;
  std::cout << trace.size() << std::endl;
  while (trace.size() > 1) {
    Node *currentNode = trace.top();
    trace.pop();
    Node *nextNode = trace.top();
    if (nextNode == nullptr)
      break;
    MoveInfo mi = GetMoveInfo(currentNode, nextNode);
    ss << mi.name << ": " << mi.from_to;
    vec_string.push_back(ss.str());
    ss.str("");
  }

  return vec_string;
}

vector<string> Load(const vector<Container> &load, ContainerGrid *parserGrid) {
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

  Node *startNode =
      new Node(nullptr, parserGrid, goalGrid,
               bufferGrid); // initialize blank goal state and buffer
  startNode->setMoveContainers(
      load, vector<Container>()); // for testing loading/unloading

  Balance *balance = new Balance(startNode);
  Node *solution = balance->aStarSearch(2); // for testing movement
  stack<Node *> trace;
  while (solution->getParent() != nullptr) // fill trace stack
  {
    trace.push(solution);
    solution = solution->getParent();
  }
  trace.push(solution);
  std::cout << trace.size() << std::endl;
  vector<string> vec_string;
  stringstream ss;
  while (trace.size() > 1) {
    Node *currentNode = trace.top();
    trace.pop();
    Node *nextNode = trace.top();
    if (nextNode == nullptr)
      break;
    MoveInfo mi = GetMoveInfo(currentNode, nextNode);
    ss << mi.name << ": " << mi.from_to;
    vec_string.push_back(ss.str());
    ss.str("");
  }

  return vec_string;
}

vector<string> Solve(ContainerGrid *parserGrid) {
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

  Node *startNode =
      new Node(nullptr, parserGrid, goalGrid,
               bufferGrid); // initialize blank goal state and buffer
  startNode->calcBalanceCost();
  Balance *balance = new Balance(startNode);
  Node *solution = balance->aStarSearch(0);
  stack<Node *> trace;
  while (solution->getParent() != nullptr) // fill trace stack
  {
    trace.push(solution);
    solution = solution->getParent();
  }

  trace.push(solution);
  vector<string> vec_string;
  stringstream ss;
  while (trace.size() > 1) {
    Node *currentNode = trace.top();
    trace.pop();
    Node *nextNode = trace.top();
    if (nextNode == nullptr)
      break;
    MoveInfo mi = GetMoveInfo(currentNode, nextNode);
    ss << mi.name << ": " << mi.from_to;
    vec_string.push_back(ss.str());
    ss.str("");
  }

  return vec_string;
}
