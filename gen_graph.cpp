#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iterator>
#include <string>
#include <fstream>

#include "match_original.cpp"

using namespace std;

namespace graph_matching {

// Our very low-budget random number generator.
const int modulo = 1000000;
static unsigned long x=123456789, y=362436069, z=521288629;
inline unsigned long rand2(void) { 
	unsigned long t;
    x ^= x << 16; x ^= x >> 5; x ^= x << 1;
    t = x; x = y; y = z;
    z = t ^ x ^ y; return z;
}

// Our low-budget random shuffler.
template <class RandomAccessIterator>
inline void rand_shuffle2(RandomAccessIterator first, RandomAccessIterator last) {
  typename iterator_traits<RandomAccessIterator>::difference_type i, n;
  n = (last-first);
  for (i=n-1; i>0; --i) {
    swap (first[i],first[rand2()%(i+1)]);
  }
}

// Populates |graph| with a randomly generated graph.  The adjacency lists are 
// randomly permuted to avoid the worst-case behaviour of |match|.
void gen_graph(float p) {
	for (int i = 0; i < n; i++) graph[i].reserve(p*(n+1));
	for (int i=0; i<n; ++i)
		for (int j=0; j<i; ++j)
			if ((rand2() % modulo) / double(modulo - 1) < p) {
				graph[i].push_back(j); graph[j].push_back(i);
			}

	for (int i = 0; i < n; i++)
		rand_shuffle2(graph[i].begin(), graph[i].end());
}

void print_graph() {
	for (int i=0; i<n; ++i) {
		for (int j=0; j<graph[i].size(); ++j)
			cout << graph[i][j] << " ";
		cout << endl;
	}
}

void write_graph(string ext) {
	ofstream file;
	file.open(string("graph-").append(ext).c_str());
	file << n << endl;
	for (int i=0; i<n; ++i) {
		for (int j=0; j<graph[i].size(); ++j) {
			file << graph[i][j] << " ";
		}
		file << -1 << endl;
	}
	file.close();
} 

} // graph_matching

// int main() {
// 	graph_matching::n = 10000;

// 	graph_matching::gen_graph(1);
// 	graph_matching::write_graph("1");

// 	graph_matching::gen_graph(0.5);
// 	graph_matching::write_graph("0.5");
	
// 	graph_matching::gen_graph(0.25);
// 	graph_matching::write_graph("0.25");
	
// 	graph_matching::gen_graph(0.1);
// 	graph_matching::write_graph("0.1");
	
// 	graph_matching::gen_graph(0.01);
// 	graph_matching::write_graph("0.01");

// 	// This is about where we lose connectedness apparently
// 	graph_matching::gen_graph(0.001);
// 	graph_matching::write_graph("0.001");

// 	graph_matching::gen_graph(0.0001);
// 	graph_matching::write_graph("0.0001");

// 	graph_matching::gen_graph(0.00001);
// 	graph_matching::write_graph("0.00001");	
// }