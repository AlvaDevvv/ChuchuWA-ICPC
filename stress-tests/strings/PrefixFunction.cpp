#include "stress-tests/utilities/template.h"
#include "content/strings/PrefixFunction.h"

int main() {
	rep(it, 0, 3000) {
		string s, p;
		rep(i, 0, rnd(0, 30)) s += char('a' + rnd(0, 2));
		rep(i, 0, rnd(1, 4)) p += char('a' + rnd(0, 2));
		vi exp;
		rep(i, 0, sz(s) - sz(p) + 1) if (s.substr(i, sz(p)) == p) exp.push_back(i);
		assert(match(s, p) == exp);
		vi pi = prefixFunction(s);
		rep(i, 0, sz(s)) {
			int best = 0;
			rep(k, 1, i + 1) if (s.substr(0, k) == s.substr(i - k + 1, k)) best = k;
			assert(pi[i] == best);
		}
	}
	cout << "PrefixFunction ok\n";
}
