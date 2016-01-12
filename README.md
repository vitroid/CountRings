#Ring Statistics Algorithm
##Counting policy

* It counts only irreducible rings (rings not having shortcut bridges).
* It counts rings purely topologically. It does not use geometrical information.
* Edge direction is not considered. (Undirected graph)

##Algorithm

1. Choose 3 successive nodes along the network.
1. Find the smallest rings passing the three nodes.
1. The ring must not have shotcuts, i.e. path connecting two vertices on the ring which is shorter than the path along the ring. (Using Dijkstra's algorithm.)
1. Put the ring in the list.
1. Repeat 1 .. 4 until all sets of 3 successive nodes are tested.
1. Eliminate the permutations of a ring in the list.
1. (Optional) Remove "crossing rings".

##Usage
Input data must be in <a href="http://theochem.chem.okayama-u.ac.jp/wiki/wiki.cgi/matto?page=%40NGPH">@NGPH</a> format. Output data will be in <a href="http://theochem.chem.okayama-u.ac.jp/wiki/wiki.cgi/matto?page=%40RNGS">@RNGS</a> format.

    % ./countrings.py 8 < test.ngph > test.rngs
    % ./countrings.py 8 < test.ngph | ./crossingrings.pl > test2.rngs

##Sample
The following is the expression of a cubic graph, which should have six 4-cycles.

    @NGPH
    8
    0 1
    1 2
    2 3
    3 0
    4 5
    5 6
    6 7
    7 4
    0 4
    1 5
    2 6
    3 7
    -1 -1

You can pick up many samples at the Vitrite database:
    http://vitrite.chem.okayama-u.ac.jp
##Known Problems
###Sample 1
<img src="sample1.png" /><br />
Number of rings in this kind of graph consisting of N bows is counted as N (N-1) / 2. It happens because of the lack of 3-dimentional geometrical information.
###Sample 2
<img src="sample2.png" /><br />
 It is a smaller version of sample 1 consisting of 4 bows. As you see, surface rings of this structure seems to be 4, while the algorithm counts as 6, because it also counts the “crossing rings” (diagonal red and blue rings). These sample topologies rarely appear in the network of water at low temperature because the z-index at the top and bottom nodes is too large. 
While, there is another sample containing crossing rings but still undistorted.
###Sample 3
<img src="sample3.png" />
##Wayarounds
The small Perl program “crossingrings.pl” looks up all the crossing rings in the ring list (in <a href="http://theochem.chem.okayama-u.ac.jp/wiki/wiki.cgi/matto?page=%40RNGS">@RNGS</a> format) and remove one of them randomly until the crossing is avoided. With the use of 3-dimentional geometrical information, there might be better walkarounds.
##Note
This program is developed for analysing the hydrogen bond network of water.
An affordable and relevant definition of the hydogen bonds is discussed <a href="http://theochem.chem.okayama-u.ac.jp/wiki/wiki.cgi/matto?page=Relevance+of+Hydrogen+Bond+Definition">here</a>.
