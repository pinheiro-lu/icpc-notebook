// Sparse Table / RMQ
//
// Descrição:
//     Responde queries estáticas em intervalos [a, b].
//     - Operações idempotentes (min, max, gcd, and, or): query em O(1).
//     - Operações associativas gerais (soma, produto): query em O(log N).
//
// Vantagens sobre SegTree / BIT:
//     - Query O(1) para operações idempotentes vs O(log N).
//     - Constante e localidade de cache excelentes (matriz contígua).
//
// Complexidade:
//     build - O(N log N)
//     query - O(1) para idempotentes / O(log N) geral

template <typename T> struct RMQ {
	vector<vector<T>> dp;
	T ops(T a, T b) { return min(a,b); }
	RMQ() {}
	RMQ(vector<T> v) {
		int n = v.size();
		int log = 32-__builtin_clz(n);
		dp.assign(log, vector<T>(n));
		copy(all(v), dp[0].begin());
		for(int l=1;l<log;l++) for(int i=0;i<n;i++) {
			auto &cur = dp[l], &ant = dp[l-1];
			cur[i] = ops(ant[i], ant[min(i+(1<<(l-1)), n-1)]);
		}
	}
	T query(int a, int b)  {
		if(a == b) return dp[0][a];
		int p = 31-__builtin_clz(b-a);
		auto &cur = dp[p];
		return ops(cur[a], cur[b-(1<<p)+1]);
	}
	T query_log(int l, int r) {
		T ans = 0;
		for (int i = dp.size() - 1; i >= 0; i--)
			if ((1 << i) <= r - l + 1)
				ans = ops(ans, dp[i][l]), l += (1 << i);
		return ans;
	}
};
