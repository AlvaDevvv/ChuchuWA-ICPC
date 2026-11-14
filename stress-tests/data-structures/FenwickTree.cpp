#include "stress-tests/utilities/template.h"
#include "content/data-structures/FenwickTree.h"

int main() {
	rep(it, 0, 2000) {
		int n = rnd(1, 30);
		vll a(n);
		for (ll& x : a) x = rnd(0, 5);
		FenwickTree ft(a), ft2(n);
		rep(i, 0, n) ft2.update(i + 1, a[i]);
		assert(ft.ft == ft2.ft);
		rep(q, 0, 60) {
			int t = rnd(0, 2);
			if (t == 0) {
				int i = rnd(0, n - 1);
				ll d = rnd(-a[i], 5);
				a[i] += d;
				ft.update(i + 1, d);
			} else if (t == 1) {
				int l = rnd(1, n), r = rnd(1, n);
				ll s = 0;
				rep(i, l, r + 1) s += a[i - 1];
				assert(ft.query(l, r) == s);
			} else {
				ll tot = accumulate(all(a), 0LL);
				ll k = rnd(1, int(tot) + 1);
				int exp = 1;
				ll s = 0;
				while (exp <= n && s + a[exp - 1] < k) s += a[exp - 1], ++exp;
				assert(ft.lower_bound(k) == exp);
			}
		}
	}
	cout << "FenwickTree ok\n";
}
