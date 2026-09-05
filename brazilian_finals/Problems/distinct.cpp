// D-Query Offline (SPOJ DQUERY / Gabriel Dalalio - Unicamp 2026)
//
// Descrição:
//     Responde Q queries de quantidade de elementos distintos em [a, b].
//     Para cada posição i, v[i] guarda a ocorrência anterior do elemento.
//     Um elemento em i é a 1ª ocorrência em [a, b] sse v[i] < a.
//     Ordena posições por v[i] e queries por 'a'. Conforme 'a' cresce,
//     adiciona na BIT posições com v[i] < a e consulta sum(b) - sum(a - 1).
//
// Complexidade: O((N + Q) log N) tempo, O(N + Q) espaço

struct Query {
	int a, b, id;
};

vector<int> dquery(const vector<int>& arr, const vector<Query>& queries) {
	int n = arr.size(), nq = queries.size();
	vector<int> v(n + 1), f1(n + 1), ans(nq);
	map<int, int> m;

	for (int i = 1; i <= n; i++) {
		int val = arr[i - 1];
		if (!m.count(val)) v[i] = -1;
		else v[i] = m[val];
		m[val] = i;
		f1[i] = i;
	}

	sort(f1.begin() + 1, f1.end(), [&](int x, int y) {
		return v[x] < v[y];
	});

	vector<int> f2(nq);
	for (int i = 0; i < nq; i++) f2[i] = i;
	sort(f2.begin(), f2.end(), [&](int x, int y) {
		return queries[x].a < queries[y].a;
	});

	vector<int> bit(n + 2, 0);
	auto add = [&](int pos, int inc) {
		for (; pos <= n + 1; pos += pos & -pos) bit[pos] += inc;
	};
	auto sum = [&](int pos) {
		int ret = 0;
		for (; pos > 0; pos -= pos & -pos) ret += bit[pos];
		return ret;
	};

	int c1 = 1;
	for (int i = 0; i < nq; i++) {
		int j = f2[i];
		while (c1 <= n && v[f1[c1]] < queries[j].a) {
			add(f1[c1++], 1);
		}
		ans[queries[j].id] = sum(queries[j].b) - sum(queries[j].a - 1);
	}
	return ans;
}
