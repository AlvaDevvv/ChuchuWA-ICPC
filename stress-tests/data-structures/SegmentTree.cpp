#include "stress-tests/utilities/template.h"
#include "content/data-structures/SegmentTree.h"

int main() {
	rep(it, 0, 500) {
		int n = rnd(1, 40);
		SegTree st(n);
		vector<ll> a(n, LLONG_MIN);
		rep(q, 0, 200) {
			if (rnd(0, 1)) {
				int i = rnd(0, n - 1);
				ll v = rnd(-1000, 1000);
				a[i] = v;
				st.update(i, v);
			} else {
				int l = rnd(0, n), r = rnd(0, n);
				if (l > r) swap(l, r);
				ll exp = LLONG_MIN;
				rep(i, l, r) exp = max(exp, a[i]);
				assert(st.query(l, r) == exp);
			}
		}
	}
	cout << "SegmentTree ok\n";
}
