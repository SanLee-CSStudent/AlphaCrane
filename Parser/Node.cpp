#include "Node.h"

bool sortContainers(Container a, Container b) //Helper function for sorting a vector of containers by weight
{
	return a.weight > b.weight;
}

Node::Node()
{
	this->parent = nullptr;
	this->cost = -1;
	this->hCost = -1;
	this->gCost = -1;
	this->minuteCost = -1;
	this->depth = -1;
	this->numChildren = 0;
}

Node::Node(Node* parent, ContainerGrid* data, ContainerGrid* goalState, ContainerGrid* buffer)
{
	this->parent = parent;
	this->data = data;
	this->goalState = goalState;
	this->buffer = buffer;
	this->cost = 0;
	this->hCost = 0;
	this->gCost = 0;
	if (this->parent != nullptr)
	{
		this->depth = this->parent->getDepth() + 1;
		this->minuteCost = parent->minuteCost;
	}
	else
	{
		this->depth = 0;
		this->minuteCost = 0;
	}
	this->numChildren = 0;
}

Node* Node::getParent()
{
	return this->parent;
}

Node* Node::getChild(int i)
{
	return this->children.at(i);
}

void Node::createBalanceChildren() //create a Node* child for every possible move for every moveable container
{
	//create a child for each possible move
	vector<Container> containers; //vector for storing containers currently on the ship that are able to be moved
	vector<Container> bufferContainers; //vector for storing containers currently on the buffer that are able to be moveed
	tuple<int, int> originalLoc;
	tuple<int, int> unusedShipLoc;
	tuple<int, int> unusedBufferLoc;
	double mDist;

	//doubles for storing the temporary values of the container in the respective position to its virtual slot to be moved to the other side...
	double tempBufferRDist;
	double tempBufferCDist;
	double tempShipRDist;
	double tempShipCDist;
	for (int row = 0; row < this->data->getRow(); row++) //loop through ship grid and add moveable containers to a list
	{
		for (int col = 0; col < this->data->getColumn(); col++)
		{
			if ((this->data->getContainer(col, row).name != "NAN" && this->data->getContainer(col, row).name != "UNUSED") && (col == 0 || this->data->getContainer(col - 1, row).name == "UNUSED"))
			{
				containers.push_back(this->data->getContainer(col, row));
			}
		}
	}
	for (int row = 0; row < this->buffer->getRow(); row++) //loop through buffer grid and add moveable containers to a list
	{
		for (int col = 0; col < this->buffer->getColumn(); col++)
		{
			if ((this->buffer->getContainer(col, row).name != "NAN" && this->buffer->getContainer(col, row).name != "UNUSED") && (col == 0 || this->buffer->getContainer(col - 1, row).name == "UNUSED"))
			{
				bufferContainers.push_back(this->buffer->getContainer(col, row));
			}
		}
	}

	//cout << endl;
	//this->buffer->print();
	//cout << endl;
	this->generatePossibleBufferMoves();
	//cout << endl;
	//this->buffer->print();
	//cout << endl;
	this->generatePossibleShipMoves();
	//loop through containers and add all of their possible moves
	for (int i = 0; i < containers.size(); i++) //iterate through containers that are within the ship and generate a child for every possible move
	{
		originalLoc = make_tuple(containers.at(i).position.column, containers.at(i).position.row);
		for (int j = 0; j < this->unusedShipLocations.size(); j++) //generate all moves to move within the ship
		{
			if (get<0>(unusedShipLocations.at(j))+1 == get<0>(originalLoc) && get<1>(unusedShipLocations.at(j)) == get<1>(originalLoc)) //don't let a container be sat on top of its old position
			{
				continue;
			}
			ContainerGrid* copiedData = copyData(this->data);
			ContainerGrid* copiedBuffer = copyData(this->buffer);
			Node* child = new Node(this, copiedData, this->goalState, copiedBuffer);
			unusedShipLoc = this->unusedShipLocations.at(j);
			mDist = (abs(get<0>(unusedShipLoc) - get<0>(originalLoc)) + abs(get<1>(unusedShipLoc) - get<1>(originalLoc)));
			child->data->moveContainer(containers.at(i), get<0>(unusedShipLoc), get<1>(unusedShipLoc));
			child->minuteCost += mDist;
			child->calcBalanceCost();
			this->children.push_back(child);
		}
		for (int k = 0; k < this->unusedBufferLocations.size(); k++) //generate all moves to move a container to the buffer
		{
			if (get<0>(unusedBufferLocations.at(k))+1 == get<0>(originalLoc) && get<1>(unusedBufferLocations.at(k))== get<1>(originalLoc)) //don't let a container be sat on top of its old position
			{
				continue;
			}
			ContainerGrid* copiedData = copyData(this->data);
			ContainerGrid* copiedBuffer = copyData(this->buffer);
			Node* child = new Node(this, copiedData, this->goalState, copiedBuffer);
			unusedBufferLoc = this->unusedBufferLocations.at(k);
			tempBufferRDist = abs(get<1>(unusedBufferLoc) - 23); //only buffer R is reduced because the side with the large number is closest to the ship
			tempBufferCDist = get<0>(unusedBufferLoc);
			tempShipRDist = get<1>(originalLoc);
			tempShipCDist = get<0>(originalLoc);
			mDist = 17 + tempBufferRDist + tempBufferCDist + tempShipRDist + tempShipCDist; //17 because 15 mins(flat time) plus 1 dist for virtual node, update later
			child->transferContainer(true, unusedBufferLoc, originalLoc, containers.at(i));
			child->minuteCost += mDist;
			child->calcBalanceCost();
			this->children.push_back(child);
		}
	}

	for (int i = 0; i < bufferContainers.size(); i++) //generate all moves to move containers that are within the buffer and generate a child for every possible move
	{
		originalLoc = make_tuple(bufferContainers.at(i).position.column, bufferContainers.at(i).position.row);
		for (int j = 0; j < this->unusedBufferLocations.size(); j++)
		{
			if (get<0>(unusedShipLocations.at(j))+1 == get<0>(originalLoc) && get<1>(unusedShipLocations.at(j)) == get<1>(originalLoc)) //don't let a container be sat on top of its old position
			{
				continue;
			}
			ContainerGrid* copiedData = copyData(this->data);
			ContainerGrid* copiedBuffer = copyData(this->buffer);
			Node* child = new Node(this, copiedData, this->goalState, copiedBuffer);
			unusedBufferLoc = this->unusedBufferLocations.at(j);
			mDist = (abs(get<0>(unusedBufferLoc) - get<0>(originalLoc)) + abs(get<1>(unusedBufferLoc) - get<1>(originalLoc)));
			child->buffer->moveContainer(bufferContainers.at(i), get<0>(unusedBufferLoc), get<1>(unusedBufferLoc));
			child->minuteCost += mDist;
			child->calcBalanceCost();
			this->children.push_back(child);
		}
		for (int k = 0; k < this->unusedShipLocations.size(); k++)
		{
			if (get<0>(unusedShipLocations.at(k))+1 == get<0>(originalLoc) && get<1>(unusedShipLocations.at(k)) == get<1>(originalLoc)) //don't let a container be sat on top of its old position
			{
				continue;
			}
			ContainerGrid* copiedData = copyData(this->data);
			ContainerGrid* copiedBuffer = copyData(this->buffer);
			Node* child = new Node(this, copiedData, this->goalState, copiedBuffer);
			unusedShipLoc = this->unusedShipLocations.at(k);
			tempBufferRDist = abs(get<1>(unusedBufferLoc) - 23);
			tempBufferCDist = get<0>(unusedBufferLoc);
			tempShipRDist = get<1>(originalLoc);
			tempShipCDist = get<0>(originalLoc);
			mDist = 17 + tempBufferRDist + tempBufferCDist + tempShipRDist + tempShipCDist; //17 because 15 mins(flat time) plus 1 dist for virtual node, update later
			child->transferContainer(false, unusedShipLoc, originalLoc, containers.at(i));
			child->minuteCost += mDist;
			child->calcBalanceCost();
			this->children.push_back(child);
		}
	}
}

void Node::createSIFTChildren() //generates children for necessary SIFT moves (FIRST VERSION COMPLETE)
{
	vector<Container> containers;
	vector<Container> bufferContainers;
	for (int row = 0; row < this->data->getRow(); row++) //loop through ship grid and add moveable containers to a list
	{
		for (int col = 0; col < this->data->getColumn(); col++)
		{
			if ((this->data->getContainer(col, row).name != "NAN" && this->data->getContainer(col, row).name != "UNUSED") && (col == 0 || this->data->getContainer(col - 1, row).name == "UNUSED"))
			{
				containers.push_back(this->data->getContainer(col, row));
			}
		}
	}
	for (int row = 0; row < this->buffer->getRow(); row++) //loop through buffer grid and add moveable containers to a list
	{
		for (int col = 0; col < this->buffer->getColumn(); col++)
		{
			if ((this->buffer->getContainer(col, row).name != "NAN" && this->buffer->getContainer(col, row).name != "UNUSED") && (col == 0 || this->buffer->getContainer(col - 1, row).name == "UNUSED"))
			{
				bufferContainers.push_back(this->buffer->getContainer(col, row));
			}
		}
	}

	this->generatePossibleBufferMoves();
	this->generatePossibleShipMoves();
	tuple<int, int> unusedShipLoc;
	tuple<int, int> unusedBufferLoc;
	tuple<int, int> originalLoc;
	double mDist; //for calculating the actual time cost

	//doubles for storing the distance to travel to and from the pink virtual cell from each original location
	double tempBufferRDist;
	double tempBufferCDist;
	double tempShipRDist;
	double tempShipCDist;
	//loop through containers and add all of their possible moves
	for (int i = 0; i < containers.size(); i++) //iterate through containers that are within the ship and generate a child for every possible move
	{
		originalLoc = make_tuple(containers.at(i).position.column, containers.at(i).position.row);
		for (int j = 0; j < this->unusedShipLocations.size(); j++) //generate all moves to move within the ship
		{
			ContainerGrid* copiedData = copyData(this->data);
			Node* child = new Node(this, copiedData, this->goalState, this->buffer);
			unusedShipLoc = this->unusedShipLocations.at(j);
			mDist = (abs(get<0>(unusedShipLoc) - get<0>(originalLoc)) + abs(get<1>(unusedShipLoc) - get<1>(originalLoc)));
			child->data->moveContainer(containers.at(i), get<0>(unusedShipLoc), get<1>(unusedShipLoc));
			child->minuteCost += mDist;
			child->calcSIFTCost();
			this->children.push_back(child);
		}
		for (int k = 0; k < this->unusedBufferLocations.size(); k++) //generate all moves to move a container to the buffer
		{
			ContainerGrid* copiedData = copyData(this->data);
			Node* child = new Node(this, copiedData, this->goalState, this->buffer);
			unusedBufferLoc = this->unusedBufferLocations.at(k);
			tempBufferRDist = abs(get<1>(unusedBufferLoc) - 23); //only buffer R is reduced because the side with the large number is closest to the ship
			tempBufferCDist = get<0>(unusedBufferLoc);
			tempShipRDist = get<1>(originalLoc);
			tempShipCDist = get<0>(originalLoc);
			mDist = 17 + tempBufferRDist + tempBufferCDist + tempShipRDist + tempShipCDist; //17 because 15 mins(flat time) plus 1 dist for virtual node, update later
			child->transferContainer(true, unusedBufferLoc, originalLoc, containers.at(i));
			child->minuteCost += mDist;
			child->calcSIFTCost();
			this->children.push_back(child);
		}
	}

	for (int i = 0; i < bufferContainers.size(); i++) //generate all moves to move containers that are within the buffer and generate a child for every possible move
	{
		originalLoc = make_tuple(bufferContainers.at(i).position.column, bufferContainers.at(i).position.row);
		for (int j = 0; j < this->unusedBufferLocations.size(); j++)
		{
			ContainerGrid* copiedData = copyData(this->data);
			Node* child = new Node(this, copiedData, this->goalState, this->buffer);
			unusedBufferLoc = this->unusedBufferLocations.at(j);
			mDist = (abs(get<0>(unusedBufferLoc) - get<0>(originalLoc)) + abs(get<1>(unusedBufferLoc) - get<1>(originalLoc)));
			child->buffer->moveContainer(bufferContainers.at(i), get<0>(unusedBufferLoc), get<1>(unusedBufferLoc));
			child->minuteCost += mDist;
			child->calcSIFTCost();
			this->children.push_back(child);
		}
		for (int k = 0; k < this->unusedShipLocations.size(); k++)
		{
			ContainerGrid* copiedData = copyData(this->data);
			Node* child = new Node(this, copiedData, this->goalState, this->buffer);
			unusedShipLoc = this->unusedShipLocations.at(k);
			tempBufferRDist = abs(get<1>(unusedBufferLoc) - 23);
			tempBufferCDist = get<0>(unusedBufferLoc);
			tempShipRDist = get<1>(originalLoc);
			tempShipCDist = get<0>(originalLoc);
			mDist = 17 + tempBufferRDist + tempBufferCDist + tempShipRDist + tempShipCDist; //17 because 15 mins(flat time) plus 1 dist for virtual node, update later
			child->transferContainer(false, unusedShipLoc, originalLoc, containers.at(i));
			child->minuteCost += mDist;
			child->calcSIFTCost();
			this->children.push_back(child);
		}
	}

}

void Node::createSIFTGoal() //creates the goal state for when performing SIFT (FIRST VERSION COMPLETE)
{
	//create the goal state for SIFT
	//obtain all containers in a vector, sort by weight, move them to their required locations, set Node's goalState to the modified ContainerGrid
	// 
	//ContainerGrid* siftGoal = new ContainerGrid(,);
	vector<Container> containers;
	for (int row = 0; row < this->data->getRow(); row++) //loop through containers and add all containers to a list
	{
		for (int col = 0; col < this->data->getColumn(); col++)
		{
			if (this->data->getContainer(col, row).name != "NAN" && this->data->getContainer(col, row).name != "UNUSED")
			{
				containers.push_back(this->data->getContainer(col, row));
			}
		}
	}
	std::sort(containers.begin(), containers.end(), sortContainers); //sorts from heaviest to lightest

	for (int col = 7; col >= 0; col--)//create the outline of the ship (only add NAN blocks, rest is unused)
	{
		for (int row = 11; row >= 0; row--)
		{
			if (this->data->getContainer(col, row).name == "NAN")
			{
				this->goalState->addContainer(col, row, this->data->getContainer(col, row));
			}
			else
			{
				Container unused = { 0, "NAN", {col, row} };
				this->goalState->addContainer(col, row, unused);
			}
		}
	}
	int modifier = 1;
	int row = (this->data->getRow() / 2) - 1;
	int col = 7;
	bool goRight = true;
	for (int n = 0; n < containers.size(); n++) //iterate through the list of containers
	{
		if (this->goalState->getContainer(col, row).name == "UNUSED")
		{
			this->goalState->addContainer(col, row, containers.at(n));
		}
		else if (this->goalState->getContainer(col, row).name == "NAN")
		{
			col--;
			row = (this->data->getRow() / 2) - 1;
			modifier = 1;
			continue; //goes up one column and returns to the middle row
			//MAYBE ADD CHECKS HERE FOR BOUNDS ON ROW AND COL
		}
		if (goRight)
		{
			//increment row to the right by modifier
			row += modifier;
			modifier += 1;
			goRight = false;
		}
		else
		{
			//decrement row to the left by modifier
			row -= modifier;
			modifier += 1;
			goRight = true;
		}
	}
}

void Node::calcBalanceCost()
{
	double leftWeight = 0;
	double rightWeight = 0;
	double balanceMass = 0;
	double deficit = 0;

	ContainerGrid* tempData = this->data;
	vector<Container> leftContainers;
	vector<Container> rightContainers;

	for (int i = 0; i < tempData->getRow(); i++) //grabs all containers on left and right side and finds the leftWeight and rightWeight
	{
		for (int j = 0; j < tempData->getColumn(); j++)
		{
			if (tempData->getContainer(j, i).name != "NAN" && tempData->getContainer(j, i).name != "UNUSED")
			{
				if (i < tempData->getRow() / 2) //used to be j and getColumn
				{
					leftWeight += tempData->getContainer(j, i).weight;
					leftContainers.push_back(tempData->getContainer(j,i));
				}
				else
				{
					rightWeight += tempData->getContainer(j, i).weight;
					rightContainers.push_back(tempData->getContainer(j,i));
				}
			}
		}
	}

	if (leftWeight <= (rightWeight + (rightWeight * 0.10)) && leftWeight >= (rightWeight - (rightWeight * 0.10))) //if the ship is balanced...
	{
		this->hCost = 0;
		this->gCost = this->depth;
		this->cost = this->depth;
		return;
	}

	sort(leftContainers.begin(), leftContainers.end(), sortContainers); //Sorts containers from largest to smallest
	sort(rightContainers.begin(), rightContainers.end(), sortContainers); //Sorts containers from largest to smallest

	if (leftWeight < rightWeight)
	{
		balanceMass = (leftWeight + rightWeight) / 2;
		deficit = balanceMass - leftWeight;
		int nearestRow = 7;
		for (int i = 0; i < rightContainers.size(); i++)
		{
			if (rightContainers.at(i).weight <= (deficit + (deficit * 0.10)) && rightContainers.at(i).weight >= (deficit - (deficit * 0.10))) //if moving this container fixes balance...
			{
				//find the distance to nearest available row, set distance
				nearestRow = get<1>(getNearestBalanceLoc(rightContainers.at(i).position.row));
				this->hCost = abs(nearestRow - rightContainers.at(i).position.row); //sets hCost to distance between rows
				this->cost = this->depth + this->hCost;
				return; //used to just break
				//break;
			}
		}
		if (this->hCost == 0 || this->hCost == -1) //there isn't one container that can solve balance issue...
		{
			//int tempDeficit = deficit;
			int tempLeftWeight = leftWeight;
			int tempRightWeight = rightWeight;
			int slotsFound = 0;
			vector<int> candidates;
			for (int i = 0; i < rightContainers.size(); i++) //move as few containers as possible to make deficit as small as possible
			{
				if (this->data->getContainer(rightContainers.at(i).position.column, (rightContainers.at(i).position.row + 1)).name != "UNUSED") //make sure the slot above the container is free so that this container can be moved
				{
					continue; //skip this container since it cannot be moved
				}
				if ((tempRightWeight <= (tempLeftWeight + (tempLeftWeight * 0.10))) && tempRightWeight >= (tempLeftWeight - (tempLeftWeight * 0.10))) //if the two sides are balanced...
				{
					break;
				}
				if ((tempRightWeight - rightContainers.at(i).weight) >= ((tempLeftWeight + rightContainers.at(i).weight) - ((tempLeftWeight + rightContainers.at(i).weight) * 0.10)))
				{
					candidates.push_back(i);
					tempRightWeight - rightContainers.at(i).weight;
					tempLeftWeight + rightContainers.at(i).weight;
				}
			}
			// need to find the closest slot for each candidate container
			int tempHCost = 0;
			tuple<int, int> tempMoveLoc;
			for (int i = 0; i < candidates.size(); i++)
			{
				tempMoveLoc = getNearestBalanceLoc(rightContainers.at(candidates.at(i)).position.row); //store the available moving location
				tempHCost += abs(get<1>(tempMoveLoc) - rightContainers.at(candidates.at(i)).position.row);
				//line above used to be: tempHCost += (get<1>(tempMoveLoc), rightContainers.at(candidates.at(i)).position.row);
			}
			// hCost = summation of distance between candidate containers and their distance to nearest available slots
			this->hCost = tempHCost;
		}
	}
	else //rightWeight < leftWeight
	{
		balanceMass = (leftWeight + rightWeight) / 2;
		deficit = balanceMass - rightWeight;
		int nearestRow = 7;
		for (int i = 0; i < leftContainers.size(); i++)
		{
			if (leftContainers.at(i).weight <= (deficit + (deficit * 0.10)) && leftContainers.at(i).weight >= (deficit - (deficit * 0.10))) //if moving this container fixes balance...
			{
				//find the distance to nearest available row, set distance
				nearestRow = get<0>(getNearestBalanceLoc(leftContainers.at(i).position.row));
				this->hCost = abs(nearestRow - leftContainers.at(i).position.row); //sets hCost to distance between rows
				break;
			}
		}
		if (this->hCost == 0 || this->hCost == -1) //there isn't one container that can solve balance issue...
		{
			//int tempDeficit = deficit;
			int tempLeftWeight = leftWeight;
			int tempRightWeight = rightWeight;
			int slotsFound = 0;
			vector<int> candidates;
			for (int i = 0; i < leftContainers.size(); i++) //move as few containers as possible to make deficit as small as possible
			{
				if (this->data->getContainer(leftContainers.at(i).position.column, (leftContainers.at(i).position.row + 1)).name != "UNUSED") //make sure the slot above the container is free so that this container can be moved
				{
					continue; //skip this container since it cannot be moved
				}
				if ((tempRightWeight <= (tempLeftWeight + (tempLeftWeight * 0.10))) && tempRightWeight >= (tempLeftWeight - (tempLeftWeight * 0.10))) //if the two sides are balanced...
				{
					break;
				}
				if ((tempLeftWeight - leftContainers.at(i).weight) >= ((tempRightWeight + leftContainers.at(i).weight) - ((tempRightWeight + leftContainers.at(i).weight) * 0.10)))
				{
					candidates.push_back(i);
					tempLeftWeight - leftContainers.at(i).weight;
					tempRightWeight + leftContainers.at(i).weight; //moves weight from left to right in attempt to balance by moving fewest containers possible
				}
			}
			// need to find the closest slot for each candidate container
			int tempHCost = 0;
			tuple<int, int> tempMoveLoc;
			for (int i = 0; i < candidates.size(); i++)
			{
				tempMoveLoc = getNearestBalanceLoc(leftContainers.at(candidates.at(i)).position.row); //store the available moving location
				tempHCost += abs(get<1>(tempMoveLoc) - leftContainers.at(candidates.at(i)).position.row);
				//line above used to be: tempHCost += (get<1>(tempMoveLoc), leftContainers.at(candidates.at(i)).position.column);
			}
			// hCost = summation of distance between candidate containers rows and their distance to nearest available slot's row
			this->hCost = tempHCost;
		}
	}
	vector<Container> bufferContainers;
	for (int col = 0; col < this->buffer->getColumn(); col++)
	{
		for (int row = 0; row < this->buffer->getRow(); row++)
		{
			if (this->buffer->getContainer(col, row).name != "UNUSED" && (col == 0 || this->buffer->getContainer(col - 1, row).name == "UNUSED"))// if there is a buffer in the container that can be moved
			{
				bufferContainers.push_back(this->buffer->getContainer(col, row));
			}
		}
	}



	this->gCost = this->depth;
	this->cost = this->hCost + this->gCost;
	return;
}

void Node::calcSIFTCost() //calculates destination to SIFT goal state. (FIRST VERSION DONE)
{
	//similar to balance cost but make heuristic factor approx. distance to siftGoal

	//gCost is depth of node
	this->gCost = this->depth;
	
	//create a vector of all containers (one for ship one for buffer)
	vector<Container> containers; //vector of containers on the ship
	vector<Container> bufferContainers;
	tuple<int, int> originalLoc;
	tuple<int, int> destinationLoc;
	double tempHCost = 0;

	double tempBufferRDist;
	double tempShipRDist;
	for (int row = 0; row < this->data->getRow(); row++) //loop through ship grid and add moveable containers to a list
	{
		for (int col = 0; col < this->data->getColumn(); col++)
		{
			if (this->data->getContainer(col, row).name != "NAN" && this->data->getContainer(col, row).name != "UNUSED")
			{
				containers.push_back(this->data->getContainer(col, row));
			}
		}
	}

	for (int row = 0; row < this->buffer->getRow(); row++) //loop through buffer grid and add moveable containers to a list
	{
		for (int col = 0; col < this->buffer->getColumn(); col++)
		{
			if (this->buffer->getContainer(col, row).name != "NAN" && this->buffer->getContainer(col, row).name != "UNUSED")
			{
				bufferContainers.push_back(this->buffer->getContainer(col, row));
			}
		}
	}

	for (int i = 0; i < containers.size(); i++)
	{
		originalLoc = make_tuple(containers.at(i).position.column, containers.at(i).position.row);
		destinationLoc = findGoalContainer(containers.at(i));
		tempHCost += (abs(get<0>(originalLoc) - get<0>(destinationLoc)) + abs(get<1>(originalLoc) - get<1>(destinationLoc)));
	}

	for (int i = 0; i < bufferContainers.size(); i++)
	{
		originalLoc = make_tuple(bufferContainers.at(i).position.column, bufferContainers.at(i).position.column);
		destinationLoc = findGoalContainer(containers.at(i));
		tempHCost += 15 + abs(23 - get<1>(originalLoc)) + get<1>(destinationLoc); //15 + cost to top right of buffer + cost from top left of ship to destination ("ROWS" ONLY)
	}

	//hCost is summation of containers' column distances to destination
	this->hCost = tempHCost;
	this->cost = this->gCost + this->hCost;
}

void Node::transferContainer(bool destination, tuple<int, int>destCoords, tuple<int, int> originalCoords, Container c)
{
	if (destination)
	{
	    //cout << endl;
		//this->buffer->print();
		this->buffer->addContainer(get<0>(destCoords), get<1>(destCoords), c); //adds the original container to the buffer zone
		//cout << endl;
		//this->buffer->print();
		//cout << endl;
		//this->data->print();
		//this->data->getContainer(get<0>(originalCoords), get<1>(originalCoords)) = dummyContainer; //changes the old slot to an UNUSED slot (DOESN'T ACTUALLY CHANGE IT)
		this->data->emptyContainer(get<0>(originalCoords), get<1>(originalCoords));
		//cout << endl;
		//this->data->print();
	}
	else
	{
		Container dummyContainer = { 0, "UNUSED", {get<0>(originalCoords), get<1>(originalCoords)} }; //dummyContainer to create UNUSED slot in buffer
		this->data->addContainer(get<0>(destCoords), get<1>(destCoords), c); //adds the original container to the ship
		this->buffer->emptyContainer(get<0>(originalCoords), get<1>(originalCoords)); //changes the old slot to an UNUSED slot
	}
}

void Node::setData(ContainerGrid* data)
{
	this->data = data;
}

void Node::addChild(Node* child)
{
	this->children.push_back(child);
}

void Node::setParent(Node* parent)
{
	this->parent = parent;
}

void Node::setDepth()
{
	this->depth = this->parent->getDepth() + 1;
}

int Node::getNumChildren()
{
	return this->children.size();
}

int Node::getDepth()
{
	return this->depth;
}

tuple<int,int> Node::getNearestBalanceLoc(int originalRow)
{
	if (originalRow <= 5) //used to be 6
	{
		for (int col = 7; col >= 0; col--) //maxs out at 7
		{
			for (int row = 6; row < 11; row++) //maxs out at 11
			{
				if (this->data->getContainer(col, row).name == "UNUSED" && (col == 7 ||this->data->getContainer(col+1,row).name != "UNUSED"))
				{
					return make_tuple(col, row);
				}
			}
		}
	}
	else
	{
		for (int col = 7; col >= 0; col--) 
		{
			for (int row = 5; row >= 0; row--) //USED TO BE: row = 0, row < 6
			{
				if (this->data->getContainer(col, row).name == "UNUSED" && (col == 7 || this->data->getContainer(col + 1, row).name != "UNUSED"))
				{
					return make_tuple(col, row);
				}
			}
		}
	}

	return make_tuple(-1,-1);
}

tuple<int, int> Node::getNearestBufferLoc(int originalRow) //checks for nearest buffer loc
{
	for (int row = 23; row >= 0; row--) //maxs out at 3(bottom row)
	{
		for (int col = 3; col >= 0; col--) //maxs out at 23(furthest right column)
		{
			if (this->buffer->getContainer(col, row).name == "UNUSED" && (col == 3 || this->buffer->getContainer(col + 1, row).name != "UNUSED"))
			{
				return make_tuple(col, row);
			}
		}
	}

	return make_tuple(-1, -1);
}

void Node::generatePossibleShipMoves()
{
	vector<tuple<int, int>> unusedShipLocations;
	for (int row = 0; row < this->data->getRow(); row++) //loop through ship grid and add moveable containers to a list
	{
		for (int col = 0; col < this->data->getColumn(); col++)
		{
			if ((this->data->getContainer(col, row).name == "UNUSED") && (col == 7 || this->data->getContainer(col + 1, row).name != "UNUSED"))
			{
				unusedShipLocations.push_back(make_tuple(col, row));
			}
		}
	}

	this->unusedShipLocations = unusedShipLocations;
}

void Node::generatePossibleBufferMoves()
{
	vector<tuple<int, int>> unusedBufferLocations;
	for (int row = 0; row < this->buffer->getRow(); row++) //loop through ship grid and add moveable containers to a list
	{
		for (int col = 0; col < this->buffer->getColumn(); col++)
		{
			if ((this->buffer->getContainer(col, row).name == "UNUSED") && (col == 3 || this->buffer->getContainer(col + 1, row).name != "UNUSED"))
			{
				unusedBufferLocations.push_back(make_tuple(col, row));
			}
		}
	}

	this->unusedBufferLocations = unusedBufferLocations;
}

double Node::getCost()
{
	return this->cost;
}

double Node::getHCost()
{
	return this->hCost;
}

double Node::getGCost()
{
	return this->gCost;
}

double Node::getMinuteCost()
{
	return this->minuteCost;
}

bool Node::isBalanced()
{
	int leftWeight = 0;
	int rightWeight = 0;

	for (int col = 0; col < this->data->getColumn(); col++)
	{
		for (int row = 0; row < this->data->getRow(); row++)
		{
			if (this->data->getContainer(col, row).name != "NAN" && this->data->getContainer(col, row).name != "UNUSED")
			{
				if (row < this->data->getRow() / 2) // CHANGE TO < ??
				{
					leftWeight += this->data->getContainer(col, row).weight;
				}
				else
				{
					rightWeight += this->data->getContainer(col, row).weight;
				}
			}
		}
	}

	if (leftWeight <= (rightWeight + (rightWeight * 0.10)) && leftWeight >= (rightWeight - (rightWeight * 0.10))) //if the two sides weigh within 10% of each other...
	{
		return true;
	}
	return false;
}

ContainerGrid* Node::getData()
{
	return this->data;
}

ContainerGrid* Node::copyData(ContainerGrid* data)
{
	//cout << endl << "Getcolumn returns: " << data->getColumn() << " Getrow returns: " << data->getRow() << endl;
	ContainerGrid* newData = new ContainerGrid(data->getColumn(), data->getRow());
	for (int column = 0; column < data->getColumn(); column++)
	{
		for (int row = 0; row < data->getRow(); row++)
		{
			newData->addContainer(column, row, data->getContainer(column,row));
		}
	}

	return newData;
}
//NEEDED FOR FINDING COST TO A GOAL STATE (ANYTHING BUT BALANCE)
tuple<int, int> Node::findGoalContainer(Container c)
{
	for (int col = 7; col >= 0; col--)
	{
		for (int row = 11; row >= 0; row--)
		{
			if (this->goalState->getContainer(col, row).name == c.name && this->goalState->getContainer(col, row).weight == c.weight)
			{
				return make_tuple(col, row);
			}
		}
	}

	return make_tuple(-1, -1);
}