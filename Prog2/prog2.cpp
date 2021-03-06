#include <stdio.h>


void header()
{
	printf("Author: Ismail Siddiqui\n");

	//prints out rules and description of game
	printf("Welcome to Trinity, the game where there's three ways\n");
	printf("to get three in a row, in one of three rows or columns,\n");
	printf("or diagonals.  Play alternates between the two players.\n");
	printf("On each move select any available piece and an open\n");
	printf("board destination location.  A winning configuration\n");
	printf("is three of the same set in a row (e.g. b a c, A B C\n"); 
	printf("or 3 2 1),  all 3 from the same set position (e.g. A 1 a,\n"); 
	printf("B 2 b or c 3 C), or three in a row where each is a\n"); 
	printf("different set member (1st, 2nd, 3rd) such as 2 A c,\n");
	printf("a B 3 or C 1 b.\n\n");   

}//---------------------------end header-------------------------------------


//displays all available pieces and grid
//nine character variables p1...p9
//nine character variables b1...b9 = empty spaces
void display(char &p1, char &p2, char &p3, char &p4, char &p5, char &p6, char &p7,
	     char &p8, char &p9, char &b1, char &b2, char &b3, char &b4, char &b5,
	     char &b6, char &b7, char &b8, char &b9)
{
	printf("------------------------------------\n");
	printf("Pieces available to play:\n");
 	printf("   %c %c %c\n", p1, p2, p3);
	printf("   %c %c %c\n", p4, p5, p6);
	printf("   %c %c %c\n", p7, p8, p9);
	printf("\n");

	printf(" -----------   Positions:\n");
	printf("| %c | %c | %c |    1  2  3\n", b1, b2, b3);
	printf(" -----------\n");
	printf("| %c | %c | %c |    4  5  6\n", b4, b5, b6);
	printf(" -----------\n");
	printf("| %c | %c | %c |    7  8  9\n", b7, b8, b9);
	printf(" -----------\n");	

}//-----------------------end display-------------------------------------------


//checks to see if user input piece is taken or is invalid
//if userPiece is not equal to one of the available pieces or is a space, return 0
//else return 1
//0 = invalid argument, 1 = valid argument, needed to help break from input loop
int checkPiece(char &p1, char &p2, char &p3, char &p4, char &p5, char &p6, char &p7,
               char &p8, char &p9, char &userPiece)
{	
	if((userPiece != p1 && userPiece != p2 && userPiece != p3 && userPiece != p4 &&
	    userPiece != p5 && userPiece != p6 && userPiece != p7 && userPiece != p8 &&
	    userPiece != p9) || userPiece == ' ')
	{
		printf("*** Piece %c is unavailable. Please retry: ", userPiece);
		return 0;
	}
	else
	{
		return 1;
	}

}//----------------------end Check Piece--------------------------------------


//checks if the submitted userPosition is vacant or if it is valid
//0 = invalid argument, 1 = valid argument, needed to help break from input loop
int checkPos(char &b1, char &b2, char &b3, char &b4, char &b5, char &b6, char &b7, 
	     char &b8, char &b9, char &userPos)
{
	switch(userPos)
	{
		case '1':
			if(b1 != ' ')
				return 0;
			return 1;
		case '2':
			if(b2 != ' ')
				return 0;
			return 1;
		case '3':
			if(b3 != ' ')
				return 0;
			return 1;
		case '4':
			if(b4 != ' ')
				return 0;
			return 1;
		case '5':
			if(b5 != ' ')
				return 0;
			return 1;
		case '6':
			if(b6 != ' ')
				return 0;
			return 1;
		case '7':
			if(b7 != ' ')
				return 0;
			return 1;
		case '8':
			if(b8 != ' ')
				return 0;
			return 1;
		case '9':
			if(b9 != ' ')
				return 0;
			return 1;
		default :
			return 0;
	}

}//-----------------------------End Check Position---------------------------------


//takes all playable pieces and pieces on board
//scans for player input
//checks if player input is valid or not
//if player input is invalid, the program will keep looping until 
//the player submits a valid argument
void inputPP(char &p1, char &p2, char &p3, char &p4, char &p5, char &p6, char &p7,
             char &p8, char &p9, char &b1, char &b2, char &b3, char &b4, char &b5,
             char &b6, char &b7, char &b8, char &b9, int turn, int playerNum, 
	     char &userPiece, char &userPos)
{
	int valid = 0;

	printf("%d. Player %d enter piece and position: ", turn, playerNum);
	
	while(valid != 1)
	{
		scanf(" %c%c", &userPiece, &userPos);

		valid = checkPiece(p1, p2, p3, p4, p5, p6, p7, p8, p9, userPiece);
		if(valid != 1)
			continue;
		
		valid = checkPos(b1, b2, b3, b4, b5, b6, b7, b8, b9, userPos);
		if(valid != 1)
		{
			printf("*** Location %c unavailabe or invalid. Please retry: ", userPos);
			continue;
		}	
	}
	
	printf("\n");

}//------------------------end inputPP----------------------------------------------------


//sets user input equal to one of the blank board pieces depending on desired position
void inputToBoard(char &b1, char &b2, char &b3, char &b4, char &b5, char &b6, char &b7,
                  char &b8, char &b9, char &userPiece, char &userPos)
{
	switch(userPos)
	{
		case '1':
			b1 = userPiece;
			break;
		case '2':
			b2 = userPiece;
			break;
		case '3':
			b3 = userPiece;
			break;
		case '4':
			b4 = userPiece;
			break;
		case '5':
			b5 = userPiece;
			break;
		case '6':
			b6 = userPiece;
			break;
		case '7':
			b7 = userPiece;
			break;
		case '8':
			b8 = userPiece;
			break;
		case '9':
			b9 = userPiece;
			break;
		default :
			printf("ERROR in inputToBoard()\n");
			break;
	}

}//----------------------------end Input to Board----------------------------------------


//removes playable pieces from display depending on what piece the user entered
void removePiece(char &p1, char &p2, char &p3, char &p4, char &p5, char &p6, char &p7,
                 char &p8, char &p9, char &userPiece)
{
	switch(userPiece)
	{
		case 'A':
			p1 = ' ';
			break;
		case 'B':
			p2 = ' ';
			break;
		case 'C':
			p3 = ' ';
			break;
		case '1':
			p4 = ' ';
			break;
		case '2':
			p5 = ' ';
			break;
		case '3':
			p6 = ' ';
			break;
		case 'a':
			p7 = ' ';
			break;
		case 'b':
			p8 = ' ';
			break;
		case 'c':
			p9 = ' ';
			break;
		default :
			printf("ERROR in removePiece()\n");
			break;
	}

}//---------------------------end Remove Piece--------------------------------------------


int checkWinning(char &b1, char &b2, char &b3, char &b4, char &b5, char b6, char &b7, 
		 char &b8, char &b9, int win1, int win2, int win3, int win4, 
		 int win5, int win6, int win7, int win8)
{
	//The ASCII values of each playing peice is unique
	//The sum of the ASCII values of the winning conditions are unique
	//Ex. A+B+C = 198, 1+2+3 = 150
	//rows	
	if(win1 == 150 || win1 == 214 || win1 == 198 || win1 == 294 || win1 == 211 || win1 == 217)
	{
		printf("Congratulations, you win with %c %c %c\n", b1, b2, b3);
		return 1;
	}
	else if(win2 == 150 || win2 == 214 || win2 == 198 || win2 == 294 || win2 == 211 || win2 == 217)
        {       
                printf("Congratulations, you win with %c %c %c\n", b4, b5, b6);
                return 1;
        }
	else if(win3 == 150 || win3 == 214 || win3 == 198 || win3 == 294 || win3 == 211 || win3 == 217)
        {       
                printf("Congratulations, you win with %c %c %c\n", b7, b8, b9);
                return 1;
        }

	//columns
	else if(win4 == 150 || win4 == 214 || win4 == 198 || win4 == 294 || win4 == 211 || win4 == 217)
        {       
                printf("Congratulations, you win with %c %c %c\n", b1, b4, b7);
                return 1;
        }
	else if(win5 == 150 || win5 == 214 || win5 == 198 || win5 == 294 || win5 == 211 || win5 == 217)
        {       
                printf("Congratulations, you win with %c %c %c\n", b2, b5, b8);
                return 1;
        }
	else if(win6 == 150 || win6 == 214 || win6 == 198 || win6 == 294 || win6 == 211 || win6 == 217)
        {       
                printf("Congratulations, you win with %c %c %c\n", b3, b6, b9);
                return 1;
        }

	//diagonals
	else if(win7 == 150 || win7 == 214 || win7 == 198 || win7 == 294 || win7 == 211 || win7 == 217)
        {       
                printf("Congratulations, you win with %c %c %c\n", b1, b5, b9);
                return 1;
        }
	else if(win8 == 150 || win8 == 214 || win8 == 198 || win8 == 294 || win8 == 211 || win8 == 217)
        {       
                printf("Congratulations, you win with %c %c %c\n", b3, b5, b7);
                return 1;
        }
	else
		return 0;

}//---------------------------------------end Check Winning-------------------------------------------


//sets rules for winning conditions based on sum of each board variable's ASCII value
//return a value of 1 if a player won, and a value of 0 if a player did not win
int winningCond(char &b1, char &b2, char &b3, char &b4, char &b5, char &b6, char &b7,
		char &b8, char &b9)
{
	int win1 = 0, win2 = 0, win3 = 0,
	    win4 = 0, win5 = 0, win6 = 0,
	    win7 = 0, win8 = 0, win  = 0;
	
	//rows
	win1 = b1 + b2 + b3;
	win2 = b4 + b5 + b6;
	win3 = b7 + b8 + b9;

	//columns
	win4 = b1 + b4 + b7;
	win5 = b2 + b5 + b8;
	win6 = b3 + b6 + b9;

	//diagonals
	win7 = b1 + b5 + b9;
	win8 = b3 + b5 + b7;

	win = checkWinning(b1, b2, b3, b4, b5, b6, b7, b8, b9, 
	      win1, win2, win3, win4, win5, win6, win7, win8);
	
	return win;

}//--------------------------Winning Conditions-----------------------------------------


int main() 
{
	char p1 = 'A', p2 = 'B', p3 = 'C',
	     p4 = '1', p5 = '2', p6 = '3',
	     p7 = 'a', p8 = 'b', p9 = 'c',
	     b1 = ' ', b2 = ' ', b3 = ' ',
	     b4 = ' ', b5 = ' ', b6 = ' ',
	     b7 = ' ', b8 = ' ', b9 = ' ',
	     userPiece, userPos;
	
	int  turn = 0,
	     playerNum = 0,
	     win = 0;

	header();
	
	//general cycle = display board - check if winning - input - check input -
	//display user input to board - remove user input from playable pieces -
	//display board - check if winning........end
	while(true)
	{
		playerNum = turn % 2 + 1;
		turn += 1;

		display(p1, p2, p3, p4, p5, p6, p7, p8, p9,
			b1, b2, b3, b4 ,b5, b6, b7, b8, b9);	
	
		win = winningCond(b1, b2, b3, b4, b5, b6, b7, b8, b9);
		//if player wins (win == 1), break from loop and end game
		if(win == 1)
			break;

		inputPP(p1, p2, p3, p4, p5, p6, p7, p8, p9,
			b1, b2, b3, b4, b5, b6, b7, b8, b9,
			turn, playerNum, userPiece, userPos);
		
		inputToBoard(b1, b2, b3, b4 ,b5, b6, b7, b8, b9,
			     userPiece, userPos);

		removePiece(p1, p2, p3, p4, p5, p6, p7, p8, p9,
			    userPiece);
	}
	return 0;
}
