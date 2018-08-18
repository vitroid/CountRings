#include "rings.hpp"


sRing
reordered(const sRing& ring)
{
  int imin = 0;
  int vmin = ring[0];
  int n = ring.size();
  for(int i=0;i<n; i++){
    if (ring[i] < vmin){
      imin = i;
      vmin = ring[i];
    }
  }
  sRing newring;
  int ip, is;
  ip = (imin - 1 + n) % n;
  is = (imin + 1) % n;
  int delta;
  if ( ring[ip] < ring[is] ){
    //reverse order
    delta = -1;
  }
  else{
    delta = +1;
  }
  int p = imin;
  for(int i=0;i<n;i++){
    newring.push_back(ring[p]);
    p = (p + delta + n)%n;
  }
  return newring;
}

// find shortcut path in the cycle
int
iscircular(const sRing& ring, int nnode, const sMatrix& dm)
{
  for(int i=0; i<nnode; i++){
    for(int j=i+1; j<nnode; j++){
      int d = min(j-i, i+nnode-j);
      if (dm.coeff(ring[i],ring[j]) < d)
	return 0;
    }
  }
  return 1;
}


void
showring(const sRing& ring)
{
  int size=ring.size();
  for(int i=0;i<ring.size();i++){
    cout << ring[i] << " ";
  }
  cout <<endl;
}

void
showringf(const sRing& ring)
{
  int size=ring.size();
  cout << size << " ";
  for(int i=0;i<ring.size();i++){
    cout << ring[i] << " ";
  }
  cout <<endl;
}


void append(sRings& rings, const sRing& ring)
{
  for(int i=0;i<rings.size(); i++){
    if (rings[i] == ring)
      return;
  }
  rings.push_back(ring);
}


void
showrings(const sRings& rings)
{
  for(int i=0;i<rings.size();i++){
    showring(reordered(rings[i]));
  }
}

void
showringsf(const sRings& rings)
{
  for(int i=0;i<rings.size();i++){
    showringf(reordered(rings[i]));
  }
}


sRings _findring(sMatrix& m, const sMatrix& dm, const sRing& ring, int maxsize)
{
  int size=ring.size();
  string indent;
  for(int i=0l;i<size-3;i++){
    indent += "   ";
  }
  if (size>maxsize){
    sRings rings;
    return rings; //empty vector
  }
  int found = false;
  sRings results;
  for(sMatrix::InnerIterator it3(m,ring[size-1]); it3; ++it3) {
    int p3 = it3.row();
    if (p3 == ring[0]){
      //loop closed
      if (iscircular(ring, size, dm)){
	sRings rings;
	append(rings,reordered(ring));
	return rings;
      }
    }
    //check shortcut
    int shortcut = 0;
    for(int i=1;i<size;i++){
      if (ring[i] == p3){
	shortcut = 1;
      }
    }
    if (!shortcut){
      sRing newring(ring);
      newring.push_back(p3);
      sRings newres = _findring(m, dm, newring, maxsize);
      if (newres.size() > 0){
	if (results.size() == 0){
	  results = newres;
	}
	else if (newres[0].size() < results[0].size()){
	  results = newres;
	}
	else if (newres[0].size() == results[0].size()){
	  for(int i=0;i<newres.size(); i++){
	    append(results, newres[i]);
	  }
	}
      }	
    }
  }
  return results;
}
		  
	      
  

sRings
countrings(sMatrix& m, const sMatrix& dm, int maxringsize)
{
  assert (maxringsize <= MAXRINGSIZE);
  sRings results;
  for(int k=0; k<m.outerSize(); ++k){ //outerSize == matrix size.
    for(sMatrix::InnerIterator it1(m,k); it1; ++it1) { //nei1
      int p0 = it1.col();
      int p1 = it1.row();
      for(sMatrix::InnerIterator it2(m,k); it2; ++it2) { //nei2
	int p2 = it2.row();
	if (p1 < p2){
	  sRing ring;
	  ring.push_back(p1);
	  ring.push_back(p0);
	  ring.push_back(p2);
	  sRings rings = _findring(m, dm, ring, maxringsize);
	  for(int i=0; i<rings.size(); i++){
	    append(results, rings[i]);
	  }
	}
      }
    }
  }
  return results;
}



