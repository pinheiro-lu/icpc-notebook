// CDQ Divide and Conquer (robert1003)
//
// 3D Partial Order (ZJc571):
// Given a set of unique 3D points S of size n (1 <= x, y, z <= n), for each
// P_i = (x_i, y_i, z_i), counts P_j in S with x_i < x_j, y_i < y_j, z_i < z_j.
//
// Adaptations from the post:
// - 2D Update + Submatrix Sum (Mokia / CF1093E):
//   View operations as (t, x, y) where t is time of operation:
//   * Type 1 (A[x, y] += a): create (t, x, y) with value a, sgn = 0.
//   * Type 2 (sum on [x1, y1]..[x2, y2]): create 4 prefix elements (t, x2, y2),
//     (t, x1-1, y2), (t, x2, y1-1), (t, x1-1, y1-1) with sgn = +1/-1, v = 0.
//   * Merge with v[a].x <= v[b].x: left updates bit(y, v) and records (y, -v);
//     right adds sgn * bit.query(y) to ans[i].
// - DP / Convex Hull Optimization (TIOJ1921):
//   For dp in [l, r): 1) cdq(l, m); 2) use dp in [l, m) to update [m, r);
//   3) cdq(m, r); 4) merge the two convex hulls from [l, m) and [m, r).
//
// Complexity: O(N log^2 N)

struct Pt {
	int x, y, z, i;
};

const int N = (int)1e5 + 5;

// Bit<T> from Data Structures/bit.cpp (0-indexed)
Bit<int> bit(0);

vector<Pt> v;
int n, ans[N];

void cdq(int l, int r) {
	if (l + 1 == r) return;
	int m = (l + r) >> 1;
	cdq(l, m); cdq(m, r);
	int a = l, b = m, sum = 0;
	// need to record the modifications on BIT in order to reset it.
	// The complexity will be O(N^2) if we resetting it brute-forcely.
	vector<int> record;
	// temporary array for merge sort
	vector<Pt> tmp;
	while (a < m && b < r) {
		if (v[a].y > v[b].y) {
			bit.update(v[a].z, 1), sum++;
			record.push_back(v[a].z);
			tmp.push_back(v[a++]);
		} else {
			ans[v[b].i] += sum - bit.query(v[b].z);
			tmp.push_back(v[b++]);
		}
	}
	while (a < m) tmp.push_back(v[a++]);
	while (b < r) {
		ans[v[b].i] += sum - bit.query(v[b].z);
		tmp.push_back(v[b++]);
	}
	for (int i = l; i < r; ++i) v[i] = tmp[i - l];
	// reset BIT
	for (auto i : record) bit.update(i, -1);
	// release used memory
	vector<int>().swap(record);
	vector<Pt>().swap(tmp);
}

void solve() {
	bit = Bit<int>(n + 1);
	// As we require > not >=
	sort(v.begin(), v.end(), [&](auto a, auto b) {
		return (a.x == b.x ? (a.y == b.y ? a.z < b.z : a.y < b.y) : a.x > b.x);
	});
	cdq(0, n);
}
