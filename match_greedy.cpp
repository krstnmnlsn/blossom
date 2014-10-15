#include <iostream>
#include <unistd.h>
#include <vector>

#include "match_original.cpp"

using namespace std;

namespace graph_matching {

// MAXN = The maximum possible number of nodes in our graph.  
// n = the actual number.
const int MAXN = 10000;
int n;

// The blossoms are handled by a union-find object.  the array pp = parent 
// pointers, the function f called on a node x returns the node's 
// representative and the function u unions (though without ranking).
int pp[MAXN];
int f(int x) { return x == pp[x] ? x : (pp[x] = f(pp[x])); } 
void u(int x, int y) {pp[x] = y;}
 
// The graph is stored as an array of vectors / adjacency lists.
vector<int> graph[MAXN]; 

// m stores the matching: 
// m[i] = -1 if node unchecked, i if unmatched, else the index of match.
int m[MAXN]; 

// BFS variables.  p stores the immediate parent of each node traversed (for 
// backtracking when an unmatched node is found).  d stores the parity of the
// distance from root to each node.  
// TODO: What exactly do c1 and c2 do.
int p[MAXN];
int d[MAXN];
int c1[MAXN], c2[MAXN];
int q[2*MAXN], *qf, *qb; // BFS queue
 
// timing variables.
double bfs_time[MAXN];
time_t mem_time = 0;

double average_bfs_time() {
  double ave = 0;
  for (int i=0; i<n; ++i) {
    // if (bfs_time[i] != 0) cout << bfs_time[i] << endl;
    ave += bfs_time[i];
  }
  return ave/n;
}

// Finds the least common ancestor of a node within the BFS tree.
int v[MAXN];
int lca(int x, int y, int r) { int i = f(x), j = f(y);
  while (i != j && v[i] != 2 && v[j] != 1) { v[i] = 1; v[j] = 2;
    if (i != r) i = f(p[i]); if (j != r) j = f(p[j]); }
  int b = i, z = j; if (v[j] == 1) swap(b, z);
  for (i = b; i != z; i = f(p[i])) v[i] = -1; v[z] = -1; return b; }
 
// Flips all matched/unmatched edges along the path from x to its ancestor r.
// TODO: ignores blossoms?
void path(int r, int x){ 
  if (r == x) return;
  if (d[x] == 0){ 
    path(r, p[p[x]]); 
    int i=p[x], j=p[p[x]]; 
    m[i]=j; m[j]=i; 
  } else {
    cout << "something has gone terribly wrong" << endl;
  }
}

 
// Searches for an augmenting path rooted at r via a bredth first search.
bool BFS() { 
  mem_time = 0;
  // Set up union-find (initially, all nodes are their own representatives)
  // Reset lca and d memory, we cut this time from the total recorded.
  time_t t = clock();
  for (int i = 0; i<n; ++i) pp[i] = i;
  memset(v, -1, sizeof(v));
  memset(d, -1, sizeof(d));
  mem_time = clock() - t;

  // Initialize distances to root = 0 and push everyone unmatched onto the queue.
  qf = qb = q;
  vector<int> que;
  for (int i=0; i<n; ++i) {
    if (m[i] == -1) {
      d[i] = 0;
      que.push_back(i);
      // *qb++ = i;
    }
  }
  // random_shuffle(que.begin(), que.end());
  for (int i=0; i<que.size(); ++i) { *qb++ = que[que.size()-1-i]; }

  while (qf < qb) {
    // Pop front of queue, take to be the current node, x.
    // Iterate over all neighbours of the current node, y.

    int x = *qf++;
    if (graph[x].size() == 0) continue;
    for (int i = 0, y = graph[x][0]; i < (int)graph[x].size(); ++i, y = graph[x][i]) {
      if (m[y] != y && f(x) != f(y)) {  // if neighbour not unmatchable and not in tree with x:
        // there are really only two cases: if we have intersected the new tree at the right point to
        // make an alternating path (do so and quit) or we have not (ignore)

          if (m[y] == -1) {             // if neighbour not matched:
            path(f(x), x);                  
            m[x] = y;                   
            m[y] = x; 
            return true;                // AUGMENTING PATH FOUND, update m and return

          // else we need to merge the two trees.  we just travelled along an unmatched edge to y, 
          // but the other tree connecting to it may have got there after a matched or unmatched edge

          } else {                      // if neighbour matched:
            if (f(y) == y) {            // if neighbour not yet in a tree
              pp[y] = f(x);             // just add y and m[y] onto the end of x's tree and iterate
              pp[m[y]] = f(x);
              p[y] = x;
              p[m[y]] = y;
              d[y] = 1;
              d[m[y]] = 0;
              *qb++ = m[y];

            } else if (d[y] == 0) {   // neighbour matched and the leaf of another tree
                                      // then we have a root-root augmenting path
              path(f(x), x);
              path(f(y), y);
              m[x] = y;
              m[y] = x;
              return true;
            }
          }
        }
      }
    }
  return false; 
}
 

void print_matching() {
  for (int i=0; i<n; ++i) {
    cout << "(" << i << ")-(" << m[i] << ")" << endl;
  }
}

int sizem() {
  int k = 0;
  for (int i=0; i<n; ++i) {
    if (m[i] != -1 && m[i] != i) ++k;
  }
  if (k%2) cout << "the number of matched nodes is not even even" << endl;
  return k/2;
}

int match() { 
  cout << "greedy matching" << endl;
  // memset arrays, we do not count this in our total time.
  // time_t t = clock();
  memset(bfs_time, 0, sizeof(bfs_time));
  memset(m, -1, sizeof(m));
  // mem_time += clock() - t;

  time_t t = clock();
  int c = 0;
  // try a greedy approach until things get too slow or we get stuck 
  // BFS();
  for (int i=0; i<n/2; ++i) {
    if (BFS()) c++;
    // sizem();
    // cout << "matching size = " << sizem() << endl;
    // print_matching();
  }

  time_t t2 = clock();
  cout << "greedy time is " << double(t2 - t)/CLOCKS_PER_SEC << endl;
  cout << "c thanks to greedy is " << c << endl;


  for (int i=0; i<n; ++i) match_orig::graph[i] = graph[i];
  match_orig::n = n;
  c = 0;
  for (int i=0; i<n; ++i) match_orig::m[i] = m[i];
  for (int i=0; i<n; ++i) {
    if (match_orig::m[i] == -1) {
      if (match_orig::BFS(i)) c++;
      else match_orig::m[i] = i;
    }
  }

  cout << "With blossoms got c = " << c << endl;
  return c; 
}

} // graph_matching

