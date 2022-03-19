#pragma once

#include "Balance.h"

Balance::Balance(Node* h)
{
	this->head = h;
	this->maxQueueSize = 0;
	this->numExplored = 0;
}

int Balance::getNumExplored()
{
	return this->numExplored;
}

int Balance::getQueueSize()
{
	return this->maxQueueSize;
}

Node* Balance::aStarSearch(char mode)
{
	chrono::time_point<chrono::system_clock> startTime = chrono::system_clock::now();
	Node* start = this->head;
	Node* curr;
	bool sameArray = true;
	bool exploredAlready = true;
	int numExplored = 0;
	int maxQueueSize = 0;
	bool firstRun = true;
	this->aStarFrontier.push(start);
	priority_queue<Node*, vector<Node*>, nodeComparator> goalQueue;

	while (this->aStarFrontier.size() > 0)
	{
		sameArray = false;
		exploredAlready = 0;
		if ((aStarFrontier.size() > 25000 || numExplored > 25000) && mode == 0) //Stops at 15000 nodes in the frontier or 15000 nodes explored to prevent impossible search, takes about 6.5 MINUTES
		{
			if (goalQueue.empty())
			{
				cout << endl << "This ship is impossible to balance." << endl;
				chrono::time_point<chrono::system_clock> endTime = chrono::system_clock::now();
				chrono::duration<double> elapsed_seconds = endTime - startTime;
				cout << "Total time spent trying: " << elapsed_seconds.count() << " seconds." << endl;
				cout << "The system will now perform SIFT to balance the ship..." << endl << endl;
				mode = 1;
				this->exploredSet.clear();
				while(aStarFrontier.size() > 0)
				{
					this->aStarFrontier.pop(); //clears the priority queue
				}
				this->head->clearChildren();
				this->head->createSIFTGoal();
				this->head->calcSIFTCost();
				this->aStarFrontier.push(this->head);
				continue; //starts the loop over
			}
			else
			{
				cout << endl << "The goal state has been found!" << endl;
				goalQueue.top()->getData()->print();
				cout << endl << "The total minute cost was: " << +goalQueue.top()->getMinuteCost() << endl;
				chrono::time_point<chrono::system_clock> endTime = chrono::system_clock::now();
				chrono::duration<double> elapsed_seconds = endTime - startTime;
				cout << "The search took " << elapsed_seconds.count() << "seconds." << endl;
				return goalQueue.top();
			}
			
		}
		if (aStarFrontier.size() > maxQueueSize)
		{
			maxQueueSize = aStarFrontier.size();
		}

		curr = this->aStarFrontier.top();
		this->aStarFrontier.pop();

		if (!(goalQueue.empty()) && goalQueue.top()->getDepth() < curr->getDepth()) //if there has been a goal found with a lower cost than the lowest cost node in the frontier (goalQueueCost < currCost??)
		{
			cout << "The current frontier contains no nodes with a lower cost than the current goal state." << endl;
			cout << "Printing goal state..." << endl << endl;
			goalQueue.top()->getData()->print();
			cout << endl << "The total minute cost was: " << +goalQueue.top()->getMinuteCost() << endl;
			chrono::time_point<chrono::system_clock> endTime = chrono::system_clock::now();
			chrono::duration<double> elapsed_seconds = endTime - startTime;
			cout << "The search took " << elapsed_seconds.count() << "seconds." << endl;
 			return goalQueue.top();
		}

		for (map<Node*, int>::iterator it = exploredSet.begin(); it != exploredSet.end(); ++it)
		{
			exploredAlready = true;
			//check if the curr has been explored already by comparing the Node's data
			for (int column = 0; column < curr->getData()->getColumn(); column++)
			{
				for (int row = 0; row < curr->getData()->getColumn(); row++)
				{
					if (curr->getData()->getContainer(column, row).name == it->first->getData()->getContainer(column, row).name)
					{
						// do nothing
					}
					else
					{
						exploredAlready = false;
						break; //breaks out of j loop
					}
				}
			}
			if (exploredAlready) //only checks buffer if already explored
			{
				for (int column = 0; column < curr->getBuffer()->getColumn(); column++)
				{
					for (int row = 0; row < curr->getBuffer()->getColumn(); row++)
					{
						if (curr->getBuffer()->getContainer(column, row).name == it->first->getBuffer()->getContainer(column, row).name)
						{
							// do nothing
						}
						else
						{
							exploredAlready = false;
							break; //breaks out of j loop
						}
					}
				}
			}
		}

		if (exploredAlready == false || firstRun == true)
		{
			firstRun = false;
			this->exploredSet.insert(pair<Node*,int>(curr, numExplored)); //Adds removed node to explored set
			numExplored++;

			if (mode == 0)
			{
				sameArray = curr->isBalanced();
			}
			else if(curr->getHCost() == 0)
			{
				sameArray = true;
			}


			if (sameArray == true)
			{
				cout << endl << "Found a goal node!" << endl;
				curr->getData()->print();
				cout << endl;
				goalQueue.push(curr);
			}
			else
			{
				cout << endl << "Expanding this node which has a cost of: " << +curr->getCost() << " gCost of: "<< +curr->getGCost() << " hCost of: "  << +curr->getHCost() << endl;
				curr->getData()->print();
				numExplored++;
				if (mode == 0)
				{
					curr->createBalanceChildren();
				}
				else if (mode == 1)
				{
					curr->createSIFTChildren();
				}
				else if (mode == 2)
				{
					curr->createMovesChildren();
				}
				for (int i = 0; i < curr->getNumChildren(); i++) //add all the children to the frontier
				{
					cout  << endl << "child cost: " << +curr->getChild(i)->getCost() << " child Gcost:" << +curr->getChild(i)->getGCost() << " child Hcost: " << +curr->getChild(i)->getHCost() << endl;
					this->aStarFrontier.push(curr->getChild(i));
					cout << endl << "child ship: " << endl;
					curr->getChild(i)->getData()->print();
				}
				if (aStarFrontier.size() > maxQueueSize)
				{
					maxQueueSize = aStarFrontier.size();
				}
			}
		}
		else
		{
			//it wont search through this node again, will delete the duplicate node
			delete curr;
			curr = NULL;
		}
	}
	this->maxQueueSize = maxQueueSize;
	this->numExplored = numExplored;
	if (!goalQueue.empty())
	{
		cout << endl << "The goal state has been found!" << endl;
		goalQueue.top()->getData()->print();
		cout << endl << "The total minute cost was: " << +goalQueue.top()->getMinuteCost() << endl;
		chrono::time_point<chrono::system_clock> endTime = chrono::system_clock::now();
		chrono::duration<double> elapsed_seconds = endTime - startTime;
		cout << "The search took " << elapsed_seconds.count() << "seconds." << endl;
	}
	return goalQueue.top(); //returns the goal with the least total cost found (sometimes returns top of goal queue without there being any added)
}