/*
 * Program 3 : Decipher
 * Author    : Ismail Siddiqui
 * Lab Time  : Tuesday 11am
 * TA        : Itica Gupta
 * Date      :  Feb. 22, 2016
 * System    : Sublime Text 2 on Ubuntu
 *---------------------------
 */
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <cassert>
using namespace std;

const int Size = 81;
const int maxNumOfWords = 34840;

//Purpose: Display header
//Recieves:
//Returns:
void header() {
	printf("Author: Ismail Siddiqui \n");
	printf("Program 3: Decipher \n");
	printf("TA: Itica Gupta, T 11:00 AM \n");
	printf("Feb 10, 2016\n\n");
}//end header---------------------------------------------------------------------------------------


//Purpose: Prompt user for input
//Recieves: Defaulted userChoice
//Returns: User input to execute program of choice
int takeInput(int &userChoice) {
	printf("Choose from the following options:\n");
	printf("   1. Encode some text \n");
	printf("   2. Decode using user-entered values \n");
	printf("   3. Decode automatically \n");
	printf("   4. Exit program\n");
	printf("Your Program Choice: ");
	scanf(" %d", &userChoice);	
	
	//return user input
	return userChoice;
}//end input----------------------------------------------------------------------------------------


//Purpose: Reverses he user entered string 
//Recieves: User entered char array
//Returns: reversed version of user entered text
void textReverse(char userText[]) {
	//N is essentially the size of the character array
	int N = strlen(userText);
	char temp;
	for(int i = 0; i < (N / 2); i += 1) {
		temp = userText[i];
		userText[i] = userText[N - (i+1)];
		userText[N - (i +1)] = temp;
	}
}//end textReverse----------------------------------------------------------------------------------


//Purpose: Takes userinput and encrypts or decrypts it by a transposition
//Recieves: Char array called encryptedText text that stores the altered 
//          entered text, the user text, and the transposition value
//Returns:
void encryptDecrypt(char encryptedText[], char userText[], int transposition) {
	int N = strlen(userText);				
	int newTranspose = 0;					// altered transpose value
	bool usingDefaultTranspose = true;		// check if default transpose value is being used

	for(int i = 0; i < N; i += 1) {
		if(transposition > 0) {
			//if the transposition makes the character fall out of bounds
			//case 1: transpose value makes char go above ASCII dec value 126
			if(userText[i] + transposition > 126) {  
				//change transpose value
				newTranspose = transposition - (127 - userText[i]);
				//wrap around
				encryptedText[i] = ' ' + newTranspose;
				//use altered transpose value
				usingDefaultTranspose = false;				
			}
		}
		if(transposition < 0) {
			//case 2: transpose value makes char go below ASCII dec value 32
			if(userText[i] + transposition < 32) {
				newTranspose = transposition + (userText[i] - 31);
				encryptedText[i] = '~' + newTranspose;
				usingDefaultTranspose = false;				
			}
		}

		if(usingDefaultTranspose)
			encryptedText[i] = userText[i] + transposition;
		else {
			//use default transpose value for next iteration
			usingDefaultTranspose = true; 					
		}
	}
}//end encrypt--------------------------------------------------------------------------------------


//Purpose: Takes the user-entered character array, alters it, and stores a 
//         transposed version of userText[] in encryptedText[] depending 
//         of the value of transposition. 
//Recieves: Empty user text string, reversed text string, encrypted text string, and 
//          initialized transposition
//Returns:
void program1(char encryptedText[], char userText[], int &transposition){
	int direction = 0;

	printf("Enter the text to be encoded: ");
	//use getchar() to take care of bad input
	getchar();
	cin.getline(userText, 80);
	
	//char newArray[81];
	//strcpy(newArray, userText);
	//printf("%s\n", newArray);

	printf("Enter direction (1 Forwards or 2 Reverse): ");
	scanf(" %d", &direction);
	if(direction == 2)
		textReverse(userText);	
		
	printf("Enter transposition value (0..25): ");
	while(true) {
		scanf(" %d", &transposition);
		if(transposition < 0 || transposition > 25) {
			printf("Invalid transposition. Try again: ");
			continue;
		} else
			break;
	}

	printf("\nPlainText and ciphertext are:\n");
	printf("%s \n", userText);
	encryptDecrypt(encryptedText, userText, transposition);
	printf("%s\n\n", encryptedText);
}//end program1-------------------------------------------------------------------------------------


//Purpose: Decodes  users submited strings
//Recieves: Empty arrays userText and encryptedText that will store user entered values and 
//			auto-edited values respectively. Also takes initialized transposition value of 0
//Returns:
void program2(char encryptedText[], char userText[], int &transposition) {
	printf("Enter the text to be decoded: ");
	getchar();
	cin.getline(userText, 80);

	printf("Enter transposition value: ");
	scanf(" %d", &transposition);

	printf("\nPlaintext and ciphertext are:\n");
	printf("%s \n", userText);
	encryptDecrypt(encryptedText, userText, transposition);
	printf("%s  <------ Forwards\n", encryptedText);
	textReverse(encryptedText);
	printf("%s  <------ Reverse\n", encryptedText);

}//end program2-------------------------------------------------------------------------------------


//Purpose: Opens "dictionaryMax6.txt" if its in the same folder as the source code and inputs all
//		   of its data into an array called theWords
//Recieves: An array to store the words 
//Returns:
void readFromFile(char theWords[][ Size], int wordRow) {
	ifstream myFile;

   	myFile.open( "dictionaryMax6.txt");
   	assert( ! myFile.fail() );  // make sure file open was OK
	
   	// Keep repeating while input from the file yields a word
   	while ( myFile >> theWords[ wordRow])
   		wordRow++;

   	myFile.close();
}//end of readFromFile------------------------------------------------------------------------------


//Purpose: To search the dictionary for words that match the words in the string
//Recieves:	The string and all the mechanisms needed to compare the string to words from dictionary
//Returns: A value of 1 or 0 to see if the program is done
int searchDictionary(char *token, char encryptedText[], char space[], int maxNumOfWords, 
					  char theWords[][ Size], int &count, int &transposition,
					  int &bestTranspose) {
	
	token = strtok(encryptedText, " ");
	while(token != NULL) {
		for(int i = 0; i < maxNumOfWords; i++) {
			if(strcmp(token, theWords[i]) == 0)
				count++;
		}
		
		if(count == strlen(encryptedText)) {
			bestTranspose = transposition;
			return 1;
		}

		token = strtok(NULL, " ");
	}

	return 0;

}//end of searchDictionary--------------------------------------------------------------------------


//Purpose: Decode's a message automatically by comparing the user inputted string to words in the
//		   dictionary
//Recieves: array to store encrypted text, array to store user text, and initialized tranposition 
//			value
//Returns:
void program3(char encryptedText[], char userText[], int transposition) {
   	
   	char theWords[ maxNumOfWords][ Size],    	// a 2D array that stores words from dictionary
   		 space[2] = " ",						// space character
   		 *token;								// a word in userText stored as a char pointer
   		 										
   	int  wordRow = 0,                           // Row for the current word in dictionary
		 count = 0,								// number of matches
		 bestTranspose = 0,						// tranpose value with most matches
		 isDone = 0;							// variable to check if search is done

	readFromFile(theWords, wordRow);
	
	printf("Enter the text to be decoded: ");
	getchar();
	cin.getline(userText, 80);

	for(transposition = -25; transposition < 26; transposition++) {
		encryptDecrypt(encryptedText, userText, transposition);
		isDone = searchDictionary(token, encryptedText, space, maxNumOfWords, theWords, count, 
						          transposition, bestTranspose);
		if(isDone == 1)
			break;

		textReverse(userText);
		encryptDecrypt(encryptedText, userText, transposition);
		isDone = searchDictionary(token, encryptedText, space, maxNumOfWords, theWords, count, 
						          transposition, bestTranspose);
		if(isDone == 1)
			break;
	}

	printf("\nYour message is:\n");
	encryptDecrypt(encryptedText, userText, bestTranspose);
	printf("%s \n", encryptedText);

}//end program3-------------------------------------------------------------------------------------


//Purpose: Directs user to program based on user input
//Recieves: user program choice number
//Returns:
void execChosenProgram(int &userChoice, char encryptedText[], char userText[], int &transposition) {
	switch(userChoice) {
		case 1  :
			//EXEC program 1
			program1(encryptedText, userText, transposition);
			break;
		case 2  :
			//EXEC program 2
			program2(encryptedText, userText, transposition);
			break;
		case 3  :
			//EXEC program 3
			program3(encryptedText, userText, transposition);
			break;
		case 4  :
			//EXEC program 4
			break; 
		default :
			//Switch should not default...
			printf("ERROR: INVALID PROGRAM CHOICE------------------------\n"); 
	}
}//end execChosenProgram----------------------------------------------------------------------------


int main() {
	int userChoice = 0,
		transposition = 0;

	char userText[Size],
		 encryptedText[Size];

	header();

	//default userChoice to zero everytime it loops
	userChoice = 0;
	//set userChoice equal to the validated user input
	userChoice = takeInput(userChoice);
	//execute program based on validated user input
	execChosenProgram(userChoice, encryptedText, userText, transposition);

	printf("\nDone.\n\n");

	return 0;
}