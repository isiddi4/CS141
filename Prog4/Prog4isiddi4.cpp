/* ------------------------------------------------ 
* CryptoFile
*
* Class: CS 141, Spring 2016.  Tues 11am Lab
* System: Linux Ubuntu, Sublime Text 2
* Author: Ismail Siddiqui
* TA: Itica Gupta
*
* -------------------------------------------------
*/
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <time.h>
#include <cctype>

using namespace std;

const int maxNumberOfChars = 1500; //max number of printable chars
const int LIMIT = 26;		       //number of chars in alphabet

//Prints header
void header(){
	cout << "Author: Dale Reed" << endl         
		 << "Program #4: CryptoFile" << endl  
		 << "TA: Itica Gupta, T 11:00 AM" << endl
		 << "Feb 29, 2016" << endl                
		 << "System: Sublime Text 2 on Ubuntu" << endl << endl;
}//end header----------------------------------------------------------------------------------------


//takes and checks user input
void promptInput(int &userChoice){
	//cout << "This is the input funciton" << endl << endl;
	cout << "Choose your option:" << endl
		 << "1. Encrypt a file" << endl
		 << "2. Decrypt a file" << endl
		 << "Your choice: ";
	
	while( true){
		cin >> userChoice;
		if( userChoice == 1 || userChoice == 2)
			break;
		cout << "Invalid Input. Please retry: ";
	}

	cout << endl;
}


//purpose: encrypt characters from text
char Encrypt( char c, int transpose){
	int newTranspose = 0;

	if( islower( c)) {
		if( c + transpose > 'z'){
			newTranspose = transpose - ( 'z' - c + 1);
			c = 'a' + newTranspose;
		} else
			c += transpose;
	}
	else if( isupper(c)) {
		if( c + transpose > 'Z'){
			newTranspose = transpose - ( 'Z' - c + 1);
			c = 'A' + newTranspose;
		} else
			c += transpose;
	}

	return c;
}


void program1(){
	ifstream inStream;		//input file stream
	ofstream outStream;		//output file stream
	char c;					//character in text file
	int count = 0;			//keeps track of how many characters printed
	srand( time( NULL));
	int transpose = rand() % 26 + 1; //transpose value for encryption

	//open files and check if open else throw an error and exit the program
	inStream.open( "sample.txt");
	assert( !inStream.fail());
	outStream.open( "Cipher.txt");
	assert( !inStream.fail());

	while( inStream.get(c)){
		//encrypt text and output result
		outStream << Encrypt( c, transpose);
		cout << Encrypt( c, transpose);
		
		count++;
		//stop printing after 1500 chars
		if( count == maxNumberOfChars)
			break;
	}

	cout << endl;
	//close files
	inStream.close();
	outStream.close();
}


//struct that contains the letters a-z and frequency of those letters
struct letterCounter{
	char letter;
	int frequency;
};


//initializes arrays, filling it with letters a-z and 0s
void initializeArray(letterCounter object[]){	
	for(int i = 0; i < LIMIT; i++){
		object[i].letter = i + 'a';
		object[i].frequency = 0;
	}
}


//uses bubble sort to sort letter frequencies and letter names respectively
void sort(letterCounter object[]){
	letterCounter temp;

	for(int i = 1; i < LIMIT; i++) {
		for(int j = 0; j < (LIMIT - 1); j++) {
			if(object[j+1].frequency > object[j].frequency) {
				temp.frequency = object[j].frequency;
				object[j].frequency = object[j+1].frequency;
				object[j+1].frequency = temp.frequency;

				temp.letter = object[j].letter;
				object[j].letter = object[j+1].letter;
				object[j+1].letter = temp.letter;
			}
		}
	}
}


//takes in character to decrypt and compares it to letters in the cipher and book structs
char Decrypt(char c, letterCounter cipher[], letterCounter book[]){
	int upperCase = 0;

	if( isupper( c)){
		upperCase = 1;
	} else {
		upperCase = 0;
	}

	for( int i = 0; i < LIMIT; i++){
		if( c == cipher[ i].letter || c == toupper(cipher[ i].letter)){
			if(upperCase == 0){
				return book[ i].letter; 
			} else
				return toupper(book[ i].letter);
		} 
	}

	//return c;
}


//read from cipher file
//get character from cipher file
//change character from cipher file:
//	1. find character from file in sorted cipher array
//	2. take its index
//	3. use that index to find the corresponding character in the sorted book array
//	4. set the input character equal to the character from step 3
//	5. check for upper case and use toupper
void program2(){
	ifstream cipherFile, bookFile;
	ofstream resultFile;
	letterCounter book[ LIMIT], cipher[ LIMIT]; //structs that store letters and letter frequencies
	initializeArray(book);
	initializeArray(cipher);
	char c;
	int count = 0;
	
	cipherFile.open( "cipher.txt");
	assert( !cipherFile.fail());
	bookFile.open( "lifeonmiss.txt");
	assert( !bookFile.fail());

	while( cipherFile.get( c)) {
		if( isalpha( c))
			cipher[ tolower( c) - 'a'].frequency++;
	} 
	while( bookFile.get( c)) {
		if( isalpha( c))
			book[ tolower( c) - 'a'].frequency++;
	}

	cipherFile.close();
	bookFile.close();

	sort(book);
	sort(cipher);

	cipherFile.open( "cipher.txt");
	assert( !cipherFile.fail());
	resultFile.open( "result.txt");
	assert( !resultFile.fail());

	while( cipherFile.get( c)){
		if( isalpha(c)){
			resultFile << Decrypt( c, cipher, book);
			cout << Decrypt( c, cipher, book);
		} else {
			resultFile << c;
			cout << c;
		}

		count++;
		if( count == maxNumberOfChars)
			break;
	} 

	cipherFile.close();
	resultFile.close();
}


//directs user to appropirate program based on user input
void chooseProgram(int &userChoice){
	promptInput(userChoice);
	cout << "You entered option " << userChoice << endl;

	switch(userChoice) {
		case 1:
			cout << "Encrypt file sample.txt" << endl;
			program1();
			break;
		case 2:
			cout << "Decrypt file cipher.txt using lifeonmiss" << endl;
			program2();
			break;
		default:
			cout << "ERROR: INVALID INPUT ------- NOW LEAVING PROGRAM" << endl << endl;
	}
}

int main(){
	int userChoice = 0;

	header();
	chooseProgram(userChoice);

	cout << endl;

	return 0;
}