#include <iostream>
#include <cmath>

// computes the expected number of cycles in a random graph G_p

using namespace std;

typedef long long ll;

double p;

ll fact(int n) {
	return n == 0 ? 1 : n*fact(n-1);
}


// computes fallfact(n/k) p^k
double gross(int n, int k) {
	double val = 0;
	for (int i=0; i<k; ++i) {
		val += log((n-i)*p);
		//cout << val << endl;
	}
	// cout << k << " " << val << endl;
	return exp(val);
}


double cliques(int n, int k) {
	double val = 0;
	for (int i=0; i<k; ++i) {
		val += log(n-i) - log(i+1);
	}
	val += log(p) * k*(k-1)/2;
	return exp(val);
}


int main(int argc, char* argv[]) {
	
	p = 1;
	if (argc > 1) {
		p = atof(argv[1]);
	}

	int n = 10000;
	double cyc = 0;
	for (int k=3; k<n; k+=2) {
		cyc += gross(n,k) / (2*k);
	}
	// cout << cyc << endl;

	for (int k=n; k>0; --k) {
		if (cliques(n, k) > 0) cout << k << " " << cliques(n, k) << endl;
	}


	return 0;
}