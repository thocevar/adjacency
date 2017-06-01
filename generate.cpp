#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

int n,m;
vector<vector<int> > edges;

int main() {
	freopen("hypergraph.in","w",stdout);
	n=1000;
	m=10000;
	int s=20;
	vector<int> ns;
	for (int i=0;i<n;i++) ns.push_back(i);
	for (int i=0;i<m;i++) {
		int k=1+rand()%s;
		random_shuffle(ns.begin(),ns.end());
		vector<int> edge;
		for (int j=0;j<k;j++) edge.push_back(ns[j]);
		sort(edge.begin(),edge.end());
		edges.push_back(edge);
	}
	printf("%d %d\n",n,m);
	for (int i=0;i<m;i++) {
		for (int e : edges[i]) {
			printf("%d ",e);
		}
		printf("\n");
	}
	return 0;
}
