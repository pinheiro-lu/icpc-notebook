// Double Geometry with std::complex
// WARNING: INPUT IN DOUBLE IS SLOW, IF POSSIBLE READ WITH INTEGER

constexpr double EPS = 1e-10;

using T = double;
using P = complex<T>;
#define X real()
#define Y imag()

bool zero(T x) {
	return abs(x) <= EPS;
}

bool eq(P a, P b) {
	return zero(a.X - b.X) && zero(a.Y - b.Y);
}

namespace std {
	bool operator<(P a, P b) {
		return a.X < b.X - EPS || (zero(a.X - b.X) && a.Y < b.Y - EPS);
	}
}

// Dot & Cross products: conj(a) * b = (a . b) + i * (a x b)
T dot(P a, P b) { return (conj(a) * b).X; }
T cross(P a, P b) { return (conj(a) * b).Y; }

// norm(p): squared magnitude (x^2 + y^2) -> squared distance is norm(p - q)
// abs(p): magnitude / Euclidean norm (sqrt(x^2 + y^2)) -> distance is abs(p - q)
// Rotate by 90 degrees: multiply by ccw90 or cw90
// Rotate by angle ang (ccw): p * polar(1.0, ang)
const P ccw90(0, 1), cw90(0, -1);

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

T area2(P a, P b, P c) { // two times signed area of triangle abc
	return cross(b - a, c - a);
}

// CORNER: BE CAREFUL WITH PRECISION WITH THESE FUNCTIONS, 
// 	IF NEEDED NORMALIZE (b-a) AND (c-a)
bool left(P a, P b, P c) {
	return area2(a, b, c) > EPS; // counterclockwise
}

bool right(P a, P b, P c) {
	return area2(a, b, c) < -EPS; // clockwise
}

bool collinear(P a, P b, P c) {
	return zero(area2(a, b, c));
}

// CORNER: a || b == (0, 0)
int parallel(P a, P b) {
	a = a / abs(a); b = b / abs(b);
	if(!collinear(P(), a, b)) return 0;
	return eq(a, b) ? 1 : -1;
}

// CORNER: a == b
struct segment {
	P a, b;

	segment(): a(), b() {}
	segment(P _a, P _b): a(_a), b(_b) {}

	P vec() { return b - a; }

	bool contains(P p) {
		return eq(a, p) || eq(b, p) || parallel(a - p, b - p) == -1;
	}

	P proj(P p) { // projection of p onto segment
		p = p - a;
		P v = vec();
		return a + v * (dot(p, v) / norm(v));
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

P line_intersection(segment r, segment s) {
	if(parallel(r, s)) return P(HUGE_VAL, HUGE_VAL);
	P vr = r.vec(), vs = s.vec();
	T cr = cross(vr, r.a), cs = cross(vs, s.a);
	return (vs * cr - vr * cs) / cross(vr, vs);
}

struct polygon {
	vector<P> vp;
	int n;

	polygon(vector<P>& _vp): vp(_vp), n(vp.size()) {
		if(area2() < -EPS) reverse(all(_vp));
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
		return hi != n ? !right(vp[lo], vp[hi], p) : norm(vp[0] - p) < norm(vp[0] - vp[n - 1]) + EPS;
	}

	T calipers() { // O(n). The polygon must be convex and in ccw order.
		T ans = 0;
		for(int i = 0, j = 1; i < n; i++) {
			P v = vp[nxt(i)] - vp[i];
			while(cross(v, vp[nxt(j)] - vp[j]) > EPS) j = nxt(j);
			// do something with vp[i] and vp[j]
			ans = max(ans, norm(vp[i] - vp[j])); // Example with polygon diameter squared
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
		T dir;
		for(int i = 0, j = 0; i < n || j < rhs.n; i += dir > -EPS, j += dir < EPS) {
			sum.push_back(vp[i % n] + rhs.vp[j % rhs.n]);
			dir = cross(vp[(i + 1) % n] - vp[i % n], rhs.vp[(j + 1) % rhs.n] - rhs.vp[j % rhs.n]);
		}
		return polygon(sum);
	}
};

// Circle
//  Basic structure of circle and operations related with it.
// 
// All operations' time complexity are O(1)

struct circle {
	P o; T r;

	circle() {}
	circle(P _o, T _r) : o(_o), r(_r) {}
	// CORNER CASE: a, b and c must NOT be collinear
	circle(P a, P b, P c) {
		b = b - a;
		c = c - a;
		T B = norm(b);
		T C = norm(c);
		T D = cross(b, c);
		o = a + P((c.Y * B - b.Y * C) / (2 * D), (b.X * C - c.X * B) / (2 * D));
		r = abs(o - a);
	}

	bool has(P p) { 
		return norm(o - p) < r * r + EPS;
	}
	bool in(circle c) { // non strict
		T d = abs(o - c.o);
		return d + r < c.r + EPS;
	}
	vector<P> operator/(circle c) { // Intersection of circles.
		vector<P> inter;            // The points in the output are in ccw order.
		T d = abs(o - c.o);
		if(r + c.r < d - EPS || d + min(r, c.r) < max(r, c.r) - EPS)
			return {};
		T x = (r * r - c.r * c.r + d * d) / (2 * d);
		T y = sqrt(r * r - x * x);
		P v = (c.o - o) / d;
		inter.pb(o + v * x + v * cw90 * y);
		if(y > EPS) inter.pb(o + v * x + v * ccw90 * y);
		return inter;
	}
	vector<P> tang(P p) {
		T d = sqrt(norm(p - o) - r * r);
		return *this / circle(p, d);
	}
	bool contains(P p) { // non strictly inside
		T d = norm(o - p);
		return d < r * r + EPS;
	}
};

// Circle Area Intersection
// O(n^2 log n) (high constant)
// https://github.com/mhunicken/icpc-team-notebook-el-vasito/blob/master/geometry/circle.cpp

vector<T> intercircles(vector<circle> c) {
	vector<T> r(c.size() + 1); // r[k]: area covered by at least k circles
	for(int i = 0; i < int(c.size()); i++) {
		int k = 1;
		auto cmp = [&](P a, P b) {
			return ang_cmp(a - c[i].o, b - c[i].o);
		};
		vector<pair<P, int>> p{
			{c[i].o + P(1, 0) * c[i].r, 0},
			{c[i].o - P(1, 0) * c[i].r, 0}
		};
		for(int j = 0; j < int(c.size()); j++) if(j != i) {
			bool b0 = c[i].in(c[j]), b1 = c[j].in(c[i]);
			if(b0 && (!b1 || i < j)) k++;
			else if(!b0 && !b1) {
				auto v = c[i] / c[j];
				if(v.size() == 2) {
					p.pb({v[0], 1});
					p.pb({v[1], -1});
					if(cmp(v[1], v[0])) k++;
				}
			}
		}
		sort(p.begin(), p.end(),
			[&](pair<P, int> a, pair<P, int> b) { return cmp(a.first, b.first); });
		for(int j = 0; j < int(p.size()); j++) {
			P p0 = p[j ? j - 1 : p.size() - 1].first, p1 = p[j].first;
			P v0 = p0 - c[i].o, v1 = p1 - c[i].o;
			T a = acos(dot(v0, v1) / (abs(v0) * abs(v1)));
			r[k] += (p0.X - p1.X) * (p0.Y + p1.Y) / 2 + c[i].r * c[i].r * (a - sin(a)) / 2;
			k += p[j].second;
		}
	}
	return r;
}
