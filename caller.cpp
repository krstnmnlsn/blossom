#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iterator>

#include "gen_graph.cpp"

// Running this file results in a random graph being generated 
// and |match| from match_original being run on it.  The results 
// and timings are printed.


int main(int argc, char* argv[]) {
	// The edge-probability in our randomly generated graph 
	// defaults to 1 unless an input is supplied.
	float p = 1;
	if (argc >= 2) {
		p = atof(argv[1]);
	}

	// The default number of vertices is 10,000
	graph_matching::n = 10000;	

	time_t gen_time = clock();
	graph_matching::gen_graph(p);
	cout << "graph gen time (s): " << double(clock()-gen_time)/CLOCKS_PER_SEC << endl;
	
	time_t tnow = clock();
	cout << "the maximum matching is " << graph_matching::match() << endl;
	time_t diff = clock() - tnow;
	cout << "time taken, including allocation time (s): " << double(diff)/CLOCKS_PER_SEC << endl;
	cout << "the average time spend in a BFS (s): " << graph_matching::average_bfs_time() << endl;
	return 0;
}
