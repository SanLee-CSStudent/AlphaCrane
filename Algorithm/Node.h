#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include "ContainerGrid.h"
using namespace std;

class Node
{
private:
	ContainerGrid* data;
	ContainerGrid* goalState;
	ContainerGrid* buffer;
	char cost;
	char hCost;
	char gCost;
	char minuteCost;
	char depth;
	Node* parent;
	vector<Node*> children;
	int numChildren;
	vector<tuple<char, char>> unusedBufferLocations;
	vector<tuple<char, char>>  unusedShipLocations;
	vector<Container> loadContainers;
	vector<Container> unloadContainers;
	vector<Container> adjustContainers;
public:
	Node();
	Node(Node* parent, ContainerGrid* data, ContainerGrid* goalState, ContainerGrid* buffer);
	~Node();

	//mutator functions
	void createBalanceChildren(); //creates a child for every container being switched to the other side
	void createSIFTChildren(); //creates a child for every container that needs to be moved to its position for SIFT
	void createMovesChildren(); //creates a child for every move that hasn't being completed to be done at this step
	void calcBalanceCost(); //calculates the cost (distance to goal) to balance this state
	void calcSIFTCost(); // calculates the cost (distance to goal) for SIFT execution
	void calcMovesCost(); //calculates the cost (distance to goal) to perform required moves
	void setData(ContainerGrid* data);
	void addChild(Node* child);
	void setParent(Node* parent);
	void setDepth();
	void createSIFTGoal();
	void transferContainer(bool destination, tuple<char, char>destCoords, tuple<char, char> originalCoords, Container c); //moves container from ship to buffer or from buffer to ship. destination == 0: to buffer
	void setMoveContainers(vector<Container> loadContainers, vector<Container> unloadContainers);

	//helper functions
	ContainerGrid* copyData(ContainerGrid* data);
	int getNumChildren();
	char getDepth();
	tuple<char, char> getNearestBalanceLoc(char originalRow); //helper function to return nearest available row on the other side
	tuple<char, char> getNearestBufferLoc();
	tuple<char, char> findGoalContainer(Container c); //function that finds where the Container (passed thru parameter) is in the goal state
	tuple<char, char> getNearestShipLoc();
	tuple<char, char> findBufferContainer(Container c);
	tuple<char, char> findShipContainer(Container c);
	tuple<char, char> adjustContainer(tuple<char, char> originalLoc);
	void generatePossibleShipMoves(); //generates all of the possible ship slots that a container can be moved to
	void generatePossibleBufferMoves(); //generates all of the possible buffer slots that a container can be moved to
	void clearChildren();

	//accessor/information functions
	char getCost();
	char getHCost();
	char getGCost();
	char getMinuteCost();
	bool isBalanced();
	ContainerGrid* getData();
	ContainerGrid* getBuffer();
	Node* getParent(); //returns the parent node of this node
	Node* getChild(int i); //returns child at ith index
};