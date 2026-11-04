// Convex Hull
// Works with double and integer
// Returns in CCW order (reversed in x in UPPER)
// Complexity: O(NlogN)

template <bool UPPER>
vector<P> hull(vector<P> v) {
	vector<P> res;
	if(UPPER) for(auto& p: v) p = -p;
	sort(v.begin(), v.end());
	for(auto& p: v) {
		if(res.empty()) { res.pb(p); continue; }
		if(zero(res.back().X - p.X)) continue;
		while(res.size() >= 2) {
			P a = res[res.size()-2], b = res.back();
			if(!left(a, b, p)) res.pop_back();
			//to include collinear points
			//if(right(a, b, p)) res.pop_back();
			else break;
		}
		res.pb(p);
	}
	if(UPPER) for(auto& p: res) p = -p;
	return res;
}
