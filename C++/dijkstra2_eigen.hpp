#ifndef _DIJKSTRA2_EIGEN
#define _DIJKSTRA2_EIGEN
//modified from
//https://github.com/harshch1803/Algorithms/blob/master/Graph%20Algorithms/Dijkastra's%20Algorithm/Dijkstra_Heap.cpp

//Running Time of O(mlogn) using Priority Queue.

#include<iostream>
#include<queue>
#include<vector>
#include<list>
#include<fstream>
#include<sstream>
#include <Eigen/Sparse>

using namespace std;
using namespace Eigen;

typedef SparseMatrix<int> sMatrix;

typedef std::pair <int, unsigned long long int> IntPair;					//Pair <Node,Dijkstra Score>

struct Compare											//To Compare The Dijkstra Score(D.S)
{												//Of Two Nodes.
	public :bool operator ()(IntPair &p1 ,IntPair &p2){return p1.second >= p2.second;}
};




std::vector<unsigned int>
Dijkstra(sMatrix& m, const int source, const int MaxDistance);

#endif

