#include "rings.hpp"

using namespace std;

/* 二値空疎対称行列の読み込み */
sMatrix LoadNGPH(istream& is) {
  int n;
  /*NGPH形式を想定。1行目は節点の数*/
  is >> n;
  /*初期化*/
  sMatrix path = sMatrix(n,n);
  /*隣接関係の読みこみ。*/
  int i,j;
  while(is >> i >> j){
    if (i<0)
      break;
    /*無向グラフとしてあつかう。*/
    path.coeffRef(i,j) = 1;
    path.coeffRef(j,i) = 1;
    //cout << i << "," << j << endl;
  }
  cerr << "Read." << endl;
  return path;
}




int main(int argc, char *argv[])
{
  sMatrix m;
  
  string buf;
  while ( cin >> buf ){
    if ( (buf.length() >= 5) && (buf.substr(0,5) == "@NGPH")){
      m = LoadNGPH(cin);
      break;
    }
  }
  //sMatrix n(3,3);
  //n.insert(0,0) = 1;
  //n.insert(2,2) = 1;
  //cout << n.outerSize() << endl;
  int n = m.outerSize();
  sMatrix dm(n,n);
  int MaxDistance = 5; // assume the biggest ring to be an octagon. Internode distance in a ring should be less than 5.
  for(int i=0;i<n; i++){
    cerr << i << endl;
    vector<unsigned int> Dscore = Dijkstra(m, i, MaxDistance);
    for(int j=0;j<n;j++){
      if (Dscore[j] != MaxDistance){
	dm.insert(i,j) = Dscore[j];
      }
    }
  }
  cerr << "DM made." << endl;
  /*
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      int v = dm.coeff(i,j);
      cout << v;
    }
    cout << endl;
  }
  */
  sRings rings = countrings(m, dm, 8);
  cout << "@RNGS" << endl;
  cout << n << endl;
  showringsf(rings);
  cout << 0 << endl;
  /*
  for(sMatrix::iterator1 it1 = m.begin1(); it1 != m.end1(); ++it1) {
    for(sMatrix::iterator2 it1 = it1.begin(); it1 !=it1.end(); ++it1) {
      std::cout << "(" << it1.index1() << "," << it1.index2() << ") = " << *it1 << endl;
    }
    cout << endl;
  }
  */
}
