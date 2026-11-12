#include "stress-tests/utilities/template.h"
#include "content/data-structures/DSU.h"

int main() {
	rep(it, 0, 2000) {
		int n = rnd(1, 30);
		DSU d(n);
		vi c(n);
		iota(all(c), 0);
		rep(q, 0, 60) {
			int a = rnd(0, n - 1), b = rnd(0, n - 1);
			if (rnd(0, 1)) {
				bool exp = c[a] != c[b];
				int ca = c[a], cb = c[b];
				for (int& x : c) if (x == cb) x = ca;
				assert(d.join(a, b) == exp);
			} else {
				assert(d.same(a, b) == (c[a] == c[b]));
				assert(d.size(a) == count(all(c), c[a]));
			}
		}
	}
	cout << "DSU ok\n";
}
