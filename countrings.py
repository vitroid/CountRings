#!/usr/bin/env python
#To confirm that countrings2 is working correctly.
#I doubted the bug in countrings2, but seems alright (See README).

#Ok, I confirmed that the results of 2 and 3 are the same.
#Use countrings2 because this is slower.

from __future__ import print_function
import sys
import heapq

def flatten(L):       # Flatten linked list of form [0,[1,[2,[]]]]
	while len(L) > 0:
		yield L[0]
		L = L[1]

        
#http://code.activestate.com/recipes/119466/
def shortest_path(G, start, end):

    q = [(0, start, ())]  # Heap of (cost, path_head, path_rest).
    visited = set()       # Visited vertices.
    while True:
        (cost, v1, path) = heapq.heappop(q)
        if v1 not in visited:
            visited.add(v1)
            if v1 == end:
                return list(flatten(path))[::-1] + [v1]
            path = (v1, path)
            for (v2, cost2) in G[v1].items():
                if v2 not in visited:
                    heapq.heappush(q, (cost + cost2, v2, path))

def test_for_shortest_path():
    G = {'s':{'u':10, 'x':5}, 'u':{'v':1, 'x':2}, 'v':{'y':4}, 'x':{'u':3, 'v':9, 'y':2}, 'y':{'s':7, 'v':6}}
    print(shortest_path(G, 's','v'))


def readNGPH(file):
    line = file.readline()
    #print line,
    n = int(line)
    network = dict()
    while True:
        line = file.readline()
        xyz = line.split()
        #print xyz
        i,j = map(int,xyz[:2])
        if i < 0:
            return (n,network)
        if  i not in network:
            network[i] = dict()
        if j not in network:
            network[j] = dict()
        network[i][j] = 1
        network[j][i] = 1


def shortcuts( network, members ):
    n = len(members)
    for i in range(0,n):
        for j in range(i+1,n):
            d = min(j-i, n-(j-i))
            path = len(shortest_path(network, members[i],members[j]))-1
            if path < d:
                return 1


def findring( network, members, max ):
    #print members, "MAX:", max
    if len(members) > max:
        return (max, [])
    s = set(members)
    last = members[-1]
    results = []
    for adj in network[last].keys():
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

def totalrings( network, maxsize ):
    rings = dict()
    for x in network.keys():
        keys = network[x].keys()
        if keys != None:
            for y in keys:
                for z in keys:
                    if y < z:
                    #print x,y,z
                        members = [y,x,z]
                        (max, results) = findring( network, members, maxsize )
                        for i in results:
                            #to remove permutations
                            #make a copy of the list
                            j = list(i)
                            #sort
                            j.sort()
                            #fix in tuple to use as the key.
                            j = tuple(j)
                            #put sorted members as the key,
                            #and original list as the value.
                            rings[j] = i
    return rings

def saveRNGS( nmol, rings ):
    s = "@RNGS\n"
    s += "%d\n" % nmol
    for i in rings.values():
        s+= "%s " % len(i) + " ".join( map(str,i) ) + "\n"
    s += "0\n"
    return s


if __name__ == "__main__":
    maxsize = 8
    countonly = 0
    if len(sys.argv) > 1:
        if sys.argv[1] == "-c":
            countonly = 1
            sys.argv.pop( 1 )
        maxsize = int(sys.argv[1])
    file = sys.stdin
    while True:
        line = file.readline()
        if not line:
            break
        if line[0:5] == "@NGPH":
            (nmol,network) = readNGPH(file)
            #print shortest_path(network, 0,3)
            rings = totalrings( network, maxsize )
            if countonly:
                count = [0] * (maxsize-2)
                for i in rings.values():
                    count[len(i)-3]+=1
                print (" ".join( map(str,count) ))
            else:
                print (saveRNGS( nmol, rings ),end="")

