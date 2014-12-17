blossom
=======

Implementations and tests of the blossom algorithm for matching in general graphs.


Algorithm (1) "The Blossom Algorithm" - match_original.cpp
Algorithm (2) "The Blossom Algorithm with Queue" - match_original_queued.cpp
Algorithm (3) "The Naive Algorithm" - match_naive.cpp

The Erdos-Renyi random graphs were generated with gen_graph.cpp.

The algorithms were tested throughout development with test_match.cpp.

Graphs without perfect matching, but an even number of vertices and single component were tested in tutte-tester.cpp.

Asymptotic values (matching size, number of blossoms contracted, number of odd components) was collected with asymp_tester.cpp.