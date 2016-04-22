#include <iostream>
#include <cctype>
#include <stdlib.h>
#include <time.h>
using namespace std;


//
//node that points to all adjacent rooms
//stores the current room#
//checks to see if there is a hazard/wumpus
struct Room {
	int roomNum;		//the room the node represents
	Room* roomNext1;	//the rooms adjacent the node
	Room* roomNext2;
	Room* roomNext3;
	bool hasWumpus;		//variable to check if wumpus is in the room
	bool hasPit;		//variable to check if there is a pit in the room
	bool hasBat;		//variable to check if super bats are in the room
	bool hasArrow;		//variable to check if the arrow is in the room
};


//
//struct that stores player location,
//whether or not they have an arrow, 
//and whether or not they have fired an arrow
struct Player {
	int location;
	bool hasArrow;
	bool firedArrow;
};


//
//struct that stores the current game state
struct GameState {
	int moveNumber;
	int wumpRoom;
	int batRoom1;
	int batRoom2;
	int arrowRoom;
	int playerRoom;
	bool playerHasArrow;
	bool playerHasFired;
	GameState *prevMove;
};


//
//prints out the header, board, and instructions
void header(){
	cout << endl
	     << "Author: Ismail Siddiqui           ______18______ " << endl	
	     << "CS 141 Program #6: Wumpus        /      |       \\ " << endl
	     << "TA: Itica Gupta, T 11:00 AM     /       _9__     \\ " << endl
	     << "April 3, 2016                  /      /    \\      \\ " << endl
	     << "System: Sublime Text          /      /      \\      \\ " << endl
	     << "        on Ubuntu            17     8        10     19 " << endl
	     << "                            |  \\   / \\      /  \\   / | " << endl
	     << "Hunt the Wumpus:            |   \\ /   \\    /    \\ /  | " << endl
	     << "The Wumpus lives in a       |    7     1---2     11  | " << endl
	     << "completely dark cave of     |    |    /     \\    |   | " << endl
	     << "20 rooms.  Each room has    |    6—---5     3---12   | " << endl
	     << "3 tunnels leading to other  |    |     \\   /     |   | " << endl
	     << "rooms as shown:             |    \\       4      /    | " << endl
	     << "                            |     \\      |     /     | " << endl
	     << "                             \\     15---14---13     / " << endl
	     << "                              \\   /            \\   / " << endl
	     << "                               \\ /              \\ / " << endl
	     << "                               16---------------20 " << endl
	     << "Hazards: " << endl
	     << "1. Two rooms have bottomless pits in them.  If you go there you fall and die." << endl
	     << "2. Two other rooms have super-bats.  If you go there, a bat grabs you and " << endl
	     << "   flys you to some random room, which could be troublesome. Then those bats " << endl
	     << "   go to some random room. " << endl
	     << "3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and " << endl
	     << "   is too heavy for a bat to lift. Usually he is asleep. Two things wake " << endl
	     << "   him up: Anytime you shoot an arrow, or you entering his room.  When he   " << endl
	     << "   wakes he moves one room 75 percent of the time, staying still the other 25 percent of " << endl
	     << "   the time. After that, if he is in your room, he eats you and you die! " << endl << endl
	     << "Moves: " << endl
	     << "On each move you can: " << endl
	     << "1. Move into an adjacent room.  To move enter 'M' followed by a space and " << endl
	     << "   then a room number. " << endl
	     << "2. Shoot your guided arrow through a list of 3 adjacent rooms, which " << endl 
	     << "   you specify.  Your arrow ends up in the final room. If an arrow " << endl
	     << "   can't go a direction because there is no connecting tunnel, it ricochets " << endl
	     << "   and moves to a randomly selected adjacent room and continues " << endl
	     << "   doing this until it has traveled 3 rooms in total.  If the arrow  " << endl 
	     << "   hits the Wumpus, you win! If the arrow hits you, you lose. You  " << endl
	     << "   automatically pick up an arrow if you go through a room with an arrow in   " << endl
	     << "   it. To move enter 'S' followed by a list of up to 3 adjacent room numbers" << endl
	     << "   separated by spaces." << endl << endl
	     << "Good Luck!" << endl;
}//end header----------------------------------------------------------------------------------------------------


//
//initializes room numbers, pointers, and other variables
//takes an array of Room pointers
void initRooms( Room* room[]){
	//initialize room variables
	for( int i = 0; i < 20; i++){
		room[ i] = new Room;
		room[ i]->roomNum = i + 1;
		room[ i]->hasWumpus = false;
		room[ i]->hasPit = false;
		room[ i]->hasBat = false;
		room[ i]->hasArrow = false;
		//connent the current room to the next room
		if( i != 0){
			room[ i]->roomNext1 = room[ i-1];
			room[ i-1]->roomNext2 = room[ i];
		}
	}

	//initialize room pointers
	for( int i = 0; i < 4; i++){
		
		//connect the inner pentagons to the outer ones
		//inner most to second rank
		room[ i]->roomNext3 = room[ i*2+7];
		room[ i*2+7]->roomNext3 = room[ i];
		//second rank to outer most
		room[ i*2+6]->roomNext3 = room[ 16+i];
		room[ 16+i]->roomNext3 = room[ i*2+6]; 	
	}
	
	//leftover connections
	//connect room 1 and 5
	room[ 0]->roomNext1 = room[ 4];
	room[ 4]->roomNext3 = room[ 0];
	//connect room 6 and 15
	room[ 5]->roomNext3 = room[ 14];
	room[ 14]->roomNext3 = room[ 5];
	//connect room 16 and 20
	room[ 15]->roomNext3 = room[ 19];
	room[ 19]->roomNext2 = room[ 15];
}//end initRooms--------------------------------------------------------------------------------------------------


//
//make sure the hazards are all in unique rooms
//if not, adjust accordingly
void sameRoomChecker( int &randPit1, int &randPit2, int &randBat1, int &randBat2, int &randWump){
	//while the first pit's room is the same as any of the other hazard rooms	
	while( randPit1 == randPit2 || randPit1 == randBat1 || randPit1 == randBat2 || randPit1 == randWump)
		randPit1 = rand() % 20;
	
	//while the second pit's room is the same as any of the other hazard rooms
	while( randPit2 == randBat1 || randPit2 == randBat2 || randPit2 == randWump)
		randPit2 = rand() % 20;

	//while the first super bat's room is the same as any of the other hazard rooms
	while( randBat1 == randBat2 || randBat1 == randWump)
		randBat1 = rand() % 20;

	//while the second super bat's room is the same as any of the other hazard rooms
	while( randBat2 == randWump)
		randBat2 = rand() % 20;
}//end sameRoomChecker--------------------------------------------------------------------------------------------


//
//creates the room that has the hazards
void generateHazard( Room* room[]){
	srand( time( NULL));
	int randPit1 = rand() % 20;  
	int randPit2 = rand() % 20;  
	int randBat1 = rand() % 20;  
	int randBat2 = rand() % 20;  
	int randWump = rand() % 20;

	sameRoomChecker( randPit1, randPit2, randBat1, randBat2, randWump);

	//set the rooms with hazards on accordingly
	room[ randPit1]->hasPit = true;
	room[ randPit2]->hasPit = true;
	room[ randBat1]->hasBat = true;
	room[ randBat2]->hasBat = true;
	room[ randWump]->hasWumpus = true;
}//end generateHazard---------------------------------------------------------------------------------------------


//
//starts the player in a room that has no hazards
int playerStart( Room* room[], Player player){
	srand( time( NULL));

	int currentRoom = player.location;
	Room* startRoom = room[ currentRoom];

	while( startRoom->hasPit || startRoom->hasBat || startRoom-> hasWumpus){
		currentRoom = rand() % 20;
		startRoom = room[ currentRoom];
	}

	return currentRoom;
}//end playerStart------------------------------------------------------------------------------------------------


//
//checks to see if the user input a valid room destination
bool canMove( Room* room[], Player player, int playerDest){
	int nextRoom1 = room[ player.location]->roomNext1->roomNum;
	int nextRoom2 = room[ player.location]->roomNext2->roomNum;
	int nextRoom3 = room[ player.location]->roomNext3->roomNum;

	if( playerDest != nextRoom1 && playerDest != nextRoom2 && playerDest != nextRoom3){
		cout << "Invalid location. Please retry...." << endl;
		return false;
	} else 
		return true;
}//end canMove----------------------------------------------------------------------------------------------------


//
//checks validity of player's desired destination
bool isValidDesitnation( Room* room[], Player &player){
	int playerDest = 0;
	
	cin >> playerDest;
	
	if( playerDest > 20 || playerDest < 1){
		cout << "The destination you wish reach is outside the movable range. Please retry...." << endl;
		return false;
	} else if( canMove( room, player, playerDest)){
		player.location = playerDest - 1;
		return true;
	} else 
		return false;
}//end isValidDestination-----------------------------------------------------------------------------------------


//
//shows the location of the super bat, pit, and wumpus rooms
void showCheats( Room* room[]){
	cout << "------------------------" << endl;
	cout << "Wumpus Room: ";
	for( int i = 0; i < 20; i++){
		if( room[ i]->hasWumpus)
			cout << room[ i]->roomNum;
	}

	cout << endl << "Pit Rooms: ";
	for( int i = 0; i < 20; i++){
		if( room[ i]->hasPit)
			cout << room[ i]->roomNum << " ";
	}

	cout << endl << "Super Bat Rooms: ";
	for( int i = 0; i < 20; i++){
		if( room[ i]->hasBat)
			cout << room[ i]->roomNum << " ";
	}

	cout << endl << "Room with Arrow: ";
	for( int i = 0; i < 20; i++){
		if( room[ i]->hasArrow)
			cout << room[ i]->roomNum << " ";
	}

	cout << endl << "------------------------" << endl;
}// end showCheats------------------------------------------------------------------------------------------------


//
//checks if the user entered a valid arrow path
int numberOfValidShots( Room* room[], int adjRoom1, int adjRoom2, int adjRoom3, int roomShot[], int numberInputs){

	for( int i = 0; i < numberInputs; i++){
		if( roomShot[ i] == adjRoom1 || roomShot[ i] == adjRoom2 || roomShot[ i] == adjRoom3){
			adjRoom1 = room[ roomShot[ i]-1]->roomNext1->roomNum;
			adjRoom2 = room[ roomShot[ i]-1]->roomNext2->roomNum;
			adjRoom3 = room[ roomShot[ i]-1]->roomNext3->roomNum;
		} else
			return i;
	}

	return numberInputs;
}//end numberOfValidShots-----------------------------------------------------------------------------------------


//
//
void sendArrowToRandomRoom( Room* room[], int roomShot[], int currentRoom, int numberDisjoint, int indexDisjoint){
	srand( time( NULL));
	int randNum = rand() % 3;
	
	for( int i = 0; i < numberDisjoint; i++){
		if( randNum == 0)
			roomShot[ indexDisjoint + i] = room[ currentRoom]->roomNext1->roomNum;
		if( randNum == 1)
			roomShot[ indexDisjoint + i] = room[ currentRoom]->roomNext2->roomNum;
		if( randNum == 2)
			roomShot[ indexDisjoint + i] = room[ currentRoom]->roomNext3->roomNum;		

		currentRoom = roomShot[ indexDisjoint + i] - 1;
	}
}//end sendArrowToRandomRoom--------------------------------------------------------------------------------------


//
//checks to see if the player killed the wumpus
void checkIfKilledWumpus( Room* roomShotInto){
	if( roomShotInto->hasWumpus){
		cout << "The arrow hit and killed the wumpus! You win!" << endl;
		exit( -1);
	}
}


//
//shoots the arrow 
void shootArrow( Room* room[], Player &player, int &moveNum){
	int roomShot[ 3] = { -1, -1, -1};
	int i = 0;
	int adjRoom1 = room[ player.location]->roomNext1->roomNum;
	int adjRoom2 = room[ player.location]->roomNext2->roomNum;
	int adjRoom3 = room[ player.location]->roomNext3->roomNum;
	int numValShots = -1;
	player.firedArrow = true;
	
	while( cin.peek() != '\n'){
		cin >> roomShot[ i];
		i++;
	}

	if( !player.hasArrow){
		cout << "You don't have any arrows to shoot!" << endl;
		return;
	}

	player.hasArrow = false;
	moveNum++;

	if( roomShot[ 1] == -1 && roomShot[ 2] == -1){
		numValShots = numberOfValidShots( room, adjRoom1, adjRoom2, adjRoom3, roomShot, 1);
		if( numValShots == 0)
			sendArrowToRandomRoom( room, roomShot, player.location, 1, 0);
	} else if( roomShot[ 2] == -1){
		numValShots = numberOfValidShots( room, adjRoom1, adjRoom2, adjRoom3, roomShot, 2);	
		if( numValShots == 0)
			sendArrowToRandomRoom( room, roomShot, player.location, 2, 0);
		else if( numValShots == 1)
			sendArrowToRandomRoom( room, roomShot, roomShot[ 1], 1, 1);
	} else if( roomShot[ 0] != -1 && roomShot[ 1] != -1 && roomShot[ 2] != -1){
		numValShots = numberOfValidShots( room, adjRoom1, adjRoom2, adjRoom3, roomShot, 3);	
		if( numValShots == 0)
			sendArrowToRandomRoom( room, roomShot, player.location, 3, 0);
		else if( numValShots == 1)
			sendArrowToRandomRoom( room, roomShot, roomShot[ 1], 2, 1);
		else if( numValShots == 2)
			sendArrowToRandomRoom( room, roomShot, roomShot[ 2], 1, 2);
	}

	if( ( roomShot[ 0] - 1) == player.location || ( roomShot[ 1] - 1) == player.location || 
		( roomShot[ 2] - 1) == player.location){
		cout << "The arrow richocheted and killed you! Game over...." << endl;
		exit( -1);
	}

	room[ roomShot[ i - 1] - 1]->hasArrow = true;
	checkIfKilledWumpus( room[ roomShot[ i - 1] - 1]);
}//end shootArrow----------------------------------------------------------------------------------				


//
//undoes move
void undoMove( Room* room[], Player &player, GameState *&pHead, int &moveNum){
	if( moveNum == 1){
		cout << "ERROR: MOVE CANNOT BE UNDONE" << endl;
		return;
	}

	room[ pHead->wumpRoom]->hasWumpus = false;
	room[ pHead->batRoom1]->hasBat = false;
	room[ pHead->batRoom2]->hasBat = false;
	room[ pHead->arrowRoom]->hasArrow = false;
	
	pHead = pHead->prevMove;

	cout << "a" << endl;
	moveNum = pHead->moveNumber;
	room[ pHead->wumpRoom]->hasWumpus = true;
	room[ pHead->batRoom1]->hasBat = true;
	room[ pHead->batRoom2]->hasBat = true;
		cout << "b" << endl;
	if( pHead->arrowRoom != -1)
		room[ pHead->arrowRoom]->hasArrow = true;
	player.location = pHead->playerRoom;
	player.hasArrow = pHead->playerHasArrow;
	player.firedArrow = pHead->playerHasFired;
}

//
//takes the player input
void playerInput( Room* room[], Player &player, GameState *&pHead, int &moveNum){
	char playerMove;
	char c;

	cout << moveNum << ". ";
	cout << "You are currently in room " << player.location + 1 << ". ";
	cout << "Tunnels lead you to rooms "
	     << room[ player.location]->roomNext1->roomNum << " "
	     << room[ player.location]->roomNext2->roomNum << " "
	   	 << room[ player.location]->roomNext3->roomNum << endl; 
	
	while( true){
		cout << "Enter your move: ";
		cin >> playerMove;
		cin.get( c);

		if( playerMove == 'm' || playerMove == 'M'){
			if( isValidDesitnation( room, player)){
				moveNum++;
				break;
			}
		} else if( playerMove == 's' || playerMove == 'S'){
			shootArrow( room, player, moveNum);
			break;
		} else if( playerMove == 'u' || playerMove == 'U'){
			//undoMove( room, player, pHead, moveNum);						
			break;
		} else if( playerMove == 'c' || playerMove == 'C'){
			showCheats( room);					
			break;
		} else 
			cout << "Invalid move type. Please retry...." << endl;
	
		continue;
	}
}//end playerInput------------------------------------------------------------------------------------------------


//
//checks if there is a hazard in an adjacent room
//alerts player if there is a hazard in an adjacent room
void checkForHazard( Room* room[], Player player){
	Room* nextRoom1 = room[ player.location]->roomNext1;
	Room* nextRoom2 = room[ player.location]->roomNext2;
	Room* nextRoom3 = room[ player.location]->roomNext3;

	//alert the player if there is a pit/bat/wumpus in a room next to the current one
	if( nextRoom1->hasPit || nextRoom2->hasPit || nextRoom3->hasPit)
		cout << "----You feel a light breeze" << endl;
	if( nextRoom1->hasBat || nextRoom2->hasBat || nextRoom3->hasBat)
		cout << "----You hear flapping" << endl;
	if( nextRoom1->hasWumpus || nextRoom2->hasWumpus || nextRoom3->hasWumpus)
		cout << "----You smell a Wumpus" << endl;
}//end checkForHazard---------------------------------------------------------------------------------------------


//
//implements the superbat hazard
//changes the player's current room and change's the bat's current room
void superBats( Room* room[], Player &player){
	srand( time( NULL));
	Room* batRoom = NULL;			//new room for the superbats
	int batLoc = 0;

	//clear the bats from the room
	room[ player.location]->hasBat = false;
	
	//assign a new room to the player
	player.location = rand() % 20;
	cout << "Super bats carried you away!" << endl;
	
	//move the bats
	batLoc = rand() % 20;
	batRoom = room[ batLoc];
	
	//make sure there are no other hazards in that room and check if the player is in that room
	while( batRoom->hasBat || batRoom->roomNum == ( player.location - 1)){
		batLoc = rand() % 20;
		batRoom = room[ batLoc];
	}

	room[ batLoc]->hasBat = true;
}//end superBats--------------------------------------------------------------------------------------------------


//
//checks if the room has a hazard in it
void executeHazard( Room* room[], Player &player){
	//checks for pit
	if( room[ player.location]->hasPit){
		cout << "You fell into a pit and died! Game over....." << endl;
		exit( -1);
	}
	
	//check for wumpus and awake
	if( player.firedArrow && room[ player.location]->hasWumpus){
		cout << "The wumpus ate you! Game over..." << endl;
		exit( -1);
	}

	//checks for bats
	if( room[ player.location]->hasBat){
		//change the current room
		superBats( room, player);
		executeHazard( room, player);
		return;
	}

	if( room[ player.location]->hasArrow){
		room[ player.location]->hasArrow = false;
		player.hasArrow = true;
		cout << " You've picked up your arrow!" << endl;
	}
}//end executeHazard----------------------------------------------------------------------------------------------


//
//performs actions of the wumpus
void wumpusMove( Room* room[], Player player){
	srand( time( NULL));
	int randNum = rand() % 4;
	int wumpusRoom = 0;

	for( int i = 0; i < 20; i++){
		if( room[ i]->hasWumpus)
			wumpusRoom = room[ i]->roomNum;
	}

	//move the wumpus
	if( randNum == 0){
		//do not move
	} else if( randNum == 1){
		room[ wumpusRoom-1]->hasWumpus = false;
		room[ wumpusRoom-1]->roomNext1->hasWumpus = true;
	} else if( randNum == 2){
		room[ wumpusRoom-1]->hasWumpus = false;
		room[ wumpusRoom-1]->roomNext2->hasWumpus = true;
	} else if( randNum == 3){
		room[ wumpusRoom-1]->hasWumpus = false;
		room[ wumpusRoom-1]->roomNext3->hasWumpus = true;
	}
}//end wumpusMove-------------------------------------------------------------------------------------------------


//
//initialized the gamestate temp pointer
void initPointer( GameState * &pTemp){
	pTemp->moveNumber = -1;
	pTemp->arrowRoom = -1;
	pTemp->wumpRoom = -1;
	pTemp->batRoom1 = -1;
	pTemp->batRoom2 = -1;
	pTemp->playerRoom = -1;
	pTemp->playerHasArrow = false;
	pTemp->playerHasFired = false;
	pTemp->prevMove = NULL;
}


//
//advances the current gameState, storing the current game values
void advanceGameState( Room* room[], Player &player, GameState *&pHead, int moveNum){
	
	GameState *pTemp;
	bool foundBats = false;
	
	initPointer( pTemp);

	pTemp->moveNumber = moveNum;

	for( int i = 0; i < 20; i++){
		if( room[ i]->hasWumpus == true){
			pTemp->wumpRoom = room[ i]->roomNum - 1;
		}
		if( foundBats && room[ i]->hasBat)
			pTemp->batRoom2 = room[ i]->roomNum - 1;
		if( !foundBats && room[ i]->hasBat){
			pTemp->batRoom1 = room[ i]->roomNum - 1;
			foundBats = true;
		}
		if( room[ i]->hasArrow)
			pTemp->arrowRoom = room[ i]->roomNum - 1;
	}

	pTemp->playerRoom = player.location;
	pTemp->playerHasArrow = player.hasArrow;
	pTemp->playerHasFired = player.firedArrow;
	pTemp->prevMove = pHead;
	pHead = pTemp;
}


int main() {
	srand( time( NULL));
	
	Room* room[ 20];					//array of structs containing all the rooms
	Player player;						//struct for player
	GameState *pHead;

	player.location = rand() % 20;		//pending starting location for the player
	player.hasArrow = true;				//start with an arrow
	player.firedArrow = false;			//have not yet fired an arrow
	int moveNum = 1;
	int movePrev = 0;

	header();	
	initRooms( room);
	generateHazard( room);
	player.location = playerStart( room, player);

	while( true) {
		if( moveNum > movePrev)
			advanceGameState( room, player, pHead, moveNum);

		checkForHazard( room, player);
		
		movePrev = moveNum;
		playerInput( room, player, pHead, moveNum);
		
		cout << endl;
		
		executeHazard( room, player);
		
		if( ( player.firedArrow || room[ player.location]->hasWumpus) && moveNum > movePrev){
			cout << "The wumpus is awake!" << endl;
			wumpusMove( room, player);
		}	
	}

	return 0;
}