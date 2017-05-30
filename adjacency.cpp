#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <list>
#include <algorithm>
#include <map>
#include <set>
#include <stack>
#include <ctime>
#include <unordered_map>
using namespace std;

#define ALL(c) (c).begin(),(c).end()
#define IN(x,c) (find(c.begin(),c.end(),x) != (c).end())
#define REP(i,n) for (int i=0;i<(int)(n);i++)
#define FOR(i,a,b) for (int i=(a);i<=(b);i++)
#define INIT(a,v) memset(a,v,sizeof(a))
#define SORT_UNIQUE(c) (sort(c.begin(),c.end()), c.resize(distance(c.begin(),unique(c.begin(),c.end()))))
template<class A, class B> A cvt(B x) { stringstream ss; ss<<x; A y; ss>>y; return y; }

typedef pair<int,int> PII;
typedef long long int64;

#define N 10000

int n,m;
vector<vector<int> > edges;

void gen() {
	n=1000;
	m=5000;
	vector<int> ns;
	REP (i,n) ns.push_back(i);
	REP (i,m) {
		int k=1+rand()%(n/2);
		random_shuffle(ALL(ns));
		vector<int> edge;
		REP (j,k) edge.push_back(ns[j]);
		sort(ALL(edge));
		edges.push_back(edge);
	}
	printf("%d %d\n",n,m);
	REP (i,m) {
		vector<int> &edge=edges[i];
		printf("%d",(int)edge.size());
		REP (j,edge.size()) {
			printf(" %d",edge[j]);
		}
		printf("\n");
	}
}

void read_input() {
	scanf("%d %d\n",&n,&m);
	REP (i,m) {
		int k;
		scanf("%d",&k);
		vector<int> edge(k);
		REP (j,k) {
			scanf("%d",&edge[j]);
		}
		edges.push_back(edge);
	}
}

int adj_ref[N][N];
void solve_n2m() {
	REP (i,m) {
		vector<int> &edge=edges[i];
		for (int x : edge) {
			for (int y : edge) {
				adj_ref[x][y]=1;
			}
		}
	}
}

int adj[N][N];
vector<int> inc[N];
int hyper_size[N], h;
int group_id[N];
vector<int> group[N]; // split-off groups
vector<int> hyper_adj[N];
int hyper_adj_mat[N][N];

void connect(int a, int b) {
	assert(a<n && b<n);
	//assert(hyper_adj_mat[a][b]==0);
	hyper_adj_mat[a][b]=hyper_adj_mat[b][a]=1;
	hyper_adj[a].push_back(b);
	hyper_adj[b].push_back(a);
}

void add_edge(int ei) {
	vector<int> new_group;
	vector<int> groups, new_groups, existing_groups;
	// partition into groups
	for (int x : edges[ei]) {
		int g=group_id[x];
		if (g==-1) new_group.push_back(x);
		else {
			if (group[g].empty()) groups.push_back(g);
			group[g].push_back(x);
		}
	}
	// add new group
	if (new_group.size()>0) {
		int g_new=h++;
		hyper_size[g_new]=new_group.size();
		new_groups.push_back(g_new);
		for (int x : new_group) group_id[x]=g_new;
	}
	// split off the groups if necessary
	for (int g : groups) {
		if ((int)group[g].size()==hyper_size[g]) {
			existing_groups.push_back(g);
		} else {
			int g_new=h++;
			// move nodes
			for (int x : group[g]) {
				hyper_size[group_id[x]]--;
				group_id[x]=g_new;
				hyper_size[group_id[x]]++;
			}
			// copy connections
			for (int g_adj : hyper_adj[g]) {
				connect(g_new,g_adj);
			}
			connect(g_new,g);
			new_groups.push_back(g_new);
		}
	}
	// interconnect split-offs
	for (int g1 : new_groups) {
		for (int g2 : new_groups) if (g1<g2) connect(g1,g2);
		for (int g2 : existing_groups) connect(g1,g2);
	}
	for (int g1 : existing_groups) {
		for (int g2 : existing_groups) if (g1<g2) connect(g1,g2);
	}
	// clean-up
	for (int g : groups) {
		group[g].clear();
	}
}

void solve_n2() {
	// init
	REP (i,m) {
		for (int x : edges[i]) {
			inc[x].push_back(i);
		}
	}
	h=0;
	REP (i,n) group_id[i]=-1;
	REP (i,n) hyper_size[i]=0;
	// add edges one by one
	REP (i,m) {
		add_edge(i);
	}
	assert(h<=n);
	// fill adjacency matrix
	REP (i,n) {
		REP (j,n) {
			int gi=group_id[i], gj=group_id[j];
			if (gi==-1 || gj==-1) continue;
			if (gi==gj || hyper_adj_mat[gi][gj]) adj[i][j]=1;
		}
	}
}

void test() {
	int st=0;
	REP (i,n) {
		REP (j,n) {
			assert(adj_ref[i][j]==adj[i][j]);
			st+=adj_ref[i][j];
		}
	}
	printf("adj: %f\n",1.0*st/(n*n));
}

int main() {
	//freopen("hypergraph03.in","w",stdout); gen(); return 0;
	freopen("hypergraph03.in","r",stdin);
	clock_t start,finish;
	start=clock(); read_input(); finish=clock();
	printf("read: %f\n",1.0*(finish-start)/CLOCKS_PER_SEC);
	start=clock(); solve_n2m(); finish=clock();
	printf("bruteforce: %f\n",1.0*(finish-start)/CLOCKS_PER_SEC);
	start=clock(); solve_n2(); finish=clock();
	printf("n^2: %f\n",1.0*(finish-start)/CLOCKS_PER_SEC);
	test();
	return 0;
}
