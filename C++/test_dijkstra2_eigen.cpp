#include "dijkstra2_eigen.hpp"

// driver program to test above function
int main()
{
  const int V=9;
  /* Let us create the example graph discussed above */
  int graph[V][V] = {{0, 4, 0, 0, 0, 0, 0, 8, 0},
		     {4, 0, 8, 0, 0, 0, 0, 11, 0},
		     {0, 8, 0, 7, 0, 4, 0, 0, 2},
		     {0, 0, 7, 0, 9, 14, 0, 0, 0},
		     {0, 0, 0, 9, 0, 10, 0, 0, 0},
		     {0, 0, 4, 14, 10, 0, 2, 0, 0},
		     {0, 0, 0, 0, 0, 2, 0, 1, 6},
		     {8, 11, 0, 0, 0, 0, 1, 0, 7},
		     {0, 0, 2, 0, 0, 0, 6, 7, 0}
  };
  sMatrix m(9,9);
  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      if(graph[i][j]){
	m.insert(i,j)=1;
      }
    }
  }
  std::vector<unsigned int> Dscore = Dijkstra(m, 0, 4+1);
  int n = Dscore.size();
  for(int j=1;j<n;j++){
    cout<<j<<" - "<<Dscore[j]<<endl;						//Print D.S of All Nodes.
  }
  
  return 0;
}
