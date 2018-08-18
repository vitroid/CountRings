#include "rings.hpp"

using namespace std;

/* 二値空疎対称行列の読み込み */
sMatrix LoadNGPH(istream& is) {
  int n;
  /*NGPH形式を想定。1行目は節点の数*/
  is >> n;
  /*初期化*/
  sMatrix path = sMatrix(n,n);
  path.reserve(VectorXi::Constant(n,8));
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
  sMatrix dm = DistanceMatrix(m, 5);
  //showmat(dm);
  /*
  sRings rings = countrings(m, dm, 8);
  cout << "@RNGS" << endl;
  cout << n << endl;
  showringsf(rings);
  cout << 0 << endl;
  
  for(sMatrix::iterator1 it1 = m.begin1(); it1 != m.end1(); ++it1) {
    for(sMatrix::iterator2 it1 = it1.begin(); it1 !=it1.end(); ++it1) {
      std::cout << "(" << it1.index1() << "," << it1.index2() << ") = " << *it1 << endl;
    }
    cout << endl;
  }
  */
}
