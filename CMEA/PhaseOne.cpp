
#include <fstream>
#include <iostream>
#include<string>

using namespace std;

#ifndef HEADER_H
#define HEADER_H
#include "Header.h"
#endif 

// CMEA block size
/* 			ADJUST THE FOLLOWING PARAMETERS ACCORDING TO THE PLAINTEXT, CIPHERTEXT FILES AND BLOCK LENGTH OF CIPHER			 */
#define BLOCK_LENGTH 3
#define NUMBEROFBLOCKS 100

//file names
char cipherFileName[30] = "cmea1Cipher.txt";
char plainTextFileName[30] = "cmea1InitPlain.txt";

//for storing the cipher and plain texts
char *plaintext;
char *ciphertext;

// size of bytes read from files
size_t size;

//methods declaration
bool calc1(int **AMat, unsigned char TZero, unsigned char guess, unsigned char P[BLOCK_LENGTH], unsigned char C[BLOCK_LENGTH]);
bool calc2(int **AMat, unsigned char TZero, unsigned char guess, unsigned char P[BLOCK_LENGTH], unsigned char C[BLOCK_LENGTH]);

//function to read plaintext and ciphertext files
void _readCipherAndPlainText(){
	ifstream plaintextFile(plainTextFileName, ios::in | ios::binary | ios::ate);
	ifstream ciphertextFile(cipherFileName, ios::in | ios::binary);

	if (!plaintextFile.is_open() || !ciphertextFile.is_open())
	{
		cout << "Files could not be opened";
		exit(1);
	}
	//get size of plaintext file
	size = plaintextFile.tellg();
	// allocate size to plaintext and cipher text
	plaintext = (char*)malloc(sizeof(char)*size);
	ciphertext = (char*)malloc(sizeof(char)*size);
	//start from starting of plaintext and read only bytes equal to size in plain and cipher text
	plaintextFile.seekg(0, ios::beg);
	plaintextFile.read(plaintext, size);
	ciphertextFile.read(ciphertext, size);
}

//method that returns the TZero assumed
unsigned char getAssumedTZero(int index){
	vector<unsigned char> v = getUniqueVector();
	return v.at(index);
}

//for given plaintext and ciphertext block update the A(i,j) Matrix if the assumed value of T is invalid
//return false if assumption was wrong else true
//This method checks the three main equations that helps in updating the A Matrix
bool updateAMatrixForAssumedTValue(int **A, unsigned char TZero, unsigned char assumedValueOfT, unsigned char P[BLOCK_LENGTH], unsigned char C[BLOCK_LENGTH]){
	unsigned char row1, row2;
	unsigned char col1, col2, col3, col4;
	//check equation 1 T( (C[0] + T(0))^1) = P[1] + T( (P[0] + T(0))^1) - C[1]
	unsigned char z1 = (C[0] + TZero) % 256;
	unsigned char y1 = (P[0] + TZero) % 256;
	unsigned char pp1 = (P[1] + assumedValueOfT) % 256;
	row1 = ((z1) ^ 1) % 256;
	col1 = (P[1] + assumedValueOfT) % 256;
	col1 = (col1 - C[1]) % 256;
	if (A[int(row1)][int(col1)] == 0)
		return false;

	////checking the other two pair of equations
	unsigned char l = (y1 ^ z1) % 256;
	unsigned char ll = (l ^ 1) % 256;
	row1 = (y1 + pp1) % 256;
	row1 = (row1 ^ 2) % 256;
	row2 = (z1 + pp1) % 256;
	row2 = (row2 ^ 2) % 256;

	col1 = (l - P[2]) % 256;
	col3 = (l - C[2]) % 256;

	col2 = (ll - P[2]) % 256;
	col4 = (ll - C[2]) % 256;

	if ((A[int(row1)][int(col1)] == 0 && A[int(row1)][int(col2)] == 0) || (A[int(row2)][int(col3)] == 0 && A[int(row2)][int(col4)] == 0))
		return false;

	return true;
}


//method that updates the A Matrix based for the assumed TZero Value and return status for TZero. It runs over all PT and CT pairs
//if status returned is false then assumed TZero is invalid else it is valid TZero
bool updateAMatrix(int **AMat, unsigned char TZero){
	bool status = false;
	vector<unsigned char> CVector = getUniqueVector();
	unsigned char P[BLOCK_LENGTH]; unsigned char C[BLOCK_LENGTH];
	int k = 0;
	bool flag = false;
	int y1, z1;
	for (int blockNumber = 0; blockNumber < NUMBEROFBLOCKS; blockNumber++){
		//read three blocks of cipher and plaintext pairs
		k = 0;
		for (int i = BLOCK_LENGTH*blockNumber; i < (BLOCK_LENGTH*blockNumber + BLOCK_LENGTH); i++){
			P[k] = plaintext[i]; C[k] = ciphertext[i]; k++;
		}
		//calculate the row for assumed T((P0 + T(0))^1) and find the assumed T value 
		y1 = (P[0] + TZero) % 256;
		z1 = (C[0] + TZero) % 256;
		
		
		y1 = (y1 ^ 1) % 256;
		z1 = (z1 ^ 1) % 256;
		//for given plaintext and cipher text pair check for all values in the row for y1
		flag = false; //this is used to keep track if Mat A is update at any time. If yes then you need to restart the whole process again
		try {
			for (int i = 0; i < 256; i++){
				//assumed T(i) = Cave table + i
				unsigned char guess = (unsigned char)i;

				// if the entry in A matrix is 1
				if (AMat[y1][guess] == 1) {
					//check for main 3 equations
					status = updateAMatrixForAssumedTValue(AMat, TZero, guess, P, C);
					//if the assumed value is impossible
					if (!status){
						// the assumed value of T was wrong and fetch the next assumed value and check its validity
						AMat[y1][guess] = 0;
						flag = true;

					}
				}


				// if the entry in A matrix is 1
				if (AMat[z1][guess] == 1) {
					//check for the 4 the equation which is similar to equation 1
					status = calc1(AMat, TZero, guess, P, C);
					//if the assumed value is impossible
					if (!status){
						// the assumed value of T was wrong and fetch the next assumed value and check its validity
						AMat[z1][guess] = 0;
						flag = true;

					}
				}
				
				// if the entry in A matrix is 1
				if (AMat[z1][guess] == 1) {
					// check for equation 5. 
					status = calc2(AMat,TZero, guess,P, C);
					if (!status){
						// the assumed value of T was wrong and fetch the next assumed value and check its validity
						AMat[z1][guess] = 0;
						flag = true;

					}
				}

				

			}
			
			//check for all Ciphertext and plaintext pairs again
			
		}
		catch (exception ex){
			cout << ex.what();
		}
	}

	//if there is any update in the A Matrix repeat the whole procedure again
	if (flag)
			updateAMatrix(AMat, TZero);

	//check if the AMatrix has any row full of 0's
	if (checkMat(AMat))
		return false;


	// Since CMEA's Plaintext and Ciphertext are reverse of each other, so update A Matrix also with Plaintext and Ciphertext blocks interchanged
	flag = false;
	for (int blockNumber = 0; blockNumber < NUMBEROFBLOCKS; blockNumber++){
		//read three blocks of cipher and plaintext pairs
		k = 0;
		for (int i = BLOCK_LENGTH*blockNumber; i < (BLOCK_LENGTH*blockNumber + BLOCK_LENGTH); i++){
			P[k] = ciphertext[i]; C[k] = plaintext[i]; k++;
		}
		//calculate the row for assumed T((P0 + T(0))^1) and find the assumed T value 
		y1 = (P[0] + TZero) % 256;
		z1 = (C[0] + TZero) % 256;
		
		y1 = (y1 ^ 1) % 256;
		z1 = (z1 ^ 1) % 256;

		//for given plaintext and cipher text pair check for all values in the row for y1
		flag = false;
		try {
			for (int i = 0; i < 256; i++){
				//assumed T(i) = Cave table + i
				unsigned char guess = (unsigned char)i;

				// if the entry in A matrix is 1
				if (AMat[y1][guess] == 1) {
					status = updateAMatrixForAssumedTValue(AMat, TZero, guess, P, C);
					//if the assumed value is impossible
					if (!status){
						// the assumed value of T was wrong and fetch the next assumed value and check its validity
						AMat[y1][guess] = 0;
						flag = true;

					}
				}

				// if the entry in A matrix is 1
				if (AMat[z1][guess] == 1) {
					status = calc1(AMat, TZero, guess, P, C);
					//if the assumed value is impossible
					if (!status){
						// the assumed value of T was wrong and fetch the next assumed value and check its validity
						AMat[z1][guess] = 0;
						flag = true;

					}
				}

				// if the entry in A matrix is 1
				if (AMat[z1][guess] == 1) {
					// check for equation 5. 
					status = calc2(AMat,TZero, guess,P, C);
					if (!status){
						// the assumed value of T was wrong and fetch the next assumed value and check its validity
						AMat[z1][guess] = 0;
						flag = true;

					}
				}

			}
			
			
		}
		catch (exception ex){
			cout << ex.what();
		}
	}
	//if there is any update in the A Matrix repeat the whole procedure again
	if (flag)
				updateAMatrix(AMat, TZero);
	//method to check if there is any row which is completely 0 in A matrix
	if (checkMat(AMat))
		return false;

	return true;
}

//equation number 4 (which is same as equation no 1). Just here we are assuming T((C0 + T0)^1)
bool calc1(int **AMat, unsigned char TZero, unsigned char guess, unsigned char P[BLOCK_LENGTH], unsigned char C[BLOCK_LENGTH]){

	guess = (guess+C[1])%256;		
	unsigned char col = (guess - P[1])%256;
	unsigned char row = (((P[0] + TZero)%256) ^1);
	return bool(AMat[row][col]);
	

}

//equation number 5 --> C[2] + T( (C[0] + T(0)) + C[1] + T( (C[0]+T[0])^1 ) ) ^2 ) = P2' where P2' have 2 values
//Equations 4 and 5 does not really help in introducing new 0's in the A matrix because all possible 0's are already introduced by the main 3 equations
bool calc2(int **AMat, unsigned char TZero, unsigned char guess, unsigned char P[BLOCK_LENGTH], unsigned char C[BLOCK_LENGTH]){

	unsigned char z1 = (C[0] + TZero) % 256;
	unsigned char y1 = (P[0] + TZero) % 256;
	
	
	////check two pair of equations
	unsigned char l = (y1 ^ z1) % 256;
	unsigned char ll = (l ^ 1) % 256;
	unsigned char row1 = (z1 + C[1]) % 256;
	row1 = (row1 + guess)%256;
	row1 = (row1 ^ 2) %256;

	unsigned char col1 = (l - C[2]) % 256;
	unsigned char col2 = (ll - C[2]) % 256;

	if (AMat[int(row1)][int(col1)] == 0 && AMat[int(row1)][int(col2)] == 0)
		return false;

	return true;	


}



//starting of intialization phase
void startPrimaryPhase(){

	//read the first 120 bytes (40 blocks) of cipher and plaintext 
	_readCipherAndPlainText();

	// count of TZero found
	int count = 0;

	//get initialized A(i,j) Matrix
	int **temp = getAMatrix();

	//this vector contains all the valid T(0)'s found after phase 1 and there corresponding A Matrix
	vector<pair<unsigned char, int **>> validTZeros;
	//vector containing all unique values of cave table
	vector<unsigned char> v = getUniqueVector();

	//get assumed T(0)
	for (int indexOfTZeroAssumed = 0; indexOfTZeroAssumed < 164; indexOfTZeroAssumed++){

		unsigned char assumedTZero = v.at(indexOfTZeroAssumed);

		int **AMatrix = new int*[256];
		//cout << "Checking for T(0) = " << std::hex << int(assumedTZero) << endl;
		
		//create a copy of A(i,j) Matrix
		for (int i = 0; i < 256; i++){
			AMatrix[i] = new int[256];
			for (int j = 0; j < 256; j++)
				AMatrix[i][j] = temp[i][j];
		}
		//set the row of assumed
		for (int i = 0; i< 256; i++) {
			if (i != int(assumedTZero))
				AMatrix[0][i] = 0;
		}

		
		// if only valid TZero found, then add them to vector
		if (updateAMatrix(AMatrix, assumedTZero)) {
			cout << "T(0) found " << std::hex << int(assumedTZero) << endl;
			count++;
			pair<unsigned char, int **> p = make_pair(assumedTZero, AMatrix);
			validTZeros.push_back(p);
		}
		
	}

	cout << "Number of TZeros found = " << std::dec << count << endl;
	//call phase 2 with valid values of TZero and their A Matrix
	startPhaseTwo(validTZeros);

}
