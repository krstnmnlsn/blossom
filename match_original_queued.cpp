#include <iostream>
#include <unistd.h>
#include <vector>
#include <deque>
#include <cmath>

using namespace std;

namespace graph_matching {
// namespace match_orig {

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
deque<int> q;


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
  }
  else if (d[x] == 1) { 
    path(m[x], c1[x]); 
    path(r, c2[x]);
    int i = c1[x], j = c2[x]; 
    m[i] = j; m[j] = i; 
  } 
}
 
int blossoms = 0;

// Shrinks one side of a blossom.
void shrink_one_side(int x, int y, int b){
  blossoms++;
  for (int i=f(x); i!=b; i=f(p[i])) {
    u(i, b);
    if (d[i]==1) { 
      c1[i]=x;
      c2[i]=y;
      // All nodes in blossom are reachable an even 
      // distance from the root, so push onto queue.
      q.push_front(i); // *qb++=i;  
    }
  }
}
 
// Searches for an augmenting path rooted at r via a bredth first search.
bool BFS(int r) { 
  if (graph[r].size() == 0) return false;

  // Set up union-find (initially, all nodes are their own representatives)
  // Reset lca and d memory, we cut this time from the total recorded.
  time_t t = clock();
  for (int i = 0; i<n; ++i) pp[i] = i;
  memset(v, -1, sizeof(v));
  memset(d, -1, sizeof(d));
  mem_time += clock() - t;

  // Initialize values and push root onto the queue.
  q.resize(0);
  d[r] = 0;
  q.push_front(r);
  while (!q.empty()) {
    // Pop front of queue, take to be the current node, x.
    // Iterate over all neighbours of the current node, y.
    int this_is_dumb = q.front(); q.pop_front();
    for (int x = this_is_dumb, i = 0, y = graph[x][0]; i < (int)graph[x].size(); ++i, y = graph[x][i]) {

      if (m[y] != y && f(x) != f(y)) {  // if neighbour not unmatchable and not in blossom with x:
        if (d[y] == -1) {               // if neighbour not in tree yet:
          if (m[y] == -1) {             // if neighbour not matched:
            path(r, x);                
            m[x] = y;                   
            m[y] = x; 
            return true;                // AUGMENTING PATH FOUND, update m and return

          } else {                      // if neighbour matched:
            p[y] = x;                   // update tree growing structure (m and d)
            p[m[y]] = y;                // and push y's match onto queue.
            d[y] = 1; 
            d[m[y]] = 0; 
            q.push_front(m[y]); 
          }
        } else if (d[f(y)] == 0) {      // if root-distance to neighbour known and even
          int b = lca(x, y, r);         // then we have found a blossom.
          shrink_one_side(x, y, b); shrink_one_side(y, x, b); 
        } 
      }
    }
  }
  return false; 
}
 
int match() { 
  q = deque<int>();
  // memset arrays, we do not count this in our total time.
  time_t t0 = clock();
  // memset(bfs_time, 0, sizeof(bfs_time));
  memset(m, -1, sizeof(m));
  mem_time += clock() - t0;

  int c = 0;
  for (int i=0; i<n; ++i) {
    if (c == floor(n/2)) continue; 
    if (m[i] == -1) { 
      // time_t t2 = clock();
      if (BFS(i)) c++; 
      // there is no augmenting path leaving i 
      // and therefore never will be (see lemma)
      else m[i] = i; 
      // bfs_time[i] = double(clock() - t2 - mem_time)/CLOCKS_PER_SEC;
    }
  }
  
  cout << double(clock() - t0 - mem_time)/CLOCKS_PER_SEC << endl;
  cout << "the number of blossoms contracted is " << blossoms/2 << endl;
  return c; 
}

// } // match_orig
} // graph_matching

