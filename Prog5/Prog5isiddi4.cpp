/* ------------------------------------------------ 
* Speech Grades
*
* Class: CS 141, Spring 2016.  Tues 11am Lab
* System: Linux Ubuntu, Sublime Text 2
* Author: Ismail Siddiqui
* TA: Itica Gupta
*
* -------------------------------------------------
*/
#include <iostream>
#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <iomanip>
using namespace std;


//struct that stores all necessary speach values
struct Speech{
	const char* candidateFile;		//stores all of the candidate file names
	float gradeLevel; 				//float that keeps track of speech grade level
	int numWords,					//number of words
		numSents,					//number of sentences
		numSylls; 					//number of syllables
};


//Prints header
void header(){
	cout << "Author: Ismail Siddiqui" << endl         
		 << "Program #5: Speech Grade" << endl  
		 << "TA: Itica Gupta, T 11:00 AM" << endl
		 << "Apr 4, 2016" << endl                
		 << "System: Sublime Text 2 on Ubuntu" << endl << endl
		 << "Presidential Announcement Speech reading grade levels:" << endl;
}//end header----------------------------------------------------------------------------------------


//checks to see if a letter in a word is a vowel or not
//takes in a character from a word
bool isVowel( char letter){
	if( 'a' == tolower( letter) || 'e' == tolower( letter) || 'i' == tolower( letter) || 
	    'o' == tolower( letter) || 'u' == tolower( letter) || 'y' == tolower( letter)){
		return true;
	} else
		return false;
}//end isVowel----------------------------------------------------------------------------------------


//based on certain conditions, counts the number of syllables in a word
//takes in speech struct, the current file index, and the current word
void countSyllables( Speech speech[], int fileNum, char* word){
	int syllsInWord = 0;		 //variable to keep track of the number of syllables in a word
	int EOW = strlen( word) - 1; //end of word

	for ( int i = 0; i <= EOW; i++){
		if( isVowel( word[ i])){
			speech[ fileNum].numSylls++;
			syllsInWord++;
		}

		if( isVowel( word[ i]) && isVowel( word[ i-1]) && i != EOW - 1){
			speech[ fileNum].numSylls--;
			syllsInWord--;
		}
	}


	if( isVowel( word[ EOW-2]) && !isVowel( word[ EOW-1]) && word[ EOW] == 'e'){
		speech[ fileNum].numSylls--;
	} else if( isVowel( word[ EOW-3]) && !isVowel( word[ EOW-2]) && word[ EOW-1] == 'e' && word[ EOW] == 's'){
		speech[ fileNum].numSylls--;
	} else if( isVowel( word[ EOW]) && syllsInWord > 1){
		speech[ fileNum].numSylls--;
	}
}//end countSyllables----------------------------------------------------------------------------------------


//retrieves the necessary values to calculate grade level
//takes in the speech struct as well as the current file indez
void getValues( Speech speech[], int fileNum){
	ifstream speechFile;										//input file stream
	const char* candidate = speech[ fileNum].candidateFile;		//variable used as shorthand
	string inputLine;											//stores a line of the file
	char* word;													//takes the current word
	char c;														//takes the current character
	bool sentenceMayEnd = false;								//checks if the sentence can end

	speechFile.open( candidate);
	assert( !speechFile.fail());

	//read the file line by line
	while( getline( speechFile, inputLine)){

		char* line = ( char*)malloc( sizeof(char*)*inputLine.length());
		strcpy( line, inputLine.c_str());

		word = strtok( line, " ,!;:.?");
		while( NULL != word){
			//increment number of words
			speech[ fileNum].numWords++;
			//count syllables
			countSyllables( speech, fileNum, word);
			//make word the next word
			word = strtok( NULL, " ,!;:.?-");
		}
	}
	speechFile.close();

	//re-open the file to check for sentece end
	speechFile.open( candidate);
	assert( !speechFile.fail());

	while( speechFile >> noskipws >> c){
		if( c == '!' || c == '.' || c == '?')
			sentenceMayEnd = true;
		if( sentenceMayEnd && ( c == 32 || c == 9 || c == 10 || c == 13 || c == 34)){
			sentenceMayEnd = false;
			speech[ fileNum].numSents++;
		}
	}
}//end getValues----------------------------------------------------------------------------------------


//calculate grade level ----> store
void calculateGrade( Speech speech[], int fileNum){
	speech[ fileNum].gradeLevel = (0.39 * speech[ fileNum].numWords / speech[ fileNum].numSents)
								  + (11.8 * speech[ fileNum].numSylls / speech[ fileNum].numWords)
								  - 15.59;
}//end calculateGrade----------------------------------------------------------------------------------------


//sort the structs based on grade level
void sort( Speech speech[]){
	Speech temp;

	for( int i = 1; i < 17; i++) {
		for( int j = 0; j < 16; j++) {
			if(speech[ j+1].gradeLevel > speech[ j].gradeLevel) {
				temp = speech[ j];
				speech[ j] = speech[ j + 1];
				speech[ j + 1] = temp;
			}
		}
	}
}//end sort----------------------------------------------------------------------------------------


//store the speeches to corresponding candidate files
void storeSpeeches( Speech speech[]){
	speech[ 0].candidateFile = "JebBush.txt";
	speech[ 1].candidateFile = "BenCarson.txt";
	speech[ 2].candidateFile = "ChrisChristie.txt";
	speech[ 3].candidateFile = "HillaryClinton.txt";
	speech[ 4].candidateFile = "TedCruz.txt";
	speech[ 5].candidateFile = "CarlyFiorina.txt";
	speech[ 6].candidateFile = "LindseyGraham.txt";
	speech[ 7].candidateFile = "MikeHuckabee.txt";
	speech[ 8].candidateFile = "JohnKasich.txt";
	speech[ 9].candidateFile = "RandPaul.txt";
	speech[ 10].candidateFile = "RickPerry.txt";
	speech[ 11].candidateFile = "MarcoRubio.txt";
	speech[ 12].candidateFile = "BernieSanders.txt";
	speech[ 13].candidateFile = "RickSantorum.txt";
	speech[ 14].candidateFile = "DonaldTrump.txt";
	speech[ 15].candidateFile = "ScottWalker.txt";
	speech[ 16].candidateFile = "JamesWebb.txt";
}//end storeSpeeches----------------------------------------------------------------------------------------


int main(){
	Speech speech[ 17];		//array of structs to store necessary info

	storeSpeeches( speech);
	
	header();

	for( int i = 0; i < 17; i++){
		speech[ i].numWords = -2; 	//-2 to account for the name at the top of the file
		speech[ i].numSylls = -3; 	//-3 to account for the name at the top of the file
		speech[ i].numSents = -2; 	//-2 to account for the name at the top of the file
		speech[ i].gradeLevel = 0; 	//initialize grade level
		getValues( speech, i);
		calculateGrade( speech, i);
	}

	sort( speech);

	for( int i = 0; i < 17; i++){
		cout << setw(20) << speech[ i].candidateFile << setw(10) << speech[ i].gradeLevel << endl;
	}

	return 0;
}