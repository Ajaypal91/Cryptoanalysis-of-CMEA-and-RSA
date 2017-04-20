//#include "HelperMethods.cpp";
using namespace std;
#include <vector>
#include<unordered_map>

void init();
vector<unsigned char> getUniqueVector();
extern unsigned char C[256];
int** getAMatrix();
extern int A[256][256];
void startPrimaryPhase();
void printAMatrix(int **mat);
bool checkMat(int **mat);
unordered_map<unsigned char, vector<int>> getCInverse();
vector<pair<int, int>> getSortedRowWithCount(int **Mat);
void startPhaseTwo(vector<pair<unsigned char, int **>> TValues);
void startCMEA(unsigned char K[8]);
