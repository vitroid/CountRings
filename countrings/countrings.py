#!/usr/bin/env python3
#To confirm that countrings2 is working correctly.
#I doubted the bug in countrings2, but seems alright (See README).

#Ok, I confirmed that the results of 2 and 3 are the same.
#Use countrings2 because this is slower.

import heapq
import logging
from collections import defaultdict

        
#http://code.activestate.com/recipes/119466/
def shortest_path(G, start, end):

    q = [(0, start, [])]  # Heap of (cost, path_head, path_rest).
    visited = set()       # Visited vertices.
    while True:
        (cost, v1, path) = heapq.heappop(q)
        if v1 not in visited:
            visited.add(v1)
            if v1 == end:
                return path + [v1]
            path.append(v1)
            for v2 in G[v1]:
                if v2 not in visited:
                    heapq.heappush(q, (cost + 1, v2, path))


def readNGPH(file):
    line = file.readline()
    #print line,
    n = int(line)
    network = defaultdict(set)
    while True:
        line = file.readline()
        xyz = line.split()
        #print xyz
        i,j = map(int,xyz[:2])
        if i < 0:
            return (n,network)
        network[i].add(j)
        network[j].add(i)


def shortcuts( network, members ):
    n = len(members)
    for i in range(0,n):
        for j in range(i+1,n):
            d = min(j-i, n-(j-i))
            path = len(shortest_path(network, members[i],members[j]))-1
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
    for x in network.keys():
        neis = network[x]
        if neis != None:
            for y in neis:
                for z in neis:
                    if y < z:
                        members = [y,x,z]
                        (max, results) = findring( network, members, maxsize )
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
    edges = {"A": ("B","D"),
             "B": ("C","D","E"),
             "C": ("E",),
             "D": ("E","F"),
             "E": ("F","G"),
             "F": ("G",)}

    print(edges)
    print("A -> E:")
    print(shortest_path(edges, "A", "E"))
    print("F -> G:")
    print(shortest_path(edges, "F", "G"))
