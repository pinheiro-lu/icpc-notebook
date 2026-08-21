/**
 * Author: Benjamin Qi, Oleksandr Kulkov, chilli
 * Date: 2020-01-12
 * License: CC0
 * Source: https://github.com/kth-competitive-programming/kactl/blob/main/content/graph/HLD.h
 * Description: Decomposes a tree into vertex disjoint heavy paths and light
 * edges such that the path from any leaf to the root contains at most log(n)
 * light edges. Code does additive modifications and max queries, but can
 * support commutative segtree modifications/queries on paths and subtrees.
 * Takes as input the full adjacency list. VALS_EDGES being true means that
 * values are stored in the edges, as opposed to the nodes. All values
 * initialized to the segtree default. Root must be 0.
 * Time: O((log N)^2)
 * Status: stress-tested against old HLD
 */

template <bool VALS_EDGES = false> struct HLD {
	int N, tim = 0;
	vector<vi> adj;
	vi par, siz, rt, pos;
	Node *tree;

	HLD(vector<vi> adj_)
		: N(adj_.size()), adj(adj_), par(N, -1), siz(N, 1),
		  rt(N), pos(N), tree(new Node(0, N)) {
		dfsSz(0); dfsHld(0);
	}
	void dfsSz(int v) {
		if (par[v] != -1) adj[v].erase(find(adj[v].begin(), adj[v].end(), par[v]));
		for (int& u : adj[v]) {
			par[u] = v;
			dfsSz(u);
			siz[v] += siz[u];
			if (siz[u] > siz[adj[v][0]]) swap(u, adj[v][0]);
		}
	}
	void dfsHld(int v) {
		pos[v] = tim++;
		for (int u : adj[v]) {
			rt[u] = (u == adj[v][0] ? rt[v] : u);
			dfsHld(u);
		}
	}
	template <class B> void process(int u, int v, B op) {
		for (; rt[u] != rt[v]; v = par[rt[v]]) {
			if (pos[rt[u]] > pos[rt[v]]) swap(u, v);
			op(pos[rt[v]], pos[v] + 1);
		}
		if (pos[u] > pos[v]) swap(u, v);
		op(pos[u] + VALS_EDGES, pos[v] + 1);
	}
	void modifyPath(int u, int v, int val) {
		process(u, v, [&](int l, int r) { tree->add(l, r, val); });
	}
	int queryPath(int u, int v) {
		int res = -inf;
		process(u, v, [&](int l, int r) {
			res = max(res, tree->query(l, r));
		});
		return res;
	}
	int querySubtree(int v) {
		return tree->query(pos[v] + VALS_EDGES, pos[v] + siz[v]);
	}
};

