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
	double cost;
	double hCost;
	double gCost;
	double minuteCost;
	int depth;
	Node* parent;
	vector<Node*> children;
	int numChildren;
	vector<tuple<int, int>> unusedBufferLocations;
	vector<tuple<int, int>>  unusedShipLocations;
public:
	Node();
	Node(Node* parent, ContainerGrid* data, ContainerGrid* goalState, ContainerGrid* buffer);
	

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
	void createMovesGoal();
	void transferContainer(bool destination, tuple<int,int>destCoords, tuple<int,int> originalCoords, Container c); //moves container from ship to buffer or from buffer to ship. destination == 0: to buffer

	//helper functions
	ContainerGrid* copyData(ContainerGrid* data);
	int getNumChildren();
	int getDepth();
	tuple<int,int> getNearestBalanceLoc(int originalRow); //helper function to return nearest available row on the other side
	tuple<int, int> getNearestBufferLoc(int originalRow);
	tuple<int, int> findGoalContainer(Container c); //function that finds where the Container (passed thru parameter) is in the goal state
	void generatePossibleShipMoves(); //generates all of the possible ship slots that a container can be moved to
	void generatePossibleBufferMoves(); //generates all of the possible buffer slots that a container can be moved to

	//accessor/information functions
	double getCost();
	double getHCost();
	double getGCost();
	double getMinuteCost();
	bool isBalanced();
	ContainerGrid* getData();
	Node* getParent(); //returns the parent node of this node
	Node* getChild(int i); //returns child at ith index
};