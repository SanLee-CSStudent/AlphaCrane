#pragma once

#include "Balance.h"

Balance::Balance(Node *h) {
  this->head = h;
  this->maxQueueSize = 0;
  this->numExplored = 0;
}

int Balance::getNumExplored() { return this->numExplored; }

int Balance::getQueueSize() { return this->maxQueueSize; }

Node *Balance::aStarSearch() {
  chrono::time_point<chrono::system_clock> startTime =
      chrono::system_clock::now();
  Node *start = this->head;
  Node *curr;
  bool sameArray = true;
  bool exploredAlready = true;
  int numExplored = 0;
  int maxQueueSize = 0;
  bool firstRun = true;
  this->aStarFrontier.push(start);
  priority_queue<Node *, vector<Node *>, nodeComparator> goalQueue;

  while (this->aStarFrontier.size() > 0) {
    sameArray = true;
    exploredAlready = 0;
    if (aStarFrontier.size() > 15000 ||
        numExplored > 15000) // Stops at 15000 nodes in the frontier or 15000
                             // nodes explored to prevent impossible search,
                             // takes about 6.5 MINUTES
    {
      if (goalQueue.empty()) {
        cout << endl << "This ship is impossible to balance." << endl;
        chrono::time_point<chrono::system_clock> endTime =
            chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = endTime - startTime;
        cout << "Total time spent trying: " << elapsed_seconds.count()
             << " seconds." << endl;
        cout << "The system will now perform SIFT to balance the ship..."
             << endl
             << endl;
        return nullptr; // exits the search if the queue size or number of nodes
                        // expanded is greater than 50000. This prevents an
                        // infinite search on impossible or extremely expensive
                        // cases.
      } else {
        cout << endl << "The ship has been balanced!" << endl;
        goalQueue.top()->getData()->print();
        cout << endl
             << "The total minute cost was: "
             << goalQueue.top()->getMinuteCost() << endl;
        chrono::time_point<chrono::system_clock> endTime =
            chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = endTime - startTime;
        cout << "The search took " << elapsed_seconds.count() << "seconds."
             << endl;
        return goalQueue.top();
      }
    }
    if (aStarFrontier.size() > maxQueueSize) {
      maxQueueSize = aStarFrontier.size();
    }

    curr = this->aStarFrontier.top();
    this->aStarFrontier.pop();

    if (!(goalQueue.empty()) &&
        goalQueue.top()->getDepth() <
            curr->getDepth()) // if there has been a goal found with a lower
                              // cost than the lowest cost node in the frontier
                              // (goalQueueCost < currCost??)
    {
      cout << "The current frontier contains no nodes with a lower cost than "
              "the current goal state."
           << endl;
      cout << "Printing goal state..." << endl << endl;
      goalQueue.top()->getData()->print();
      cout << endl
           << "The total minute cost was: " << goalQueue.top()->getMinuteCost()
           << endl;
      chrono::time_point<chrono::system_clock> endTime =
          chrono::system_clock::now();
      chrono::duration<double> elapsed_seconds = endTime - startTime;
      cout << "The search took " << elapsed_seconds.count() << "seconds."
           << endl;
      return goalQueue.top();
      // break;
    }

    for (map<Node *, int>::iterator it = exploredSet.begin();
         it != exploredSet.end(); ++it) {
      exploredAlready = true;
      // check if the curr has been explored already by comparing the Node's
      // data
      for (int column = 0; column < curr->getData()->getColumn();
           column++) // currently doesn't find differences between
      {
        for (int row = 0; row < curr->getData()->getColumn(); row++) {
          if (curr->getData()->getContainer(column, row).name ==
              it->first->getData()->getContainer(column, row).name) {
            // do nothing
          } else {
            exploredAlready = false;
            break; // breaks out of j loop
          }
        }
      }
    }

    if (exploredAlready == false || firstRun == true) {
      firstRun = false;
      this->exploredSet.insert(pair<Node *, int>(
          curr, numExplored)); // Adds removed node to explored set
      numExplored++;
      // cout << "The best state to expand with g(n) = " << curr->getDepth() <<
      // "and h(n) = " << curr->getHCost() << "is ... " << endl;
      // curr->getData()->print();

      // loop to check if it is a goal state (Node is balanced)

      sameArray = curr->isBalanced();

      if (sameArray == true) {
        goalQueue.push(curr);
        //	cout << endl << "This is a goal node and will not be expanded."
        //<< endl;
      } else {
        cout << endl << "Expanding this node..." << endl;
        curr->getData()->print();
        numExplored++;
        curr->createBalanceChildren();
        for (int i = 0; i < curr->getNumChildren();
             i++) // add all the children to the frontier
        {
          this->aStarFrontier.push(curr->getChild(i));
        }
        if (aStarFrontier.size() > maxQueueSize) {
          maxQueueSize = aStarFrontier.size();
        }
      }
    } else {
      // it will skil searching through this node again
      aStarFrontier.size(); // for debugging purposes
    }
  }
  this->maxQueueSize = maxQueueSize;
  this->numExplored = numExplored;
  if (!goalQueue.empty()) {
    cout << endl << "The ship has been balanced!" << endl;
    goalQueue.top()->getData()->print();
    cout << endl
         << "The total minute cost was: " << goalQueue.top()->getMinuteCost()
         << endl;
    chrono::time_point<chrono::system_clock> endTime =
        chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = endTime - startTime;
    cout << "The search took " << elapsed_seconds.count() << "seconds." << endl;
  }
  return goalQueue
      .top(); // returns the goal with the least total cost found (sometimes
              // returns top of goal queue without there being any added)
}
