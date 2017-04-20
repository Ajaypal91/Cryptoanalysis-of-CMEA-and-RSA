// CryptoChallenge.cpp : Defines the entry point for the console application.
//

#ifndef HELPERMETHODS_H
#define HELPERMETHODS_H
#include "Header.h"
#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;



int main()
{	
	//Intialize various variables used throughtout phase 1 and 2
	init();
	//call phase 1. At the end phase 1 automatically calls phase 2
	startPrimaryPhase();
	return 0;
}

#endif 
