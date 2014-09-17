#include <fstream>
#include <string>
#include <assert.h>

#include "match_original.cpp"

using namespace std;

namespace graph_matching {

// Note:  components should appear around 0.001
const int num_graphs = 8;
string exts[num_graphs] = { "1", "0.5", "0.25", "0.1", "0.01", "0.001", "0.0001", "0.00001" };

int exp_c[num_graphs] = { 5000, 5000, 5000, 5000, 5000, 5000, 2723, 450 };

void read_graph(string ext) {
	fstream file;
	file.open(string("graph-").append(ext).c_str());

	if (!file.is_open()) return;

	int val = 0;
	file >> n;
	for (int i=0; i<n; ++i) {
		while (file >> val, val != -1) {
			graph[i].push_back(val);
		} 
	}
	file.close();
}

} // graph_matching

int main() {
	for (int i=0; i<graph_matching::num_graphs; ++i) {
		graph_matching::read_graph(graph_matching::exts[i]);
		int c = graph_matching::match();
		assert(c == graph_matching::exp_c[i]);
		memset(graph_matching::graph, 0, sizeof(graph_matching::graph));
	}
	return 0;
}