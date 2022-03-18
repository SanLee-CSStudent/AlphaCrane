#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include "Node.h"
#include "Balance.h"
#include "../Parser/Container.h"
#include "../Parser/ContainerGrid.h"
#include "../Parser/parser.h"
using namespace std;


int main()
{
	ContainerGrid* testGrid = new ContainerGrid(8, 12);
	ContainerGrid* goalGrid = new ContainerGrid(8, 12);
	ContainerGrid* bufferGrid = new ContainerGrid(4, 24);

	for (int col = 0; col < 4; col++) //use addContainer to fill bufferGrid with UNUSED space
	{
		for (int row = 0; row < 24; row++)
		{
			Container empty = { 0, "UNUSED", {col, row} };
			bufferGrid->addContainer(col, row, empty);
		}
	}

	Parser parser = Parser(testGrid);
	string file = "C:\\Users\\amidd\\source\\repos\\Balance Algorithm CS179\\Balance Algorithm CS179\\ShipCase1.txt";
	parser.parse(file);


	Node* startNode = new Node(nullptr, parser.getParseGrid(), goalGrid, bufferGrid); //initialize blank goal state and buffer
	cout << "We are asked to balance the following ship: " << endl;
	startNode->getData()->print();
	cout << endl << endl;
	startNode->calcBalanceCost();
	cout << endl << "startNode size is: " << sizeof(startNode) << endl;
	cout << "It has a starting cost of: " << startNode->getCost() << " with an gCost of: " << startNode->getGCost() << " and an hCost of: " << startNode->getHCost() << endl;
	Balance* balance = new Balance(startNode);
	Node* solution = balance->aStarSearch();
	stack<Node*> trace;
	while (solution->getParent() != nullptr) //fill trace stack
	{
		trace.push(solution);
		solution = solution->getParent();
	}
	trace.push(solution);
	cout << endl << endl << endl << "Now printing trace: " << endl << endl;
	while (trace.size() != 0)
	{
		Node* printNode = trace.top();
		cout << endl;
		printNode->getData()->print();
		cout << endl;
		trace.pop();
	}
	

	return 0;
}