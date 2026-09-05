// Nested Ranges Count (CSES 2169)
//
// Dado um conjunto de N intervalos [l, r], calcula para cada um:
// - contains[i]: quantidade de intervalos contidos dentro do intervalo i
// - contained[i]: quantidade de intervalos que contem o intervalo i
//
// Complexidade: O(N log N) tempo, O(N) espaco

struct Range {
	int l, r, id;
};

template<typename T = int> struct NestedRanges {
	int n;
	vector<Range> v;
	vector<int> contains, contained;

	NestedRanges(int n = 0) : n(n), contains(n, 0), contained(n, 0) {}

	void add(int id, int l, int r) {
		v.push_back({l, r, id});
	}

	void solve() {
		n = v.size();
		contains.assign(n, 0);
		contained.assign(n, 0);

		// 1. Compressao de coordenadas em R
		vector<int> R;
		for (auto& x : v) R.push_back(x.r);
		sort(R.begin(), R.end());
		R.erase(unique(R.begin(), R.end()), R.end());
		auto get_r = [&](int r) {
			return (int)(lower_bound(R.begin(), R.end(), r) - R.begin()) + 1;
		};

		// 2. Ordena: L crescente, e em empate R decrescente
		sort(v.begin(), v.end(), [](const Range& a, const Range& b) {
			if (a.l != b.l) return a.l < b.l;
			return a.r > b.r;
		});

		int m = R.size();
		vector<int> bit(m + 1, 0);
		auto update = [&](int idx, int val) {
			for (; idx <= m; idx += idx & -idx) bit[idx] += val;
		};
		auto query = [&](int idx) {
			int sum = 0;
			for (; idx > 0; idx -= idx & -idx) sum += bit[idx];
			return sum;
		};

		// 3. Contained: quantos contem o intervalo atual (L_ant <= L_i e R_ant >= R_i)
		for (int i = 0; i < n; i++) {
			int r = get_r(v[i].r);
			contained[v[i].id] = i - query(r - 1);
			update(r, 1);
		}

		// 4. Contains: quantos o intervalo atual contem (L_pos >= L_i e R_pos <= R_i)
		fill(bit.begin(), bit.end(), 0);
		for (int i = n - 1; i >= 0; i--) {
			int r = get_r(v[i].r);
			contains[v[i].id] = query(r);
			update(r, 1);
		}
	}
};
