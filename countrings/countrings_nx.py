#!/usr/bin/env python3
#To confirm that countrings2 is working correctly.
#I doubted the bug in countrings2, but seems alright (See README).

#Ok, I confirmed that the results of 2 and 3 are the same.
#Use countrings2 because this is slower.

import logging
import networkx as nx
import itertools

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


def shortcuts( network, members ):
    n = len(members)
    for i in range(0,n):
        for j in range(i+1,n):
            d = min(j-i, n-(j-i))
            path = len(nx.shortest_path(network, members[i],members[j]))-1
            if path < d:
                return True
    return False


def findring( network, members, max ):
    #print members, "MAX:", max
    if len(members) > max:
        return (max, [])
    s = set(members)
    last = members[-1]
    results = []
    for adj in network[last]:
        if adj in s:
            if adj == members[0]:
                #Ring is closed.
                #It is the best and unique answer.
                if not shortcuts( network, members ):
                    return (len(members), [members])
            else:
                #Shortcut ring
                pass
        else:
            (newmax,newres) = findring( network,members + [adj], max )
            if newmax < max:
                max = newmax
                results = newres
            elif newmax == max:
                results += newres
    return (max, results)


def rings_iter( network, maxsize ):
    logger = logging.getLogger()
    rings = set()
    for x in network:
        neis = sorted(network[x])
        for y,z in itertools.combinations(neis, 2):
            triplet = [y,x,z]
            (max, results) = findring( network, triplet, maxsize )
            for i in results:
                #Make i immutable for the key.
                j = frozenset(i)
                #and original list as the value.
                if j not in rings:
                    logger.debug("({0}) {1}".format(len(i),i))
                    yield i
                    rings.add(j)


def totalrings( network, maxsize ):
    logger = logging.getLogger()
    logger.info("totalrings() is outdated. Use rings_iter.")
    rings = dict()
    for ring in rings_iter( network, maxsize ):
        s = frozenset(ring)
        rings[s] = ring
    return rings
        

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
    print(saveRNGS( n, rings_iter(network, 8) ))

