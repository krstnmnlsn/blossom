#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iterator>
#include <string>
#include <sstream>
#include "count_comp.cpp"

// Running this file results in the 'anti-tutte' graph being read 
// and |match| from match_original being run on it.  The results 
// and timings are printed.

int main(int argc, char* argv[]) {
	// The edge-probability in our randomly generated graph 
	// defaults to 1 unless an input is supplied.
	float p = 1;
	if (argc >= 2) {
		p = atof(argv[1]);
	}

	for (int i=1; i<6; ++i) {
		time_t gen_time = clock();
		// graph_matching::gen_graph(p);
		stringstream ss; ss << i;
		graph_matching::read_graph("graph-anti-tutte-" + ss.str());
		cout << "graph gen time (s): " << double(clock()-gen_time)/CLOCKS_PER_SEC << endl;
		
		time_t tnow = clock();
		int c = graph_matching::match();
		time_t diff = clock() - tnow;
		cout << "the maximum matching is " << c << endl;
		cout << "time taken, including allocation time (s): " << double(diff)/CLOCKS_PER_SEC << endl;
		// cout << "the average time spend in a BFS (s): " << graph_matching::average_bfs_time() << endl;
		int sup1;
		int comp = components::count_comp(sup1);
		cout << "the number of components is " << comp << endl;
	
		cout << "========" << endl;

		time_t gen_time2 = clock();
		// graph_matching::gen_graph(p);
		stringstream ss2; ss2 << i;
		graph_matching::read_graph("graph-tutte-" + ss2.str());
		cout << "graph gen time (s): " << double(clock()-gen_time2)/CLOCKS_PER_SEC << endl;
		
		time_t tnow2 = clock();
		int c2 = graph_matching::match();
		time_t diff2 = clock() - tnow2;
		cout << "the maximum matching is " << c2 << endl;
		cout << "time taken, including allocation time (s): " << double(diff2)/CLOCKS_PER_SEC << endl;
		// cout << "the average time spend in a BFS (s): " << graph_matching::average_bfs_time() << endl;
		int sup2;
		int comp2 = components::count_comp(sup2);
		cout << "the number of components is " << comp2 << endl;
		cout << endl;
		cout << endl;
	}

	return 0;
}
