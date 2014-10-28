#include <iostream>
#include <cstring>
#include <map>
#include <queue>

#include "gen_graph.cpp"

using namespace std;

typedef long long ll;

namespace components {

// visited for count comp
int v[graph_matching::MAXN];

int seen = 0;

void dfs(int i) {
	v[i] = 1;
	++seen;
	for (int k=0; k<graph_matching::graph[i].size(); ++k)
		if (!v[graph_matching::graph[i][k]]) dfs(graph_matching::graph[i][k]);
}

int count_comp() {	
	memset(v, 0, sizeof(v));
	int c = 0;
	map<int, int> compsz;

	for (int i=0; i<graph_matching::n; ++i) {
		if (!v[i]) {
			int before = seen;
			dfs(i);
			if (compsz.count(seen - before)) ++compsz[seen-before];
			else compsz[seen-before] = 1;
			++c;
		}
	}

	for (map<int,int>::iterator it = compsz.begin(); it != compsz.end(); ++it) {
		cout << endl << it->second << " components of size " << it->first << endl;
	}

	return c;
}

}  // components