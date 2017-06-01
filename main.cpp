#include <cassert>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#include "adjacency.h"

ifstream fin("hypergraph.in");

HyperGraph read_input() {
	HyperGraph hg;
	fin >> hg.n >> hg.m;
	string line;
	getline(fin,line);
	for (int i=0;i<hg.m;i++) {
		getline(fin,line);
		stringstream ss(line);
		Vect edge;
		int tok;
		while (ss >> tok) {
			edge.push_back(tok);
		}
		hg.edges.push_back(edge);
	}
	return hg;
}

Vect2 naive(HyperGraph &hg) {
	Vect2 a = Vect2(hg.n, Vect(hg.n));
	for (Vect &edge : hg.edges) {
		for (int x : edge) {
			for (int y : edge) {
				a[x][y]=1;
			}
		}
	}
	return a;
}

int main() {
	HyperGraph hg = read_input();
	hg.adjacency_matrix();
	assert(hg.adj==naive(hg));
	return 0;
}
