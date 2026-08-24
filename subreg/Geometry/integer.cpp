// Integer Geometry with std::complex
// Sources:
// - Antti Laaksonen (Competitive Programmer's Handbook, Ch. 29)
// - Victor Lecomte (Handbook of Geometry for Competitive Programmers)
// - Codeforces Blog 22175 (https://codeforces.com/blog/entry/22175)

using T = ll;
using P = complex<T>;
#define X real()
#define Y imag()

namespace std {
	bool operator<(P a, P b) {
		return a.X != b.X ? a.X < b.X : a.Y < b.Y;
	}
}

// Dot & Cross products: conj(a) * b = (a . b) + i * (a x b)
T dot(P a, P b) { return (conj(a) * b).X; }
T cross(P a, P b) { return (conj(a) * b).Y; }

bool zero(T x) {
	return x == 0;
}

// angular comparison in [0, 2pi)
// smallest is (1, 0)
// CORNER: a || b == (0, 0)
bool ang_cmp(P a, P b) {
	auto quad = [](P p) -> bool {
		// 0 if ang in [0, pi), 1 if in [pi, 2pi)
		return p.Y < 0 || (p.Y == 0 && p.X < 0);
	};
	using tup = tuple<bool, T>;
	return tup{quad(a), 0} < tup{quad(b), cross(a, b)};
}

T dist2(P p, P q) { // squared distance
    return norm(p - q);
}

T area2(P a, P b, P c) { // two times signed area of triangle abc
	return cross(b - a, c - a);
}

bool left(P a, P b, P c) {
	return area2(a, b, c) > 0; // counterclockwise
}

bool right(P a, P b, P c) {
	return area2(a, b, c) < 0; // clockwise
}

bool collinear(P a, P b, P c) {
	return zero(area2(a, b, c));
}

// CORNER: a || b == (0, 0)
int parallel(P a, P b) {
	if(!zero(cross(a, b))) return 0;
	return (a.X > 0) == (b.X > 0) && (a.Y > 0) == (b.Y > 0) ? 1 : -1;
}

// CORNER: a == b
struct segment {
	P a, b;
	
	segment(): a(), b() {}
	segment(P _a, P _b): a(_a), b(_b) {}

	P vec() { return b - a; }

	bool contains(P p) {
		return a == p || b == p || parallel(a - p, b - p) == -1;
	}
};

bool intersects(segment r, segment s) {
	if(r.contains(s.a) || r.contains(s.b) || s.contains(r.a) || s.contains(r.b)) return 1;
	return left(r.a, r.b, s.a) != left(r.a, r.b, s.b) && 
		left(s.a, s.b, r.a) != left(s.a, s.b, r.b);
}

bool parallel(segment r, segment s) {
	return parallel(r.vec(), s.vec());
}

struct polygon {
	vector<P> vp;
	int n;

	polygon(vector<P>& _vp): vp(_vp), n(vp.size()) {
		if(area2() < 0) reverse(all(_vp));
	}

	int nxt(int i) const { return i + 1 < n ? i + 1 : 0; }
	int prv(int i) const { return i ? i - 1 : n - 1; }

	// If positive, the polygon is in ccw order. It is in cw order otherwise.
	T area2() { // O(n)
		T acum = 0;
		for(int i = 0; i < n; i++)
			acum += cross(vp[i], vp[nxt(i)]);
		return acum;
	}

	bool has(P p) { // O(log n). The polygon must be convex and in ccw order
		if(right(vp[0], vp[1], p) || left(vp[0], vp[n - 1], p)) return 0;
		int lo = 1, hi = n;
		while(lo + 1 < hi) {
			int mid = (lo + hi) / 2;
			if(!right(vp[0], vp[mid], p)) lo = mid;
			else hi = mid;
		}
		return hi != n ? !right(vp[lo], vp[hi], p) : dist2(vp[0], p) <= dist2(vp[0], vp[n - 1]);
	}

	T calipers() { // O(n). The polygon must be convex and in ccw order.
		T ans = 0;
		for(int i = 0, j = 1; i < n; i++) {
			P v = vp[nxt(i)] - vp[i];
			while(cross(v, vp[nxt(j)] - vp[j]) > 0) j = nxt(j);
			ans = max(ans, dist2(vp[i], vp[j])); // Example with polygon diameter squared
		}
		return ans;
	}

	// returns the maximal point using comparator cmp
	// example: 
	// 	extreme([&](P p, P q) {return dot(p, v) > dot(q, v);});
	// 	returns point with maximal dot product with v
	int extreme(const function<bool(P, P)> &cmp) {
		auto is_extreme = [&](int i, bool& cur_dir) -> bool {
			cur_dir = cmp(vp[nxt(i)], vp[i]);
			return !cmp(vp[prv(i)], vp[i]) && !cur_dir;
		};
		bool last_dir, cur_dir;
		if(is_extreme(0, last_dir)) return 0;
		int lo = 0, hi = n; 
		while(lo + 1 < hi) {
			int m = (lo + hi) / 2;
			if(is_extreme(m, cur_dir)) return m;
			bool rel_dir = cmp(vp[m], vp[lo]);
			if((!last_dir && cur_dir) || (last_dir == cur_dir && rel_dir == cur_dir)) {
				lo = m;
				last_dir = cur_dir;
			} else hi = m;
		}
		return lo;
	}

	pair<int, int> tangent(P p) { // O(log n) for convex polygon in ccw orientation
		// Finds the indices of the two tangents to an external point q
		auto left_tangent = [&](P r, P s) -> bool {
			return right(p, r, s);
		};
		auto right_tangent = [&](P r, P s) -> bool {
			return left(p, r, s);
		};
		return {extreme(left_tangent), extreme(right_tangent)};
	}

	void normalize() { // p[0] becomes the lowest leftmost point 
		rotate(vp.begin(), min_element(all(vp)), vp.end());
	}

	polygon operator+(polygon& rhs) { // Minkowsky sum
		normalize();
		rhs.normalize();
		vector<P> sum;
		for(int i = 0, j = 0, dir; i < n || j < rhs.n; i += dir >= 0, j += dir <= 0) {
			sum.push_back(vp[i % n] + rhs.vp[j % rhs.n]);
			dir = cross(vp[(i + 1) % n] - vp[i % n], rhs.vp[(j + 1) % rhs.n] - rhs.vp[j % rhs.n]);
		}
		return polygon(sum);
	}
};
