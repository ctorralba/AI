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
#include <queue>
#include <vector>
#include <algorithm>

#include "linkedlist.h"

using namespace std;

int nodeCreation = 0;
int gridsCreated = 0;

struct Alligator{
	int coordX;
	int coordY;
	int index;
	char direction;
	Alligator();
	Alligator(const Alligator& rhs);
	const Alligator& operator = (const Alligator& rhs);
};

Alligator::Alligator(){
	coordX = 0;
	coordY = 0;
	index = 0;
	direction = 'N';
}

Alligator::Alligator(const Alligator& rhs){
	coordX = rhs.coordX;
	coordY = rhs.coordY;
	index = rhs.index;
	direction = rhs.direction;
}

const Alligator& Alligator :: operator= (const Alligator& rhs){
	coordX = rhs.coordX;
	coordY = rhs.coordY;
	index = rhs.index;
	direction = rhs.direction;
	return(*this);
}


struct Turtle{
	int coordX;
	int coordY;
	int index;
	char direction;
	Turtle();
	Turtle(const Turtle& rhs);
	const Turtle& operator = (const Turtle& rhs);
	bool operator!= (const Turtle& rhs) const;
};

Turtle::Turtle(){
	coordX = 0;
	coordY = 0;
	index = 0;
	direction = 'N';
}

Turtle::Turtle(const Turtle& rhs){
	coordX = rhs.coordX;
	coordY = rhs.coordY;
	index = rhs.index;
	direction = rhs.direction;
}
const Turtle& Turtle :: operator = (const Turtle& rhs){
	coordX = rhs.coordX;
	coordY = rhs.coordY;
	index = rhs.index;
	direction = rhs.direction;
	return(*this);
}

struct Tree{
	int coordX;
	int coordY;
	int index;
	Tree();
	Tree(const Tree& rhs);
	const Tree&  operator = (const Tree& rhs);
	bool operator== (const Tree& rhs) const;
};

Tree::Tree(){
	coordX = 0;
	coordY = 0;
	index = 0;
}

Tree::Tree(const Tree& rhs){
	coordX = rhs.coordX;
	coordY = rhs.coordY;
	index = rhs.index;
}
const Tree& Tree ::  operator = (const Tree& rhs){
	coordX = rhs.coordX;
	coordY = rhs.coordY;
	index = rhs.index;
	return(*this);
}


/*Used to store a "State" contains information that defines a state such as: boat, animals, radiation, etc*/
struct State{
	int radiation;
	int boatX; //where the boat is on (x,y)
	int boatY; //where the boat is on (x,y)
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
	int radX;
	int radY;
	int radMag;
	int radDecay;

	// Pre: performs a deep copy of the data from rhs into this state
    // Parameters: rhs is linked list to be copied
    // Returns: *this
    // Post: this list contains same data values (in the same order)
    //     as are in rhs;
    
    State& setState(State* writeState);
	
    void createAGrid(int rows, int columns);
    State();
    State(const State& rhs);
  	const State&  operator = (const State& rhs);
  	
  	//Debugging functions
  	void displayGrid();
    void displayData();
};

State::State(){

	boatDir = 'A';
	boatX = 0;
	boatY = 0;
	goalX = 0;
	goalY = 0;
	gridHeight = 1;
	gridWidth = 1;
	radiation = 0;
	numAlligators = 1;
	numTurtles = 1;
	numTrees = 1;
	radX=0;
	radY=0;
	radMag=0;
	radDecay=0;
}

void State :: createAGrid(int rows, int columns){
	
	char** obsGrid;
	obsGrid = new char*[columns];
    for(int k=0; k < columns; k++){
      obsGrid[k] = new char[rows];
    }
	obsGrid[1][1] = 'R';
    for (int i = 0; i < columns; i++){
		for (int j = 0; j < rows; j++){
			obsGrid[i][j] = 'E';
		}
	}
}

void State::displayData(){
	cout << "BoatDir: " << boatDir << ", (" << boatX << "," << boatY << ")" << endl;
	cout << "Goal: (" << goalX << "," << goalY << ")" << endl;
	cout << "Grid Size: " << gridWidth << "x" << gridHeight << endl;
	cout << "Radiation: " << radiation << endl;
	cout << "numAlligators: " << numAlligators << ", numTurtles: " << numTurtles << ", numTrees: " << numTrees << endl;
	cout << "RadDecay-" << radDecay << " (" << radX << "," << radY << ")" << " Rad Mag: "<< radMag << endl;
}

void State::displayGrid(){
  char** obsGrid = new char*[gridHeight];
  for (int i = 0; i < gridHeight; i++){
  	obsGrid[i] = new char[gridWidth];
  }
  for (int i = 0; i < gridWidth; i++){
  	for (int j = 0; j < gridHeight; j++){
  		obsGrid[j][i] = 'E';
	  }
  }
   
  int inputX, inputY;
  
  inputX = boatX;
  inputY = boatY;
  obsGrid[inputY][inputX] = 'B';
  switch(boatDir){
  	case 'U':
  		inputY--;
  	break;
  	case 'R':
  		inputX++;
  	break;
  	case 'L':
  		inputX--;
  	break;
  	case 'D':
  		inputY++;
	break;
  }
  obsGrid[inputY][inputX] = 'B';
  
  LinkedList<Alligator>* A = aList.getFirstPtr();
  while (A -> m_next != NULL){
  	inputX = A->m_data.coordX;
  	inputY = A->m_data.coordY;
  	obsGrid[inputY][inputX] = 'A';
  	switch(A->m_data.direction){
	  	case 'U':
	  		obsGrid[inputY-2][inputX] = 'A';
	  		obsGrid[inputY-1][inputX] = 'A';
	  	break;
	  	case 'R':
	  		obsGrid[inputY][inputX+2] = 'A';
	  		obsGrid[inputY][inputX+1] = 'A';
	  	break;
	  	case 'L':
	  		obsGrid[inputY][inputX+1] = 'A';
	  		obsGrid[inputY][inputX+2] = 'A';
	  	break;
	  	case 'D':
	  		obsGrid[inputY+2][inputX] = 'A';
	  		obsGrid[inputY+1][inputX] = 'A';
	  	break;
	}
  	A = A->m_next;
  }
  
  LinkedList<Turtle>* T = tuList.getFirstPtr();
  while (T -> m_next != NULL){
  	inputX = T->m_data.coordX;
  	inputY = T->m_data.coordY;
  	obsGrid[inputY][inputX] = 'T';
  	switch(T->m_data.direction){
	  	case 'U':
	  		inputY--;
	  	break;
	  	case 'R':
	  		inputX++;
	  	break;
	  	case 'L':
	  		inputX--;
	  	break;
	  	case 'D':
	  		inputY++;
	  	break;
	}
	obsGrid[inputY][inputX] = 'T';
  	T = T->m_next;
  }
  
  LinkedList<Tree>* C = trList.getFirstPtr();
  while (C -> m_next != NULL){
  	inputX = C->m_data.coordX;
  	inputY = C->m_data.coordY;
  	obsGrid[inputY][inputX] = 'C';
  	C=C->m_next;
  }
  for (int i = 0; i < gridHeight; i++){
	for (int j = 0; j < gridWidth; j++){
		cout << obsGrid[i][j] << " ";
	}
	cout << endl;
  }
  
  for (int i = 0; i < gridHeight; i++){
	delete [] obsGrid [i];
  }
  delete [] obsGrid;
}

State::State(const State& copyMe){
	boatDir = copyMe.boatDir;
	boatX = copyMe.boatX;
	boatY = copyMe.boatY;
	goalX = copyMe.goalX;
	goalY = copyMe.goalY;
	radX = copyMe.radX;
	radY = copyMe.radY;
	radMag = copyMe.radMag;
	radDecay = copyMe.radDecay;
	gridHeight = copyMe.gridHeight;
	gridWidth = copyMe.gridWidth;
	radiation = copyMe.radiation;
	numAlligators = copyMe.numAlligators;
	numTrees = copyMe.numTrees;
	numTurtles = copyMe.numTurtles;
	  aList = copyMe.aList;	
	  trList = copyMe.trList;	
	  tuList = copyMe.tuList;
}

 //Pre: Each state has values that are filled
 //Post: The State on the left side are filled in with values on the right side.
const State& State :: operator = (const State& rhs)
{
	cout << "In operator = for state" << endl;
	if (this != &rhs)
	{
		State* p = this;
		const State* q = &rhs;
		
		boatDir = q->boatDir;
		boatX = q->boatX;
		boatY = q->boatY;
		goalX = q->goalX;
		goalY = q->goalY;
		gridHeight = q->gridHeight;
		gridWidth = q->gridWidth;
		radiation = q->radiation;
		numAlligators = q->numAlligators;
		numTrees = q->numTrees;
		numTurtles = q->numTurtles;
		aList = q->aList;
		trList = q->trList;
		tuList = q->tuList;
		radiation = q->radiation;
		q = NULL;
		gridsCreated++;	
	  }
	  return *this;
}


struct Action{
	char movObj; //boat alligator turtle
	int index;
	char movement; //c clockwise n counterclock u up d down l left r right
	
	Action();
};

Action::Action(){
	movObj = 'N';
	index = 0;
	movement = 'U';
}

//holds the state, a value pointing to it's parent that created it along with its state.
struct Node{
	Node* parent;
	State* St;
	Action parentsAct;
	int pathCost;
	int hVal;
	
	~Node(){}
	//Purpose: "empties" the list, 
    //          Turn the list into an empty list.
    // Postconditions: ALL dynamically allocated memory for elements is deallocated
	//void clear();
	
	// Pre: performs a deep copy of the data from rhs into this node
    //const Node&  operator = (const Node& rhs);
  	Node();
  	Node(const Node& rhs);
  	//Node(int rows, int columns);
  	
  	/*void clear();*/
  	void displayNode();
};

Node::Node(){
	parent = NULL;
	St = NULL;
	pathCost = 0;
	hVal = 0;
}


Node* clone(const Node* N) {
	if (N == NULL) {
		return NULL;
	}
	else {
		Node* new_Node = new Node;
		new_Node->parentsAct.index = N->parentsAct.index;
		new_Node->parentsAct.movement = N->parentsAct.movement;
		new_Node->parentsAct.movObj = N->parentsAct.movObj;
		new_Node->pathCost = N->pathCost;
		new_Node->St = N->St;
		new_Node->hVal = N->hVal;
		new_Node->parent = clone(N->parent);
		return (new_Node);
	}
}

Node::Node(const Node& rhs){
	parent = clone(rhs.parent);
	//parent should be set to it's parent node
	//while the parent node should still have there stuff
  	parentsAct.index = rhs.parentsAct.index;
  	parentsAct.movement = rhs.parentsAct.movement;
  	parentsAct.movObj = rhs.parentsAct.movObj;
  	pathCost = rhs.pathCost;
  	St = rhs.St;
  	hVal = rhs.hVal;
}

//Used for debugging purposes
void Node:: displayNode(){
	cout << "---------------------" << endl;
	cout << "Displaying Node: " << endl;
	if (parentsAct.movObj != 'N'){
		cout << "Parent's Actions:" << endl;
		cout << "movObj- " << parentsAct.movObj << ". movement-" << 
		parentsAct.movement << ". index-" << parentsAct.index << endl;
	}
	else{
		cout << "There's no parent action." << endl;
	}
	cout << "Path Cost: " << pathCost << endl;
	cout << "Boat: (" << St->boatX << "," << St->boatY << ") Direction: " << St->boatDir << endl;
	cout << "Goal Location: (" << St->goalX << "," << St->goalY << ")" << endl;
	cout << "Grid Width x Height : " << St->gridWidth << "x" << St->gridHeight << endl;
	cout << "Num Alligators - " << St->numAlligators << ", Num Turtles - " << St->numTurtles << ", num Trees - " << St->numTrees << endl;
	cout << "Radiation: " << St->radiation << endl;
	
	St->displayGrid();
	
	if (!St->aList.isEmpty()){
		LinkedList<Alligator>* A = St->aList.getFirstPtr();
		while (A -> m_next != NULL){
			cout << "Alligator #" << A->m_data.index << ", facing - " << A->m_data.direction << " is in (" 
			<< A->m_data.coordX << "," << A->m_data.coordY << ")" << endl;
			A = A->m_next;
		}
	}
	else{
		cout << "There's no Alligators" << endl;
	}
	if (!St->tuList.isEmpty()){
		LinkedList<Turtle>* A = St->tuList.getFirstPtr();
		while (A -> m_next != NULL){
			cout << "Turtle #" << A->m_data.index << ", facing - " << A->m_data.direction << " is in (" 
			<< A->m_data.coordX << "," << A->m_data.coordY << ")" << endl;
			A = A->m_next;
		}
	}
	else{
		cout << "There's no Turtles" << endl;
	}
	if (!St->trList.isEmpty()){
		LinkedList<Tree>* A = St->trList.getFirstPtr();
		while (A -> m_next != NULL){
			cout << "Tree #" << A->m_data.index << " is in (" << A->m_data.coordX << "," << A->m_data.coordY << ")" << endl;
			A = A->m_next;
		}
	}
	else{
		cout << "There's no Trees" << endl;
	}
	cout << "---------------------" << endl;
}

//This is primarily used in checking if a state is in the explored or the frontier set.
//Node is used to determine if the node is equal to another state.
bool isStateEq(Node N, State* q){
	bool dataEqual = true;
	
	State* p = N.St;
	if (!(p->boatDir == q->boatDir && p->boatX == q->boatX && p->boatY == q->boatY)){
		dataEqual = false;
	}
	LinkedList<Alligator>* Ap = p->aList.getFirstPtr();
	LinkedList<Alligator>* Aq = q->aList.getFirstPtr();
	while (Ap->m_next != NULL || Aq->m_next != NULL){
		if (!(Ap->m_data.coordX == Aq->m_data.coordX && Ap->m_data.coordY == Aq->m_data.coordY &&
		      Ap->m_data.index == Aq->m_data.index)){
			dataEqual = false;
		}
		Ap = Ap->m_next;
		Aq = Aq->m_next;
	}
	LinkedList<Turtle>* Tp = p->tuList.getFirstPtr();
	LinkedList<Turtle>* Tq = q->tuList.getFirstPtr();
	while (Tp->m_next != NULL && Tq->m_next != NULL){
		if (!(Tp->m_data.coordX == Tq->m_data.coordX && Tp->m_data.coordY == Tq->m_data.coordY && Tp->m_data.index == Tq->m_data.index)){
			dataEqual = false;
		}
		Tp = Tp->m_next;
		Tq = Tq->m_next;
	}

	return (dataEqual);
}


//Pre: The state must have a grid height, width, radiation
//      magnitude, decay, and radiation source, with boat info.
//Post: Gives the value of the radiation for the boat for the given state.
//Used to calculate radiation for a given state
int calcRad(State* actionState){
	
	int** radGrid = new int*[actionState->gridHeight];
    for(int k=0; k < actionState->gridHeight; k++){
      radGrid[k] = new int[actionState->gridWidth];
    }

	//writing rads to grid
	for (int i = 0; i < actionState->gridWidth; i++){
		for (int j = 0; j < actionState->gridHeight; j++){
			radGrid[j][i] = actionState->radMag - ((abs(j-actionState->radY)+abs(i-actionState->radX))*actionState->radDecay);
		}
	}
	
	int rad = radGrid[actionState->boatY][actionState->boatX];
	if (actionState->boatDir == 'U'){
		rad = rad + radGrid[actionState->boatY-1][actionState->boatX];
	}
	if (actionState->boatDir == 'L'){
		rad = rad + radGrid[actionState->boatY][actionState->boatX-1];
	}
	if (actionState->boatDir == 'R'){
		rad = rad + radGrid[actionState->boatY][actionState->boatX+1];
	}
	if (actionState->boatDir == 'D'){
		rad = rad + radGrid[actionState->boatY+1][actionState->boatY];
	}
	for(int i = 0; i < actionState->gridHeight; ++i) {
    	delete [] radGrid[i];
	}
	delete [] radGrid;
	return rad;
}

/*Pre: Determines if an action is valid with a state.
Parameters: State with values for boat and animals, and the action being performed.
Post: Returns a boolean, if the action is able to be performed. For example this would
return false if the boat is trying to move out of bounds.*/
State* validAction(State* S, Action& A);
State* validAction(State* S, Action& A){
  int coordX;
  int coordY;
  char direction;
  
  //**************
  //Creating the Grid Space
  //***************
  char** obsGrid = new char*[S->gridHeight];
  for (int i = 0; i < S->gridHeight; i++){
  	obsGrid[i] = new char[S->gridWidth];
  }
  
  for (int i = 0; i < S->gridWidth; i++){
  	for (int j = 0; j < S->gridHeight; j++){
  		obsGrid[j][i] = 'E';
	  }
  }
  
  int inputX, inputY;
  
  inputX = S->boatX;
  inputY = S->boatY;
  obsGrid[inputY][inputX] = 'B';
  switch(S->boatDir){
  	case 'U':
  		inputY--;
  	break;
  	case 'R':
  		inputX++;
  	break;
  	case 'L':
  		inputX--;
  	break;
  	case 'D':
  		inputY++;
  	break;
  }
  obsGrid[inputY][inputX] = 'B';
  
  LinkedList<Alligator>* Aptr = S->aList.getFirstPtr();
  while (Aptr -> m_next != NULL){
  	inputX = Aptr->m_data.coordX;
  	inputY = Aptr->m_data.coordY;
  	obsGrid[inputY][inputX] = 'A';
  	switch(Aptr->m_data.direction){
	  	case 'U':
	  		obsGrid[inputY-2][inputX] = 'A';
	  		obsGrid[inputY-1][inputX] = 'A';
	  	break;
	  	case 'R':
	  		obsGrid[inputY][inputX+2] = 'A';
	  		obsGrid[inputY][inputX+1] = 'A';
	  	break;
	  	case 'L':
	  		obsGrid[inputY][inputX-2] = 'A';
	  		obsGrid[inputY][inputX-1] = 'A';
	  	break;
	  	case 'D':
	  		obsGrid[inputY+1][inputX] = 'A';
	  		obsGrid[inputY+2][inputX] = 'A';
	  	break;
	}
  	Aptr = Aptr->m_next;
  }
  
  LinkedList<Turtle>* T = S->tuList.getFirstPtr();
  while (T -> m_next != NULL){
  	inputX = T->m_data.coordX;
  	inputY = T->m_data.coordY;
  	obsGrid[inputY][inputX] = 'T';
  	switch(T->m_data.direction){
	  	case 'U':
	  		inputY--;
	  	break;
	  	case 'R':
	  		inputX++;
	  	break;
	  	case 'L':
	  		inputX--;
	  	break;
	  	case 'D':
	  		inputY++;
	  	break;
	}
	obsGrid[inputY][inputX] = 'T';
  	T = T->m_next;
  }
  
  LinkedList<Tree>* C = S->trList.getFirstPtr();
  while (C -> m_next != NULL){
  	inputX = C->m_data.coordX;
  	inputY = C->m_data.coordY;
  	obsGrid[inputY][inputX] = 'C';
  	C=C->m_next;
  }
  
  /*Creating a new state for the node*/
  State* newState = new State(*S);

  if (A.movObj == 'B'){
    if (A.movement == 'u'){
    	if (S->boatDir != 'U'){
    		return NULL;
		}
		if (S->boatY - 2 < 0){
			return NULL;
		}
		if (!(obsGrid[S->boatY-2][S->boatX] == 'E')){
			return NULL;
		}
		newState->boatY = newState->boatY-1;
	}
	else if (A.movement == 'l'){
		if (S->boatDir != 'L'){
			return NULL;
		}
		if (S->boatX-2 < 0){
			return NULL;
		}
		if (!(obsGrid[S->boatY][S->boatX-2] == 'E')){
			return NULL;
		}
		newState->boatX = newState->boatX-1;
	}
	else if (A.movement == 'r'){
		if (S->boatDir != 'R'){
			return NULL;
		}
		if (S->boatX >= S->gridWidth-2){
			return NULL;
		}
		if (!(obsGrid[S->boatY][S->boatX+2] == 'E')){
			return NULL;
		}
		newState->boatX = newState->boatX+1;
	}
	else if (A.movement == 'd'){
		if (S->boatDir != 'D'){
			return NULL;
		}
		if (S->boatY >= S->gridHeight-2){
			return NULL;
		}
		if (!(obsGrid[S->boatY+2][S->boatX] == 'E')){
			return NULL;
		}
		newState->boatY = newState->boatY+1;
	}
	else if (A.movement == 'c'){
		if (S->boatX == 0 && S->boatDir == 'D'){
			return NULL;
		}
		if (S->boatY == 0 && S->boatDir == 'L'){
			return NULL;
		}
		if (S->boatY == S->gridHeight-1 && S->boatDir == 'R'){
			return NULL;
		}
		if (S->boatX == S->gridWidth-1 && S->boatDir == 'U'){
			return NULL;
		}
		if (S->boatDir == 'U'){
			if (!(obsGrid[S->boatY][S->boatX+1] == 'E' &&
			obsGrid[S->boatY-1][S->boatX+1] == 'E')){
				return NULL;
			}
			newState->boatDir = 'R';
		}
		else if (S->boatDir == 'R'){
			if (!(obsGrid[S->boatY+1][S->boatX] == 'E'  &&
			obsGrid[S->boatY+1][S->boatX+1] == 'E')){
				return NULL;
			}
			newState->boatDir = 'D';
		}
		else if (S->boatDir == 'D'){
			if (!(obsGrid[S->boatY][S->boatX-1] == 'E'  &&
			obsGrid[S->boatY+1][S->boatX-1] == 'E')){
				return NULL;
			}
			newState->boatDir = 'L';
		}
		else{// (S->boatDir == 'L')
			if (!(obsGrid[S->boatY-1][S->boatX-1] == 'E' &&
			obsGrid[S->boatY-1][S->boatX] == 'E')){
				return NULL;
			}
			newState->boatDir = 'U';
		}
	}
	else{// (A.movement == 'n'){
		if (S->boatY == 0 && S->boatDir ==  'R'){
			return NULL;
		}
		if (S->boatX == 0 && S->boatDir ==  'U'){
			return NULL;
		}
		if (S->boatX == S->gridWidth-1 && S->boatDir == 'D'){
			return NULL;
		}
		if (S->boatY == S->gridHeight-1 && S->boatDir == 'L'){
			return NULL;
		}
		if (S->boatDir == 'U'){
			if (!(obsGrid[S->boatY][S->boatX-1] == 'E' &&
			obsGrid[S->boatY-1][S->boatX-1] == 'E')){
				return NULL;
			}
			newState->boatDir = 'L';
		}
		else if (S->boatDir == 'R'){
			if (!(obsGrid[S->boatY-1][S->boatX] == 'E'  &&
			obsGrid[S->boatY-1][S->boatX+1] == 'E')){
				return NULL;
			}
			newState->boatDir = 'U';
		}
		else if (S->boatDir == 'D'){
			if (!(obsGrid[S->boatY][S->boatX+1] == 'E'  &&
			obsGrid[S->boatY+1][S->boatX+1] == 'E')){
				return NULL;
			}
			newState->boatDir = 'R';
		}
		else {//if (S->boatDir == 'L'){
			if (!(obsGrid[S->boatY+1][S->boatX-1] == 'E' &&
			obsGrid[S->boatY+1][S->boatX] == 'E')){
				return NULL;
			}
			newState->boatDir = 'D';
		}
	}
  }
  if (A.movObj == 'A'){
  	LinkedList<Alligator>* aPtr = newState->aList.findIndex(A.index);
		coordX = aPtr->m_data.coordX;
		coordY = aPtr->m_data.coordY;
		direction = aPtr->m_data.direction;
		
  	if (A.movement == 'u'){
    	if (!(direction == 'U')||(direction =='D')){
    		return NULL;
		}
		if (direction == 'U'){
		  if (coordY < 3){
			return NULL;
		  }
		  if (!(obsGrid[coordY-3][coordX] == 'E')){
			return NULL;
	      }
	    }
	    if (direction == 'D'){
	      if (coordY <= 0){
			return NULL;
		  }
	      if (!(obsGrid[coordY-1][coordX] == 'E')){
	    	return NULL;
		  }
		}
		aPtr->m_data.coordY = aPtr->m_data.coordY-1;
	}
	else if (A.movement == 'l'){
		if (!(direction == 'L' || direction == 'R')){
			return NULL;
		}
		if (direction == 'L'){
			if (coordX < 3){
			  return NULL;
			}
			if (!(obsGrid[coordY][coordX-3] == 'E')){
			  return NULL;
			}
		}
		if (direction == 'R'){
			if (coordX <= 0){
			  return NULL;
		    }
			if (!(obsGrid[coordY][coordX-1] == 'E')){
				return NULL;
			}
		}
		aPtr->m_data.coordX = aPtr->m_data.coordX-1;
	}
	else if (A.movement == 'r'){
		if (!(direction == 'R' || direction == 'L')){
			return NULL;
		}
		if (direction == 'R'){
		  if (coordX + 3 >= S->gridWidth){
			return NULL;
		  }
		  if (!(obsGrid[coordY][coordX+3] == 'E')){
			return NULL;
	      }
		}
		if (direction == 'L'){
		  if (coordX >= S->gridWidth-1){
			return NULL;
		  }
		  if (!(obsGrid[coordY][coordX+1] == 'E')){
			return NULL;
	      }
		}
		aPtr->m_data.coordX = aPtr->m_data.coordX+1;
	}
	else {//if (A.movement == 'd'){
		if (!(direction == 'D' || direction == 'U')){
			return NULL;
		}
		if (direction == 'D'){
		  if (coordY + 3 >= S->gridHeight){
			return NULL;
		  }
		  if (!(obsGrid[coordY+3][coordX] == 'E')){
			return NULL;
	      }
	    }
	    if (direction == 'U'){
	      if (coordY + 1 >= S->gridHeight){
			return NULL;
		  }
	      if (!(obsGrid[coordY+1][coordX] == 'E')){
			return NULL;
	      }	
		}
		aPtr->m_data.coordY = aPtr->m_data.coordY+1;
	}
	aPtr = NULL;
  }
  if (A.movObj == 'T'){
		LinkedList<Turtle>* tuPtr = newState->tuList.findIndex(A.index);
		coordX = tuPtr->m_data.coordX;
		coordY = tuPtr->m_data.coordY;
		direction = tuPtr->m_data.direction;
		if (A.movement == 'u'){
	    	if (!(direction == 'U' || direction == 'D')){
	    		return NULL;
			}
			if (direction == 'U'){
				if (coordY < 2){
				  return NULL;
				}
				if (!(obsGrid[coordY-2][coordX] == 'E')){
					return NULL;
				}
			}
			if (direction == 'D'){
				if (coordY <= 0){
				  return NULL;
				}
				if (!(obsGrid[coordY-1][coordX] == 'E')){
					return NULL;
				}
			}
			tuPtr->m_data.coordY = tuPtr->m_data.coordY-1;
		}
		else if (A.movement == 'l'){
			if (!(direction == 'L' || direction == 'R')){
				return NULL;
			}
			if (direction == 'L'){
			  if (coordX < 2){
				return NULL;
			  }	
			  if (!(obsGrid[coordY][coordX-2] == 'E')){
			    return NULL;
			  }
			}
			if (direction == 'R'){
		      if (coordX <= 0){
				return NULL;
			  }
			  if (!(obsGrid[coordY][coordX-1] == 'E')){
			  	return NULL;
			  }
			}
			tuPtr->m_data.coordX = tuPtr->m_data.coordX-1;
		}
		else if (A.movement == 'r'){
			if (!(direction == 'R' || direction == 'L')){
				return NULL;
			}
			if (direction == 'R'){
			  if (coordX > S->gridWidth-3){
				return NULL;
			  }
			  if (!(obsGrid[coordY][coordX+2] == 'E')){
			    return NULL;
			  }
			}
			if (direction == 'L'){
				if (coordX > S->gridWidth-1){
				  return NULL;
			    }
			  if (!(obsGrid[coordY][coordX+1] == 'E')){
			    return NULL;
			  }
			}
			tuPtr->m_data.coordX = tuPtr->m_data.coordX+1;
		}
		else{// if (A.movement == 'd'){
			if (!(direction == 'D' || direction == 'U')){
				return NULL;
			}
			if (direction == 'D'){
				if (coordY >= S->gridHeight-2){
				  return NULL;
			    }
				if (!(obsGrid[coordY+2][coordX] == 'E')){
					return NULL;
				}
			}
			if (direction == 'U'){
				if (coordY >= S->gridHeight-1){
				  return NULL;
			    }
				if (!(obsGrid[coordY+1][coordX] == 'E')){
					return NULL;
				}
			}
			tuPtr->m_data.coordY = tuPtr->m_data.coordY+1;
		}
		tuPtr = NULL;
   }
   newState->radiation = calcRad(newState);
   return (newState);
}

/*This creates the node for the graph
  Pre: The Parent Node, the action from the parent being performed, and the state the action
       is GOING to be performed on.
  Post: Returns a pointer to the node of the child node that was created. This child node holds
  values for it's path cost, the parent that created it, etc.*/
void childNode(Node* Parent, Action parentAction, Node* child);
void childNode(Node* Parent, Action parentAction, Node* child){
	nodeCreation++;
	child->parent = Parent;
	child->St = validAction(Parent->St, parentAction); //returns a new state.
	gridsCreated--;
	child->parentsAct.index = parentAction.index;
	child->parentsAct.movement = parentAction.movement;
	child->parentsAct.movObj = parentAction.movObj;
	child->pathCost = Parent->pathCost + child->St->radiation;
	return;
}

/*Determines if the current state the boat is in is a goal state
Pre: a state with a valid state and goal.
Post: returns true if boat is on goal (x,y) else false*/
bool checkGoal(State* goalState);
bool checkGoal(State* goalState){
	if (goalState->boatX == goalState->goalX && goalState->boatY == goalState->goalY){
		return true;	
	}
	switch(goalState->boatDir){
		case ('U'):
			if (goalState->boatY-1 == goalState->goalY && goalState->boatX == goalState->goalX){
				return true;
			}
		break;
		case ('L'):
			if (goalState->boatX-1 == goalState->goalX && goalState->boatY == goalState->goalY){
				return true;
			}
		break;
		case ('D'):
			if (goalState->boatY+1 == goalState->goalY && goalState->boatX == goalState->goalX){
				return true;
			}
		break;
		case('R'):
			if (goalState->boatX+1 == goalState->goalX && goalState->boatY == goalState->goalY){
				return true;
			}
		break;
	}
	return false;
}

//Used to define all the actions that can happen in a state.
//Returns a pointer to an array of actions.
Action* allActions(int numA, int numT){
	Action *tmp = new Action[6+(numA*4)+(numT*4)];
	for (int i = 0; i < 6; i++){
		tmp[i].movObj = 'B';
		tmp[i].index = 0;
		switch (i){
			case 0:
				tmp[i].movement = 'u';
			break;
			case 1:
				tmp[i].movement = 'r';
			break;
			case 2:
				tmp[i].movement = 'd';
			break;
			case 3:
				tmp[i].movement = 'l';
			break;
			case 4:
				tmp[i].movement = 'c';
			break;
			case 5:
				tmp[i].movement = 'n';
			break;
		}
	}//0 to 5

	//6 to 6+numAlligators*4
	for (int i = 6; i < 6+(numA*4); i++){
		tmp[i].movObj = 'A';
		tmp[i].index = ((i-6)/4);
		switch ((i-6) % 4){
			case 0:
				tmp[i].movement = 'u';
			break;
			case 1:
				tmp[i].movement = 'r';
			break;
			case 2:
				tmp[i].movement = 'd';
			break;
			case 3:
				tmp[i].movement = 'l';
			break;
		}
	}
	//6+numAlligators*4 to 6+numAlligators*4+numTurtles*4
	for (int i = 6+(numA*4); i < (6+(numA*4)+(numT*4)); i++){
		tmp[i].movObj = 'T';
		tmp[i].index = ((i-6-(numA*4))/4);
		switch ((i-6-(numA*4)) % 4){
			case 0:
				tmp[i].movement = 'u';
			break;
			case 1:
				tmp[i].movement = 'r';
			break;
			case 2:
				tmp[i].movement = 'd';
			break;
			case 3:
				tmp[i].movement = 'l';
			break;
		}
	}
	return tmp;
}

bool operator<(const Node& lhs, const Node& rhs)
{
	return lhs.hVal > rhs.hVal;
}

//h(n) calculation
int calcHValue(Node N) {
	int h;
	h = abs(N.St->boatX - N.St->goalX) + abs(N.St->boatY - N.St->goalY);
	return h;
}

//h(n) is the manhattan distance from the back of the boat to the goal
//This creates a frontier and an explored set. The frontier is a priority queue sorted by the closest states
//to the goal, states where the boat is near the goal takes priority.
//From there it evaluates the top node from the priority queue and repeats till it finds the goal.
Node GrBeFGS(Node root){
	bool inExplored = false;
	char wait;
	priority_queue<Node> frontier;
	vector<Node> explored;

	root.hVal = calcHValue(root);
	Node eval = root;
	frontier.push(eval);
	Action* checkValid = allActions(root.St->numAlligators, root.St->numTurtles);
	State* checkState;
	while (!checkGoal(eval.St)) {
		explored.push_back(eval);
		frontier.pop();
		for (int i = 0; i < 6 + (root.St->numAlligators * 4) + (root.St->numTurtles * 4); i++) { //generating frontier before evaluation
			checkState = validAction(eval.St, checkValid[i]);
			if (checkState != NULL) {
				inExplored = false;
				for (int k = 0; k < explored.size(); k++) {
					if (isStateEq(explored[k], checkState)) {
						inExplored = true;
					}
				}
				if (!inExplored) {
					Node child;	 //Create the node
					child.St = checkState;
					child.parentsAct = checkValid[i];
					child.parent = &explored.at(explored.size()-1);
					child.pathCost = child.parent->pathCost + calcRad(child.St);
					child.hVal = calcHValue(child);
					frontier.push(child);
				}
			}
		}//after all possible actions for the state
		eval = frontier.top();
	}

	//Use this to see how it got to goal
	/*Node* path = &eval;
	cout << "Goal Has Been Found" << endl;
	while (path->parent != NULL) {
		path->St->displayGrid();
		cout << "---------" << endl;
		path = path->parent;
	}*/
	return eval;
}

int main()
{
	for (int w = 0; w < 3; w++){		
		ifstream puzzleFile;
		ofstream solutionFile;
		string line;
		//Variable Declerations
		int coordX, coordY; //x y and index for inserting obstacles to linked lists.
		int numAnimals = 0;
		char directionFace;
		LinkedList<Alligator> aList;
		LinkedList<Turtle> tuList;
		LinkedList<Tree> trList;
	    Alligator gator;
	    Turtle turtle;
	    Tree tree;
	    
	    Node root;
	    root.pathCost = 0;
		
		//Reading-in Input
		//Setting Initial Node
		
		if (w == 0){
			puzzleFile.open("puzzles/puzzle1.txt");
		}
		else if (w == 1) {
			puzzleFile.open("puzzles/puzzle2.txt");
		}
		else {
			puzzleFile.open("puzzles/puzzle3.txt");
		}
		
		cout << endl;
	 	root.St = new State;
		puzzleFile >> root.St->gridWidth;
		puzzleFile >> root.St->gridHeight;
		//****************************
		//Initializing Radiation
		//*****************************
		puzzleFile >> root.St->radX >> root.St->radY >> root.St->radMag >> root.St->radDecay;
			
		//***********************
		//Animals/Obstacles
		//***********************
		puzzleFile >> root.St->numAlligators;
		puzzleFile >> root.St->numTurtles;
		puzzleFile >> root.St->numTrees;
		for (int i = 0; i  < root.St->numAlligators; i++){
		  puzzleFile >> coordX;
		  puzzleFile >> coordY;
		  puzzleFile >> directionFace;
		  
		  gator.coordX = coordX;
		  gator.coordY = coordY;
		  gator.direction = directionFace;
		  gator.index = i;
		  root.St->aList.insert_front(gator);
		}
	
		//Turtle storage for initial node
		for (int i = 0; i < root.St->numTurtles; i++){
		  puzzleFile >> coordX;
		  puzzleFile >> coordY;
		  puzzleFile >> directionFace;
		  	
		  turtle.coordX = coordX;
		  turtle.coordY = coordY;
		  turtle.direction = directionFace;
		  turtle.index = i;
		  root.St->tuList.insert_front(turtle);
		}
	
		//tree storage for initial node
		for (int i = 0; i  < root.St->numTrees; i++){
		  puzzleFile >> tree.coordX;
		  puzzleFile >> tree.coordY;
		  tree.index = i;
		  root.St->trList.insert_front(tree);
		}
		
		puzzleFile >> coordX;
		puzzleFile >> coordY;
		puzzleFile >> directionFace;
		
		//***********
		//Boat Information
		//***********
		root.St->boatX = coordX;
		root.St->boatY = coordY;
		root.St->boatDir = directionFace;
		
		//********
		//Getting Goal Information
		//********
		puzzleFile >> root.St->goalX;
		puzzleFile >> root.St->goalY;
		
		puzzleFile.close();	
		
		//***********
		//Main Algorithm
		//************
		clock_t begin = clock();
		Node goalNode = GrBeFGS(root);
		clock_t end = clock();
		
		double time_spent = double(end-begin)/CLOCKS_PER_SEC;
		time_spent = time_spent * 1000000; //conversion from sec to ms to microsec
		//***********
		//Output
		//***********
		if (w == 0){
			solutionFile.open("solutions/solution1.txt");
		}
	    else if (w == 1) {
			solutionFile.open("solutions/solution2.txt");
		}
		else {
			solutionFile.open("solutions/solution3.txt");
		}
		
		//Outputting Time Spent
		cout << time_spent << endl;
		solutionFile << time_spent << "\n";
	
		int rads = 0;
		int steps = 0;
		Node* path = &goalNode;
		vector<Action> pathMove;

		while (path->parent != NULL) {
			steps++;
			rads = rads + path->St->radiation;
			pathMove.push_back(path->parentsAct);
			path = path->parent;
		}

		//subtracting final radiation
		rads = rads - goalNode.St->radiation;

		//Displaying Steps, grid, and radiation
		cout << rads << endl;
		solutionFile << rads << "\n";
		cout << steps << endl;
		solutionFile << steps << "\n";

		for (int i = pathMove.size()-1; i >= 0 ; i--) {
			cout << pathMove[i].movObj << " " << pathMove[i].index << " " << pathMove[i].movement;
			if (i != 0) {
				cout << ",";
			}
		}
		solutionFile << "\n";
		cout << endl;
		
		cout << root.St->gridWidth << " " << root.St->gridHeight << endl;
		cout << root.St->radX << " " << root.St->radY << endl;
		cout << root.St->radMag << " " << root.St->radDecay << endl;

		solutionFile << root.St->gridWidth << " " << root.St->gridHeight << "\n";
		solutionFile << root.St->radX << " " << root.St->radY << "\n";
		solutionFile << root.St->radMag << " " << root.St->radDecay << "\n";
		
		
		//Displaying num of animals/tree
		cout << root.St->numAlligators << " " << root.St->numTurtles << " " << root.St->numTrees << endl;
		solutionFile << root.St->numAlligators << " " << root.St->numTurtles << " " << root.St->numTrees << "\n";
		
		//Displaying animal and tree locations
		
		goalNode.St->aList.reverse();
		LinkedList<Alligator>* A = goalNode.St->aList.getFirstPtr();
		while (A->m_next != NULL){
			cout << A->m_data.coordX << " " << A->m_data.coordY << " " << A->m_data.direction << endl;
			solutionFile << A->m_data.coordX << " " << A->m_data.coordY << " " << A->m_data.direction << "\n";
			A = A->m_next;
		}
		goalNode.St->tuList.reverse();
		LinkedList<Turtle>* T = goalNode.St->tuList.getFirstPtr();
		while (T->m_next != NULL){
			cout << T->m_data.coordX << " " << T->m_data.coordY << " " << T->m_data.direction << endl;
			solutionFile << T->m_data.coordX << " " << T->m_data.coordY << " " << T->m_data.direction << "\n";
			T = T->m_next;
		}
		goalNode.St->trList.reverse();
		LinkedList<Tree>* C = goalNode.St->trList.getFirstPtr();
		while (C->m_next != NULL){
			cout << C->m_data.coordX << " " << C->m_data.coordY << endl;
			solutionFile << C->m_data.coordX << " " << C->m_data.coordY << "\n";
			C = C->m_next;
		}
		cout << goalNode.St->boatX << " " << goalNode.St->boatY << " " << goalNode.St->boatDir << endl;
		solutionFile << goalNode.St->boatX << " " << goalNode.St->boatY << " " << goalNode.St->boatDir << "\n";
		cout << root.St->goalX << " " << root.St->goalY << endl;
		solutionFile << root.St->goalX << " " << root.St->goalY << "\n";
	}
	return 0;
}