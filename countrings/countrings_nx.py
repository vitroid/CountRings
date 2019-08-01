#!/usr/bin/env python3
#To confirm that countrings2 is working correctly.
#I doubted the bug in countrings2, but seems alright (See README).

#Ok, I confirmed that the results of 2 and 3 are the same.
#Use countrings2 because this is slower.

import logging
import networkx as nx
import itertools
from methodtools import lru_cache



def readNGPH(file):
    logger = logging.getLogger()
    line = file.readline()
    #print line,
    n = int(line)
    network = nx.Graph()   #undirected
    logger.debug("Uses NetworkX")
    while True:
        line = file.readline()
        xyz = line.split()
        #print xyz
        i,j = map(int,xyz[:2])
        if i < 0:
            return (n,network)
        network.add_edge(i,j)



# Make a class to cache the pair distance dict
class CountRings(nx.Graph):
    dist = dict()
    def __init__(self, network):
        super(CountRings, self).__init__(network)
        self.network = network

    #shortes_pathlen is a stateless function, so the cache is useful to avoid re-calculations.
    # Dubious memory leak
    @lru_cache(maxsize=None)
    def shortest_pathlen(self, pair):
        return len(nx.shortest_path(self.network, *pair)) - 1

        
    def shortcuts( self, members ):
        n = len(members)
        for i in range(0,n):
            for j in range(i+1,n):
                d = min(j-i, n-(j-i))
                if d > self.shortest_pathlen(frozenset((members[i],members[j]))):
                    return True
        return False


    def findring( self, members, max ):
        #print members, "MAX:", max
        if len(members) > max:
            return (max, [])
        s = set(members)
        last = members[-1]
        results = []
        for adj in self[last]:
            if adj in s:
                if adj == members[0]:
                    #Ring is closed.
                    #It is the best and unique answer.
                    if not self.shortcuts( members ):
                        return (len(members), [members])
                else:
                    #Shortcut ring
                    pass
            else:
                (newmax,newres) = self.findring( members + [adj], max )
                if newmax < max:
                    max = newmax
                    results = newres
                elif newmax == max:
                    results += newres
        return (max, results)


    def rings_iter( self, maxsize ):
        logger = logging.getLogger()
        rings = set()
        for x in self:
            neis = sorted(self[x])
            for y,z in itertools.combinations(neis, 2):
                triplet = [y,x,z]
                (max, results) = self.findring( triplet, maxsize )
                for i in results:
                    #Make i immutable for the key.
                    j = frozenset(i)
                    #and original list as the value.
                    if j not in rings:
                        # logger.debug("({0}) {1}".format(len(i),i))
                        yield i
                        rings.add(j)
        

def saveRNGS( nmol, ri ):  #ri is a rings_iter
    s = "@RNGS\n"
    s += "%d\n" % nmol
    for ring in ri:
        s+= "%s " % len(ring) + " ".join( map(str,ring) ) + "\n"
    s += "0\n"
    return s



if __name__ == "__main__":
    file = open("../samples/test.ngph")
    while True:
        line = file.readline()
        if 0 == line.find("@NGPH"):
            n, network = readNGPH(file)
            break
    print(saveRNGS( n, CountRings(network).rings_iter(8) ))

