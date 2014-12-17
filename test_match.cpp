#include <fstream>
#include <string>
#include <assert.h>

#include "gen_graph.cpp"

using namespace std;

namespace graph_matching {

// Note:  components should appear around 0.001
const int num_graphs = 8;
string exts[num_graphs] = { "1", "0.5", "0.25", "0.1", "0.01", "0.001", "0.0001", "0.00001" };

int exp_c[num_graphs] = { 5000, 5000, 5000, 5000, 5000, 5000, 2723, 450 };

} // graph_matching

int main() {
	for (int i=1; i<graph_matching::num_graphs; ++i) {
		graph_matching::read_graph(string("graph-").append(graph_matching::exts[i]).c_str());
		time_t tnow = clock();
		int c = graph_matching::match();
		time_t diff = clock() - tnow;
		cout << "time taken, including allocation time (s): " << double(diff)/CLOCKS_PER_SEC << endl;
		cout << "matching alg found c = " << c << " correct c = " << graph_matching::exp_c[i] << endl;
		// assert(c == graph_matching::exp_c[i]);
		memset(graph_matching::graph, 0, sizeof(graph_matching::graph));
	}
	return 0;
}