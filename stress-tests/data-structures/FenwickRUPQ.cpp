#include "stress-tests/utilities/template.h"
#include "content/data-structures/FenwickRUPQ.h"

int main() {
	rep(it, 0, 2000) {
		int n = rnd(1, 30);
		vll A(n + 1, 0);
		rep(i, 1, n + 1) A[i] = rnd(-1000, 1000);
		FenwickRUPQ ft(A);
		rep(q, 0, 60) {
			if (rnd(0, 1)) {
				int l = rnd(1, n), r = rnd(l, n);
				ll v = rnd(-1000, 1000);
				ft.range_update(l, r, v);
				rep(i, l, r + 1) A[i] += v;
			} else {
				int i = rnd(1, n);
				assert(ft.point_query(i) == A[i]);
			}
		}
	}
	cout << "FenwickRUPQ ok\n";
}
