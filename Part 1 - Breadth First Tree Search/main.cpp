/*
Christopher Torralba
Sec 1A
The main file for isotope boat
*/

#include <iostream>
#include <cmath>
#include <fstream>
#include <ctime>
#include <stdio.h>
#include <ctype.h>

#include "linkedlist.h"
#include "queue.h"
using namespace std;

struct Alligator{
	int coordX;
	int coordY;
	int index;
	char direction;
};

struct Turtle{
	int coordX;
	int coordY;
	int index;
	char direction;
};

struct Tree{
	int coordX;
	int coordY;
	int index;
};


/*Used to store a "State" contains information that defines a state such as: boat, animals, radiation, etc*/
struct State{
	int radiation;
	int boatX; //where the boat is on (x,y)
	int boatY; // where the boat is on (x,y)
	char boatDir;
	
	//Need something for animal location and direction FOR EACH animal.
	LinkedList<Alligator> aList;
	LinkedList<Turtle> tuList;
	LinkedList<Tree> trList;
	
	//consistent information
	int gridHeight;
	int gridWidth;
	int goalX;
	int goalY;
	int numAlligators;
	int numTurtles;
	int numTrees;
	int** radGrid;
	char** obsGrid; //visual representation for objects can't be moved into
	
	// Pre: performs a deep copy of the data from rhs into this state
    // Parameters: rhs is linked list to be copied
    // Returns: *this
    // Post: this list contains same data values (in the same order)
    //     as are in rhs;
  	const State&  operator = (const State& rhs);
};

 //Pre: Each state has values that are filled
 //Post: The State on the left side are filled in with values on the right side.
const State& State :: operator = (const State& rhs)
{
	if (this != &rhs)
	{
		State* p = this;
		const State* q = &rhs;
		
		p->boatDir = q->boatDir;
		p->boatX = q->boatX;
		p->boatY = q->boatY;
		p->goalX = q->goalX;
		p->goalY = q->goalY;
		p->gridHeight = q->gridHeight;
		p->gridWidth = q->gridWidth;
		p->radiation = q->radiation;
		p->numAlligators = q->numAlligators;
		p->numTrees = q->numTrees;
		p->numTurtles = q->numTurtles;
		p->aList = q->aList;
		p->trList = q->trList;
		p->tuList = q->tuList;
		
		p->obsGrid = new char*[q->gridWidth];
		p->radGrid = new int*[q->gridWidth];
	    for(int k=0; k < q->gridWidth; k++){
	      p->obsGrid[k] = new char[q->gridHeight];
	      p->radGrid[k] = new int [q->gridHeight];
	    }
		
		for (int i = 0; i < q->gridHeight; i++){
			for (int j = 0; j < q->gridWidth; j++){
				p->obsGrid[j][i] = q->obsGrid[j][i];
				p->radGrid[j][i] = q->radGrid[j][i];
			}
		}
		q = NULL;	
	}
  return *this;
}


struct Action{
	char movObj; //boat alligator turtle
	int index;
	char movement; //c clockwise n counterclock u up d down l left r right
};


//holds the state, a value pointing to it's parent that created it along with its state.
struct Node{
	Node* parent = NULL;
	State St;
	Action parentsAct;
	int pathCost;
	
	~Node();
	//Purpose: "empties" the list, 
    //          Turn the list into an empty list.
    // Postconditions: ALL dynamically allocated memory for elements is deallocated
	void clear();
};


void Node :: clear()
{
  if(parent != NULL)
  {
    parent -> clear();
    //deleting lists
    this->St.aList.clear();
    this->St.tuList.clear();
    this->St.trList.clear();
    //Deleting maps
    for(int k=0; k < this->St.gridWidth; k++){
	    delete [] this->St.radGrid[k];
	  }
	delete [] this->St.radGrid;
	
	for(int k=0; k < this->St.gridWidth; k++){
	    delete [] this->St.obsGrid[k];
	  }
	delete [] this->St.obsGrid;
    /*Deleting the parent if parent is null*/
    delete parent;
    parent = NULL; 
  }
}


Node::~Node(){
	this->clear();
}

/*Pre: actionState must have a valid boat value.
  Parameters: actionState holds information about the state, such as the radiation and the location and direction
  of the boat.
  Post: Returns the value of the radiation from the boat being in the spot of radiation
*/
int calcRad(State actionState){
	int rad;
	rad = actionState.radGrid[actionState.boatX][actionState.boatY];
	if (actionState.boatDir == 'U'){
		rad = rad + actionState.radGrid[actionState.boatX][actionState.boatY-1];
	}
	if (actionState.boatDir == 'L'){
		rad = rad + actionState.radGrid[actionState.boatX-1][actionState.boatY];
	}
	if (actionState.boatDir == 'R'){
		rad = rad + actionState.radGrid[actionState.boatX+1][actionState.boatY];
	}
	if (actionState.boatDir == 'D'){
		rad = rad + actionState.radGrid[actionState.boatX][actionState.boatY+1];
	}
	return rad;
}


//Pre: Must check if an action is a valid state or not.
//Parameters: A state and an action that is being performed
//Post: This creates a NEW with the affected action from the old state and returns it.
State result(State oldState, Action change){
	
	int coordX, coordY;
	char direction;
	State newState;
	
	newState = oldState; //overloaded operator =
	
  if (change.movObj == 'B'){

  	if (change.movement == 'u'){
  		newState.obsGrid[newState.boatX][newState.boatY-2] = 'B';
  		newState.obsGrid[newState.boatX][newState.boatY] = 'E';
  		newState.boatY--;
	}
	if (change.movement == 'l'){
		newState.obsGrid[newState.boatX-2][newState.boatY] = 'B';
  		newState.obsGrid[newState.boatX][newState.boatY] = 'E';
  		newState.boatX--;
	}
	if (change.movement == 'd'){
		newState.obsGrid[newState.boatX][newState.boatY+2] = 'B';
  		newState.obsGrid[newState.boatX][newState.boatY] = 'E';
  		newState.boatY++;
	}
	if (change.movement == 'r'){
		newState.obsGrid[newState.boatX+2][newState.boatY] = 'B';
  		newState.obsGrid[newState.boatX][newState.boatY] = 'E';
  		newState.boatX++;
	}
	if (change.movement == 'c'){
		if (newState.boatDir == 'U'){
			newState.obsGrid[newState.boatX][newState.boatY-1] = 'E';
	  		newState.obsGrid[newState.boatX+1][newState.boatY] = 'B';
			newState.boatDir = 'R';
		}
  		else if (oldState.boatDir == 'R'){
  			newState.obsGrid[newState.boatX+1][newState.boatY] = 'E';
	  		newState.obsGrid[newState.boatX][newState.boatY+1] = 'B';
			newState.boatDir = 'D';
		}
		else if (oldState.boatDir == 'D'){
			newState.obsGrid[newState.boatX][newState.boatY+1] = 'E';
	  		newState.obsGrid[newState.boatX-1][newState.boatY] = 'B';
			newState.boatDir = 'L';
		}
		else{ // if (oldState.boatDir == 'L'){
			newState.obsGrid[newState.boatX-1][newState.boatY] = 'E';
	  		newState.obsGrid[newState.boatX][newState.boatY-1] = 'B';
			newState.boatDir = 'U';
		}
	}
	if (change.movement == 'n'){
  		if (oldState.boatDir == 'U'){
  			newState.obsGrid[newState.boatX][newState.boatY-1] = 'E';
	  		newState.obsGrid[newState.boatX-1][newState.boatY] = 'B';
			newState.boatDir = 'L';
		}
  		else if (newState.boatDir == 'R'){
  			newState.obsGrid[newState.boatX+1][newState.boatY] = 'E';
	  		newState.obsGrid[newState.boatX][newState.boatY-1] = 'B';
			newState.boatDir = 'U';
		}
		else if (oldState.boatDir == 'D'){
			newState.obsGrid[newState.boatX][newState.boatY+1] = 'E';
	  		newState.obsGrid[newState.boatX+1][newState.boatY] = 'B';
			newState.boatDir = 'R';
		}
		else{ //(oldState.boatDir == 'L'){
			newState.obsGrid[newState.boatX-1][newState.boatY] = 'E';
	  		newState.obsGrid[newState.boatX][newState.boatY+1] = 'B';
			newState.boatDir = 'D';
		}
	}
  }
  if (change.movObj == 'A'){
  	
  		LinkedList<Alligator>* aPtr = newState.aList.findIndex(change.index);
		coordX = aPtr->m_data.coordX;
		coordY = aPtr->m_data.coordY;
		direction = aPtr->m_data.direction;
  		
  	if (change.movement == 'u'){
  		if (direction == 'U'){
  		  newState.obsGrid[coordX][coordY] = 'E';
	  	  newState.obsGrid[coordX][coordY-3] = 'A';
	    }
	    if (direction == 'D'){
	    	newState.obsGrid[coordX][coordY+2] = 'E';
	    	newState.obsGrid[coordX][coordY-1] = 'A';
		}
		aPtr->m_data.coordY = aPtr->m_data.coordY-1;
	}
	if (change.movement == 'l'){
		if (direction == 'L'){
		  newState.obsGrid[coordX][coordY] = 'E';
	  	  newState.obsGrid[coordX-3][coordY] = 'A';
  	    }
  	    if (direction == 'R'){
  	      newState.obsGrid[coordX+2][coordY] = 'E';
	  	  newState.obsGrid[coordX-1][coordY] = 'A';
		}
		aPtr->m_data.coordX = aPtr->m_data.coordX-1;
	}
	if (change.movement == 'd'){
		if (direction == 'D'){
			newState.obsGrid[coordX][coordY] = 'E';
	  		newState.obsGrid[coordX][coordY+3] = 'A';
		}
		if (direction == 'U'){
			newState.obsGrid[coordX][coordY-2] = 'E';
	  		newState.obsGrid[coordX][coordY+1] = 'A';
		}
  		aPtr->m_data.coordY = aPtr->m_data.coordY+1;
	}
	if (change.movement == 'r'){
		if (direction == 'R'){
		  newState.obsGrid[coordX][coordY] = 'E';
	  	  newState.obsGrid[coordX+3][coordY] = 'A';
	    }
	    if (direction == 'L'){
		  newState.obsGrid[coordX-2][coordY] = 'E';
	  	  newState.obsGrid[coordX+1][coordY] = 'A';
	    }
	     aPtr->m_data.coordX = aPtr->m_data.coordX+1;
	}
	aPtr = NULL;
  }
  if (change.movObj == 'T'){
  	LinkedList<Turtle>* tuPtr = newState.tuList.findIndex(change.index);
		coordX = tuPtr->m_data.coordX;
		coordY = tuPtr->m_data.coordY;
		direction = tuPtr->m_data.direction;

  	if (change.movement == 'u'){
  		if (direction == 'U'){
  		  newState.obsGrid[coordX][coordY] = 'E';
	  	  newState.obsGrid[coordX][coordY-2] = 'T';
  		}
  		if (direction == 'D'){
  		  newState.obsGrid[coordX][coordY+1] = 'E';
	  	  newState.obsGrid[coordX][coordY-1] = 'T';
  		}
  		tuPtr->m_data.coordY = tuPtr->m_data.coordY-1;
	}
	if (change.movement == 'l'){
		if (direction == 'L'){
		  newState.obsGrid[coordX][coordY] = 'E';
	  	  newState.obsGrid[coordX-2][coordY] = 'T';
  	    }
  	    if (direction == 'R'){
		  newState.obsGrid[coordX+1][coordY] = 'E';
	  	  newState.obsGrid[coordX-1][coordY] = 'T';
  	    }
  	    tuPtr->m_data.coordX = tuPtr->m_data.coordX-1;
  	    
	}
	if (change.movement == 'd'){
		if (direction == 'D'){
		  newState.obsGrid[coordX][coordY] = 'E';
	  	  newState.obsGrid[coordX][coordY+2] = 'T';
	    }
	    if (direction == 'U'){
		  newState.obsGrid[coordX][coordY-1] = 'E';
	  	  newState.obsGrid[coordX][coordY+1] = 'T';
	    }
  		tuPtr->m_data.coordY = tuPtr->m_data.coordY+1;
	}
	if (change.movement == 'r'){
		if (direction == 'R'){
		  newState.obsGrid[coordX][coordY] = 'E';
	  	  newState.obsGrid[coordX+2][coordY] = 'T';
	    }
	    if (direction == 'L'){
	      newState.obsGrid[coordX-1][coordY] = 'E';
	  	  newState.obsGrid[coordX+1][coordY] = 'T';
		}
  		tuPtr->m_data.coordX = tuPtr->m_data.coordX+1;
	}
	tuPtr = NULL;
  }
  	newState.radiation = calcRad(newState);
  	return (newState);
}

/*Pre: Determines if an action is valid with a state.
Parameters: State with values for boat and animals, and the action being performed.
Post: Returns a boolean, if the action is able to be performed. For example this would
return false if the boat is trying to move out of bounds.*/
bool validAction(State S, Action A);
bool validAction(State S, Action A){
  int coordX;
  int coordY;
  char direction;
  if (A.movObj == 'B'){
    if (A.movement == 'u'){
    	if (S.boatDir != 'U'){
    		return false;
		}
		if (!(S.boatY - 2 < 0)){
			return false;
		}
		if (!(S.obsGrid[S.boatX][S.boatY-2] == 'E')){
			return false;
		}
	}
	if (A.movement == 'l'){
		if (S.boatDir != 'L'){
			return false;
		}
		if (S.boatX-2 < 0){
			return false;
		}
		if (!(S.obsGrid[S.boatX-2][S.boatY] == 'E')){
			return false;
		}
	}
	if (A.movement == 'r'){
		if (S.boatDir != 'R'){
			return false;
		}
		if (S.boatX + 2 >= S.gridWidth){
			return false;
		}
		if (!(S.obsGrid[S.boatX+2][S.boatY] == 'E')){
			return false;
		}
	}
	if (A.movement == 'd'){
		if (S.boatDir != 'D'){
			return false;
		}
		if (S.boatY + 2 >= S.gridHeight){
			return false;
		}
		if (!(S.obsGrid[S.boatX][S.boatY+2] == 'E')){
			return false;
		}
	}
	if (A.movement == 'c'){
		if (S.boatX == 0 && S.boatDir == 'D'){
			return false;
		}
		if (S.boatY == 0 && S.boatDir == 'L'){
			return false;
		}
		if (S.boatDir == 'U'){
			if (!(S.obsGrid[S.boatX+1][S.boatY] == 'E' &&
			S.obsGrid[S.boatX+1][S.boatY-1] == 'E')){
				return false;
			}
		}
		if (S.boatDir == 'R'){
			if (!(S.obsGrid[S.boatX][S.boatY+1] == 'E'  &&
			S.obsGrid[S.boatX+1][S.boatY+1] == 'E')){
				return false;
			}
		}
		if (S.boatDir == 'D'){
			if (!(S.obsGrid[S.boatX-1][S.boatY] == 'E'  &&
			S.obsGrid[S.boatX-1][S.boatY+1] == 'E')){
				return false;
			}
		}
		if (S.boatDir == 'L'){
			if (!(S.obsGrid[S.boatX+1][S.boatY-1] == 'E' &&
			S.obsGrid[S.boatX][S.boatY-1] == 'E')){
				return false;
			}
		}
	}
	
	if (A.movement == 'n'){
		if (S.boatY == 0 && S.boatDir ==  'R'){
			return false;
		}
		if (S.boatX == S.gridWidth-1 && S.boatDir == 'D'){
			return false;
		}
		if (S.boatDir == 'U'){
			if (!(S.obsGrid[S.boatX-1][S.boatY] == 'E' &&
			S.obsGrid[S.boatX-1][S.boatY-1] == 'E')){
				return false;
			}
		}
		if (S.boatDir == 'R'){
			if (!(S.obsGrid[S.boatX][S.boatY-1] == 'E'  &&
			S.obsGrid[S.boatX+1][S.boatY-1] == 'E')){
				return false;
			}
		}
		if (S.boatDir == 'D'){
			if (!(S.obsGrid[S.boatX+1][S.boatY] == 'E'  &&
			S.obsGrid[S.boatX+1][S.boatY+1] == 'E')){
				return false;
			}
		}
		if (S.boatDir == 'L'){
			if (!(S.obsGrid[S.boatX-1][S.boatY+1] == 'E' &&
			S.obsGrid[S.boatX][S.boatY+1] == 'E')){
				return false;
			}
		}
	}
  }
  else if (A.movObj == 'A'){
  	LinkedList<Alligator>* aPtr = S.aList.findIndex(A.index);
		coordX = aPtr->m_data.coordX;
		coordY = aPtr->m_data.coordY;
		direction = aPtr->m_data.direction;
		aPtr = NULL;
		
  	if (A.movement == 'u'){
    	if (!(direction == 'U')||(direction =='D')){
    		return false;
		}
		if (direction == 'U'){
		  if (coordY < 3){
			return false;
		  }
		  if (!(S.obsGrid[coordX][coordY-3] == 'E')){
			return false;
	      }
	    }
	    if (direction == 'D'){
	      if (coordY <= 0){
			return false;
		  }
	      if (!(S.obsGrid[coordX][coordY-1] == 'E')){
	    	return false;
		  }
		}
	}
	if (A.movement == 'l'){
		if (!(direction == 'L' || direction == 'R')){
			return false;
		}
		if (direction == 'L'){
			if (coordX < 3){
			  return false;
			}
			if (!(S.obsGrid[coordX-3][coordY] == 'E')){
			  return false;
			}
		}
		if (direction == 'R'){
			if (coordX <= 0){
			  return false;
		    }
			if (!(S.obsGrid[coordX-1][coordY] == 'E')){
				return false;
			}
		}
	}
	if (A.movement == 'r'){
		if (!(direction == 'R' || direction == 'L')){
			return false;
		}
		if (direction == 'R'){
		  if (coordX + 3 >= S.gridWidth){
			return false;
		  }
		  if (!(S.obsGrid[coordX+3][coordY] == 'E')){
			return false;
	      }
		}
		if (direction == 'L'){
		  if (coordX >= S.gridWidth-1){
			return false;
		  }
		  if (!(S.obsGrid[coordX+1][coordY] == 'E')){
			return false;
	      }
		}
	}
	if (A.movement == 'd'){
		if (!(direction == 'D' || direction == 'U')){
			return false;
		}
		if (direction == 'D'){
		  if (coordY + 3 >= S.gridHeight){
			return false;
		  }
		  if (!(S.obsGrid[coordX][coordY+3] == 'E')){
			return false;
	      }
	    }
	    if (direction == 'U'){
	      if (coordY + 1 >= S.gridHeight){
			return false;
		  }
	      if (!(S.obsGrid[coordX][coordY+1] == 'E')){
			return false;
	      }	
		}
	}
  }
  else{
		LinkedList<Turtle>* tuPtr = S.tuList.findIndex(A.index);
		coordX = tuPtr->m_data.coordX;
		coordY = tuPtr->m_data.coordY;
		direction = tuPtr->m_data.direction;
		tuPtr = NULL;
			
		if (A.movement == 'u'){
	    	if (!(direction == 'U' || direction == 'D')){
	    		return false;
			}
			if (direction == 'U'){
				if (coordY < 2){
				  return false;
				}
				if (!(S.obsGrid[coordX][coordY-2] == 'E')){
					return false;
				}
			}
			if (direction == 'D'){
				if (coordY <= 0){
				  return false;
				}
				if (!(S.obsGrid[coordX][coordY-1] == 'E')){
					return false;
				}
			}
		}
		if (A.movement == 'l'){
			if (!(direction == 'L' || direction == 'R')){
				return false;
			}
			if (direction == 'L'){
			  if (coordX < 2){
				return false;
			  }	
			  if (!(S.obsGrid[coordX-2][coordY] == 'E')){
			    return false;
			  }
			}
			if (direction == 'R'){
		      if (coordX <= 0){
				return false;
			  }
			  if (!(S.obsGrid[coordX-1][coordY]) == 'E'){
			  	return false;
			  }
			}
		}
		if (A.movement == 'r'){
			if (!(direction == 'R' || direction == 'L')){
				return false;
			}
			if (direction == 'R'){
			  if (coordX > S.gridWidth-3){
				return false;
			  }
			  if (!(S.obsGrid[coordX+2][coordY] == 'E')){
			    return false;
			  }
			}
			if (direction == 'L'){
				if (coordX > S.gridWidth-1){
				  return false;
			    }
			  if (!(S.obsGrid[coordX+1][coordY] == 'E')){
			    return false;
			  }
			}
		}
		if (A.movement == 'd'){
			if (!(direction == 'D' || direction == 'U')){
				return false;
			}
			if (direction == 'D'){
				if (coordY > S.gridHeight-3){
				  return false;
			    }
				if (!(S.obsGrid[coordX][coordY+2] == 'E')){
					return false;
				}
			}
			if (direction == 'U'){
				if (coordY > S.gridHeight-1){
				  return false;
			    }
				if (!(S.obsGrid[coordX][coordY+1] == 'E')){
					return false;
				}
			}
		}
   }
   return true;
}

/*This creates the node for a tree
  Pre: The Parent Node, the action from the parent being performed, and the state the action
       is GOING to be performed on.
  Post: Returns a pointer to the node of the child node that was created. This child node holds
  values for it's path cost, the parent that created it, etc.*/
Node* childNode(Node* Parent, Action parentAction, State parentState);
Node* childNode(Node* Parent, Action parentAction, State parentState){

	Node* nodePtr = new Node;	
	nodePtr->parent = Parent;
	nodePtr->St = result(parentState, parentAction); //returns a new state.
	nodePtr->parentsAct.index = parentAction.index;
	nodePtr->parentsAct.movement = parentAction.movement;
	nodePtr->parentsAct.movObj = parentAction.movObj;
	nodePtr->pathCost = Parent->pathCost + nodePtr->St.radiation;
	return nodePtr;
}

/*Determines if the current state the boat is in is a goal state
Pre: a state with a valid state and goal.
Post: returns true if boat is on goal (x,y) else false*/
bool checkGoal(State goalState);
bool checkGoal(State goalState){
	if (goalState.boatX == goalState.goalX && goalState.boatY == goalState.goalY){
		return true;	
	}
	switch(goalState.boatDir){
		case ('U'):
			if (goalState.boatY-1 == goalState.goalY && goalState.boatX == goalState.goalX){
				return true;
			}
		break;
		case ('L'):
			if (goalState.boatX-1 == goalState.goalX && goalState.boatY == goalState.goalY){
				return true;
			}
		break;
		case ('D'):
			if (goalState.boatY+1 == goalState.goalY && goalState.boatX == goalState.goalX){
				return true;
			}
		break;
		case('R'):
			if (goalState.boatX+1 == goalState.goalX && goalState.boatY == goalState.goalY){
				return true;
			}
		break;
	}
	return false;
}


/*The breadth first tree search algorithm
  It first creates a pregenerated list of actions that can be performed on a state whether
  invalid or valid and then from the root parameter it puts it onto a frontier queue.
  The frontier queue gets evaluated and checked if its a goal state. Returns a pointer
  if the goal state was found.
*/
Node* BFTS(Node* root);
Node* BFTS(Node* root){
	//list possible actions of the root
	//put it into childNode(root, action, state of root)
	//each node has one parent
	ArrayQueue<Node*> frontier;
	frontier.enqueue(root);
	LinkedList<Action> actList;
	Action act;
	/*Pre generated list of actions*/
	for (int i = 0; i < 6; i++){
		act.movObj = 'B';
		act.index = 0;
		switch (i){
			case 0:
				act.movement = 'u';
			break;
			case 1:
				act.movement = 'r';
			break;
			case 2:
				act.movement = 'd';
			break;
			case 3:
				act.movement = 'l';
			break;
			case 4:
				act.movement = 'c';
			break;
			case 5:
				act.movement = 'n';
			break;
		}
		actList.insert_front(act);
	}//0 to 5

	//6 to 6+numAlligators*4
	for (int i = 6; i < 6+(root->St.numAlligators*4); i++){
		act.movObj = 'A';
		act.index = ((i-6)/4);
		switch ((i-6) % 4){
			case 0:
				act.movement = 'u';
			break;
			case 1:
				act.movement = 'r';
			break;
			case 2:
				act.movement = 'd';
			break;
			case 3:
				act.movement = 'l';
			break;
		}
		actList.insert_front(act);
	}
	//6+numAlligators*4 to 6+numAlligators*4+numTurtles*4
	for (int i = 6+(root->St.numAlligators*4); i < (6+(root->St.numAlligators*4)+(root->St.numTurtles*4)); i++){
		act.movObj = 'T';
		act.index = ((i-6-(root->St.numAlligators*4))/4);
		switch ((i-6-(root->St.numAlligators*4)) % 4){
			case 0:
				act.movement = 'u';
			break;
			case 1:
				act.movement = 'r';
			break;
			case 2:
				act.movement = 'd';
			break;
			case 3:
				act.movement = 'l';
			break;
		}
		actList.insert_front(act);
	}
	
	/* Done generating all the actions */
	LinkedList<Action>* possibleAction = actList.getFirstPtr();
	
	Node* eval;
	Action action;
	eval = frontier.front();
	while(!checkGoal(eval->St)){
		frontier.dequeue();
		while (possibleAction->m_next != NULL){
			
			if (validAction(eval->St, possibleAction->m_data)){
				action = possibleAction->m_data; 
				Node* createdNode = childNode(eval, action, eval->St); //parent's node, action, state
				frontier.enqueue(createdNode);
			}
			possibleAction = possibleAction -> m_next;
		}
		possibleAction = actList.getFirstPtr();
		eval = frontier.front();
	}
	if (checkGoal(eval->St)){
		frontier.clear();
		return (eval);
	}
}

int main()
{
	ifstream puzzleFile;
	ofstream solutionFile;
	string line;
	/*Variable Declerations*/
	int gridW, gridH, radSourceX, radSourceY, radMag, radDecay, numAlligators, numTurtles, numTrees,
	coordX, coordY, index;
	int numAnimals = 0;
	char directionFace;
	LinkedList<Alligator> aList;
	LinkedList<Turtle> tuList;
	LinkedList<Tree> trList;
    Alligator gator;
    Turtle turtle;
    Tree tree;
    
	Node* root = new Node;
	/*Reading-in Input*/
	/*Setting Initial Node*/
	
	puzzleFile.open("puzzles/puzzle1.txt");
    puzzleFile >> gridW >> gridH >> radSourceX >> radSourceY >> radMag >> radDecay >> numAlligators >> numTurtles >> numTrees;    
    
	root->parent = NULL;
	
	root->St.gridHeight = gridH;
	root->St.gridWidth = gridW;
	
	
	//Create the Radiation Grid
	//allocating to Radiation Grid
	root->St.radGrid = new int*[gridW];
    for(int k=0; k < gridW; k++){
      root->St.radGrid[k] = new int[gridH];
    }

	root->St.radGrid[radSourceX][radSourceY] = radMag;
	for (int i = 0; i < gridW; i++){
		for (int j = 0; j < gridH; j++){
			root->St.radGrid[i][j] = radMag - ((abs(j-radSourceY)+abs(i-radSourceX))*radDecay);
		}
	}
	
	//Obstacles
	root->St.obsGrid = new char*[gridW];
    for(int k=0; k < gridW; k++){
      root->St.obsGrid[k] = new char[gridH];
    }
    
    for (int i = 0; i < gridW; i++){
		for (int j = 0; j < gridH; j++){
			root->St.obsGrid[i][j] = 'E';
		}
	}

	root->St.numAlligators = numAlligators;
	root->St.numTurtles = numTurtles;
	root->St.numTrees = numTrees;
	for (int i = 0; i  < numAlligators; i++){
	  puzzleFile >> coordX;
	  puzzleFile >> coordY;
	  puzzleFile >> directionFace;
	  
	  root->St.obsGrid[coordX][coordY] = 'A';
	  if (directionFace == 'U'){
	  	root->St.obsGrid[coordX][coordY-1] = 'A';
	  	root->St.obsGrid[coordX][coordY-2] = 'A';
	  }
	  if (directionFace == 'L'){
	  	root->St.obsGrid[coordX-1][coordY] = 'A';
	  	root->St.obsGrid[coordX-2][coordY] = 'A';
	  }
	  if (directionFace == 'R'){
	  	root->St.obsGrid[coordX+2][coordY] = 'A';
	  	root->St.obsGrid[coordX+1][coordY] = 'A';
	  }
	  if (directionFace == 'D'){
	  	root->St.obsGrid[coordX][coordY+1] = 'A';
	  	root->St.obsGrid[coordX][coordY+2] = 'A';
	  }
	  
	  gator.coordX = coordX;
	  gator.coordY = coordY;
	  gator.direction = directionFace;
	  gator.index = i;
	  aList.insert_front(gator);
	}
	
	root->St.aList = aList;
	
	//Turtle storage for initial node
	for (int i = 0; i < numTurtles; i++){
	  puzzleFile >> coordX;
	  puzzleFile >> coordY;
	  puzzleFile >> directionFace;
	  	
	  root->St.obsGrid[coordX][coordY] = 'T';
	  if (directionFace == 'U'){
	  	root->St.obsGrid[coordX][coordY-1] = 'T';
	  }
	  if (directionFace == 'L'){
	  	root->St.obsGrid[coordX-1][coordY] = 'T';
	  }
	  if (directionFace == 'R'){
	  	root->St.obsGrid[coordX+1][coordY] = 'T';
	  }
	  if (directionFace == 'D'){
	  	root->St.obsGrid[coordX][coordY+2] = 'T';
	  }
	  
	  turtle.coordX = coordX;
	  turtle.coordY = coordY;
	  turtle.direction = directionFace;
	  turtle.index = i;
	  tuList.insert_front(turtle);
	}

	root->St.tuList = tuList;
	  
	//tree storage for initial node
	for (int i = 0; i  < numTrees; i++){
	  puzzleFile >> coordX;
	  puzzleFile >> coordY;
	  root->St.obsGrid[coordX][coordY] = 'G';
	  tree.coordX = coordX;
	  tree.coordY = coordY;
	  tree.index = i;
	  trList.insert_front(tree);
	}
	root->St.trList = trList;

	puzzleFile >> coordX;
	puzzleFile >> coordY;
	puzzleFile >> directionFace;
	
	root->St.radiation = 0;
	root->St.boatX = coordX;
	root->St.boatY = coordY;
	root->St.boatDir = directionFace;
	
	root->St.obsGrid[coordX][coordY] = 'B';
	  if (directionFace == 'U'){
	  	root->St.obsGrid[coordX][coordY-1] = 'B';
	  }
	  if (directionFace == 'L'){
	  	root->St.obsGrid[coordX-1][coordY] = 'B';
	  }
	  if (directionFace == 'R'){
	  	root->St.obsGrid[coordX+1][coordY] = 'B';
	  }
	  if (directionFace == 'D'){
	  	root->St.obsGrid[coordX][coordY+2] = 'B';
	  }

	/*Getting Goal Information*/  
	puzzleFile >> coordX;
	puzzleFile >> coordY;
	root->St.goalX = coordX;
	root->St.goalY = coordY;
	root->pathCost = 0;
	
	puzzleFile.close();	
	/*Data for creating the tree*/
	LinkedList<Action> solution;
	
	/*Putting Root to the frontier*/

	puzzleFile.close();
	/*Create Tree is the main algorithm function*/
	clock_t begin = clock();
	Node* results = BFTS(root);
	clock_t end = clock();
	
	
	double time_spent = double(end-begin) / CLOCKS_PER_SEC;
	time_spent = time_spent * 1000; //conversion from sec to ms
	
	/*Output*/
	solutionFile.open("solutions/solution1.txt");
	//Outputting Time Spent
	cout << time_spent << endl;
	solutionFile << time_spent << "\n";
	Node* iter = results;
	LinkedList<Action>* actionResults;
	//Outputing radiation
	cout << results->pathCost - results->St.radiation << endl;
	solutionFile << results->pathCost - results->St.radiation << "\n";
	int actionCount = 0;
	//output actions
	while (iter->parent != NULL){
		actionCount++;
		solution.insert_front(iter->parentsAct);
		iter = iter->parent;
	}
	//outputting number of actions
	cout << actionCount << endl;
	solutionFile << actionCount << "\n";
	actionResults = solution.getFirstPtr();
	while (actionResults -> m_next != NULL){
		cout << actionResults->m_data.movObj <<" " << actionResults->m_data.index << " " << (char)toupper(actionResults->m_data.movement);
		solutionFile << actionResults->m_data.movObj <<" " << actionResults->m_data.index << " " << (char)toupper(actionResults->m_data.movement) << "\n";
		actionResults = actionResults -> m_next;
		if ((actionResults -> m_next != NULL)){
			cout << ",";
			solutionFile << ",";
		}
	}
	cout << endl;
	//Outputting Grid Information
	
	cout << gridW << " " << gridH << endl;
	solutionFile << gridW << " " << gridH << "\n";
	cout << radSourceX << " " << radSourceY << endl;
	solutionFile << radSourceX << " " << radSourceY << "\n";
	cout << radMag << " " << radDecay << endl;
	solutionFile << radMag << " " << radDecay << "\n";
	cout << numAlligators << " " << numTurtles << " " << numTrees << endl;
	solutionFile << numAlligators << " " << numTurtles << " " << numTrees <<"\n";
	//Animal Information
	LinkedList<Alligator>* A = results->St.aList.getFirstPtr();
	A->reverse();
	while (A->m_next != NULL) {
		cout << A->m_data.coordX << " " << A->m_data.coordY << " " << A->m_data.direction << endl;
		solutionFile << A->m_data.coordX << " " << A->m_data.coordY << " " << A->m_data.direction << "\n";
		A = A->m_next;
	}
	LinkedList<Turtle>* T = results->St.tuList.getFirstPtr();
	T->reverse();
	while (T->m_next != NULL) {
		cout << T->m_data.coordX << " " << T->m_data.coordY << " " << T->m_data.direction << endl;
		solutionFile << T->m_data.coordX << " " << T->m_data.coordY << " " << T->m_data.direction << "\n";
		T = T->m_next;
	}
	LinkedList<Tree>* treeptr = results->St.trList.getFirstPtr();
	treeptr->reverse();
	while (treeptr->m_next != NULL) {
		cout << treeptr->m_data.coordX << " " << treeptr->m_data.coordY << endl;
		solutionFile << treeptr->m_data.coordX << " " << treeptr->m_data.coordY << "\n";
		treeptr = treeptr->m_next;
	}
	//Boat & Goal Information
	cout << results->St.boatX << " " << results->St.boatY << " " << results->St.boatDir << endl;
	solutionFile << results->St.boatX << " " << results->St.boatY << " " << results->St.boatDir << "\n";
	cout << results->St.goalX << " " << results->St.goalY << endl;
	solutionFile << results->St.goalX << " " << results->St.goalY << "\n";
    solutionFile.close();
	delete root;
	return 0;
}
