#pragma once
#ifndef __BALANCE__
#define __BALANCE__

#include "Node.h"
#include <vector>
#include <queue>
#include <map>
#include <chrono>
#include <ctime>

using namespace std;

struct nodeComparator
{
	bool operator()(Node* a, Node* b) const
	{
		return a->getMinuteCost() > b->getMinuteCost(); //changed from getCost to getMinuteCost
	}
};

struct frontierComparator
{
	bool operator()(Node* a, Node* b) const
	{
		return a->getCost() > b->getCost(); //changed from getCost to getMinuteCost
	}
};


class Balance
{
private:
	Node* head;
	priority_queue<Node*, vector<Node*>, frontierComparator> aStarFrontier;
	map<Node*, int> exploredSet;
	int maxQueueSize;
	int numExplored;
public:
	Balance(Node* h);
	Node* aStarSearch(char mode);
	int getQueueSize();
	int getNumExplored();
};

#endif //__BALANCE__