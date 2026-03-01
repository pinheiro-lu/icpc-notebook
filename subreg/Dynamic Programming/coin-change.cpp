// Given a set of coins and a target amount, find the minimum number of coins
// required to make the target amount.
// value[x] = minimum number of coins required to make amount x
vi coins; // coin denominations
array<int, N> value;

void coin_change(int n) {
	value[0] = 0;
	for (int x = 1; x <= n; x++) {
		value[x] = INF;
		for (auto c : coins) {
			if (x-c >= 0) {
				value[x] = min(value[x], value[x-c]+1);
			}
		}
	}
}
