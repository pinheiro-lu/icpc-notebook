// Centro e Diametro de Arvore (UFMG)
//
// Retorna o diametro e o(s) centro(s) da arvore.
// Toda arvore tem 1 ou 2 centros, que estao exatamente no meio do diametro.
//
// Tree Compass (Pintar arvore com minimo de operacoes de raio (v, d)):
// - 1 centro: usar (c, r) para r = 0, 1, ..., diam/2
// - 2 centros: usar (c1, r) e (c2, r) para r impares ate (diam+1)/2
//
// Complexidade: O(N) tempo e espaco

vector<int> g[N];
int d[N], par[N];

pair<int, vector<int>> center(int n) {
	int f = 0, df = -1;
	auto dfs = [&](auto& self, int u, int p) -> void {
		if (d[u] > df) f = u, df = d[u];
		for (int v : g[u]) if (v != p) {
			d[v] = d[u] + 1;
			par[v] = u;
			self(self, v, u);
		}
	};

	par[0] = -1; d[0] = 0; df = -1;
	dfs(dfs, 0, -1);
	int root = f;
	par[root] = -1; d[root] = 0; df = -1;
	dfs(dfs, root, -1);

	vector<int> c;
	while (f != -1) {
		if (d[f] == df / 2 || d[f] == (df + 1) / 2) c.push_back(f);
		f = par[f];
	}
	return {df, c}; // {diametro, vetor com os centros (1 ou 2)}
}
