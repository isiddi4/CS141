#include <iostream>
#include <cctype>
#include <stdlib.h>
#include <time.h>
using namespace std;

const int startRoom = 20;


//
//node that points to all adjacent rooms
//stores the current room#
//checks to see if there is a hazard/wumpus
//
struct Room {
	int roomNum;
	Room* roomNext1;
	Room* roomNext2;
	Room* roomNext3;
	bool hasWumpus;
	bool hasPit;
	bool hasBat;
};//end struct----------------------------------------------------------------------------------------------------


//
//prints out the header, board, and instructions
//
void header(){
cout    << endl 
		<< "Author: Ismail Siddiqui           ______18______ " << endl
	    << "CS 141 Program #6: Wumpus        /      |       \\ " << endl
	    << "TA: Itica Gupta, T 11:00 AM     /      _9__      \\ " << endl
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
	    << "2. Two other rooms have super-bats.  If you go there, a bats grab you and " << endl
	    << "   fly you to some random room, which could be troublesome.  Then those bats " << endl
	    << "   go to some random room. " << endl
	    << "3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and " << endl
	    << "   is too heavy for a bat to lift.  Usually he is asleep.  Two things wake " << endl
	    << "   him up: Anytime you shoot an arrow, or you entering his room.  When he   " << endl
	    << "   wakes he moves one room 75 percent of the time, staying still the other 25 percent of " << endl
	    << "   the time. After that, if he is in your room, he eats you and you die! " << endl << endl
	    << "Moves: " << endl
	    << "On each move you can: " << endl
	    << "1. Move into an adjacent room.  To move enter 'M' followed by a space and " << endl
	    << "   then a room number. " << endl
	    << "2. Shoot your guided arrow through a list of four adjacent rooms, which " << endl 
	    << "   you specify.  Your arrow ends up in the final room. If an arrow " << endl
	    << "   can't go a direction because there is no connecting tunnel, it ricochets " << endl
	    << "   and moves to a randomly selected adjacent room and continues " << endl
	    << "   doing this until it has traveled 4 rooms in total.  If the arrow  " << endl 
	    << "   hits the Wumpus, you win! If the arrow hits you, you lose. You  " << endl
	    << "   automatically pick up an arrow if you go through a room with an arrow in   " << endl
	    << "   it. To move enter 'S' followed by a list of up to 4 adjacent room numbers" << endl
	    << "   separated by spaces." << endl << endl
	    << "Good Luck!" << endl;
}//end header----------------------------------------------------------------------------------------------------


//
//initializes room numbers, pointers, and other variables
//takes an array of Room pointers
//
void initRooms( Room* room[]){
	//initialize room variables
	for( int i = 0; i < 20; i++){
		room[ i] = new Room;
		room[ i]->roomNum = i + 1;
		room[ i]->hasWumpus = false;
		room[ i]->hasPit = false;
		room[ i]->hasBat = false;
		//connent the current room to the next room
		if( i != 0){
			room[ i]->roomNext1 = room[ i-1];
			room[ i-1]->roomNext2 = room[ i];
		}
	}
	//
	//initialize room pointers
	for( int i = 0; i < 4; i++){
		//
		//connect the inner pentagons to the outer ones
		//
		//inner most to second rank
		room[ i]->roomNext3 = room[ i*2+7];
		room[ i*2+7]->roomNext3 = room[ i];
		//second rank to outer most
		room[ i*2+6]->roomNext3 = room[ 16+i];
		room[ 16+i]->roomNext3 = room[ i*2+6]; 	
	}
	//
	//leftover connections
	//
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
		randPit1 = rand() % 20;

	//while the first super bat's room is the same as any of the other hazard rooms
	while( randBat1 == randBat2 || randBat1 == randWump)
		randPit1 = rand() % 20;

	//while the second super bat's room is the same as any of the other hazard rooms
	while( randBat2 == randWump)
		randPit1 = rand() % 20;
}//end sameRoomChecker--------------------------------------------------------------------------------------------


//
//creates the room that has the hazards
//
void generateHazard( Room* room[]){
	srand( time( NULL));
	int randPit1 = rand() % 20,
		randPit2 = rand() % 20,
		randBat1 = rand() % 20,
		randBat2 = rand() % 20,
		randWump = rand() % 20;

	sameRoomChecker( randPit1, randPit2, randBat1, randBat2, randWump);

	//set the rooms with hazards on accordingly
	room[ randPit1]->hasPit = true;
	room[ randPit2]->hasPit = true;
	room[ randBat1]->hasBat = true;
	room[ randBat2]->hasBat = true;
	room[ randWump]->hasWumpus = true;
}


//
//checks to see if the user input a valid room destination
//
bool canMove( Room* room[], int currentRoom, int playerDest){
	int nextRoom1 = room[ currentRoom-1]->roomNext1->roomNum;
	int nextRoom2 = room[ currentRoom-1]->roomNext2->roomNum;
	int nextRoom3 = room[ currentRoom-1]->roomNext3->roomNum;

	if( playerDest != nextRoom1 && playerDest != nextRoom2 && playerDest != nextRoom3){
		cout << "Invalid location. Please retry...." << endl;
		return false;
	} else 
		return true;
}//end canMove----------------------------------------------------------------------------------------------------


//
//checks if the room has a hazard in it
//
void checkHazard( Room* room[], int currentRoom){
	if( room[ currentRoom-1]->hasPit){
		
	}
}//end checkHazard------------------------------------------------------------------------------------------------


//
//takes the player input
//
void playerInput( Room* room[], int &currentRoom){
	char playerMove = ' ';
	int playerDest = 0;
	
	cout << "You are currently in room " << currentRoom << endl;
	
	while( true){
		cout << "Your move: ";
		cin >> playerMove >> playerDest;
		
		//checks validity of input
		if( playerDest > 20 || playerDest < 1){
			cout << "The destination you wish reach is outside the movable range. Please retry...." << endl;
			continue;
		} else if( playerMove == 'm' || playerMove == 'M'){
			if( canMove( room, currentRoom, playerDest))
				break;
		} else
			cout << "Invalid move type. Please retry...." << endl;
	}

	currentRoom = playerDest;
}//end playerInput------------------------------------------------------------------------------------------------


int main() {
	Room* room[ 20];
	int currentRoom = startRoom;
	
	header();	
	initRooms( room);
	generateHazard( room);

	while( true) {
		playerInput( room, currentRoom);	
		checkHazard( room, currentRoom);
	}

	return 0;
}