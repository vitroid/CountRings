#include "dijkstra2_eigen.hpp"

//modified from
//https://github.com/harshch1803/Algorithms/blob/master/Graph%20Algorithms/Dijkastra's%20Algorithm/Dijkstra_Heap.cpp

//Running Time of O(mlogn) using Priority Queue.


priority_queue<IntPair, vector<IntPair>, Compare> Heap;

std::vector<unsigned int>
Dijkstra(sMatrix& m, const int source, const int MaxDistance)								//src = Source Vertex.
{
  int n = m.outerSize();
  bool NodeExplored[n];								//To check if Node is Explored.
  std::vector<unsigned int> Dscore;						//To Store Dijkstra Score(D.S).
  for(int i=0;i<n;i++){
    Dscore.push_back(MaxDistance);							// Intialize all D.S to 1000000.
    NodeExplored[i] = false;							//Mark all Nodes Unexplored.
  }    
	
  Dscore[source] = 0;									//D.S of Source = 0.
  //NodeExplored[source] = true;							//Mark all Nodes Unexplored.
	
  Heap.push(make_pair(source,Dscore[source]));						//Reinsert Pair with Updated Key.
	
  while(!Heap.empty()){
    IntPair minIndex = Heap.top();							//Stores Pair with Min D.S.
    Heap.pop();									//Delete the Pair from The Heap.
		
    int cv = minIndex.first;							//Stores The Vertex with Min D.S.
												//cv = Current Vertx.
    if(NodeExplored[cv] == true)							//If Node Already Explored -> Skip.
      continue;
		
    NodeExplored[cv] = true;							//Else Mark The Node Explored.
		
    for (sMatrix::InnerIterator it(m, cv); it; ++it){
      //for(auto i = G[cv].begin(); i!= G[cv].end();++i){
      int dv = it.row();							// Points to Node Connected to Node 'cv'
										//dv = Destination Vertex.
															
      if(Dscore[cv]+1 < Dscore[dv]&& NodeExplored[dv]==false&& Dscore[cv]<MaxDistance){
										//Check if Node 'dv' is explored.
		 								//Check if Node 'cv' is Isolated. 
		 								//Check if ew + D.S(cv) < D.S(dv).
				
	Dscore[dv] = Dscore[cv] + 1;					//If Yes, Update D.S of Node 'dv'.
	Heap.push(make_pair(dv,Dscore[dv]));				//Reinsert The Pair with Updated Key.
      }
    }
  }
  return Dscore;
}
