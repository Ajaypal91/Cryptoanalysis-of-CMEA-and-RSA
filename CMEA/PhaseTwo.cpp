
#include <vector>
#include<iostream>

#include<unordered_set>
using namespace std;

#ifndef HEADER_H
#define HEADER_H
#include "Header.h"
#endif 

// find all possible pairs of (a, Ta)
vector<pair<int,int>> findTaTbTcTd(int **Mat){
	
	//sort the matrix Mat based on the number of 1 in each row of the matrix. 
	//This vector contains the row number and the correspondng count of 1 in it
	vector<pair<int, int>> sortedMat = getSortedRowWithCount(Mat);
	//declare the results vecotor which will contain all pairs of (a,T(a))
	vector<pair<int, int>> v;
	//find the row count of the row containing least number of 0. It will always be containing 1. which is row count of T(0).
	int firstRowCount = sortedMat.at(0).second;

	//only add values of rows if there count is lowest by comparing with first row count. 
	//in practice there may be rows which does not have count of 1 in it equal to 1. In such cases add other least row counts
	for (int i = 0; i < sortedMat.size(); i++){
		if (sortedMat.at(i).second == firstRowCount){
			int row = sortedMat.at(i).first;
			for (int j = 0; j < 256; j++){
				if (Mat[row][j] == 1){
					v.push_back(make_pair(row, j));
				}
			}

		}
		else {
			break;
		}
	}
	
	return v;
}

//calculate x' = (C[(x^K0]) + K1] + x ) ^ K2
unsigned char getADashValues(unsigned char K[3], unsigned char a){
	unsigned char aa;
	
	unsigned char x = (a ^ K[0]) % 256;
	x = (x + K[1]) % 256;
	unsigned char p = C[int(x)];
	p = (p + a) % 256;
	aa = (p ^ K[2]) % 256;
	return aa;
}

//calculate the values of X(a) such that T(a) = C[(X(a) ^ K0) + K1] + a
vector<unsigned char> getXa(unsigned char K[2], unsigned char a, unsigned char Ta){
	
	//get the C inverse table
	unordered_map<unsigned char, vector<int>> CInv = getCInverse();
	vector<unsigned char> results;
	unsigned char LHS = (Ta - a) % 256;
	// if LHS is present as key in the map
	if (CInv.find(LHS) != CInv.end()){
		//find all indexes where this value is present
		vector<int> indexes = CInv.at(LHS);

		for (int i = 0; i < indexes.size(); i++){
			
			unsigned char ind = indexes.at(i);
			unsigned char x = (ind - K[1]) % 256;
			x = (x ^ K[0]) % 256;
			results.push_back(x);
		}

	}
	return results;
}


vector<unsigned char> findAllDoubleDashValues(vector<unsigned char> Sa, unsigned char a, unsigned char tempK[2]){
	
	vector<unsigned char> Ra, aadash;
	unordered_set<unsigned char> s;
	//get the C inverse table
	unordered_map<unsigned char, vector<int>> CInv = getCInverse();

	for (int i = 0; i < Sa.size(); i++){

		unsigned char t = Sa.at(i);
		Ra = getXa(tempK, a, t);
		//for every S(a), R(a) pair
		for (int j = 0; j < Ra.size(); j++){
			unsigned char rr = (Ra.at(j) - a) % 256;
			if (CInv.find(rr) != CInv.end()){
				vector<int> t = CInv.at(rr);
				for (int k = 0; k < t.size(); k++){
					s.insert(t.at(k));
				}
			}
		}
	}
	aadash = vector<unsigned char>(s.begin(), s.end());
	return aadash;
}

//Main method that performs the secondary phase and find all putative keys and checks them.
void findKey(vector<pair<int, int>> TPairs, vector<pair<unsigned char, unsigned char>> k6k7, vector<pair<unsigned char, unsigned char>> k4k5){

	cout << endl << "Trying to find Key" << endl;

	//K0 K1 and K2 of the key
	unsigned char K[3];

	//lookup table M key = (a' -d'),(b'-d'),(c'-d') and values are (a'b'c'd',Key)
	unordered_map<unsigned int, vector<pair<unsigned int, int>>> M;
	//get the C inverse table
	unordered_map<unsigned char, vector<int>> CInv = getCInverse();
	
	unsigned char aa, bb, cc, dd;
	unsigned char a, b, c, d;
	unsigned char Ta, Tb, Tc, Td;
	unsigned char ad, bd, cd;
	unsigned int abcd, adbdcd;
	vector<pair<unsigned int, int>> temp;

	a = TPairs.at(0).first; Ta = TPairs.at(0).second;
	b = TPairs.at(1).first; Tb = TPairs.at(1).second;
	c = TPairs.at(2).first; Tc = TPairs.at(2).second;
	d = TPairs.at(3).first; Td = TPairs.at(3).second;

	//start part 1
	//for keys 0 to 2^24 construct a table M
	for (int tempKey = 0; tempKey <= 0xffffff; tempKey++){

		//get the three bytes from the key
		K[2] = (tempKey & 0xff) %256;
		K[1] = ((tempKey >> 8) & 0xff) % 256;
		K[0] = ((tempKey >> 16) & 0xff) % 256;
		
		// find a', b', c' and d'
		aa = getADashValues(K, a);
		bb = getADashValues(K, b);
		cc = getADashValues(K, c);
		dd = getADashValues(K, d);
		abcd = dd;
		abcd = (cc << 8) ^ abcd;
		abcd = (bb << 16) ^ abcd;
		abcd = (aa << 24) ^ abcd;

		//calculate (a'-d'), (b'-d'), (c'-d')
		ad = (aa - dd) % 256;
		bd = (bb - dd) % 256;
		cd = (cc - dd) % 256;
		adbdcd = cd;
		adbdcd = (bd << 8) ^ adbdcd;
		adbdcd = (ad << 16) ^ adbdcd;

		//add entry to table indexed with (a'-d'), (b'-d'), (c'-d') and rows with a'b'c'd' and key 
		temp.clear();
		if (M.find(adbdcd) == M.end()){
			temp.push_back(make_pair(abcd, tempKey));
			M.insert({ adbdcd, temp });
		}
		else{
			temp = M.at(adbdcd);
			temp.push_back(make_pair(abcd, tempKey));
			M.insert({ adbdcd, temp });
		}
	}

	cout << "M Matrix build. Proceeding to part 2 " << endl;

	//start part 2
	unsigned char KK[4] , tempK[2], fullKey[8];
	vector<unsigned char> Sa, Sb, Sc, Sd, Ra, Rb, Rc, Rd;
	vector<unsigned char> aadash, bbdash, ccdash, dddash;
	
	unsigned long long int count = 0;
	//for every k4k5 pairs found
	for (int ind = 0; ind < k4k5.size(); ind++){
		unsigned char l = k4k5.at(ind).first;
		unsigned char ll = k4k5.at(ind).second;
		for (int p = 0; p < k6k7.size(); p++){

			//get the three bytes from the key K4 = KK0, K5 = KK1, K6 = KK2, K7 = KK3
			KK[0] = l;
			KK[1] = ll;
			KK[2] = k6k7.at(p).first;
			KK[3] = k6k7.at(p).second;

			//store K6 and K7 in two keys to find S(a)
			tempK[0] = KK[2];
			tempK[1] = KK[3];
			//get S(a), S(b), S(c), S(d)
			Sa = getXa(tempK, a, Ta);
			Sb = getXa(tempK, b, Tb);
			Sc = getXa(tempK, c, Tc);
			Sd = getXa(tempK, d, Td);

			//store key K4 and K5
			tempK[0] = KK[0];
			tempK[1] = KK[1];

			//for every S(a), S(b), S(c), S(d) find all a'',b'',c'',d''
			aadash = findAllDoubleDashValues(Sa, a, tempK);
			bbdash = findAllDoubleDashValues(Sb, b, tempK);
			ccdash = findAllDoubleDashValues(Sc, c, tempK);
			dddash = findAllDoubleDashValues(Sd, d, tempK);


			for (int i = 0; i < aadash.size(); i++){
				for (int j = 0; j < bbdash.size(); j++){
					for (int m = 0; m < ccdash.size(); m++){
						for (int n = 0; n < dddash.size(); n++){

							//cout << "checking for lookup in M" << endl;

							unsigned char t1 = aadash.at(i);
							unsigned char t2 = bbdash.at(j);
							unsigned char t3 = ccdash.at(m);
							unsigned char t4 = dddash.at(n);

							//calculate (a''-d''),(b''-d''),(c''-d'')
							unsigned int tableKey;
							unsigned char aadddash, bbdddash, ccdddash;
							aadddash = (t1 - t4) % 256;
							bbdddash = (t2 - t4) % 256;
							ccdddash = (t3 - t4) % 256;
							tableKey = ccdddash;
							tableKey = (bbdddash << 8) ^ tableKey;
							tableKey = (aadddash << 16) ^ tableKey;

							//if this key is in table
							if (M.find(tableKey) != M.end()){
								vector<pair<unsigned int, int>> v = M.at(tableKey);
								for (int l = 0; l < v.size(); l++){
									unsigned int abcd_Dash = v.at(l).first;
									unsigned char aDash = (abcd_Dash >> 24) % 256;
									unsigned char aDoubleDash = t1;
									unsigned char K3 = (aDoubleDash - aDash) % 256;

									int firstKey = v.at(l).second;
									unsigned char K2 = (firstKey & 0xff) % 256;
									unsigned char K1 = ((firstKey >> 8) & 0xff) % 256;
									unsigned char K0 = ((firstKey >> 16) & 0xff) % 256;
									//form a valid putative full key									
									fullKey[0] = K0; fullKey[1] = K1; fullKey[2] = K2; fullKey[3] = K3;
									fullKey[4] = KK[0]; fullKey[5] = KK[1]; fullKey[6] = KK[2]; fullKey[7] = KK[3];
									count++;
									//check if the putative key is correct or not			
									startCMEA(fullKey);									
									//for debugging purposes
									if(count%10000 == 0)
										cout << "Valid Keys tested = " << std::dec << count << endl;
									
									
								}
							}

						}
					}
				}


			}
		}

	}

	cout << "Part 2 finished " << endl;
	cout << "Total Putative Keys tested " << std::dec<< count<< endl;

}
//method that finds possible k6k7 pairs using backtracking
vector<pair<unsigned char, unsigned char>> findK6K7(int **Mat){

	bool found = false;
	bool broke = true;
	vector<pair<unsigned char, unsigned char>> keys;
	vector<unsigned char> unique_C = getUniqueVector();

	//for k6
	for (int i = 0; i < 256; i++){
		//for k7
		for (int j = 0; j < 256; j++){
			broke = false;
			
			//for each row
			for (int x = 0; x < 256; x++){
				//for each cave table entry
				found = false;
				for (int c = 0; c < unique_C.size(); c++){
					
					unsigned char o = unique_C.at(c);
					unsigned char t = (o + x) % 256;
					unsigned char y = (C[(((t)^i) + j) % 256] + x)%256;

					if (Mat[x][y] == 1){
						found = true;
						break;
					}

				}

				if (!found){
					broke = true;
					break;
				}
			}
			if (!broke){
				keys.push_back(make_pair(i, j));
			}
		}

	}
	//cout << "K6K7 keys found = " << std::dec << keys.size() <<endl;

	return keys;
}


/* This method finds out all pairs of K4k5k6k7 values that are compatible with each other. But not using it as it did not helped solving CMEA 2
vector<pair<pair<unsigned char, unsigned char>,pair<unsigned char, unsigned char>>> findK4K5(int **Mat, vector<pair<unsigned char, unsigned char>> k6k7){

	bool found = false;
	bool broke = true;
	vector<pair<pair<unsigned char, unsigned char>,pair<unsigned char, unsigned char>>> keys;
	vector<unsigned char> unique_C = getUniqueVector();

	//for k4
	for (int i = 0; i < 256; i++){
		//for k5
		for (int j = 0; j < 256; j++){
			
			
			for (int ind = 0; ind < k6k7.size(); ind++) {
				broke = false;
				for (int x = 0; x < 256; x++){					
					found = false;
					for (int c = 0; c < unique_C.size(); c++){
						//for each row
						unsigned char o = unique_C.at(c);
								
						unsigned char t = (o + x) % 256;
						unsigned char y = (C[(((t)^i) + j) % 256] + x)%256;
								
						unsigned char k6 = k6k7.at(ind).first;	
						unsigned char k7 = k6k7.at(ind).second;
										
						unsigned char Tx = C[ ((y^k6)+k7)%256] + x;						
						if (Mat[x][Tx] == 1){
							found = true;
							break;
						}
					}
					if (!found) {
						broke = true;	break;
					}
				}
				if (!broke){
					pair<unsigned char, unsigned char> k4k5pair = make_pair(i,j);
					keys.push_back(make_pair(k4k5pair,k6k7.at(ind)));			
				}


				
	
			}

				
			
		}

	}
	//cout << "K6K7 keys found = " << std::dec << keys.size() <<endl;

	return keys;
}*/

//method that finds possible k4k5 pairs using backtracking
vector<pair<unsigned char, unsigned char>> findK4K5(int **Mat, vector<pair<unsigned char, unsigned char>> k6k7){

	bool found = false;
	bool broke = true;
	vector<pair<unsigned char, unsigned char>> keys;
	vector<unsigned char> unique_C = getUniqueVector();

	//for k4
	for (int i = 0; i < 256; i++){
		//for k5
		for (int j = 0; j < 256; j++){
			broke = false;
			

			//for each row
			for (int x = 0; x < 256; x++){
				//for each cave table entry
				found = false;				
				for (int c = 0; c < unique_C.size(); c++){
					
					unsigned char o = unique_C.at(c);
					unsigned char t = (o + x) % 256;
					unsigned char y = (C[(((t)^i) + j) % 256] + x)%256;

					for (int ind = 0; ind < k6k7.size(); ind++) {				
						unsigned char k6 = k6k7.at(ind).first;	
						unsigned char k7 = k6k7.at(ind).second;
												
						unsigned char Tx = C[ ((y^k6)+k7)%256] + x;						
						if (Mat[x][Tx] == 1){
							found = true;
							break;
						}

					}
					if (found){
						break;
					}

				}

				if (!found){
					broke = true;
					break;
				}
			}
			if (!broke){
				keys.push_back(make_pair(i, j));
			}
		}

	}
	//cout << "K6K7 keys found = " << std::dec << keys.size() <<endl;

	return keys;
}


//Start of phase two Arrgument contains the vector of Tzero along with its A Matrix
void startPhaseTwo(vector<pair<unsigned char, int **>> TValues){
	
	//vector to store 4 pairs of (a, T(a)) values
	vector<pair<int, int>> pairsOfFour;

	for (int i = 0; i < TValues.size(); i++){
		
		//get all pairs possible		
		vector<pair<int, int>> Tpairs;
		Tpairs = findTaTbTcTd(TValues.at(i).second);
		
		// vectors to store k4k5 and k6k6 values found by backtracking
		vector<pair<unsigned char, unsigned char>> k6k7, k4k5;
	
		//vector<pair<pair<unsigned char, unsigned char>,pair<unsigned char, unsigned char>>> k4k5k6k7;
		
		//// only if it contains atleast 4 unique rows. 
		if (Tpairs.size() > 4){
			//cout << "Checking for 4 pairs of Ta, a" << endl;
			//find all permuatations of size 4 from the Total TPairs
			pairsOfFour.push_back(Tpairs.at(0));
			pairsOfFour.push_back(Tpairs.at(1));
			pairsOfFour.push_back(Tpairs.at(2));
			pairsOfFour.push_back(Tpairs.at(3));
			//find k6k7 from backtracking. TValues.at(i).second contains the A Matrix
			k6k7 = findK6K7(TValues.at(i).second);

			//find k4k5 from k6k7 using backtracking. TValues.at(i).second contains the A Matrix
			k4k5 = findK4K5(TValues.at(i).second, k6k7);

			cout<<std::hex<<"T0: "<<(int)TValues.at(i).first<<endl;
			cout<<std::dec<<"K6K7 Size: "<<k6k7.size()<<endl;
			cout<<std::dec<<"k4k5 Size: "<<k4k5.size()<<endl << endl;

			//find the key.
			findKey(pairsOfFour, k6k7, k4k5);
			
		}
		else{
		cout << "T(0) 0x" << int(TValues.at(i).first) <<" does not have unique rows " << endl;
		k6k7 = findK6K7(TValues.at(i).second);
		k4k5 = findK4K5(TValues.at(i).second, k6k7);
		cout<<std::dec<<"K6K7 Size: "<<k6k7.size()<<endl;
		cout<<std::dec<<"K4K5 Size: "<<k4k5.size()<<endl << endl;
		}
	
		// comment out the following code to print number of 0's in the A Matrix
		int **M = TValues.at(i).second;

		/*int count = 0;
		for (int m = 0; m < 256; m++){
			for (int n = 0; n < 256; n++){
				if (M[m][n] == 0)
					count++;
			}
		}
		cout << std::dec<< "Count of 0 = " << int(count) << endl;
		*/
		


	}
	

}
