#include <iostream>
#include <stdio.h>
using namespace std;

int testBoard( int size, char board[]){
	int index[ 101];
	//int count = 0;

	for(int i = 0; i < (size * size); i++){
		//index[ i] = i;

		cout << board[ i] << " ";
		
		int positionVal = i + 1;
		if( positionVal >= size && ( positionVal % size == 0))
			cout << endl;
	}
	cout << endl;

	char topLeft = board[ 0];
	char topRight = board[ size -1];
	char bottomLeft = board[ size * size - size];
	char bottomRight = board[ size * size - 1];

	/*for(int i = 0; i < (size * size); i += size + 1){
		cout << index[ i] << " ";
	}
	cout << endl;

	for(int i = 0; i < (size * size); i += size + 1){
		cout << board[ i] << " ";
		
		if( board[ 0] == board[ i]){
			count++;
		}
	}
	cout << endl;

	if(count == size)
		return 1;
	else
	*/	return 0;
}

int main(){
	int size;              // Size of one side of the board between 1 and 10
                           //    i.e. for a 3x3 board it would be 3.
    char board[ 101];      // Stores a user input line, plus the ending NULL character
    char c;                // Used to read the blank character separating size from other values
    
    printf("On the next line enter the dimension, a space, then the board char values to be tested \n");
    // Here are some examples of what the input could look like:
    //
    // 2 1010
    // 3 xoxoooxox
    // 4 abcdabcdabcdabcd
    // 5 argonxenonnenonrabidladen
    // 10 1010011011010011101011010100100110011001000110011001101110001000011110111001101010011001111100001001
    //
    cin >> size;                // Side of one size of the board
    c = getchar();              // Read the space separator between the size and the rest of the board
    cin.getline( board, 101);   // Characters representing the rest of the board
    
    // Test the board.
    if( testBoard( size, board) == 1) {
        printf("\n YES, Passed the test\n");
    }
    else {
        printf("\n No, did NOT pass test\n");
    }
    
    printf("\n");
    printf("Exiting program.\n");
    return 0;
}