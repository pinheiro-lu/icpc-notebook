// Segment Tree (Point Update, Range Query)
// vector<int> p: array to build the segment tree (0-indexed)
// vector<int> st: segment tree (1-indexed)
// query: O(log n)
// update: O(log n)
// build: O(n)

struct SegTree {
	vector<int> st;
	int (*comp)(int, int);

	void build(int i, int l, int r) {
		if (l == r) st.at(i) = p.at(l);
		else {
			int m = (l + r) / 2;
			build(2 * i, l, m);
			build(2 * i + 1, m + 1, r);
			st.at(i) = comp(st.at(2 * i), st.at(2 * i + 1));
		}
	}

	SegTree(int (*comp)(int, int)) : comp(comp) {
		st.resize(4 * n);
		build(1, 0, n - 1);
	}

	int query(int i, int l, int r, int a, int b) {
		if (l > b || a > r) return -1;
		if (l >= a && b >= r) return st.at(i);
		int m = (l + r) / 2;
		return comp(query(i * 2, l, m, a, b), query(i * 2 + 1, m + 1, r, a, b));
	}

	void update(int i, int l, int r, int j, int x) {
		if (l > j || r < j) return;
		if (l == r) st.at(i) = x;
		else {
			int m = (l + r) / 2;
			update(i * 2, l, m, j, x);
			update(i * 2 + 1, m + 1, r, j, x);
			st.at(i) = comp(st.at(2 * i), st.at(2 * i + 1));
		}
	}
};
