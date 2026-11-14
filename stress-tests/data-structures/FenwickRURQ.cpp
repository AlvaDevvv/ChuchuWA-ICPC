#include "stress-tests/utilities/template.h"
#include "content/data-structures/FenwickRURQ.h"

int main() {
	rep(it, 0, 2000) {
		int n = rnd(1, 30);
		FenwickRURQ ft(n);
		vll a(n + 1, 0);
		rep(q, 0, 60) {
			int l = rnd(1, n), r = rnd(1, n);
			if (rnd(0, 1)) {
				if (l > r) swap(l, r);
				ll v = rnd(-1000, 1000);
				ft.range_update(l, r, v);
				rep(i, l, r + 1) a[i] += v;
			} else {
				ll s = 0;
				rep(i, l, r + 1) s += a[i];
				assert(ft.query(l, r) == s);
			}
		}
	}
	cout << "FenwickRURQ ok\n";
}
