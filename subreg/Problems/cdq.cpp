// DP de Dominacao 3D / CDQ Divide & Conquer (UFMG)
//
// Computa para todo ponto i:
// dp[i] = 1 + max_{j dominado por i} dp[j]
// em que ser dominado eh ter as 3 coordenadas estritamente menores (x, y, z).
//
// Como adaptar para outros problemas:
// 1. Contagem de pontos dominados em 3D:
//    - Na BIT, trocar max por soma: bit[p-1] += 1 (update) e ans[id] += q (query).
// 2. Problemas Dinâmicos Offline (Updates + Queries):
//    - Coordenada X = Timestamp (tempo da operacao).
//    - vv[0] (esquerda) = updates no passado que inserem na BIT.
//    - vv[1] (direita) = queries no futuro que consultam a BIT.
// 3. Ordem nao-estrita (<= em vez de <):
//    - No while, trocar '< y' por '<= y'.
//
// Complexidade: O(N log^2 N) tempo, O(N) memoria

#include <vector>
#include <tuple>
#include <algorithm>
using namespace std;

void cdq_lis2d(vector<vector<tuple<int, int, int>>>& v, vector<int>& dp, int l, int r) {
	if (l == r) {
		for (int i = 0; i < (int)v[l].size(); i++) {
			int ii = get<2>(v[l][i]);
			dp[ii] = max(dp[ii], 1);
		}
		return;
	}
	int m = (l + r) / 2;
	cdq_lis2d(v, dp, l, m);

	vector<tuple<int, int, int>> vv[2];
	vector<int> Z;
	for (int i = l; i <= r; i++) for (auto it : v[i]) {
		vv[i > m].push_back(it);
		Z.push_back(get<1>(it));
	}
	sort(vv[0].begin(), vv[0].end());
	sort(vv[1].begin(), vv[1].end());
	sort(Z.begin(), Z.end());
	auto get_z = [&](int z) { return (int)(lower_bound(Z.begin(), Z.end(), z) - Z.begin()); };
	vector<int> bit(Z.size(), 0);

	int i = 0;
	for (auto [y, z, id] : vv[1]) {
		while (i < (int)vv[0].size() and get<0>(vv[0][i]) < y) {
			auto [y2, z2, id2] = vv[0][i++];
			for (int p = get_z(z2) + 1; p <= (int)Z.size(); p += p & -p)
				bit[p - 1] = max(bit[p - 1], dp[id2]);
		}
		int q = 0;
		for (int p = get_z(z); p; p -= p & -p) q = max(q, bit[p - 1]);
		dp[id] = max(dp[id], q + 1);
	}
	cdq_lis2d(v, dp, m + 1, r);
}

vector<int> solve_cdq(vector<tuple<int, int, int>> v) {
	int n = v.size();
	vector<tuple<int, int, int, int>> vv;
	for (int i = 0; i < n; i++) {
		auto [x, y, z] = v[i];
		vv.emplace_back(x, y, z, i);
	}
	sort(vv.begin(), vv.end());

	vector<vector<tuple<int, int, int>>> V;
	for (int i = 0; i < n; i++) {
		int j = i;
		V.emplace_back();
		while (j < n and get<0>(vv[j]) == get<0>(vv[i])) {
			auto [x, y, z, id] = vv[j++];
			V.back().emplace_back(y, z, id);
		}
		i = j - 1;
	}
	vector<int> dp(n, 0);
	cdq_lis2d(V, dp, 0, (int)V.size() - 1);
	return dp;
}
