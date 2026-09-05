// Euler Tour Technique / Tree Flattening (USACO Guide / CP-Algorithms)
//
// Mapeia uma arvore em um array continuo [start_time[u], end_time[u]].
//
// Propriedades e Aplicacoes:
// 1. Subarvore de 'u': intervalo continuo [start_time[u], end_time[u]].
//    - Update/Query em subarvore de 'u': operar em [start_time[u], end_time[u]] na SegTree/BIT.
// 2. Ancestralidade em O(1):
//    - 'u' eh ancestral de 'v' sse start_time[u] <= start_time[v] && end_time[v] <= end_time[u].
// 3. Caminho da Raiz ate 'u' com BIT (Truque do Dalalio):
//    - Alterar peso do no 'u': add(start_time[u], +val), add(end_time[u] + 1, -val).
//    - Soma do caminho Raiz -> 'u': sum(start_time[u]) na BIT.
//
// Complexidade: O(N) tempo e espaco

#include <vector>
using namespace std;

struct EulerTour {
	int n, timer = 0;
	vector<vector<int>> adj;
	vector<int> start_time, end_time;

	EulerTour(int n = 0) : n(n), adj(n + 1), start_time(n + 1), end_time(n + 1) {}

	void add_edge(int u, int v) {
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	void dfs(int u, int p) {
		start_time[u] = ++timer;
		for (int v : adj[u]) if (v != p) {
			dfs(v, u);
		}
		end_time[u] = timer;
	}

	void build(int root = 1) {
		timer = 0;
		dfs(root, 0);
	}

	bool is_ancestor(int u, int v) {
		return start_time[u] <= start_time[v] && end_time[v] <= end_time[u];
	}

	pair<int, int> subtree_range(int u) {
		return {start_time[u], end_time[u]};
	}
};
