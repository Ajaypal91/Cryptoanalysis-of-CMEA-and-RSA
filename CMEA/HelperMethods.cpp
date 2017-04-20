
#include <iostream>
#include <vector>
#include<algorithm>
#include <unordered_map>
#include <unordered_set>
using namespace std;

/*
///////////////////		DECLARING GLOBAL VARIABLE 	 ////////////////////////////
*/

// globally defined the cave table
unsigned char C[256] = {
	0xd9, 0x23, 0x5f, 0xe6, 0xca, 0x68, 0x97, 0xb0, 0x7b, 0xf2, 0x0c, 0x34, 0x11, 0xa5, 0x8d, 0x4e,
	0x0a, 0x46, 0x77, 0x8d, 0x10, 0x9f, 0x5e, 0x62, 0xf1, 0x34, 0xec, 0xa5, 0xc9, 0xb3, 0xd8, 0x2b,
	0x59, 0x47, 0xe3, 0xd2, 0xff, 0xae, 0x64, 0xca, 0x15, 0x8b, 0x7d, 0x38, 0x21, 0xbc, 0x96, 0x00,
	0x49, 0x56, 0x23, 0x15, 0x97, 0xe4, 0xcb, 0x6f, 0xf2, 0x70, 0x3c, 0x88, 0xba, 0xd1, 0x0d, 0xae,
	0xe2, 0x38, 0xba, 0x44, 0x9f, 0x83, 0x5d, 0x1c, 0xde, 0xab, 0xc7, 0x65, 0xf1, 0x76, 0x09, 0x20,
	0x86, 0xbd, 0x0a, 0xf1, 0x3c, 0xa7, 0x29, 0x93, 0xcb, 0x45, 0x5f, 0xe8, 0x10, 0x74, 0x62, 0xde,
	0xb8, 0x77, 0x80, 0xd1, 0x12, 0x26, 0xac, 0x6d, 0xe9, 0xcf, 0xf3, 0x54, 0x3a, 0x0b, 0x95, 0x4e,
	0xb1, 0x30, 0xa4, 0x96, 0xf8, 0x57, 0x49, 0x8e, 0x05, 0x1f, 0x62, 0x7c, 0xc3, 0x2b, 0xda, 0xed,
	0xbb, 0x86, 0x0d, 0x7a, 0x97, 0x13, 0x6c, 0x4e, 0x51, 0x30, 0xe5, 0xf2, 0x2f, 0xd8, 0xc4, 0xa9,
	0x91, 0x76, 0xf0, 0x17, 0x43, 0x38, 0x29, 0x84, 0xa2, 0xdb, 0xef, 0x65, 0x5e, 0xca, 0x0d, 0xbc,
	0xe7, 0xfa, 0xd8, 0x81, 0x6f, 0x00, 0x14, 0x42, 0x25, 0x7c, 0x5d, 0xc9, 0x9e, 0xb6, 0x33, 0xab,
	0x5a, 0x6f, 0x9b, 0xd9, 0xfe, 0x71, 0x44, 0xc5, 0x37, 0xa2, 0x88, 0x2d, 0x00, 0xb6, 0x13, 0xec,
	0x4e, 0x96, 0xa8, 0x5a, 0xb5, 0xd7, 0xc3, 0x8d, 0x3f, 0xf2, 0xec, 0x04, 0x60, 0x71, 0x1b, 0x29,
	0x04, 0x79, 0xe3, 0xc7, 0x1b, 0x66, 0x81, 0x4a, 0x25, 0x9d, 0xdc, 0x5f, 0x3e, 0xb0, 0xf8, 0xa2,
	0x91, 0x34, 0xf6, 0x5c, 0x67, 0x89, 0x73, 0x05, 0x22, 0xaa, 0xcb, 0xee, 0xbf, 0x18, 0xd0, 0x4d,
	0xf5, 0x36, 0xae, 0x01, 0x2f, 0x94, 0xc3, 0x49, 0x8b, 0xbd, 0x58, 0x12, 0xe0, 0x77, 0x6c, 0xda };

//inverse of cave table
unordered_map<unsigned char, vector<int>> CInverse;
// declaring global variable to store unique values of cave table
vector<unsigned char> unique_C;
// A Matrix associated with each T(0)
int **A = new int*[256];


//methods declaration
void inverseCaveTable();
void printCInverse();


//Initialize unique_C, and A[][]
void init(){
	//create unique set of values in cave table
	unordered_set<int> set_C(C, C + sizeof(C) / sizeof(C[0]));

	//add those values in the cave table
	unique_C = vector<unsigned char>(set_C.begin(), set_C.end());
	//sort vector
	sort(unique_C.begin(), unique_C.end());

	//intialize the A(i,j) table with 1
	for (int i = 0; i < 256; i++) {
		A[i] = new int[256];
		for (int j = 0; j < 256; j++) {
			A[i][j] = 1;
		}
	}

	//update the A(i,j) table with value of 0 for which T(i) - i is not in cave table 
	for (int i = 0; i < 256; i++){
		for (int j = 0; j < 256; j++){

			unsigned char t = j - i;
			if (set_C.find(t) == set_C.end())
			{
				A[i][j] = 0;
			}
		}
	}

	//calculate the inverse of Cave Table (used in Phase 2 (Secondary phase))	
	inverseCaveTable();
	
	//printCInverse();

}

//getter method to get the vector containing unique values of cave table
vector<unsigned char> getUniqueVector(){
	return unique_C;
}

//getter method to get the initialized A matrix for T(0) assumed
int** getAMatrix(){
	return A;
}

//print 100x100 matrix A
void printAMatrix(int **mat){
	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 100; j++){
			cout << " " << mat[i][j];
		}
		cout << endl;
	}
}

//check if any row of given matrix contains all 0. True if any row values are zero else false;
bool checkMat(int **mat){
	for(int i = 0; i<256;i++) {
		bool found = true;		
		for (int j = 0; j < 256; j++){
			if (mat[i][j] == 1){
				found = false;
				break;
			}	
		}
		if (found)
			return true;
	}
	return false;
}

//method to compute inverse of the cave table
void inverseCaveTable(){

	for (int i = 0; i < 256; i++){
		unsigned char val = C[i];
		if (CInverse.find(val) != CInverse.end()){
			vector<int> v = CInverse.at(val);
			v.push_back(i);
			CInverse[val] = v;
		}
		else {
			vector<int> v;
			v.push_back(i);
			CInverse[val] = v;
		}
	}

}

//print the CInverse hash map
void printCInverse(){
	int count = 0;
	for (auto hm = CInverse.begin(); hm != CInverse.end(); hm++){
		cout << "Key = " << std::hex << int(hm->first) << " indexes = ";
		vector<int> v = hm->second;
		count++;
		for (int i = 0; i < v.size(); i++){
			cout << std::dec << v.at(i) << " , ";
		}
		cout << endl;
	}
	cout << "Total inverse cavetable entries = " << count << endl;
}

//getter method for Inverse of Cave table
unordered_map<unsigned char, vector<int>> getCInverse(){
	return CInverse;
}

//operator to sort vector based on second argument
bool myFunction(pair<int, int> i, pair<int, int> j){
		return (i.second < j.second);
	}

/*this checks the count of each row in mat where value == 1, sorts them on the basis of count and return the vector of row number with 
its count */
vector<pair<int, int>> getSortedRowWithCount(int **Mat){
	
	vector<pair<int, int>> v;
	int count = 0;
	for (int i = 0; i < 256; i++){
		count = 0;
		for (int j = 0; j < 256; j++){
			if (Mat[i][j] == 1)
				count++;
		}
		v.push_back(make_pair(i, count));
	}

	sort(v.begin(), v.end(), myFunction);
	return v;
}
