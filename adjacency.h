#include <cmath>
#include <vector>
#include <unordered_set>

typedef std::vector<int> Vect;
typedef std::vector<Vect> Vect2;
typedef unordered_set<long long> HashSet;

struct GroupGraph {
	int n,h;
	Vect2 edges, splitoff, group_adj;
	Vect group_id, group_size;
	HashSet group_edges;

	GroupGraph(int n_) {
		n=n_;
	}

	long long encode(int a, int b) {
		if (b<a) swap(a,b);
		return a*(long long)n+b;
	}

	void connect(int a, int b) {
		long long ab=encode(a,b);
		if (!group_edges.count(ab)) {
			group_edges.insert(ab);
			group_adj[a].push_back(b);
			group_adj[b].push_back(a);
		}
	}

	int add_group() {
		group_size.push_back(0);
		splitoff.push_back(Vect());
		group_adj.push_back(Vect());
		return h++;
	}

	void process_edge(Vect &edge) {
		// partition edge nodes by groups
		Vect intersected_groups, new_group;
		for (int x : edge) {
			int g=group_id[x];
			if (g==-1) new_group.push_back(x);
			else {
				if (splitoff[g].empty()) intersected_groups.push_back(g);
				splitoff[g].push_back(x);
			}
		}
		Vect groups;
		// add new group
		if (new_group.size()>0) {
			int id=add_group();
			for (int x : new_group) group_id[x]=id;
			group_size[id]=new_group.size();
			groups.push_back(id);
		}
		// split off the groups if necessary
		for (int g : intersected_groups) {
			if ((int)splitoff[g].size()==group_size[g]) {
				groups.push_back(g);
			} else {
				int g_new=add_group();
				// move nodes
				for (int x : splitoff[g]) {
					group_size[group_id[x]]--;
					group_id[x]=g_new;
					group_size[group_id[x]]++;
				}
				// copy connections
				for (int g_adj : group_adj[g]) {
					connect(g_new,g_adj);
				}
				connect(g_new,g);
				groups.push_back(g_new);
			}
		}
		// interconnect contained groups
		for (int g1 : groups) {
			for (int g2 : groups) if (g1<g2) connect(g1,g2);
		}
		// clean-up
		for (int g : intersected_groups) {
			splitoff[g].clear();
		}
	}

	void process() {
		h=0;
		group_id = Vect(n, -1);
		group_size = Vect(n, 0);
		splitoff = Vect2(n, Vect());
		group_edges = HashSet();
		for (Vect &edge : edges) {
			process_edge(edge);
		}
	}

	int query(int i, int j) {
		assert(h>0);
		int gi=group_id[i], gj=group_id[j];
		if (gi==-1 || gj==-1) return 0;
		return (gi==gj || group_edges.count(encode(gi,gj)));
	}
};

struct HyperGraph {
	int n,m;
	Vect2 edges;

	vector<GroupGraph> sections;

	void preprocess(double eps=1.0) {
		int k=max(1.0,eps*log2(m));
		GroupGraph g(n);
		for (int i=0;i<m;i++) {
			if (i%k==0) {
				if (i!=0) sections.push_back(g);
				g=GroupGraph(n);
			}
			assert(edges[i].size()>0);
			g.edges.push_back(edges[i]);
		}
		sections.push_back(g);
		for (GroupGraph &g : sections) {
			g.process();
		}
	}

	int query(int i, int j) {
		for (GroupGraph &g : sections) {
			if (g.query(i,j)) return 1;
		}
		return 0;
	}

	Vect2 adj;

	void adjacency_matrix(double eps=1.0) {
		preprocess(eps);
		adj = Vect2(n, Vect(n));
		for (int i=0;i<n;i++) {
			for (int j=0;j<n;j++) {
				adj[i][j]=query(i,j);
			}
		}
	}
};
