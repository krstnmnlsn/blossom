#include <iostream>
#include <cstring>
#include "gen_graph.cpp"


using namespace std;

namespace components {

int v[graph_matching::MAXN];

void dfs(int i) {
	v[i] = 1;
	for (int k=0; k<graph_matching::graph[i].size(); ++k)
		if (!v[graph_matching::graph[i][k]]) dfs(graph_matching::graph[i][k]);
}


int count_comp() {	
	memset(v, 0, sizeof(v));
	int c = 0;

	for (int i=0; i<graph_matching::n; ++i) {
		if (!v[i]) {
			dfs(i);
			++c;
		}
	}
	return c;
}

}  // components